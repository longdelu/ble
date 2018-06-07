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
 * \brief USART查询方式下接收发送数据例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. UART1_RX->PTE1  UART1_TX->PTE0 
 *   2. 串口输出"USART test in polling mode:"；
 *   3. 串口输出接收到的字符串。
 *
 * \note 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）
 *
 * \par 源代码
 * \snippet demo_std_uart_polling.c src_std_uart_polling 
 *
 * \internal
 * \par Modification History
 * - 1.00 15-07-06  xym, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_std_uart_polling
 * \copydoc demo_std_uart_polling.c
 */
 
 /** [src_std_uart_polling] */
#include "ametal.h"
#include "am_board.h"
const uint8_t ch[]={"STD-UART test in polling mode:\r\n"};
/**
 * \brief 主函数入口
 */

int main (void)
{
    uint8_t          uart1_buf[5];    /**< \brief 数据缓冲区           */
    am_uart_handle_t handle;          /**< \brief 串口标准服务句柄     */
    /* 板级初始化  */
    am_board_init();
    /* 平台初始化  */
    amhw_plfm_uart1_init();
    /* UART初始化  */
    handle = amdr_uart_init(&g_uart1_dev, &g_uart1_devinfo);

    am_uart_poll_send(handle,ch,sizeof(ch));

    while (1) {

        /* 接收字符 */
        am_uart_poll_receive(handle, uart1_buf, 1);
        /* 发送刚刚接收的字符 */
        am_uart_poll_send(handle, uart1_buf, 1);

    }
}


 /** [src_std_uart_polling] */
 
/* end of file */

