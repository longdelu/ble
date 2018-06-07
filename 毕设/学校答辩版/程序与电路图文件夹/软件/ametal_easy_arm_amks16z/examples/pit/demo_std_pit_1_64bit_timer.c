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
 * \brief PITʵ�ֱ�׼��ʱ�����̣�ͨ����׼�ӿ�ʵ��
 *
 * - ʵ������
 *   1. LED��0.5Hz��Ƶ����˸��
 *
 * \note ����PIT����ʱ��ԴΪ24M,��32λ��ʱ����ɼ�ʱ 
 *           0xFFFFFFFF / 20M = 179�룬����64λ��ʱ����ɶ�����ʱ�䡣
 *
 * \par Դ����
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
 * \brief  ��ʱ��0�ص�����
 * \param[in] p_arg : �û��Զ����������am_timer_callback_set() �������ݡ�
 * \return  ��
 */
void pit_timing_callback (void *p_arg)
{
   am_led_toggle(LED0);
}

/**
 * \brief ���������
 */
int main (void)
{
    am_timer_handle_t pit_timing_handle;
    am_timer_info_t   pit_timing_info;

    
    /* �弶��ʼ�� */
    am_board_init();
    
    /* ƽ̨��ʼ�� */
    amhw_plfm_pit0_timing_init();
    
    AM_DBG_INFO("The PIT demo for standard timing service\r\n");
    AM_DBG_INFO("The led toggle in 10Hz \r\n");
    
    /* ��ʼ����ʱ��0Ϊ��ʱ���� */
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
     * �趨�ж�Ƶ��1/2=0.5Hz,���ó�64λ��ʱ��ʱ��ͨ����������
     */
    am_timer_enable64(pit_timing_handle, 0, (uint64_t)pit_timing_info.clk_frequency * 2);

    while (1) {
        AM_DBG_INFO("The applicaion is running\r\n");
        am_mdelay(500);
    }
}

/** [src_std_pit_1_64bit_timer] */

/* end of file */
