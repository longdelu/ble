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
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief 通过按键异步唤醒处于睡眠状态的MCU
 *
 * - 操作步骤：
 *   1. PIOA_1引脚连接PC串口的TXD；
 *   2. PIOA_2引脚连接PC串口的RXD；
 *   3. 将板子J14跳线中KEY-PTC0连接在一起；
 *   4. 当串口助手打印完“sleep...”之后，进去睡眠模式，切换RST按键可以退出睡眠模式；
 *
 * - 实验现象：
 *   1. 串口打印“sleep...”后，进入深度睡眠模式，muc耗电变低。
 *   2. RST按键按下后，串口继续打印“wake up...”，mcu被激活。
 *
 * \par 源代码
 * \snippet demo_hw_awic_pint.c src_hw_awic_pint
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-16  xjc, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_hw_awic_pint
 * \copydoc demo_hw_awic_pint.c
 */
 
/** [src_hw_awic_pint] */
#include "ametal.h"
#include "am_board.h"
/** \brief 定义串口（中断模式）设备结构体，用于分配相关空间 */
am_uart_int_dev_t  g_uart1_int_dev;


/** \brief 按钮事件 */
void key_event (void * p_arg)
{
    /* 清中断标志 */
    amhw_port_pin_isf_clr (AMHW_PORT, PIOC_0);

    am_led_toggle(0);
}

/** \brief 按键初始化 */
void key_init (void)
{
    am_gpio_pin_cfg(PIOC_0, AM_GPIO_INPUT | AM_GPIO_PULLUP);
    am_gpio_trigger_cfg(PIOC_0, AM_GPIO_TRIGGER_FALL);

    am_int_disable(INUM_PORTC_PORTD);
    am_int_connect(INUM_PORTC_PORTD, key_event, NULL);
    am_int_enable(INUM_PORTC_PORTD);
}
/**
 * \brief 主函数入口
 */
int main (void)
{  
    int step = 0;

    /* 板级初始化  */
    am_board_init();

    key_init();

    printf("AWIC Demo:\r\n");

    while (1) {
        switch (step) {

        case 0:
            step = 1;
            printf("runing.\r\n");
            break;

        case 1:
            step = 2;
            printf("runing..\r\n");
            break;

        case 2:
            step = 0;
            printf("runing...\r\n");
            printf("sleep...\r\n");
        
            am_udelay(300);
        
            if (amdr_pmu_mode_into(AMDR_PMU_MODE_VLPS) != AM_OK) {
                printf("Enter STOP_MODE Failed !\r\n");
            }           
            am_udelay(50);
        
            printf("wake up...\r\n");
            break;

        default:break;
        }
        am_mdelay(500);
    }
}

/** [src_hw_awic_pint] */
 
/* end of file */

