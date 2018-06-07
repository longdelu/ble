/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief SPI drivers implementation
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-10-20  liqing, first implementation.
 * \endinternal
 */


#include "amdr_spi.h"
#include "am_gpio.h"
#include "am_common.h"
#include "amdr_clk.h"

/*******************************************************************************
* 宏定义                                                                       *
*******************************************************************************/

/* 获取SPI的输入频率 */
#define __SPI_FRQIIN_GET(p_hw_spi)    amdr_clk_periph_rate_get(p_hw_spi)

/* 获取SPI的支持的最大速度 */
#define __SPI_MAXSPEED_GET(p_hw_spi) (__SPI_FRQIIN_GET(p_hw_spi) / 2)

/* 获取SPI支持的最小速度 */
#define __SPI_MINSPEED_GET(p_hw_spi) (__SPI_FRQIIN_GET(p_hw_spi) / 4096)


#define __SPI_CS_HIGH       1   /* CS高电平为有效状态 */
#define __SPI_CS_LOW        0   /* CS低电平为有效状态 */

#define __SPI_CS_ACTIVE     1   /* CS为有效状态       */
#define __SPI_CS_INACTIVE   0   /* CS为无效状态       */

#define __SPI_CS_ACTIVE_LEV_GET(f_dev) (((f_dev)->mode & AM_SPI_CS_HIGH) ?     \
                                                        __SPI_CS_HIGH :        \
                                                        __SPI_CS_LOW)
/* 配置设备的CS引脚片选状态
 * f_dev       ： 设备
 * activelevel ： 有效状态电平
 * state       :   状态
 */
#define __SPI_DEV_CS_CFG(f_dev,activlevel,state)                               \
{                                                                              \
    if ((f_dev)->pfunc_cs == NULL) {                                           \
        am_gpio_pin_cfg((f_dev)->cs_pin, AM_GPIO_OUTPUT);                      \
        am_gpio_set((f_dev)->cs_pin, (state) ? (activlevel) : (!activlevel));  \
    } else {                                                                   \
        (f_dev)->pfunc_cs((f_dev), (state));                                   \
    }                                                                          \
}

/*******************************************************************************
* 状态及事件定义                                                               *
*******************************************************************************/

/* SPI工作状态 */
#define __SPI_ST_IDLE               0       /* 空闲状态            */
#define __SPI_ST_TRANS_START        1       /* 准备传输状态        */
#define __SPI_ST_M_SEND_DATA        2       /* 主机发送数据状态    */
#define __SPI_ST_M_RECV_DATA        3       /* 主机接收数据状态    */

/* 事件切换 */
#define __SPI_NEXT_STATE(s, e)      \
            do {                    \
                p_dev->state = (s); \
                new_event = (e);    \
            } while(0)


/* 32-bits, bit[15: 0] : 事件号
 *          bit[31:16] ：事件参数
 */
#define __SPI_EVT(evt_num, evt_par) (((evt_num) & 0xFFFF) | ((evt_par) << 16))
#define __SPI_EVT_NUM_GET(event)    ((event) & 0xFFFF)
#define __SPI_EVT_PAR_GET(event)    ((event >> 16) & 0xFFFF)

/* SPI 发送事件 */
#define __SPI_EVT_NONE           __SPI_EVT(0, 0) /* 无事件       */
#define __SPI_EVT_TRANS_LAUNCH   __SPI_EVT(1, 0) /* 启动事件     */
#define __SPI_EVT_M_SEND_DATA    __SPI_EVT(2, 0) /* 发送数据事件 */
#define __SPI_EVT_M_RECV_DATA    __SPI_EVT(3, 0) /* 接收数据事件 */

/*******************************************************************************
* Functions declaration                                                        *
*******************************************************************************/

/* SPI驱动函数声明 */

static int __spi_info_get (void *p_drv, am_spi_info_t *p_info);
static int __spi_spi_setup (void *p_drv, struct am_spi_device *p_f_dev);
static int __spi_connect (void *p_drv);
static int __spi_transfer (void                   *p_drv,
                           struct am_spi_device   *p_f_dev,
                           struct am_spi_transfer *p_trans);

/* 内部函数声明 */

static void __spi_irq_handler (void *p_arg);
static void __spi_trans_complete (amdr_spi_dev_t *p_dev, int state);
static int __spi_event_trans (amdr_spi_dev_t *p_dev, uint32_t event);
static void __spi_data_read (amdr_spi_dev_t *p_dev);
static void __spi_data_write (amdr_spi_dev_t *p_dev);

static void __spi_dev_config (amdr_spi_dev_t *p_dev, am_spi_device_t *p_f_dev);
static void __spi_cs_cfg (amdr_spi_dev_t *p_dev, am_spi_device_t *p_f_dev);
static uint32_t __spi_speed_cfg (amdr_spi_dev_t *p_dev, uint32_t target_baud);


/*******************************************************************************
* 全局变量定义                                                                 *
*******************************************************************************/

/**
 * \brief SPI驱动函数结构体
 */
static am_const struct am_spi_drv_funcs __g_spi_drv_funcs = {
    __spi_info_get,
    __spi_spi_setup,
    __spi_transfer,
    __spi_connect
};

/*******************************************************************************
* 函数实现                                                                     *
*******************************************************************************/

/**
 * \brief 获取SPI支持信息
 */
int __spi_info_get (void *p_drv, am_spi_info_t *p_info)
{
    amdr_spi_dev_t   *p_dev     = (amdr_spi_dev_t *)p_drv;
    amhw_spi_t       *p_hw_spi  = p_dev->p_devinfo->p_hw_spi;

    p_info->max_speed = __SPI_MAXSPEED_GET(p_hw_spi);
    p_info->min_speed = __SPI_MINSPEED_GET(p_hw_spi);
    p_info->features  = AM_SPI_CPHA      | /* 时钟相位可设置     */
                        AM_SPI_CPOL      | /* 时钟极性可设置     */
                        AM_SPI_CS_HIGH   | /* 片选拉高有效       */
                        AM_SPI_LSB_FIRST | /* 支持低位优先发送   */
                        AM_SPI_NO_CS;      /* 支持不使用CS片选   */

    return AM_OK;
}

/**
 * \brief 建立设备，检验设备是否支持
 */
int __spi_spi_setup (void *p_drv, struct am_spi_device *p_f_dev)
{
    amdr_spi_dev_t   *p_dev     = (amdr_spi_dev_t *)p_drv;
    amhw_spi_t       *p_hw_spi  = p_dev->p_devinfo->p_hw_spi;

    if(p_hw_spi == NULL) {
        return -AM_EINVAL;
    }

    /* 不支持以下功能 */
    if (p_f_dev->bits_per_word > 16                                  ||
       (p_f_dev->mode & (AM_SPI_LOOP | AM_SPI_READY | AM_SPI_3WIRE)) ||
        p_f_dev->max_speed_hz > __SPI_MAXSPEED_GET(p_hw_spi)) {
        return -AM_ENOTSUP;
    }

    __spi_dev_config(p_dev, p_f_dev); /* 将特性配置到当前设备 */

    return AM_OK;
}

/**
 * \brief SPI传输数据
 */
int __spi_transfer (void              *p_drv,
                    am_spi_device_t   *p_f_dev,
                    am_spi_transfer_t *p_trans)
{
    amdr_spi_dev_t *p_dev    = (amdr_spi_dev_t *)p_drv;
    amhw_spi_t     *p_hw_spi = p_dev->p_devinfo->p_hw_spi;

    if(p_hw_spi == NULL ||
       p_f_dev  ==NULL  ||
       p_trans  == NULL ||
      (p_trans->p_txbuf == NULL &&  p_trans->p_rxbuf == NULL)) {

        return -AM_EINVAL;
    }

    /* 正在进行传输 */
    if (p_dev->state != __SPI_ST_IDLE) {
        return -AM_EBUSY;
    }

    /* 当前设备与目标设备不同，重新配置特性 */
    if ( p_f_dev != p_dev->p_f_dev) {
        __spi_dev_config(p_dev, p_f_dev);
    }

    /* p_trans数据宽度覆盖设备数据宽度 */
    if (p_trans->bits_per_word != 0) {
        uint8_t bits_per_word = p_trans->bits_per_word;

        amhw_spi_feature_cfg(p_hw_spi,
                            (bits_per_word > 9) ? AMHW_SPI_CFG_16BIT_WIDTH :
                                                  AMHW_SPI_CFG_8BIT_WIDTH);
        p_dev->cur_bpw = bits_per_word;
    }

    /* p_trans速度覆盖设备速度 */
    if (p_trans->speed_hz != 0) {
        uint32_t speed = p_trans->speed_hz;

        if ((speed > (p_dev->cur_speed + p_dev->speed_diff)) ||
            (speed < p_dev->cur_speed)) {
            __spi_speed_cfg(p_dev, speed);
            /* 速度和速度死区已经更新的设备中 */
        }
    }

    /* 配置片选有效（非单设备总线） */
    if ((AM_SPI_NO_CS & p_f_dev->mode) == 0) {
        __spi_cs_cfg(p_dev, p_f_dev);
    }

    p_dev->p_trans  = p_trans;
    p_dev->send_ptr = 0;
    p_dev->recv_ptr = 0;

    __spi_event_trans(p_dev, __SPI_EVT_TRANS_LAUNCH);

    return AM_OK;
}

/**
 * \brief SPI中断开启函数
 */
int __spi_connect (void *p_drv)
{
    amdr_spi_dev_t *p_dev = (amdr_spi_dev_t *)p_drv;

    am_int_connect(p_dev->p_devinfo->inum, __spi_irq_handler, (void *)p_drv);
    am_int_enable(p_dev->p_devinfo->inum);
    
    return AM_OK;
}

/**
 * \brief SPI 初始化函数（初始化主机，使能SPI,配置标准层没有定义的特性）
 */
am_spi_handle_t amdr_spi_init (amdr_spi_dev_t           *p_dev,
                               const amdr_spi_devinfo_t *p_devinfo)
{
    amhw_spi_t *p_hw_spi = p_devinfo->p_hw_spi;

    p_dev->spi_serve.p_funcs = (struct am_spi_drv_funcs *)&__g_spi_drv_funcs;
    p_dev->spi_serve.p_drv   = p_dev;
    p_dev->p_devinfo         = p_devinfo;

    p_dev->p_trans  = NULL;
    p_dev->send_ptr = 0;
    p_dev->recv_ptr = 0;

    p_dev->p_f_dev  = NULL;

    if (0 == __spi_speed_cfg(p_dev, p_devinfo->ini_speed)) {
        return NULL;
    }

    /* 配置SPI模式（时钟相位和极性） */
    amhw_spi_mode_cfg(p_hw_spi, p_devinfo->mode_flag << 2);

    /* 配置驱动层定义的特性 */
    amhw_spi_feature_cfg(p_hw_spi, p_devinfo->flags      |
                                   AMHW_SPI_CFG_TDMA_DIS | /* 禁能DMA发送 */
                                   AMHW_SPI_CFG_RDMA_DIS); /* 禁能DMA接收 */

    /* 不使用自带CS片选 */
    amhw_spi_cs_cfg(p_hw_spi, AMHW_SPI_CS_GPIO);

    /* 配置为主机模式 */
    amhw_spi_workmode_cfg(p_hw_spi, AMHW_SPI_WMODE_MASTER);

    amhw_spi_enable(p_hw_spi);

    p_dev->cs_state = FALSE;
    p_dev->state    = __SPI_ST_IDLE;

    return &(p_dev->spi_serve);
}

/**
 * \brief SPI 去初始化函数
 */
void amdr_spi_deinit (amdr_spi_dev_t *p_dev)
{
    amhw_spi_t *p_hw_spi = p_dev->p_devinfo->p_hw_spi;

    p_dev->spi_serve.p_funcs = NULL;
    p_dev->spi_serve.p_drv   = NULL;
    p_dev->p_trans           = NULL;
    p_dev->p_f_dev           = NULL;

    amhw_spi_disable(p_hw_spi);
    am_int_disable(p_dev->p_devinfo->inum);
    am_int_disconnect(p_dev->p_devinfo->inum, __spi_irq_handler, (void *)p_dev);
}

/**
 * \brief SPI中断回调函数
 * \param [in] p_arg  : 回调参数
 * \return 无
 */
void __spi_irq_handler (void *p_arg)
{
    amdr_spi_dev_t   *p_dev    = (amdr_spi_dev_t *)p_arg;
    amhw_spi_t       *p_hw_spi = p_dev->p_devinfo->p_hw_spi;

    uint8_t sta_flag = amhw_spi_stat_get(p_hw_spi);

    /* 接收数据 */
    if((sta_flag & AMHW_SPI_STAT_R_FULL) != 0) {
        __spi_event_trans(p_dev, __SPI_EVT_M_RECV_DATA);
    }

    /* 发送数据 */
    if((sta_flag & AMHW_SPI_STAT_T_EMPTY) && (p_hw_spi->c1 & AMHW_SPI_IRQ_T)) {
        __spi_event_trans(p_dev, __SPI_EVT_M_SEND_DATA);
    }
}

/**
 * \brief 数据传输事件处理
 *        根据自身状态和事件完成异步发送数据
 * \param [in] p_dev  : 指向SPI设备的指针
 * \param [in] event  : 处理事件
 * \return 无
 */
int __spi_event_trans (amdr_spi_dev_t *p_dev, uint32_t event)
{
    amhw_spi_t  *p_hw_spi = p_dev->p_devinfo->p_hw_spi;

    volatile uint32_t new_event = __SPI_EVT_NONE;

    while (1) {

        if (new_event != __SPI_EVT_NONE) {
            event      = new_event;
            new_event  = __SPI_EVT_NONE;
        }

        switch (p_dev->state) {

        case __SPI_ST_IDLE:          /* 空闲状态 */
        {
            if (event != __SPI_EVT_TRANS_LAUNCH) {
                return -AM_EINVAL;
            }
            /* 切换到开始状态，不用break */
        }

        case __SPI_ST_TRANS_START:   /* 开始状态 */
        {

            if (event != __SPI_EVT_TRANS_LAUNCH) {
                return -AM_EINVAL;
            }

            p_dev->send_ptr = 0;
            p_dev->recv_ptr = 0;

            __SPI_NEXT_STATE(__SPI_ST_M_SEND_DATA, __SPI_EVT_M_SEND_DATA);

            break;
        }

        case __SPI_ST_M_SEND_DATA:  /* 发送状态 */
        {
            if (event != __SPI_EVT_M_SEND_DATA) {
                return -AM_EINVAL;
            }

            /* 没有更多的数据需要发送 */
            if (p_dev->send_ptr >= p_dev->p_trans->nbytes) {

                /* 关闭所有中断 */
                amhw_spi_int_disable(p_hw_spi, AMHW_SPI_IRQ_T  |
                                               AMHW_SPI_IRQ_R_MODDEF);

                /* 数据接收完成处理 */
                __spi_trans_complete(p_dev, AM_OK);

                __SPI_NEXT_STATE(__SPI_ST_IDLE, __SPI_EVT_NONE);

            /* 需要发送数据 */
            } else {

                if ((amhw_spi_stat_get(p_hw_spi) & AMHW_SPI_STAT_T_EMPTY) != 0) {

                    amhw_spi_int_disable(p_hw_spi, AMHW_SPI_IRQ_T);

                    __spi_data_write(p_dev);

                    /* 等待接收 */
                    __SPI_NEXT_STATE(__SPI_ST_M_RECV_DATA, __SPI_EVT_NONE);

                    amhw_spi_int_enable(p_hw_spi, AMHW_SPI_IRQ_R_MODDEF);

                /* 使能发送中断 */
                } else {
                    amhw_spi_int_enable(p_hw_spi, AMHW_SPI_IRQ_T);
                }
            }
            break;
        }

        case __SPI_ST_M_RECV_DATA:     /* 接收状态 */
        {

            if (event != __SPI_EVT_M_RECV_DATA) {
                return -AM_EINVAL;
            }

            __spi_data_read(p_dev);

            /* 接收到的数据等于发送的数据 */
            if (p_dev->send_ptr == p_dev->recv_ptr) {

                __SPI_NEXT_STATE(__SPI_ST_M_SEND_DATA, __SPI_EVT_M_SEND_DATA);

                amhw_spi_int_disable(p_hw_spi, AMHW_SPI_IRQ_R_MODDEF |
                                               AMHW_SPI_IRQ_T);

            } else {
                /* 不应该执行到这 */
            }

            break;
        }

        default:
            break;
        } /* switch */

        /* 没有事件，退出事件循环 */
        if (new_event == __SPI_EVT_NONE) {
            break;
        }
    } /* while */

    return AM_OK;
}

/**
 * \brief SPI发送数据
 * \param[in] p_dev  : 指向SPI设备的指针
 * \return 无
 */
void __spi_data_write (amdr_spi_dev_t *p_dev)
{
    amhw_spi_t  *p_hw_spi = p_dev->p_devinfo->p_hw_spi;

    if (p_dev->p_trans->bits_per_word > 9) {
        if (p_dev->p_trans->p_txbuf == NULL) {
            amhw_spi_data16_wirte(p_hw_spi, 0x0000);
        } else {
            amhw_spi_data16_wirte(
                  p_hw_spi,
               *((uint16_t*)((uint32_t)p_dev->p_trans->p_txbuf +p_dev->send_ptr)));
        }

        p_dev->send_ptr += 2;

    } else {
        if (p_dev->p_trans->p_txbuf == NULL) {
            amhw_spi_data8_wirte(p_hw_spi, 0x00);
        } else {
            amhw_spi_data8_wirte(
                  p_hw_spi,
               *((uint8_t*)((uint32_t)p_dev->p_trans->p_txbuf +p_dev->send_ptr)));
        }

        p_dev->send_ptr += 1;
    }
}

/**
 * \brief SPI接收数据
 * \param [in] p_dev  : 指向SPI设备的指针
 * \return 无
 */
void __spi_data_read (amdr_spi_dev_t *p_dev)
{
    amhw_spi_t  *p_hw_spi = p_dev->p_devinfo->p_hw_spi;

    if (p_dev->p_trans->bits_per_word > 9) {
        if (p_dev->p_trans->p_rxbuf == NULL) {
            amhw_spi_data16_read(p_hw_spi);
        } else {
            *(uint16_t*)((uint32_t)p_dev->p_trans->p_rxbuf + p_dev->recv_ptr) =
                         amhw_spi_data16_read(p_hw_spi);
        }

        p_dev->recv_ptr += 2;

    } else {
        if (p_dev->p_trans->p_rxbuf == NULL) {
            amhw_spi_data8_read(p_hw_spi);
        } else {
            *((uint8_t*)((uint32_t)p_dev->p_trans->p_rxbuf + p_dev->recv_ptr)) =
                         amhw_spi_data8_read(p_hw_spi);
        }

        p_dev->recv_ptr += 1;
    }
}

/**
 * \brief 传输完成处理函数
 *
 * \param [in] p_dev  : 指向SPI设备的指针
 * \param [in] state ：传输结果
 *
 * \return 无
 */
void __spi_trans_complete (amdr_spi_dev_t *p_dev, int state)
{
    int active_level  = __SPI_CS_ACTIVE_LEV_GET(p_dev->p_f_dev);

    bool_t trans_end = (p_dev->p_trans->flags & AM_SPI_TRANS_EOT) ? TRUE : FALSE;
    bool_t cs_change = (p_dev->p_trans->cs_change) ? TRUE : FALSE;

    //改变片选状态延迟？

    /* 发生错误，片选置为无效 */
    if(state != AM_OK) {
        __SPI_DEV_CS_CFG(p_dev->p_f_dev, active_level, __SPI_CS_INACTIVE);
        p_dev->cs_state = FALSE;

    } else {
        if((!cs_change && trans_end) || (cs_change && !trans_end)) {
            __SPI_DEV_CS_CFG(p_dev->p_f_dev, active_level, __SPI_CS_INACTIVE);
            p_dev->cs_state = FALSE;
        }
    }

    /* 回调 */
    if(p_dev->p_trans->pfn_callback != NULL) {
        p_dev->p_trans->pfn_callback(p_dev->p_trans->p_arg, state);
    }
}

/**
 * \brief 配置SPI设备特性
 *
 * \param [in] p_dev   : 指向SPI设备的指针
 * \param [in] p_f_dev ：指向SPI传输设备描述的指针
 *
 * \return 无
 */
void  __spi_dev_config (amdr_spi_dev_t    *p_dev,
                        am_spi_device_t   *p_f_dev)
{
    amhw_spi_t *p_hw_spi = p_dev->p_devinfo->p_hw_spi;

    uint32_t speed         = p_f_dev->max_speed_hz;
    uint8_t  bits_per_word = p_f_dev->bits_per_word;

    /* 速度不在死区范围内才需要重新配置 */
    if ((speed > (p_dev->cur_speed + p_dev->speed_diff)) ||
        (speed < p_dev->cur_speed)) {

        __spi_speed_cfg(p_dev, p_f_dev->max_speed_hz);
    }

    /* 配置数据宽度度 */
    if (bits_per_word != p_dev->cur_bpw) {
        amhw_spi_feature_cfg(p_hw_spi,
                            (bits_per_word >9) ? AMHW_SPI_CFG_16BIT_WIDTH :
                                                 AMHW_SPI_CFG_8BIT_WIDTH);
        p_dev->cur_bpw = bits_per_word;
    }

    /* 配置特性 */
    if (p_f_dev->mode != p_dev->p_f_dev->mode) {

        if (p_f_dev->mode & AM_SPI_LSB_FIRST) {
            amhw_spi_feature_cfg(p_hw_spi, AMHW_SPI_CFG_LSB);
        } else {
            amhw_spi_feature_cfg(p_hw_spi, AMHW_SPI_CFG_MSB);
        }

        /* 配置时钟相位和极性 */
        amhw_spi_mode_cfg(p_hw_spi, p_f_dev->mode << 2);
    }

    p_dev->p_f_dev = p_f_dev;
}

/**
 * \brief 片选配置(使设备片选有效)
 *
 * \param [in] p_dev   : 指向SPI设备的指针
 * \param [in] p_f_dev ：指向SPI传输设备描述的指针
 *
 * \return 无
 */
void __spi_cs_cfg (amdr_spi_dev_t *p_dev, am_spi_device_t *p_f_dev)
{
        int active_level_o;  /* 旧设备有效状态   */
        int active_level_n;  /* 新设备的有效状态 */

        active_level_o = __SPI_CS_ACTIVE_LEV_GET(p_dev->p_f_dev);
        active_level_n = __SPI_CS_ACTIVE_LEV_GET(p_f_dev);

        /* 旧设备片选无效，配置新设备片选有效 */
        if (p_dev->cs_state == FALSE) {
            __SPI_DEV_CS_CFG(p_f_dev, active_level_n, __SPI_CS_ACTIVE);

        /*  旧设备片选有效，需要比较是否是同一个设备
         * - 不同设备：使旧设备片选无效，新设备有效；
         * - 同一设备：不做操作。
         */
        } else {

            /* 设备不同 */
            if (p_dev->p_f_dev != p_f_dev) {
                __SPI_DEV_CS_CFG(p_dev->p_f_dev, active_level_o, __SPI_CS_INACTIVE);
                __SPI_DEV_CS_CFG(p_f_dev, active_level_n, __SPI_CS_ACTIVE);
            }

        }

        p_dev->cs_state = TRUE;
}


/**
 * \brief SPI传输速度配置
 *
 * \param[in]  p_dev        : 指向SPI设备的指针
 * \param[in]  target_speed : 目标速度
 *
 * \retval 0 ：不支持目标速度,没有配置成功
 * \return 最终配置的速度，可能与目标速度不同
 *
 * \note   配置成功，会更新SPI设备中的cur_speed和min_diff
 */
uint32_t __spi_speed_cfg (amdr_spi_dev_t *p_dev, uint32_t target_speed)
{

    uint32_t real_speed, best_speed;  /* 计算出的速度    */
    uint32_t pdiv, best_pdiv;         /* 预分频值        */
    uint32_t div, best_div;           /* 分配值          */
    uint32_t diff, min_diff;          /* 速度差值        */
    uint32_t max_speed, min_speed;    /* 支持速度范围    */
    uint32_t spi_freq;                /* SPI输入时钟频率 */

    amhw_spi_t *p_hw_spi = p_dev->p_devinfo->p_hw_spi;

    spi_freq  = __SPI_FRQIIN_GET(p_hw_spi);
    max_speed = __SPI_MAXSPEED_GET(p_hw_spi);
    min_speed = __SPI_MINSPEED_GET(p_hw_spi);

    if (target_speed > max_speed || target_speed < min_speed) {
        return 0;
    }

   min_diff = 0xFFFFFFFFU;

   /* 设置为最大的分频值,速度为最小 */
   best_pdiv  = 7;
   best_div   = 8;
   best_speed = min_speed;

   /* 查找最合适的速度 */
   for (pdiv = 0; (pdiv <= 7) && min_diff; pdiv++)
   {
       for (div = 0; (div <= 8) && min_diff; div++)
       {
           /* all_div = (pdiv+1) * 2^(div+1) */
           real_speed = (spi_freq / ((pdiv + 1) << (div+1)));

           /* 实际速度应小于目标速度 */
           if (target_speed >= real_speed)
           {
               diff = target_speed-real_speed;

               if (min_diff > diff)
               {
                   /* 最合适的速度 */
                   min_diff   = diff;
                   best_pdiv  = pdiv;
                   best_div   = div;
                   best_speed = real_speed;
               }
           }
       }
   }

   amhw_spi_div_cfg(p_hw_spi, best_pdiv, best_div);

   p_dev->cur_speed  = best_speed; /* 保存当前速度                       */
   p_dev->speed_diff = min_diff;   /* 保存死区，死区范围内不再计算速度   */

   return p_dev->cur_speed;
}

/* end of file */
