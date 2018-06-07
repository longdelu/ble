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
 * \brief TPMʵ�ֲ����ܣ�ͨ����׼�ӿ�ʵ��
 *
 * - ʵ������
 *   1. ������ò����ܵõ���PWM�źŵ����ں�Ƶ�ʡ�
 *
 * - ע�⣺
 *   1. TPM1ͨ��PIOE_20�������PWM��
 *   2. TPM0��������ͨ��0ʹ��PIOD_0���ţ�
 *   3. ����PIOE_20���ź�PIOD_0���ţ�ʹ�ò������PWMƵ�ʡ�
 *
 * \par Դ����
 * \snippet demo_std_tpm_cap.c src_std_tpm_cap
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  cuishengen, first implementation.
 * \endinternal
 */ 
 
/**
 * \addtogroup demo_if_std_tpm_cap
 * \copydoc demo_std_tpm_cap.c
 */
 
/** [src_std_tpm_cap] */
#include "ametal.h"
#include "am_board.h"

volatile bool_t   g_flag    = FALSE;    /**< \brief �����־   */
volatile uint32_t g_time_ns = 0;        /**< \brief �������ֵ */

void cap_callback (void *p_arg, unsigned int cap_val)
{
    static uint32_t count1;
    static bool_t   first      = TRUE;
    am_cap_handle_t cap_handle = (am_cap_handle_t)p_arg;
    
    if (g_flag == FALSE) {
    
        if (first == TRUE) {
            count1 = cap_val;
            first  = FALSE;
        } else {
            if(count1 < cap_val) {              /* �������ʱ���ݴ��� */
                am_cap_count_to_time(cap_handle,
                                     0,
                                     count1,
                                     cap_val,
                                     (unsigned int *)&g_time_ns);
            }
            first = TRUE;
            
            /* �ñ�־Ϊ�棬����������� */
            g_flag = TRUE;
        }
    }
}

/**
 * \brief ���������
 */
int main (void)
{
    am_pwm_handle_t tpm1_pwm_handle;
    am_cap_handle_t tpm0_cap_handle;
    uint32_t        freq;
    
    /* �弶��ʼ�� */
    am_board_init();

    /* ƽ̨��ʼ�� */
    amhw_plfm_tpm1_pwm_init();

    amhw_plfm_tpm0_cap_init();
    
    AM_DBG_INFO("The TPM demo for standard CAP service\r\n");
    
    tpm1_pwm_handle = amdr_tpm_pwm_init(&g_tpm1_pwm_dev,
                                           &g_tpm1_pwm_devinfo);
    
    tpm0_cap_handle = amdr_tpm_cap_init(&g_tpm0_cap_dev,
                                          &g_tpm0_cap_devinfo);

    /* TPM1ͨ��0ʹ��PIOE_20���� amhw_timer_cap_config.c ������ */
    am_pwm_config(tpm1_pwm_handle, 0, 500000 / 2, 500000); /* TƵ��Ϊ2KHz */
    am_pwm_enable(tpm1_pwm_handle, 0);
    
    /* TPM0ͨ��0ʹ��PIOD_0���� amhw_timer_cap_config.c ������ */
    am_cap_config(tpm0_cap_handle,
                  0,
                  AM_CAP_TRIGGER_RISE, 
                  cap_callback,
                  tpm0_cap_handle);
    
    am_cap_enable(tpm0_cap_handle, 0);

    while (1) {
        if (g_flag == TRUE) {
            
            freq = 1000000000 / g_time_ns;
            AM_DBG_INFO("The period is %d ns, The freq is %d Hz \r\n", g_time_ns, freq);
            g_flag = FALSE;   
        }
        am_mdelay(200);
        am_led_toggle(LED0);
    }
}
 
/** [src_std_tpm_cap] */

/* end of file */
