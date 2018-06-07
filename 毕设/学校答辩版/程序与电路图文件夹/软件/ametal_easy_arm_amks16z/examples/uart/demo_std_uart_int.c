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
 * \brief UART中断方式下接收发送数据例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 串口输出"STD example---UART test in interrupt mode:"；
 *   2. 串口输出接收到的字符串。
 *
 * \note 例程没有使用传输缓冲区或则接收缓冲区（像环形缓冲区），只使用了为接收和
 *       发送而设置的用户数据缓冲区，因此每次发送或接收数据要等上次发送或接收数
 *       据完成后才能进行。
 *
 * \par 源代码
 * \snippet demo_std_uart_int.c src_std_uart_int
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-27  xym, first implementation.
 * \endinternal
 */


/**
 * \addtogroup demo_if_std_uart_int
 * \copydoc demo_std_uart_int.c
 */

 /** [src_std_uart_int] */
#include "ametal.h"
#include "am_board.h"

/** \brief 定义串口（中断模式）设备结构体，用于分配相关空间 */
am_uart_int_dev_t  g_uart1_int_dev;

const uint8_t ch[] = {"STD example---UART test in interrupt mode:\r\n"};
/**
 * \brief 主函数入口
 */
int main (void)
{
    uint8_t              uart1_buf[5];  /**< \brief 数据缓冲区           */
    am_uart_handle_t     uart_handle;   /**< \brief 串口标准服务句柄     */
    am_uart_int_handle_t handle;        /**< \brief 串口终端标准服务句柄 */

    /* 板级初始化  */
    am_board_init();

    /* 平台初始化 */
    amhw_plfm_uart1_init();

    /* UART初始化 */
    uart_handle = amdr_uart_init(&g_uart1_dev, &g_uart1_devinfo);

    /* UART初始化为中断模式 */
    handle      = am_uart_int_init(&g_uart1_int_dev, uart_handle);

    am_uart_int_send(handle,ch,sizeof(ch));

    while (1) {
        /* 接收一个字符 */
        am_uart_int_receive(handle, uart1_buf, 1);

        /* 发送刚刚接收的字符 */
        am_uart_int_send(handle, uart1_buf, 1);
    }
}

 /** [src_std_uart_int] */

/* end of file */
