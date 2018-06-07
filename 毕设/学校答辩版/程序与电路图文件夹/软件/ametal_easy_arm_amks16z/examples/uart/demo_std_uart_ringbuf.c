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
 * \brief UART环形缓冲区方式下接收发送数据例程，通过标准接口实现
 *
 *
 * - 实验现象：
 *   1. 串口输出"UART interrupt mode(Add ring buffer) test:"；
 *   2. 串口输出接收到的字符串。
 *
 * \par 源代码
 * \snippet demo_std_uart_ringbuf.c src_std_uart_ringbuf
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-27  xym, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_uart_ringbuf
 * \copydoc demo_std_uart_ringbuf.c
 */

/** [src_std_uart_ringbuf] */
#include "ametal.h"
#include "am_board.h"


#define UART1_RX_BUF_SIZE  128  /**< \brief 接收环形缓冲区大小，应该为2^n  */
#define UART1_TX_BUF_SIZE  128  /**< \brief 发送环形缓冲区大小，应该为2^n  */

static char uart1_rxbuf[UART1_RX_BUF_SIZE];
static char uart1_txbuf[UART1_TX_BUF_SIZE];
const uint8_t ch[]={"UART interrupt mode(Add ring buffer) test:\r\n"};
am_uart_ringbuf_dev_t  g_uart1_ringbuf_dev;

/**
 * \brief 主函数入口
 */
int main (void)
{
    uint8_t                  uart1_buf[5]; /**< \brief 数据缓冲区            */
    am_uart_handle_t         uart_handle;  /**< \brief 串口标准服务句柄      */
    am_uart_ringbuf_handle_t handle;       /**< \brief 串口环形缓冲区服务句柄*/

    /* 板级初始化  */
    am_board_init();

    /* 平台初始化 */
    amhw_plfm_uart1_init();

    /* UART初始化    */
    uart_handle = amdr_uart_init(&g_uart1_dev, &g_uart1_devinfo);

    /* UART初始化为环形缓冲区模式 */
    handle = am_uart_ringbuf_init(&g_uart1_ringbuf_dev,
                                   uart_handle,
                                   uart1_rxbuf,
                                   UART1_RX_BUF_SIZE,
                                   uart1_txbuf,
                                   UART1_TX_BUF_SIZE);

    am_uart_ringbuf_send(handle,ch,sizeof(ch));

    while (1) {

        /* 接收一个字符 */
        am_uart_ringbuf_receive(handle, uart1_buf, 1);

        /* 发送刚刚接收的字符 */
        am_uart_ringbuf_send(handle, uart1_buf, 1);

    }
}

/** [src_std_uart_ringbuf] */

/* end of file */
