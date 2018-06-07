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
 * \brief WDT 喂狗和复位例子，通过硬件层实现
 *
 * - 实验现象：
 *   1. 看门狗设置超时时间为256ms，延时50ms后喂狗，程序正常执行，
 *      串口打印喂狗信息；
 *   2. 程序执行到延时1s时，喂狗超时，系统复位，程序重新开始执行。
 *
 *
 * \par 源代码
 * \snippet demo_hw_wdt.c src_hw_wdt
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-16  wxj, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_hw_wdt
 * \copydoc demo_hw_wdt.c
 */
 
/** [src_hw_wdt] */
#include "ametal.h"
#include "am_board.h"

/**
 * \brief 主函数入口
 */
int main (void)
{
    am_uart_handle_t uhandle;          /**< \brief 串口标准服务句柄     */

    /* 板级初始化  */
    am_board_init();

    /* 平台初始化  */
    amhw_plfm_uart1_init();

    /* UART初始化  */
    uhandle = amdr_uart_init(&g_uart1_dev, &g_uart1_devinfo);

    /* 配置为普通模式，时钟为内部1KHZ，超时时间256ms */
    amhw_sim_cop_cfg(AMHW_SIM_COP_MODE_NORMAL,
                     AMHW_SIM_COP_CLK_INTERNAL,
                     AMHW_SIM_COP_TIMOUT_VALUE2);

    am_uart_poll_send(uhandle,
                     (uint8_t *)("等待喂狗...\r\n"),
                      sizeof("等待喂狗...\r\n"));

    am_mdelay(50);

    amhw_sim_srvcop_set(0x55);     /* 喂狗，先写0x55 */
    amhw_sim_srvcop_set(0XAA);     /* 喂狗，再写0xAA */

    am_uart_poll_send(uhandle,
                     (uint8_t *)("50ms后喂狗成功！\r\n"),
                      sizeof("50ms后喂狗成功！\r\n"));

    am_uart_poll_send(uhandle,
                     (uint8_t *)("等待喂狗...\r\n"),
                      sizeof("等待喂狗...\r\n"));

    am_mdelay(1000);

    /* 如果喂狗超时，下面语句将不被执行 */
    amhw_sim_srvcop_set(0x55);     /* 喂狗，先写0x55 */
    amhw_sim_srvcop_set(0XAA);     /* 喂狗，再写0xAA */

    am_uart_poll_send(uhandle,
                     (uint8_t *)("1s后喂狗成功！\r\n"),
                      sizeof("1s后喂狗成功！\r\n"));

    while(1) {
        ;
    }
}

/** [src_hw_wdt] */
 
/* end of file */
