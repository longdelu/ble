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
 * \brief WDT 喂狗和复位例子，通过驱动层实现
 *
 * - 实验现象：
 *   1. 看门狗设置超时时间为256ms，延时50ms后喂狗，程序正常执行，
 *      串口打印喂狗信息；
 *   2. 程序执行到延时1s时，系统复位，程序重新开始执行。
 *
 *
 * \par 源代码
 * \snippet demo_std_wdt.c src_std_wdt
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-16  wxj, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_std_wdt
 * \copydoc demo_std_wdt.c
 */
 
/** [src_std_wdt] */
#include "ametal.h"
#include "am_board.h"

/**
 * \brief 主函数入口
 */
int main (void)
{
    am_wdt_handle_t  handle = NULL;     /**< \brief 看门狗标准服务句柄 */
    am_wdt_info_t    info;
    am_uart_handle_t uhandle;           /**< \brief 串口标准服务句柄 */

    /* 板级初始化  */
    am_board_init();

    /* 平台初始化  */
    amhw_plfm_uart1_init();

    /* UART初始化  */
    uhandle = amdr_uart_init(&g_uart1_dev, &g_uart1_devinfo);

    /* 初始化WDT */
    handle = amdr_wdt_init(&g_wdt_dev, &g_wdt_devinfo);

    am_wdt_info_get(handle, &info);

    am_wdt_enable(handle, 256);

    am_uart_poll_send(uhandle,
                     (uint8_t *)("等待喂狗...\r\n"),
                      sizeof("等待喂狗...\r\n"));

    am_mdelay(50);
    am_wdt_feed(handle);

    am_uart_poll_send(uhandle,
                     (uint8_t *)("50ms后喂狗成功！\r\n"),
                      sizeof("50ms后喂狗成功！\r\n"));

    am_uart_poll_send(uhandle,
                     (uint8_t *)("等待喂狗...\r\n"),
                      sizeof("等待喂狗...\r\n"));

    am_mdelay(1000);

    /* 如果喂狗超时，下面语句将不被执行 */
    am_wdt_feed(handle);

    am_uart_poll_send(uhandle,
                     (uint8_t *)("1s后喂狗成功！\r\n"),
                      sizeof("1s后喂狗成功！\r\n"));

    while(1) {
        ;
    }
}

/** [src_std_wdt] */
 
/* end of file */
