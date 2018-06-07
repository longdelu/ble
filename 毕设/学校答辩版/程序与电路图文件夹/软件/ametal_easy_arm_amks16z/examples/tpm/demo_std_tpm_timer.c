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
 * \brief TPM实现标准定时器例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 定时器0以20Hz中断一次，LED一亮一灭的的频率为10Hz，亮0.05s,灭0.05s。
 *
 * \par 源代码
 * \snippet demo_std_tpm_timer.c src_std_tpm_timer
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  cuishengen, first implementation.
 * \endinternal
 */

 
/**
 * \addtogroup demo_if_std_tpm_timer
 * \copydoc demo_std_tpm_timer.c
 */
 
/** [src_std_tpm_timer] */
#include "ametal.h"
#include "am_board.h"

/** 
 * \brief  定时器0回调函数
 * \param[in] p_arg : 用户自定义参数，由am_timer_callback_set() 函数传递。
 * \return  无
 */
void tpm0_timing_callback (void *p_arg)
{
   am_led_toggle(LED0);
}

/**
 * \brief 主函数入口
 */
int main (void)
{
    am_timer_handle_t tpm0_timing_handle;
    am_timer_info_t   tpm0_timing_info;

    
    /* 板级初始化 */
    am_board_init();
    
    /* 平台初始化 */
    amhw_plfm_tpm0_timing_init();
    
    AM_DBG_INFO("The TPM demo for standard timing service\r\n");
    AM_DBG_INFO("The led toggle in 20Hz \r\n");
    
    /* 初始化定时器0为定时功能 */
    tpm0_timing_handle = amdr_tpm_timing_init(&g_tpm0_timing_dev, 
                                              &g_tpm0_timing_devinfo); 
    am_timer_connect(tpm0_timing_handle);
    
    am_timer_info_get(tpm0_timing_handle, &tpm0_timing_info);
    
    AM_DBG_INFO("The timer size is : %d-bit \r\n", 
                    AM_TIMER_SIZE_GET(tpm0_timing_info.features));
                    
    AM_DBG_INFO("The timer clk_frequency: %d Hz\r\n",
                    tpm0_timing_info.clk_frequency);
  
    am_timer_callback_set(tpm0_timing_handle, 0, tpm0_timing_callback, NULL);
    
    /*
     * 设定中断频率20Hz
     */
    am_timer_enable(tpm0_timing_handle, 0, tpm0_timing_info.clk_frequency / 20);

    while (1) {
        am_mdelay(50);
        am_led_toggle(LED1);
    }
}

/** [src_std_tpm_timer] */

/* end of file */
