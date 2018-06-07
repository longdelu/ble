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
 * \brief TPMʵ�ֲ����ܣ�ͨ��HW��Ľӿ�ʵ��
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
 * \snippet demo_hw_tpm_cap.c src_hw_tpm_cap
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  cuishengen, first implementation.
 * \endinternal
 */ 
 
/**
 * \addtogroup demo_if_hw_tpm_cap
 * \copydoc demo_hw_tpm_cap.c
 */
 
/** [src_hw_tpm_cap] */
#include "ametal.h"
#include "am_board.h"

/** \brief �����ز��� */
#define TPM_CAP_TRIGGER_RISE  1

/** \brief �½��ز��� */
#define TPM_CAP_TRIGGER_FALL  2

/** \brief TPMͨ���� */
#define TPM_CAP_CH_NUM        AMHW_TPM_CH(0)

volatile bool_t   g_flag    = FALSE;    /**< \brief �����־���Ƿ񲶻��µ��¼�   */
volatile uint32_t g_time_ns = 0;        /**< \brief �������ֵ */
uint32_t          g_tpm_clock;          /**< \brief tpm���õ�ʱ��Դ��С */
uint32_t          g_freq;               /**< \brief ���񵽵�Ƶ�� */

void tpm_cap_isr (void *p_arg)
{
    static uint32_t count1;
    static bool_t first    = TRUE;
    amhw_tpm_t   *p_hw_tpm = (amhw_tpm_t *)p_arg;
    uint32_t      cap_val;
    
    if (amhw_tpm_stat_flag_get(p_hw_tpm) & AMHW_TPM_STAT_FLAG(TPM_CAP_CH_NUM)) {
        cap_val = amhw_tpm_ch_value_get(p_hw_tpm, TPM_CAP_CH_NUM);
        if (g_flag == FALSE) {
            if (first == TRUE) {
                count1 = cap_val;
                first = FALSE;
            } else {
                if(cap_val > count1) {
                    g_time_ns = (uint64_t)1000000000 * (cap_val - count1) / g_tpm_clock;
                }
                first = TRUE;
                /* �ñ�־Ϊ�棬����������� */
                g_flag = TRUE;
            } 
        }

        /* ���ͨ����־ */
        amhw_tpm_stat_flag_clear(p_hw_tpm, AMHW_TPM_STAT_FLAG(TPM_CAP_CH_NUM));
    }
}

/**
 * \brief �����ʼ��
 * \param[in] p_tpm : ָ��TPM�Ĵ������ָ��
 * \return ��
 * \note Ĭ������TPM0��ͨ��0���ţ���ѡ������ͨ�������ڴ��޸�
 */
void tpm_cap_init (amhw_tpm_t *p_hw_tpm)
{
    /* �ر�ʱ�ӣ�ֹͣ���� */
    amhw_tpm_clock_mode(p_hw_tpm, AMHW_TPM_CLK_SRC_NONE);

    /* ��������� */
    amhw_tpm_count_clear(p_hw_tpm);

    /* Ԥ��Ƶ���� */
    amhw_tpm_prescale_set(p_hw_tpm, AMHW_TPM_DIVIDED_BY_1);

    /* ��ȡtpmʱ��ԴƵ�ʣ����ں������ */
    g_tpm_clock = amhw_clock_tpm_rate_get();

    /* ������������ */
    am_gpio_pin_cfg(PIOD_0, PIOD_0_TPM0_CH0);
}

/**
 * \brief ��ʼ��һ·PWM�����
 *
 * \param[in] p_tpm    : ָ��TPM�Ĵ������ָ�롣
 * \param[in] cap_num  : �����š�
 * \param[in] cap_edge : �������ѡ�� (#TPM_CAP_TRIGGER_RISE) �� (#TPM_CAP_TRIGGER_FALL)
 *
 * \return  ��
 */
void tpm_cap_chan_config (amhw_tpm_t *p_hw_tpm, uint8_t cap_num, uint8_t cap_edge)
{
    if (cap_edge == TPM_CAP_TRIGGER_RISE) {
        amhw_tpm_ch_mode(p_hw_tpm, cap_num, AMHW_TPM_COUNTING_UP,
                           AMHW_TPM_CHSC_MS(0) | AMHW_TPM_CHSC_EL_SEL(1));
    } else {
        amhw_tpm_ch_mode(p_hw_tpm, cap_num, AMHW_TPM_COUNTING_UP,
                           AMHW_TPM_CHSC_MS(0) | AMHW_TPM_CHSC_EL_SEL(2));
    }
}

void tpm_cap_chan_enable (amhw_tpm_t *p_hw_tpm, uint32_t cap_num)
{
    /* ʹ��ͨ���ж� */
    amhw_tpm_ch_sc_set(p_hw_tpm, cap_num, AMHW_TPM_CHSC_IE);

    /* ��ʱ�ӣ��������� */
    amhw_tpm_clock_mode(p_hw_tpm, AMHW_TPM_CLK_SRC_MODULE);
}

/**
 * \brief ���������
 */
int main (void)
{
    am_pwm_handle_t tpm1_pwm_handle;
    
    /* �弶��ʼ�� */
    am_board_init();

    /* ƽ̨��ʼ����������ʱ�� */
    amhw_plfm_tpm1_pwm_init();

    /* ƽ̨��ʼ����������ʱ�� */
    amhw_plfm_tpm0_cap_init();
    
    AM_DBG_INFO("The TPM demo for CAP service\r\n");
    
    tpm1_pwm_handle = amdr_tpm_pwm_init(&g_tpm1_pwm_dev,
                                           &g_tpm1_pwm_devinfo);
    
    /* TPM1ͨ��0ʹ��PIOE_20���� amhw_timer_cap_config.c ������ */
    am_pwm_config(tpm1_pwm_handle, 0, 500000 / 2, 500000); /* TƵ��Ϊ2KHz */
    am_pwm_enable(tpm1_pwm_handle, 0);
    
    am_int_connect(INUM_TPM0, tpm_cap_isr, (void *)AMHW_TPM0);
    am_int_enable(INUM_TPM0);

    tpm_cap_init(AMHW_TPM0);
    tpm_cap_chan_config(AMHW_TPM0, TPM_CAP_CH_NUM, TPM_CAP_TRIGGER_RISE);
    tpm_cap_chan_enable(AMHW_TPM0, TPM_CAP_CH_NUM);

    while (1) {
        if (g_flag == TRUE) {
            
            g_freq = 1000000000 / g_time_ns;           /* ת����Ƶ�� */
            AM_DBG_INFO("The period is %d ns, The freq is %d Hz \r\n", g_time_ns, g_freq);
            g_flag = FALSE;   
        }
        am_mdelay(200);
        am_led_toggle(LED0);
    }
}
 
/** [src_hw_tpm_cap] */

/* end of file */
