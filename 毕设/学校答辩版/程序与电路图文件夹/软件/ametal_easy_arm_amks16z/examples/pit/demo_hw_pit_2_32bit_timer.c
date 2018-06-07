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
 * \brief PIT 32位定时器实现定时功能的例程，通过HW层的接口实现
 *
 * - 实验现象：
 *   1. LED以10Hz的频率闪烁。
 *
 * \par 源代码
 * \snippet demo_hw_pit_2_32bit_timer.c src_hw_pit_2_32bit_timer
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-28  cuishengen, first implementation.
 * \endinternal
 */
 
 
/**
 * \addtogroup demo_if_hw_pit_2_32bit_timer
 * \copydoc demo_hw_pit_2_32bit_timer.c
 */
 
/** [src_hw_pit_2_32bit_timer] */
#include "ametal.h"
#include "am_board.h"


/** 
 * \brief  PIT0中断服务函数
 * \param[in]  p_arg : 用户自定义参数，由 am_int_connect() 函数传递。
 * \return  无
 */
void pit0_irq_handler(void *p_arg)
{
    /* 通道0产生中断 */
    if (amhw_pit_timerout_flag_check(AMHW_PIT0, AMHW_PIT_CH(0)) == TRUE ) {
        
        am_led_toggle(LED0);
        
        /* 清除标志 */
        amhw_pit_timerout_flag_clr(AMHW_PIT0, AMHW_PIT_CH(0));
    }
}

/** 
 * \brief 初始化PIT为定时器功能。
 *
 * \param[in] p_pit : 指向PIT寄存器块的指针
 * \param[in] freq  : 中断频率。
 *
 * \return 无
 */
void pit_timer_init (amhw_pit_t *p_hw_pit, uint32_t freq)
{
    uint32_t pit_clk, load_val;

    /* 计算装载值 */
    pit_clk = amhw_clock_periph_freq_get(p_hw_pit);
    load_val = pit_clk / freq;

    /* 使能PIT模块 */
    amhw_pit_module_enable(p_hw_pit);
    
    /* 设置装载值 */
    amhw_pit_ldval_set(p_hw_pit, AMHW_PIT_CH(0), load_val);
    
    /* 打开中断并使能计数 */
    amhw_pit_timer_ctrl_set(p_hw_pit,
                            AMHW_PIT_CH(0),
                            AMHW_PIT_TCTL_TI_EN | AMHW_PIT_TCTL_TMR_EN);
   
    am_int_connect(INUM_PIT, pit0_irq_handler, (void *)0);
    am_int_enable(INUM_PIT);
}


/** 
 * \brief 使能PIT为定时器(开始计数)
 * \param[in] p_pit : 指向PIT寄存器块的指针
 * \return 无
 */
void pit_timer_enable (amhw_pit_t *p_hw_pit)
{
    /* 开始运行 */
    amhw_pit_timer_ctrl_set(p_hw_pit,
                            AMHW_PIT_CH(0),
                            AMHW_PIT_TCTL_TMR_EN);
}

/** 
 * \brief  禁止PIT为定时器(停止计数)
 * \param[in] p_pit : 指向PIT寄存器块的指针
 * \return  无
 */
void pit_timer_disable (amhw_pit_t *p_hw_pit)
{
    /* 置停止位，PIT停止运行 */
    amhw_pit_timer_ctrl_clear(p_hw_pit,
                              AMHW_PIT_CH(0),
                              AMHW_PIT_TCTL_TMR_EN);
}

/**
 * \brief 主函数入口
 */
int main (void)
{  
    /* 板级初始化 */
    am_board_init();

    /* 平台初始化 */
    amhw_plfm_pit0_timing_init();

    AM_DBG_INFO("The demo for PIT timing:\r\n");
    AM_DBG_INFO("The led toggle in 10Hz \r\n");

    /* 使用PIT实现定时功能，中断频率为10Hz */
    pit_timer_init(AMHW_PIT0, 10);
    
    /* 使能PIT计数 */
    pit_timer_enable(AMHW_PIT0);
    
    while (1) {

    }
}

/** [src_hw_pit_2_32bit_timer] */

/* end of file */
