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
 * \brief 
 *
 * - 操作步骤：
 *   1. PIOA_1引脚连接PC串口的TXD；
 *   2. PIOA_2引脚连接PC串口的RXD；
 *   3. 外部晶振8MHz时，波特率为115200
 *
 * - 实验现象：
 *   1. LED灯闪动；
 *   2. 串口连续打印语句"The program is running!"。
 *
 * \par 源代码
 * \snippet demo_main.c src_main
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-16  xjc, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_main
 * \copydoc demo_main.c
 */
 
/** [src_main] */
#include "ametal.h"
#include "am_board.h"
#include "am_clk.h"
#include "amks16z_clk_id.h"

/**
 * \brief 主函数入口
 */
int main (void)
{  
    /* 板级初始化  */
    am_board_init();

    while (1) {

        printf("The program is running!\r\n");
        printf("core_clk %d, bus_clk %d",
               am_clk_rate_get(CLK_SYSTEM),
               am_clk_rate_get(CLK_BUS));

        am_mdelay(500);
        am_led_toggle(LED0) ;/* LED0反转 */
    }
}

/** [src_main] */
 
/* end of file */

