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
 * \brief I2C drivers implementation
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-25  LXY , first implementation.
 * \endinternal
 */

#include "amdr_i2c.h"
#include "ametal.h"
#include "amdr_clk.h"



#define __I2C_STATE_IDLE   0x01   /* I2C 空闲状态        */
#define __I2C_STATE_READ   0x02   /* I2C 读状态             */
#define __I2C_STATE_WRITE  0x04   /* I2C 写状态             */
#define __I2C_STATE_START  0x08   /* I2C 读启动状态   */

/*******************************************************************************
  Functions declaration
*******************************************************************************/

/* I2C 驱动实现函数   */
static int __i2c_transfer(void *p_arg , am_i2c_transfer_t *p_trans);
static int __i2c_connect(void *p_drv);

/* I2C 硬件初始化配置函数   */
static int __i2c_speed_set(amdr_i2c_dev_t *p_dev , uint32_t speed);
static int __i2c_hard_init(amdr_i2c_dev_t *p_dev);

/* I2C 数据发送与接收函数   */
static int __i2c_send_data (void *p_drv);
static int __i2c_recv_data (void *p_drv);

/* I2C 中断处理函数   */
void __i2c_irq_handler(void *p_arg);
void __i2c_mst_sm_event(void *p_arg);

/**
 * \brief I2C 驱动函数定义
 */
static am_const struct am_i2c_drv_funcs __g_i2c_drv_funcs = {
    __i2c_transfer,
    __i2c_connect,
};

/**
 * \brief I2C时钟配置结构体
 */
typedef struct __i2c_div_table {
    uint8_t   icr;         /**< \brief I2C 乘数因子        */
    uint16_t  scl_div;     /**< \brief I2C 分频因子        */
} __i2c_div_table_t;

/******************************************************************************/

/**
 * \brief I2C 硬件初始化函数
 */
static int __i2c_hard_init (amdr_i2c_dev_t *p_dev)
{
    amhw_i2c_t  *p_hw_i2c = p_dev->p_devinfo->p_hw_i2c;

    if (p_dev == NULL) {
        return  -AM_EINVAL;
    }

    amhw_i2c_disable(p_hw_i2c);

    /* I2C 波特率设置  */
    __i2c_speed_set(p_dev, p_dev->p_devinfo->bus_speed);

    amhw_i2c_enable(p_hw_i2c);

    return AM_OK;
}

/**
 * \brief I2C 波特率设置函数
 */
static int __i2c_speed_set(amdr_i2c_dev_t *p_dev , uint32_t speed)
{
    amhw_i2c_t  *p_hw_i2c   = p_dev->p_devinfo->p_hw_i2c;
    uint8_t  mult , i , multiplier;
    uint32_t source_clk , rate , abs_error;
    uint32_t min_error = 0xffffffff;
    uint32_t best_mult = 0;
    uint32_t best_icr;

    /* I2C 时钟分频因子列表  */
    const __i2c_div_table_t div_table[50] = {
        { 0x00, 20 },   { 0x01, 22 },   { 0x02, 24 },   { 0x03, 26 },
        { 0x04, 28 },   { 0x05, 30 },   { 0x09, 32 },   { 0x06, 34 },
        { 0x0a, 36 },   { 0x07, 40 },   { 0x0c, 44 },   { 0x10, 48 },
        { 0x11, 56 },   { 0x12, 64 },   { 0x0f, 68 },   { 0x13, 72 },
        { 0x14, 80 },   { 0x15, 88 },   { 0x19, 96 },   { 0x16, 104 },
        { 0x1a, 112 },  { 0x17, 128 },  { 0x1c, 144 },  { 0x1d, 160 },
        { 0x1e, 192 },  { 0x22, 224 },  { 0x1f, 240 },  { 0x23, 256 },
        { 0x24, 288 },  { 0x25, 320 },  { 0x26, 384 },  { 0x2a, 448 },
        { 0x27, 480 },  { 0x2b, 512 },  { 0x2c, 576 },  { 0x2d, 640 },
        { 0x2e, 768 },  { 0x32, 896 },  { 0x2f, 960 },  { 0x33, 1024 },
        { 0x34, 1152 }, { 0x35, 1280 }, { 0x36, 1536 }, { 0x3a, 1792 },
        { 0x37, 1920 }, { 0x3b, 2048 }, { 0x3c, 2304 }, { 0x3d, 2560 },
        { 0x3e, 3072 }, { 0x3f, 3840 }
    };

    /*  获取当前I2C运行时钟  */
    source_clk = amdr_clk_periph_rate_get(p_hw_i2c);

    if(speed > 1000000) {
        return  -AM_EINVAL;
    }
    for (mult = 0 ; mult <= 2 ; mult++) {
        multiplier = 1 << mult;
        for (i = 0 ; i < 50 ; i++) {
            rate = source_clk / (multiplier * div_table[i].scl_div);
            abs_error = speed > rate ? speed - rate : rate - speed;
            if (abs_error < min_error) {
                best_mult = mult;
                best_icr  =  div_table[i].icr;
                min_error = abs_error;
            }
            if (min_error == 0) {
                break;
            }
        }
    }

    /* 写入分频因子，设置波特率  */
    amhw_i2c_clk_div_set(p_hw_i2c , (best_mult << 6)|best_icr);
    return AM_OK;
}

/**
 * \brief I2C 错误处理函数
 */
static void __i2c_end_with_error (amdr_i2c_dev_t *p_dev , int error)
{
    amhw_i2c_t        *p_hw_i2c = p_dev->p_devinfo->p_hw_i2c;
    am_i2c_transfer_t *p_trans  = p_dev->p_transfer;

    /* STOP信号，释放总线  */
    amhw_i2c_stop_signal_send(p_hw_i2c);

    p_dev->state = __I2C_STATE_IDLE;
    p_dev->busy  = FALSE;

    amhw_i2c_int_disable(p_hw_i2c, AMHW_I2C_INT_IICIE);

    if(p_trans->pfn_callback != NULL) {
        p_trans->pfn_callback(p_trans->p_arg,error);
    }
}

/**
 * \brief I2C 发送设备地址，开始进行传输
 */
static int __i2c_transfer (void *p_arg , am_i2c_transfer_t *p_trans)
{
    amdr_i2c_dev_t *p_dev    = (amdr_i2c_dev_t *)p_arg;
    amhw_i2c_t     *p_hw_i2c = p_dev->p_devinfo->p_hw_i2c;
    uint16_t        dev_addr = (p_trans->addr) << 1;

    if (p_dev == NULL || p_trans == NULL) {
        return  -AM_EINVAL;  /* I2C 参数无效   */
    }
    if (p_dev->busy == TRUE) {
        return  -AM_EBUSY;  /* I2C 设备忙   */
    }
    if (p_trans->flags & AM_I2C_ADDR_10BIT) {
        return  -AM_ENOTSUP;  /*  不支持   */
    }
    p_dev->busy       = TRUE;
    p_dev->p_transfer = p_trans;
    p_dev->data_ptr   = 0;
    
    if (p_trans->flags & AM_I2C_M_RD) {
        p_dev->state = __I2C_STATE_START; /*  读启动状态   */
    } else {
        p_dev->state = __I2C_STATE_WRITE; /* 写状态                */
    }	

    if (!(p_trans->flags & AM_I2C_M_NOSTART)) {
        if (p_trans->flags & AM_I2C_M_RD) {
            dev_addr |= 0x1;
            amhw_i2c_restart_signal_send(p_hw_i2c); /* 读状态重新启动  */
        } else {
            dev_addr |= 0x0;
        }

        /* 开始信号 */
        amhw_i2c_start_signal_send(p_hw_i2c);

        /* 发送模式   */
        amhw_i2c_transmode_set(p_hw_i2c , AMHW_I2C_TRANSMODE_SEND);

        /* 写入从设备地址   */
        amhw_i2c_data_write(p_hw_i2c , dev_addr);

    } else {
        if (!(p_trans->flags & AM_I2C_M_RD)) {
             __i2c_send_data(p_dev);
        }
	}

    /* 使能中断  */
    amhw_i2c_int_enable(p_hw_i2c, AMHW_I2C_INT_IICIE);
    return AM_OK;
}

/**
 * \brief I2C 中断连接函数
 */
static int __i2c_connect (void *p_drv)
{
    amdr_i2c_dev_t *p_dev = (amdr_i2c_dev_t *)p_drv;

    if (p_dev == NULL) {
        return -AM_EINVAL; /* 无效参数 */
    }

    am_int_connect(p_dev->p_devinfo->inum, __i2c_irq_handler, (void *)p_drv);
    am_int_enable(p_dev->p_devinfo->inum);
   
    return AM_OK;
}

/**
 * \brief I2C 发送数据
 */
static int __i2c_send_data (void *p_drv)
{
    amdr_i2c_dev_t    *p_dev    = (amdr_i2c_dev_t *)p_drv;
    amhw_i2c_t        *p_hw_i2c = p_dev->p_devinfo->p_hw_i2c;
    am_i2c_transfer_t *p_trans  = p_dev->p_transfer;

    if (p_dev ==NULL) {
        return  -AM_EINVAL;
    }

    if (p_dev->data_ptr < p_trans->length) {

        amhw_i2c_transmode_set(p_hw_i2c , AMHW_I2C_TRANSMODE_SEND);
        amhw_i2c_data_write(p_hw_i2c , ((uint8_t *)(p_trans->p_buf))[p_dev->data_ptr++]);

    } else {
        p_dev->data_ptr = 0;
        p_dev->busy     = FALSE;
        amhw_i2c_int_disable(p_hw_i2c, AMHW_I2C_INT_IICIE);

        if (p_trans->flags & AM_I2C_M_STOP) {
            amhw_i2c_stop_signal_send(p_hw_i2c);
        }

        p_dev->state = __I2C_STATE_IDLE;

        if (p_trans->pfn_callback != NULL) {
            p_trans->pfn_callback(p_trans->p_arg , AM_OK);
        }
    }
    return AM_OK;
}

/**
 * \brief I2C 接受数据
 */
static int __i2c_recv_data (void *p_drv)
{
    amdr_i2c_dev_t     *p_dev         = (amdr_i2c_dev_t *)p_drv;
    amhw_i2c_t         *p_hw_i2c      = p_dev->p_devinfo->p_hw_i2c;
    am_i2c_transfer_t  *p_trans       = p_dev->p_transfer;

    if (p_dev == NULL) {
        return  -AM_EINVAL;
    }

    amhw_i2c_transmode_set(p_hw_i2c , AMHW_I2C_TRANSMODE_RECV);

    if ((p_trans->length > 1) && (p_dev->data_ptr < p_trans->length - 2)) {

        /* 发送ACK   */
        amhw_i2c_ack_respond(p_hw_i2c);

    } else {

        /* 发送NACK   */
        amhw_i2c_nak_respond(p_hw_i2c);
    }
	
    if (p_dev->data_ptr >= p_trans->length -1) {
    
        amhw_i2c_int_disable(p_hw_i2c, AMHW_I2C_INT_IICIE);

        if (p_trans->flags & AM_I2C_M_STOP) {
            amhw_i2c_stop_signal_send(p_hw_i2c);
        }
        amhw_i2c_ack_respond(p_hw_i2c);
    }

    ((uint8_t *)(p_trans->p_buf))[p_dev->data_ptr++] = amhw_i2c_data_read(p_hw_i2c);

    if (p_dev->data_ptr >= p_trans->length ) {
        if (p_trans->pfn_callback != NULL) {
		    p_trans->pfn_callback(p_trans->p_arg , AM_OK);
		}
        p_dev->data_ptr = 0;
        p_dev->busy     = FALSE;
        p_dev->state    = __I2C_STATE_IDLE;   
    }

    return AM_OK;
}

/**
 * \brief I2C 状态机
 */
void __i2c_mst_sm_event(void *p_arg)
{
    amdr_i2c_dev_t  *p_dev    = (amdr_i2c_dev_t *)p_arg;

    switch(p_dev->state) {
    case __I2C_STATE_IDLE:
        break;

    case __I2C_STATE_READ:
        __i2c_recv_data(p_dev);
        break;
		 
    case __I2C_STATE_WRITE:
        __i2c_send_data(p_dev);
        break;

    case __I2C_STATE_START:
        amhw_i2c_transmode_set(p_dev->p_devinfo->p_hw_i2c , AMHW_I2C_TRANSMODE_RECV);
         ((uint8_t *)(p_dev->p_transfer->p_buf))[0] = amhw_i2c_data_read(p_dev->p_devinfo->p_hw_i2c);
        p_dev->state = __I2C_STATE_READ;
		if (p_dev->p_transfer->length > 1) {
            amhw_i2c_ack_respond(p_dev->p_devinfo->p_hw_i2c);
        } else {
            amhw_i2c_nak_respond(p_dev->p_devinfo->p_hw_i2c);
        }
        break;

    default:
        break;
    }
}

/**
 * \brief I2C 中断函数
 */
void __i2c_irq_handler (void *p_arg)
{
    amdr_i2c_dev_t  *p_dev    = (amdr_i2c_dev_t *)p_arg;
    amhw_i2c_t      *p_hw_i2c = p_dev->p_devinfo->p_hw_i2c;

    uint8_t i2c_status  = amhw_i2c_stat_get(p_hw_i2c);
    uint8_t i2c_timeout = amhw_i2c_tmout_flag_get(p_hw_i2c);

    if (i2c_status == 0) {
        return;
    }

    if (i2c_status & AMHW_I2C_STAT_IICIF) {
        if (i2c_status & AMHW_I2C_STAT_ARBL) {
            amhw_i2c_stat_clr (p_hw_i2c , AMHW_I2C_STAT_ARBL);
            __i2c_end_with_error(p_dev , -AM_ENXIO);
        } else if (i2c_timeout & AMHW_I2C_TMOUT_FLAG_SCL_LOW) {
            amhw_i2c_tmout_flag_clr(p_hw_i2c , AMHW_I2C_TMOUT_FLAG_SCL_LOW);
            __i2c_end_with_error(p_dev , -AM_ENXIO);
        } else if (i2c_timeout & AMHW_I2C_TMOUT_FLAG_SDA_LOW) {
            amhw_i2c_tmout_flag_clr(p_hw_i2c , AMHW_I2C_TMOUT_FLAG_SDA_LOW);
            __i2c_end_with_error(p_dev , -AM_ENXIO);
        } else if(i2c_status & AMHW_I2C_STAT_TCF) {
            __i2c_mst_sm_event(p_dev);
        }
        amhw_i2c_stat_clr(p_hw_i2c , AMHW_I2C_STAT_IICIF);
    }
}

/**
 * \brief I2C 初始化
 */
am_i2c_handle_t amdr_i2c_init(amdr_i2c_dev_t            *p_dev ,
                              const amdr_i2c_devinfo_t  *p_devinfo)
{
    p_dev->i2c_serv.p_drv_funcs = (struct am_i2c_drv_funcs *)&__g_i2c_drv_funcs;
    p_dev->i2c_serv.p_drv       = p_dev;

    p_dev->p_devinfo = p_devinfo;

    p_dev->busy        = FALSE;
    p_dev->p_transfer  = NULL;
    p_dev->data_ptr    = 0;
    p_dev->state       = __I2C_STATE_IDLE;

    __i2c_hard_init(p_dev);

    return &(p_dev->i2c_serv);
}

/**
 * \brief I2C 解初始化
 */
void amdr_i2c_deinit (amdr_i2c_dev_t *p_dev)
{
    amhw_i2c_t *p_hw_i2c  = p_dev->p_devinfo->p_hw_i2c;
    p_dev->i2c_serv.p_drv_funcs = NULL;
    p_dev->i2c_serv.p_drv       = NULL;

    amhw_i2c_disable(p_hw_i2c);
    am_int_disable(p_dev->p_devinfo->inum);
}

/* end of file */
