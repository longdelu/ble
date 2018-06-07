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
 * \brief LPTMRʵ�ֱ�׼��ʱ�����̣�ͨ����׼�ӿ�ʵ��
 *
 * - ʵ������
 *   1. LED0��10Hz��Ƶ����˸��
 *
 * \note LPTMRʱ��Դ��4��ѡ����amdr_hwconfig_lptmr_timing.c�н������ã�ѡ��ͬ��С
 *       ��ʱ��Դ����ʱ�ķ�Χ�;��ȴ��ڲ��
 *
 * \par Դ����
 * \snippet demo_std_lptmr_timer.c src_std_lptmr_timer
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  cuishengen, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_std_lptmr_timer
 * \copydoc demo_std_lptmr_timer.c
 */
 
/** [src_std_lptmr_timer] */
#include "ametal.h"
#include "am_board.h"

/** 
 * \brief  ��ʱ��0�ص�����
 * \param[in] p_arg : �û��Զ����������am_timer_callback_set() �������ݡ�
 * \return  ��
 */
void lptmr_timing_callback (void *p_arg)
{
   am_led_toggle(LED0);
}

/**
 * \brief ���������
 */
int main (void)
{
    am_timer_handle_t lptmr_timing_handle;
    am_timer_info_t   lptmr_timing_info;
    
    /* �弶��ʼ�� */
    am_board_init();
    
    /* ƽ̨��ʼ�� */
    amhw_plfm_lptmr_timing_init();
    
    AM_DBG_INFO("The LPTMR demo for standard timing service\r\n");
    AM_DBG_INFO("The led toggle in 20Hz \r\n");
    
    /* ��ʼ����ʱ��LPTMR */
    lptmr_timing_handle = amdr_lptmr_timing_init(&g_lptmr_timing_dev, 
                                              &g_lptmr_timing_devinfo); 
    am_timer_connect(lptmr_timing_handle);
    
    am_timer_info_get(lptmr_timing_handle, &lptmr_timing_info);
    
    AM_DBG_INFO("The timer size is : %d-bit \r\n", 
                    AM_TIMER_SIZE_GET(lptmr_timing_info.features));

    AM_DBG_INFO("The timer clk_frequency: %d Hz\r\n",
                    lptmr_timing_info.clk_frequency);

    am_timer_callback_set(lptmr_timing_handle, 0, lptmr_timing_callback, NULL);
    
    /*
     * �趨�ж�Ƶ��20Hz
     */
    am_timer_enable(lptmr_timing_handle, 0, lptmr_timing_info.clk_frequency / 20);

    while (1) {
        am_mdelay(50);
        am_led_toggle(LED1);

    }
}

/** [src_std_lptmr_timer] */

/* end of file */
