/*******************************************************************************
*                                 AMetal
*                       ---------------------------
*                       innovating embedded systems
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief UART 驱动实现
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-10-20 xym, first implementation.
 * \endinternal
 */
#include "amdr_uart.h"
#include "amdr_clk.h"
#include "am_int.h"

/*******************************************************************************
* Functions declaration
*******************************************************************************/

/**
 * \brief 串口模式（查询或中断）设置
 */
int __uart_mode_set (amdr_uart_dev_t *p_dev, uint32_t new_mode);

/**
 * \brief 串口硬件设置
 */
int __uart_opt_set (amdr_uart_dev_t *p_dev, uint32_t opts);


/* KS16 串口驱动函数声明 */
static int __uart_ioctl (void *p_drv, int, void *);

static int __uart_tx_startup (void *p_drv);

static int __uart_callback_set (void *p_drv,
                                int   callback_type,
                                int  (*pfn_callback)(void *),
                                void  *callback_arg);

static int __uart_poll_getchar (void *p_drv, char *p_char);

static int __uart_poll_putchar (void *p_drv, char outchar);

static int __uart_connect (void *p_drv);

/** \brief 标准层接口函数实现 */
static const struct am_uart_drv_funcs __g_uart_drv_funcs = {
         __uart_ioctl,
         __uart_tx_startup,
         __uart_callback_set,
         __uart_poll_getchar,
         __uart_poll_putchar,
         __uart_connect,
};

/******************************************************************************/

/**
 * \brief 串口IO控制
 *
 * 可设置/获取波特率、模式（中断或轮询）、串口选项（校验，数据位数，停止位）
 * 以及和调制控制（RTS/CTS）。
 *
 * \param[in] p_drv   : 指向需控制的串口设备结构体
 * \param[in] request : 控制指令
   - AM_UART_BAUD_SET : 设置波特率， p_arg为uint32_t类型，值为波特率
   - AM_UART_BAUD_GET : 获取波特率， p_arg为uint32_t指针类型
   - AM_UART_OPTS_SET : 设置硬件参数，p_arg 为uint32_t类型（如：#AM_UART_CS8）
   - AM_UART_OPTS_GET : 获取当前的硬件参数设置，p_arg为uint32_t指针类型
   - AM_UART_MODE_SET : 设置模式，p_arg值为AM_UART_MODE_POLL或者AM_UART_MODE_INT
   - AM_UART_MODE_GET : 获取当前模式， p_arg为uint32_t指针类型
   - AM_UART_AVAIL_MODES_GET : 获取当前可用的模式， p_arg为uint32_t指针类型
 * \param[in,out] p_arg      : 该指令对应的参数
 *
 * \retval        AM_OK      : 控制指令执行成功
 * \retval       -AM_EIO     : 指令错误
 */
static int __uart_ioctl (void *p_drv, int request, void *p_arg)
{
    amdr_uart_dev_t *p_dev     = (amdr_uart_dev_t *)p_drv;
    int              status    = AM_OK;
    amhw_uart_t     *p_hw_uart = p_dev->p_devinfo->p_hw_uart;

    switch (request) {

    /* 波特率设置 */
    case AM_UART_BAUD_SET:
        p_hw_uart = p_dev->p_devinfo->p_hw_uart;
        /* 等待空闲*/
        while (!(amhw_uart_intstat_get(p_hw_uart) & AMHW_UART_INTSTAT_S1_IDLE));

        status = amhw_uart_baudrate_set(p_dev->p_devinfo->p_hw_uart,
                      amdr_clk_periph_rate_get((void *)(p_hw_uart)),
                                       (uint32_t)p_arg);

        if (status > 0) {
            p_dev->baud_rate = status;
            status = AM_OK;
        } else {
            status = -AM_EIO;
        }
        break;

     /* 波特率获取 */
    case AM_UART_BAUD_GET:
        *(int *)p_arg = p_dev->baud_rate;
        break;

    /* 模式设置 */
    case AM_UART_MODE_SET:
        status = (__uart_mode_set(p_dev, (int)p_arg) == AM_OK)
                 ? AM_OK : -AM_EIO;
        break;

    /* 模式获取 */
    case AM_UART_MODE_GET:
        *(int *)p_arg = p_dev->channel_mode;
        break;

    /* 获取串口可设置的模式 */
    case AM_UART_AVAIL_MODES_GET:
        *(int *)p_arg = AM_UART_MODE_INT | AM_UART_MODE_POLL;
        break;

    /* 串口选项设置 */
    case AM_UART_OPTS_SET:
        status = (__uart_opt_set(p_dev, (int)p_arg) == AM_OK) ? AM_OK : -AM_EIO;
        break;

    /* 串口选项获取 */
    case AM_UART_OPTS_GET:
        *(int *)p_arg = p_dev->options;
        break;
    default:
        status = -AM_EIO;
        break;
    }

    return (status);
}

/**
 * \brief 启动串口发送(仅限中断模式)
 *
 * \param p_drv  : 指向需控制的串口设备结构体
 *
 * \return AM_OK : 启动成功
 */
int __uart_tx_startup (void *p_drv)
{
    amdr_uart_dev_t *p_dev     = (amdr_uart_dev_t *)p_drv;
    amhw_uart_t     *p_hw_uart = p_dev->p_devinfo->p_hw_uart;

    /* 使能发送中断 */
    amhw_uart_int_enable(p_hw_uart, AMHW_UART_INT_C2_TCIE);

    return AM_OK;
}

/**
 * \brief 配置中断服务回调函数
 *
 * 设置串口发送和接收中断回调函数中的
 * 获取发送字符（get）和存放接收字符（put）的回调函数
 *
 * \param     p_drv         : 指向需控制的串口设备结构体
 * \param[in] callback_type : 指明设置的何种回调函数
 *            - AM_UART_CALLBACK_GET_TX_CHAR  : 获取一个发送字符函数
 *            - AM_UART_CALLBACK_PUT_RCV_CHAR : 提交一个接收到的字符给应用程序
 *            - AM_UART_CALLBACK_ERROR        : 错误回调函数
 * \param[in] pfn_callback  : 指向回调函数的指针
 * \param[in] callback_arg  : 回调函数的用户参数
 *
 * \retval  AM_OK           : 回调函数设置成功
 * \retval -AM_ENOTSUP      : 不支持该回调函数类型
 */
static int __uart_callback_set ( void  *p_drv,
                                 int    callback_type,
                                 int  (*pfn_callback)(void *),
                                 void  *callback_arg)
{
    amdr_uart_dev_t *p_dev = (amdr_uart_dev_t *)p_drv;

    switch (callback_type) {

    /* 设置发送回调函数中的获取发送字符回调函数 */
    case AM_UART_CALLBACK_GET_TX_CHAR:
        p_dev->pfn_txchar_get = (pfn_uart_txchar_get_t)pfn_callback;
        p_dev->txget_arg      = callback_arg;
        return (AM_OK);

    /* 设置接收回调函数中的存放接收字符回调函数 */
    case AM_UART_CALLBACK_PUT_RCV_CHAR:
        p_dev->pfn_rxchar_put = (pfn_uart_rxchar_put_t)pfn_callback;
        p_dev->rxput_arg      = callback_arg;
        return (AM_OK);

    /* 设置串口异常回调函数 */
    case AM_UART_CALLBACK_ERROR:
        p_dev->pfn_err = (pfn_uart_err_t)pfn_callback;
        p_dev->err_arg = callback_arg;
        return (AM_OK);

    default:
        return (-AM_ENOTSUP);
    }
}

/**
 * \brief 轮询模式下发送一个字符
 *
 * \param[in] p_dev     : 指向需控制的串口设备结构体
 * \param[in] outchar   : 待发送字符
 *
 * \retval    AM_OK     : 发送数据成功
 * \retval   -AM_EAGAIN : 发送数据未就绪，调用失败
*/
static int __uart_poll_putchar (void *p_drv, char outchar)
{
    amdr_uart_dev_t *p_dev     = (amdr_uart_dev_t *)p_drv;
    amhw_uart_t     *p_hw_uart = p_dev->p_devinfo->p_hw_uart;
    bool_t          idle_stat  = (bool_t)(amhw_uart_intstat_get(p_hw_uart)>>4);

   // amhw_uart_poll_send(p_hw_uart,((uint8_t *)(&outchar)),1);

    /* 发送模块是否空闲 */
    if((idle_stat & amhw_uart_stat1_tdre_get(p_hw_uart)) == 0) {
        return (-AM_EAGAIN);
    } else {
      /* 发送一个字符 */
      amhw_uart_txdata_write(p_hw_uart, outchar);
    }

    return (AM_OK);
}

/**
 * \brief 轮询模式下接收字符
 * \param[in]  p_dev  : 指向需控制的串口设备结构体
 * \param[out] p_char : 指向待存放接收数据的存储单元
 *
 * \retval  AM_OK     : 接收数据成功
 * \retval -AM_EAGAIN : 接收数据未就绪，调用失败
 */
static int __uart_poll_getchar (void *p_drv, char *p_char)
{
    amdr_uart_dev_t *p_dev     = (amdr_uart_dev_t *)p_drv;
    amhw_uart_t     *p_hw_uart = p_dev->p_devinfo->p_hw_uart;
    uint8_t         *p_inchar  = (uint8_t *)p_char;
    bool_t          idle_stat  = (bool_t)(amhw_uart_intstat_get(p_hw_uart)>>4);
    //amhw_uart_poll_receive(p_hw_uart,p_inchar,1);

    /* 接收模块是否空闲 */
    if((idle_stat & amhw_uart_stat1_rdre_get(p_hw_uart)) == 0) {
        return (-AM_EAGAIN);
      }else {
          /* 接收一个字符 */
          *p_inchar = amhw_uart_rxdata_read(p_hw_uart);
      }

    return (AM_OK);
}

/**
 * \brief 配置串口模式
 *
 * \param[in] p_dev    : 指向需控制的串口设备结构体
 * \param[in] new_mode : 配置模式
 *
 * \retval    AM_OK    : 模式配置成功
 * \retval    AM_ERROR : 模式不支持，调用失败
 */
int __uart_mode_set (amdr_uart_dev_t *p_dev, uint32_t new_mode)
{
    amhw_uart_t *p_hw_uart = p_dev->p_devinfo->p_hw_uart;

    /* 仅支持以下模式 */
    if ((new_mode != AM_UART_MODE_POLL) && (new_mode != AM_UART_MODE_INT)) {
        return (AM_ERROR);
    }

    if (new_mode == AM_UART_MODE_INT) {
        /* 使能RDRF接收准中断 */
        amhw_uart_int_enable(p_hw_uart, AMHW_UART_INT_C2_IRIE);
    } else {
        /* 关闭所有串口中断 */
        amhw_uart_int_disable(p_hw_uart, AMHW_UART_INT_ALL);
    }

    p_dev->channel_mode = new_mode;

    return (AM_OK);
}

/**
 * \brief 串口选项配置
 *
 * \param[in] p_dev     : 指向需控制的串口设备结构体
 * \param[in] options   : 配置选项参数
 *
 * \retval    AM_OK     : 配置成功
 * \retval   -AM_EINVAL : 设备为NULL，调用失败
 *
 * \note 支持数据长度，校验方式、停止位配置
 */
int __uart_opt_set (amdr_uart_dev_t *p_dev, uint32_t options)
{
    amhw_uart_t *p_hw_uart = p_dev->p_devinfo->p_hw_uart;
    uint8_t      cfg_flags = 0;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    p_hw_uart = p_dev->p_devinfo->p_hw_uart;

    /* 在改变UART寄存器值前 接收发送禁能 */
    amhw_uart_disable(p_hw_uart);

    /* 配置数据长度 */
    switch (options & AM_UART_CSIZE) {

    case AM_UART_CS7:
        cfg_flags &= ~(1 << 4);
        cfg_flags |= AMHW_UART_C1_M_8BIT;
        break;

    case AM_UART_CS8:
        cfg_flags &= ~(1 << 4);
        cfg_flags |= AMHW_UART_C1_M_9BIT;
        break;

    default:
        break;
    }

    /* 配置停止位 */
    if (options & AM_UART_STOPB) {
        cfg_flags &= ~(0x01 << 5);
        cfg_flags |= AMHW_UART_BDH_SBNS_STOP_2;
    } else {
        cfg_flags &= ~(0x01 << 5);
        cfg_flags |= AMHW_UART_BDH_SBNS_STOP_1;
    }

    /* 配置检验方式 */
    if (options & AM_UART_PARENB) {
        cfg_flags &= ~(0x03 << 0);

        if (options & AM_UART_PARODD) {
            cfg_flags |= AMHW_UART_C1_PARITY_ODD;
        } else {
            cfg_flags |= AMHW_UART_C1_PARITY_EVEN;
        }
    } else {
        cfg_flags &= ~(0x03 << 0);
        cfg_flags |= AMHW_UART_C1_PARITY_NO;
    }

    /* 保存和生效配置 */

    amhw_uart_stop_bit_set (p_hw_uart, (cfg_flags & 0x20));
    amhw_uart_data_mode_set(p_hw_uart, (cfg_flags & 0x10));
    amhw_uart_parity_set(p_hw_uart,    (cfg_flags & 0x03));

    amhw_uart_enable(p_hw_uart);

    p_dev->options = options;

    return (AM_OK);
}



/******************************************************************************
  UART interrupt request handler
******************************************************************************/
/**
 * \brief 串口接收中断服务
 * \param[in] p_dev ： 指向需控制的串口设备结构体
 */
void __uart_irq_rx_handler (amdr_uart_dev_t *p_dev)
{
    amhw_uart_t *p_hw_uart = p_dev->p_devinfo->p_hw_uart;
    uint32_t      int_stat   = amhw_uart_intstat_get(p_hw_uart);

    char data;

    /* 是否为接收Rx中断 */
    if ((int_stat & AMHW_UART_INTSTAT_S1_RDRF) != 0) {

        /* 获取新接收数据 */
        data = amhw_uart_rxdata_read(p_hw_uart);

        /* 存放新接收数据 */
        p_dev->pfn_rxchar_put(p_dev->rxput_arg, data);
    }
}

/**
 * \brief 串口发送中断服务
 * \param[in] p_dev : 指向需控制的串口设备结构体
 */
void __uart_irq_tx_handler (amdr_uart_dev_t *p_dev)
{
    amhw_uart_t *p_hw_uart = p_dev->p_devinfo->p_hw_uart;
    uint32_t    int_stat   = amhw_uart_intstat_get(p_hw_uart);

    char data;

    if (((int_stat & AMHW_UART_INTSTAT_S1_TDRE) != 0) || /* 是否为发送Tx中断 */
        ((int_stat & AMHW_UART_INTSTAT_S1_IDLE) != 0)) {

        /* 获取发送数据并发送 */
        if ((p_dev->pfn_txchar_get(p_dev->txget_arg, &data)) == AM_OK) {
            amhw_uart_txdata_write(p_hw_uart, data);
        } else {
            /* 没有数据传送就关闭发送中断 */
            amhw_uart_int_disable(p_hw_uart, AMHW_UART_INT_C2_TCIE);
        }

    }
}

/**
 * \brief 串口中断服务函数
 * \param[in] p_arg : 指向需控制的串口设备结构体 ，在am_int_connect()注册
 */
void __uart_irq_handler (void *p_arg)
{
    amdr_uart_dev_t     *p_dev      = (amdr_uart_dev_t *)p_arg;
    amhw_uart_t         *p_hw_uart = p_dev->p_devinfo->p_hw_uart;

    uint32_t uart_int_stat = amhw_uart_intstat_get(p_hw_uart);


    if (uart_int_stat & AMHW_UART_INTSTAT_S1_RDRF) {
         __uart_irq_rx_handler(p_dev);
    } else if (uart_int_stat & AMHW_UART_INTSTAT_S1_TDRE) {
        __uart_irq_tx_handler(p_dev);
    } else {

    }
    /* 其他中断 */
    if ((p_dev->other_int_enable & uart_int_stat) != 0) {

        uart_int_stat &= p_dev->other_int_enable;

        if (p_dev->pfn_err != NULL) {
            p_dev->pfn_err(p_dev->err_arg,
                           AMDR_UART_ERRCODE_UART_OTHER_INT,
                           (void *)uart_int_stat,
                           1);
        }
    }

}

/**
 * \brief UART中断函数连接，仅使用中断模式时需要调用此函数
 *
 * \param[in] p_dev : 指向需控制的串口设备结构体
 *
 * \retval    AM_OK : 连接成功
 */
int __uart_connect (void *p_drv)
{
    amdr_uart_dev_t *p_dev = (amdr_uart_dev_t *)p_drv;

    /* 关联中断向量号，开启中断 */
    am_int_connect(p_dev->p_devinfo->inum, __uart_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);
    amhw_uart_int_enable(p_dev->p_devinfo->p_hw_uart,
                          p_dev->other_int_enable);

    return AM_OK;
}

/**
 * \brief 默认回调函数
 *
 * \returns AW_ERROR
 */
static int __uart_dummy_callback (void *p_arg, char *p_outchar)
{
    return (AM_ERROR);
}

/**
 * \brief 串口初始化函数
 *
 * \param[in] p_dev     : 指向需控制的串口设备结构体
 * \param[in] p_devinfo : 指向串口设备的信息结构体常量
 *
 * \return 串口标准服务句柄
 */
am_uart_handle_t amdr_uart_init (amdr_uart_dev_t           *p_dev,
                                  const amdr_uart_devinfo_t *p_devinfo)
{
    amhw_uart_t           *p_hw_uart;
    uint32_t                tmp;

    if (p_devinfo == NULL || p_devinfo->p_hw_uart == NULL) {
        return NULL;
    }

    /* 获取配置参数 */
    p_hw_uart               = p_devinfo->p_hw_uart;
    p_dev->p_devinfo         = p_devinfo;
    p_dev->uart_serv.p_funcs = (struct am_uart_drv_funcs *)&__g_uart_drv_funcs;
    p_dev->uart_serv.p_drv   = p_dev;
    p_dev->baud_rate         = p_devinfo->baud_rate;
    p_dev->options           = 0;

    /* 初始化默认回调函数 */
    p_dev->pfn_txchar_get    = (int (*) (void *, char*))__uart_dummy_callback;
    p_dev->txget_arg         = NULL;
    p_dev->pfn_rxchar_put    = (int (*) (void *, char ))__uart_dummy_callback;
    p_dev->rxput_arg         = NULL;
    p_dev->pfn_err           =
                     (int (*) (void *, int, void*, int))__uart_dummy_callback;

    p_dev->err_arg           = NULL;

    p_dev->other_int_enable = p_devinfo->other_int_enable &
                                  ~(AMHW_UART_INT_C2_TCIE | 
                                  AMHW_UART_INT_C2_IRIE   |
                                  AMHW_UART_INT_C2_ILIE   |
                                  AMHW_UART_INT_C2_TIE);

    /* 获取串口数据长度配置选项 */
    tmp = p_devinfo->cfg_flags;
    tmp = (tmp >> 4) & 0x01;

    switch (tmp) {

    case 0:
        p_dev->options |= AM_UART_CS7;
        break;

    case 1:
        p_dev->options |= AM_UART_CS8;
        break;

    default:
        p_dev->options |= AM_UART_CS8;
        break;
    }

    /* 获取串口检验方式配置选项 */
    tmp = p_devinfo->cfg_flags;
    tmp = (tmp >> 0) & 0x03;

    if (tmp == 2) {
        p_dev->options |= AM_UART_PARENB;
    } else if (tmp == 3) {
        p_dev->options |= (AM_UART_PARENB | AM_UART_PARODD);
    }else{
    }
    /* 获取串口停止位配置选项 */
    if (p_devinfo->cfg_flags & AMHW_UART_BDH_SBNS_STOP_2) {
        p_dev->options |= AM_UART_STOPB;
    }else{

    }
    __uart_opt_set (p_dev, p_dev->options);

    /* 设置波特率 */
    p_dev->baud_rate = amhw_uart_baudrate_set(p_hw_uart,
                                              amdr_clk_periph_rate_get((void *)(p_hw_uart)),
                                              p_devinfo->baud_rate);
    /* 默认轮询模式  */
    __uart_mode_set(p_dev, AM_UART_MODE_POLL);

    /* uart使能 */
    amhw_uart_enable(p_hw_uart);

    return &(p_dev->uart_serv);
}

/**
 * \brief 串口去初始化
 *
 * \param[in] p_dev : 指向需控制的串口设备结构体
 *
 */
void amdr_uart_deinit (amdr_uart_dev_t *p_dev)
{
    amhw_uart_t *p_hw_uart   = p_dev->p_devinfo->p_hw_uart;
    p_dev->uart_serv.p_funcs = NULL;
    p_dev->uart_serv.p_drv   = NULL;

    if (p_dev->channel_mode == AM_UART_MODE_INT) {
        /* 默认为轮询模式 */
        __uart_mode_set(p_dev, AM_UART_MODE_POLL);
    }
    /* 关闭串口 */
    amhw_uart_enable(p_hw_uart);

    am_int_disable(p_dev->p_devinfo->inum);
}

/* end of file */
