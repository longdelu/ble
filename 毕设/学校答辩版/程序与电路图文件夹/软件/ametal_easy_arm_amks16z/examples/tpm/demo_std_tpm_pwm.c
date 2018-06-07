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
 * \brief TPMʵ��PWM��׼�ӿ����̣�ͨ����׼�ӿ�ʵ��
 *
 * - ʵ������
 *   1. PIOD_0(TPM_OUT1)���4kHz��PWM��ռ�ձ�Ϊ50%��
 *   2. PIOD_1(TPM_OUT2)���4kHz��PWM��ռ�ձ�Ϊ25%��
 *
 * \par Դ����
 * \snippet demo_std_tpm_pwm.c src_std_tpm_pwm
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  cuishengen, first implementation.
 * \endinternal
 */ 

/**
 * \addtogroup demo_if_std_tpm_pwm
 * \copydoc demo_std_tpm_pwm.c
 */
 
/** [src_std_tpm_pwm] */
#include "ametal.h"
#include "am_board.h"


/**
 * \brief ���������
 */
int main (void)
{
    am_pwm_handle_t tpm0_pwm_handle;
    
    /* �弶��ʼ�� */
    am_board_init();
    
    /* ƽ̨��ʼ�� */
    amhw_plfm_tpm0_pwm_init();
    
    AM_DBG_INFO("The TPM demo for standard pwm service\r\n");
    AM_DBG_INFO("The PIOD_0(TPM_OUT0) PWM: freq is 4kHz, The duty ratio is 50% \r\n");
    AM_DBG_INFO("The PIOD_1(TPM_OUT1) PWM: freq is 4kHz, The duty ratio is 25% \r\n");
    
    /* ��ʼ����ʱ��0ΪPWM���� */
    tpm0_pwm_handle = amdr_tpm_pwm_init(&g_tpm0_pwm_dev, 
                                        &g_tpm0_pwm_devinfo);

    /* ͨ��0ʹ��PIOD_0���� amdr_hwconfig_tpm_pwm.c ������ */
    am_pwm_config(tpm0_pwm_handle, 0, 250000 / 2, 250000);
    am_pwm_enable(tpm0_pwm_handle, 0);

    /* ͨ��1ʹ��PIOD_1���� amdr_hwconfig_tpm_pwm.c ������ */
    am_pwm_config(tpm0_pwm_handle, 1, 250000 / 4, 250000);
    am_pwm_enable(tpm0_pwm_handle, 1);
    
    while (1) {
        
        am_led_toggle(LED0);
        am_mdelay(500);
    }
}

/** [src_std_tpm_pwm] */

/* end of file */
