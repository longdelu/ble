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
 * \brief TPMʵ�ֱ�׼��ʱ�����̣�ͨ����׼�ӿ�ʵ��
 *
 * - ʵ������
 *   1. ��ʱ��0��20Hz�ж�һ�Σ�LEDһ��һ��ĵ�Ƶ��Ϊ10Hz����0.05s,��0.05s��
 *
 * \par Դ����
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
 * \brief  ��ʱ��0�ص�����
 * \param[in] p_arg : �û��Զ����������am_timer_callback_set() �������ݡ�
 * \return  ��
 */
void tpm0_timing_callback (void *p_arg)
{
   am_led_toggle(LED0);
}

/**
 * \brief ���������
 */
int main (void)
{
    am_timer_handle_t tpm0_timing_handle;
    am_timer_info_t   tpm0_timing_info;

    
    /* �弶��ʼ�� */
    am_board_init();
    
    /* ƽ̨��ʼ�� */
    amhw_plfm_tpm0_timing_init();
    
    AM_DBG_INFO("The TPM demo for standard timing service\r\n");
    AM_DBG_INFO("The led toggle in 20Hz \r\n");
    
    /* ��ʼ����ʱ��0Ϊ��ʱ���� */
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
     * �趨�ж�Ƶ��20Hz
     */
    am_timer_enable(tpm0_timing_handle, 0, tpm0_timing_info.clk_frequency / 20);

    while (1) {
        am_mdelay(50);
        am_led_toggle(LED1);
    }
}

/** [src_std_tpm_timer] */

/* end of file */
