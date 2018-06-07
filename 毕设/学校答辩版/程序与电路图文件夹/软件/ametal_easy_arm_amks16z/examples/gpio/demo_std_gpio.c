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
 * \brief GPIO 驱动层引脚输入输出例子
 *
 * 实现现象：程序下载到demo板后，用导线将PTC0接地，demo板上的LED0会闪。
 *
 * \par 源代码
 * \par 源代码
 * \snippet demo_std_gpio.c src_std_gpio
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-16  wxj, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_std_gpio
 * \copydoc demo_std_gpio.c
 */
 
/** [src_std_gpio] */
#include "ametal.h"
#include "am_board.h"

/**
 * \brief 主函数入口
 */
int main (void)
{
    int pin_val=0;
    

    /* 板级初始化  */
    am_board_init();

    /* 设置 PORTC 0号管脚为GPIO功能,输入模式,内部上拉 */
    am_gpio_pin_cfg(PIOE_1, AM_GPIO_INPUT | AM_GPIO_PULLDOWN);

    /* 设置 PORTC 4号管脚为GPIO功能,输出模式 */
    am_gpio_pin_cfg(PIOE_1, AM_GPIO_OUTPUT);

    /* 设置GPIOC 4号管脚为输出为低 */
    am_gpio_set(PIOE_1, 0);
     

    while (1) {

        am_mdelay(500);

        /* 读取 PIOC_0 引脚电平 */
        pin_val = am_gpio_get(PIOC_0);

        if (pin_val == 0) {

            /* 翻转 PIOC_4 输出电平 */
            am_gpio_toggle(PIOE_1);
        }
    }
}

/** [src_std_gpio] */
 
/* end of file */
