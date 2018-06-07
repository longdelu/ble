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
 * \brief FGPIO 硬件层引脚输入输出例子
 *
 * 实现现象：程序下载到demo板后，用导线将PTC0接地，demo板上的LED0会闪。
 *
 * \par 源代码
 * \snippet demo_hw_fgpio.c src_hw_fgpio
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-16  wxj, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_hw_fgpio
 * \copydoc demo_hw_fgpio.c
 */
 
/** [src_hw_fgpio] */
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

    /* 设置 PORTC 0,4号管脚为GPIO功能 */
    amhw_port_pin_func_cfg(AMHW_PORT, PIOC_4, 0x1);
    amhw_port_pin_func_cfg(AMHW_PORT, PIOC_0, 0x1);

    /* 管脚上/下拉使能 */
    amhw_port_pin_pull_enable(AMHW_PORT, PIOC_0);

    /* 引脚输入上拉 */
    amhw_port_pin_pull_up(AMHW_PORT, PIOC_0);

    /* 设置GPIOC 4 号管脚初始电平 */
    amhw_fgpio_pin_init_out_low(AMHW_FGPIO, PIOC_4);

    /* 设置FGPIOC 4号管脚为输出方向 */
    amhw_fgpio_pin_dir_output(AMHW_FGPIO, PIOC_4);

    /* 设置FGPIOC 0号管脚为输入方向 */
    amhw_fgpio_pin_dir_input(AMHW_FGPIO, PIOC_0);

    /* 设置FGPIOC 4号管脚为输出为高,8号管脚输出为低 */
    amhw_fgpio_pin_out_high(AMHW_FGPIO, PIOC_4);

    while (1) {

        am_mdelay(1000);

        /* 获取引脚输入电平 */
        pin_val = amhw_fgpio_pin_input_get(AMHW_FGPIO, PIOC_0);

        if (pin_val == 0) {

            /* 翻转引脚输出电平 */
            amhw_fgpio_pin_out_toggle (AMHW_FGPIO, PIOC_4);
        }
    }
}

/** [src_hw_fgpio] */
 
/* end of file */
