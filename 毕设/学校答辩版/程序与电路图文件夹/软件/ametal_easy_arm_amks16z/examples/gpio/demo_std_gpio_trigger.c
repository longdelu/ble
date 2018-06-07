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
 * \brief GPIO 驱动层引脚中断例程
 *
 * 实现现象：程序下载到demo板后，用导线将PTC0接地，然后拔开，demo板上的LED0会闪。
 *
 * \par 源代码
 * \snippet demo_std_gpio_trigger.c src_std_gpio_trigger
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-16  wxj, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_std_gpio_trigger
 * \copydoc demo_std_gpio_trigger.c
 */
 
/** [src_std_gpio_trigger] */
#include "ametal.h"
#include "am_board.h"

/**
 * \brief 引脚中断服务函数
 */
void gpio_isr (void *p_arg)
{
    /* PIOC_4引脚电平翻转 */
    am_gpio_toggle(PIOC_8);
}

/**
 * \brief 主函数入口
 */
int main (void)
{
    /* 板级初始化  */
    am_board_init();

    /* PIOC_4配置为输出 */
    am_gpio_pin_cfg(PIOC_8, PIOC_8_GPIO | AM_GPIO_OUTPUT);

    /* PIOC_0配置为GPIO，用作中断引脚 */
    am_gpio_pin_cfg(PIOC_0, PIOC_0_GPIO);

    /* 配置引脚中断触发方式 */
    am_gpio_trigger_cfg(PIOC_0, AM_GPIO_TRIGGER_RISE);

    /* 连接引脚中断服务函数 */
    am_gpio_trigger_connect(PIOC_0, gpio_isr, (void *)0);

    /* 开启引脚中断 */
    am_gpio_trigger_on(PIOC_0);

    while(1){
        ;
    }
}

/** [src_std_gpio_trigger] */
 
/* end of file */
