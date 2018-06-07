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
 * \brief NVIC引脚中断测试例程，按键中断，通过标准层实现
 *
 * - 操作步骤：
 *   1. 将板子J14跳线中KEY-PTC0连接在一起；
 *   2. 通过切换RST按键，观察LE0变化；
 *
 * - 实验现象：
 *   1. 每按一次按键，LED0取反一次。
 *
 * \par 源代码
 * \snippet demo_std_int_keyscan.c src_std_int_keyscan
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-16  xjc, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_std_int_keyscan
 * \copydoc demo_std_int_keyscan.c
 */
 
/** [src_std_int_keyscan] */
#include "ametal.h"
#include "am_board.h"

/** \brief 按钮事件 */
void key_event (void * p_arg)
{
    uint32_t key;
    /* 清中断标志 */
    amhw_port_pin_isf_clr (AMHW_PORT, PIOC_0);

    key = am_int_cpu_lock();
    am_led_toggle(0);
    am_int_cpu_unlock(key);
}

/** \brief 按键初始化 */
void key_init (void)
{
    am_gpio_pin_cfg(PIOC_0, AM_GPIO_INPUT);
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
    /* 板级初始化  */
    am_board_init();
    
    key_init();

    am_led_toggle(0);

    while (1) {
        
        
    }
}

/** [src_std_int_keyscan] */
 
/* end of file */

