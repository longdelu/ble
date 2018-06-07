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
 * \brief TPMʵ�ֱ�׼��ʱ�����̣�ͨ��HW��Ľӿ�ʵ��
 *
 * - ʵ������
 *   1. LED��10Hz��Ƶ����˸��
 *
 *\note TPM��4��ʱ��Դ����ѡ��Ĭ������ʱ��Դ��СΪ20M,���жϵ���СƵ��Ϊ:
 *              fre = 20000000/(128*65535) = 2.38Hz  ��16λ��ʱ�������128��Ƶ��
 *
 * \par Դ����
 * \snippet demo_hw_tpm_timer.c src_hw_tpm_timer
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  cuishengen, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_hw_tpm_timer
 * \copydoc demo_hw_tpm_timer.c
 */
 
/** [src_hw_tpm_timer] */
#include "ametal.h"
#include "am_board.h"

/** 
 * \brief  ��ʱ��0�ص�����
 * \param[in] p_arg : �û��Զ����������am_timer_callback_set() �������ݡ�
 * \return  ��
 */
void tpm0_timing_callback (void *p_arg)
{
    /*
     * �ж��Ƿ��Ƕ�ʱ������ж�
     */
    if (amhw_tpm_stat_flag_get(AMHW_TPM0) & AMHW_TPM_STAT_OV_FLAG ) {

        am_led_toggle(LED0);
        /* ��������־ */
        amhw_tpm_stat_flag_clear(AMHW_TPM0, AMHW_TPM_STAT_OV_FLAG);
    }
}

/**
 * \brief ��ʼ��TPMΪ��ʱ�����ܡ�
 *
 * \param[in] p_tpm : ָ��TPM�Ĵ������ָ��
 * \param[in] freq  : �ж�Ƶ�ʡ�
 *
 * \retval  AM_OK      : ��ʼ���ɹ�
 * \retval  -AM_EINVAL : ��������
 */
int tpm_timer_init (amhw_tpm_t *p_hw_tpm, uint32_t freq)
{
    uint32_t tpm_rate, match, temp;
    uint8_t  pre_real, pre_reg = 0;

    tpm_rate = amhw_clock_tpm_rate_get();
    match    = tpm_rate / freq;

    if (match > 0xffff * 128) {             /* 16λ��ʱ����128��Ƶ */
        return -AM_EINVAL;
    }

    temp = match / 0xffffu + 1;
    /* ֻ֧�ַ�Ƶ��С1,2,4,8...128����ô��ڷ�Ƶ������С����2^n */
    for (pre_real = 1; pre_real < temp; ) {
        pre_reg++;                        /* ����д��Ĵ����ķ�Ƶֵ0,1,2,... */
        pre_real = pre_real << 1;           /* ��Ƶ��2^n */
    }
    match = match / pre_real;
    amhw_tpm_prescale_set(p_hw_tpm, (amhw_tpm_prescale_t)pre_reg);
    amhw_tpm_mod_set(p_hw_tpm, match - 1);

    amhw_tpm_ctrl_set(p_hw_tpm, AMHW_TPM_SC_TOI_EN);        /* ���ж� */
    am_int_connect(INUM_TPM0, tpm0_timing_callback, (void *)0);
    am_int_enable(INUM_TPM0);

    return AM_OK;
}

/**
 * \brief ʹ��TPM��ʱ��(��ʼ����)
 * \param[in] p_tpm : ָ��TPM�Ĵ������ָ��
 * \return ��
 */
void tpm_timer_enable (amhw_tpm_t *p_hw_tpm)
{
    /* �ر�ʱ�ӣ�TPM��ʼ���� */
    amhw_tpm_clock_mode(p_hw_tpm, AMHW_TPM_CLK_SRC_MODULE);
}

/**
 * \brief  ��ֹTPM��ʱ��(ֹͣ����)
 * \param[in] p_hw_tpm : ָ��TPM�Ĵ������ָ��
 * \return  ��
 */
void sct_timer_disable (amhw_tpm_t *p_hw_tpm)
{
    /* �ر�ʱ�ӣ�TPMֹͣ���� */
    amhw_tpm_clock_mode(p_hw_tpm, AMHW_TPM_CLK_SRC_NONE);
}

/**
 * \brief ���������
 */
int main (void)
{
    /* �弶��ʼ�� */
    am_board_init();
    
    /* ƽ̨��ʼ�� */
    amhw_plfm_tpm0_timing_init();
    
    AM_DBG_INFO("The demo for TPM timing:\r\n");
    AM_DBG_INFO("The led toggle in 10Hz \r\n");
  
    /* ʹ��TPM0,�ж�Ƶ��Ϊ10Hz */
    tpm_timer_init(AMHW_TPM0, 10);
    
    /* ��ʼ���� */
    tpm_timer_enable(AMHW_TPM0);

    while (1) {
    }
}

/** [src_hw_tpm_timer] */

/* end of file */
