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
 * \brief PIT实现标准定时器例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. LED以0.5Hz的频率闪烁。
 *
 * \note 假如PIT计数时钟源为24M,则32位定时器最长可计时 
 *           0xFFFFFFFF / 20M = 179秒，采用64位定时器则可定更长时间。
 *
 * \par 源代码
 * \snippet demo_std_pit_1_64bit_timer.c src_std_pit_1_64bit_timer
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  cuishengen, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_std_pit_1_64bit_timer
 * \copydoc demo_std_pit_1_64bit_timer.c
 */
 
/** [src_std_pit_1_64bit_timer] */
#include "ametal.h"
#include "am_board.h"

/** 
 * \brief  定时器0回调函数
 * \param[in] p_arg : 用户自定义参数，由am_timer_callback_set() 函数传递。
 * \return  无
 */
void pit_timing_callback (void *p_arg)
{
   am_led_toggle(LED0);
}

/**
 * \brief 主函数入口
 */
int main (void)
{
    am_timer_handle_t pit_timing_handle;
    am_timer_info_t   pit_timing_info;

    
    /* 板级初始化 */
    am_board_init();
    
    /* 平台初始化 */
    amhw_plfm_pit0_timing_init();
    
    AM_DBG_INFO("The PIT demo for standard timing service\r\n");
    AM_DBG_INFO("The led toggle in 10Hz \r\n");
    
    /* 初始化定时器0为定时功能 */
    pit_timing_handle = amdr_pit_timing_init(&g_pit0_timing_dev,
                                              &g_pit0_timing_devinfo);
    am_timer_connect(pit_timing_handle);
    
    am_timer_info_get(pit_timing_handle, &pit_timing_info);
    
    AM_DBG_INFO("The timer size is : %d-bit \r\n", 
                    AM_TIMER_SIZE_GET(pit_timing_info.features));
                    
    AM_DBG_INFO("The timer clk_frequency: %d Hz\r\n",
                    pit_timing_info.clk_frequency);
  
    am_timer_callback_set(pit_timing_handle, 0, pit_timing_callback, NULL);
    
    /*
     * 设定中断频率1/2=0.5Hz,设置成64位定时器时，通道号无意义
     */
    am_timer_enable64(pit_timing_handle, 0, (uint64_t)pit_timing_info.clk_frequency * 2);

    while (1) {
        AM_DBG_INFO("The applicaion is running\r\n");
        am_mdelay(500);
    }
}

/** [src_std_pit_1_64bit_timer] */

/* end of file */
