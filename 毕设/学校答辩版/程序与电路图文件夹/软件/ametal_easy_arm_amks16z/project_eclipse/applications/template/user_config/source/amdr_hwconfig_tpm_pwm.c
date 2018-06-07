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
 * \brief KS16z TPM ����PWM��� �û������ļ�
 * \sa amdr_hwconfig_tpm_pwm.c
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-10-20  cuishengen, first implementation.
 * \endinternal
 */

#include "amdr_hwconfig_tpm_pwm.h"

/**
 * \addtogroup amdr_if_hwconfig_src_tpm_pwm
 * \copydoc amdr_hwconfig_tpm_pwm.c
 * @{
 */

/*******************************************************************************
 * TPM0 ����
 ******************************************************************************/

/** \brief TPM0����PWM���ܵ�����������Ϣ�б� */
amdr_tpm_pwm_ioinfo_t __g_tpm0_pwm_ioinfo_list[] = {
    {PIOD_0, PIOD_0_TPM0_CH0, PIOD_0_GPIO | AM_GPIO_INPUT},  /**< \brief ͨ��0 */
    {PIOD_1, PIOD_1_TPM0_CH1, PIOD_1_GPIO | AM_GPIO_INPUT},  /**< \brief ͨ��1 */
    {PIOD_2, PIOD_2_TPM0_CH2, PIOD_2_GPIO | AM_GPIO_INPUT},  /**< \brief ͨ��2 */
    {PIOD_3, PIOD_3_TPM0_CH3, PIOD_3_GPIO | AM_GPIO_INPUT},  /**< \brief ͨ��3 */
    {PIOD_4, PIOD_4_TPM0_CH4, PIOD_4_GPIO | AM_GPIO_INPUT},  /**< \brief ͨ��4 */
    {PIOD_5, PIOD_5_TPM0_CH5, PIOD_5_GPIO | AM_GPIO_INPUT},  /**< \brief ͨ��5 */
};

/** \brief TPM0����PWM�豸��Ϣ */
const amdr_tpm_pwm_devinfo_t  g_tpm0_pwm_devinfo = {
    AMHW_TPM0,                  /**< \brief ָ��TPM0�Ĵ������ָ�� */
    6,                          /**< \brief 6��PWM���ͨ��        */
    &__g_tpm0_pwm_ioinfo_list[0],
};

/** \brief TPM0����PWM�豸���� */
amdr_tpm_pwm_dev_t  g_tpm0_pwm_dev;

/** \brief TPM0����PWM��� ƽ̨��ʼ�� */
void amhw_plfm_tpm0_pwm_init (void)
{
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_TPM0);
    amhw_sim_tpm_src_set(AMHW_SIM_TPMSRC_PLLFLLCLK);
}

/** \brief ���TPM0ƽ̨��ʼ�� */
void amhw_plfm_tpm0_pwm_deinit (void)
{
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_TPM0);
}

/*******************************************************************************
 * TPM1 ����
 ******************************************************************************/

/** \brief TPM1����PWM���ܵ�����������Ϣ�б� */
amdr_tpm_pwm_ioinfo_t __g_tpm1_pwm_ioinfo_list[] = {
    {PIOE_20, PIOE_20_TPM1_CH0, PIOE_20_GPIO | AM_GPIO_INPUT},  /**< \brief ͨ��0 */
    {PIOE_21, PIOE_21_TPM1_CH1, PIOE_21_GPIO | AM_GPIO_INPUT},  /**< \brief ͨ��1 */
};

/** \brief TPM1����PWM�豸��Ϣ */
const amdr_tpm_pwm_devinfo_t  g_tpm1_pwm_devinfo = {
    AMHW_TPM1,                  /**< \brief ָ��TPM1�Ĵ������ָ�� */
    2,                          /**< \brief 2��PWM���ͨ��        */
    &__g_tpm1_pwm_ioinfo_list[0],
};

/** \brief TPM1����PWM�豸���� */
amdr_tpm_pwm_dev_t  g_tpm1_pwm_dev;

/** \brief TPM1����PWM��� ƽ̨��ʼ�� */
void amhw_plfm_tpm1_pwm_init (void)
{
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_TPM1);
    amhw_sim_tpm_src_set(AMHW_SIM_TPMSRC_PLLFLLCLK);
}

/** \brief ���TPM1ƽ̨��ʼ�� */
void amhw_plfm_tpm1_pwm_deinit (void)
{
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_TPM1);
}

/*******************************************************************************
 * TPM2 ����
 ******************************************************************************/

/** \brief TPM2����PWM���ܵ�����������Ϣ�б� */
amdr_tpm_pwm_ioinfo_t __g_tpm2_pwm_ioinfo_list[] = {
    {PIOE_22, PIOE_22_TPM2_CH0, PIOE_22_GPIO | AM_GPIO_INPUT},  /**< \brief ͨ��0 */
    {PIOE_23, PIOE_23_TPM2_CH1, PIOE_23_GPIO | AM_GPIO_INPUT},  /**< \brief ͨ��1 */
};

/** \brief TPM2����PWM�豸��Ϣ */
const amdr_tpm_pwm_devinfo_t  g_tpm2_pwm_devinfo = {
    AMHW_TPM2,                  /**< \brief ָ��TPM2�Ĵ������ָ�� */
    2,                          /**< \brief 2��PWM���ͨ��        */
    &__g_tpm2_pwm_ioinfo_list[0],
};

/** \brief TPM2����PWM�豸���� */
amdr_tpm_pwm_dev_t  g_tpm2_pwm_dev;

/** \brief TPM2����PWM��� ƽ̨��ʼ�� */
void amhw_plfm_tpm2_pwm_init (void)
{
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_TPM2);
    amhw_sim_tpm_src_set(AMHW_SIM_TPMSRC_PLLFLLCLK);
}

/** \brief ���TPM2ƽ̨��ʼ�� */
void amhw_plfm_tpm2_pwm_deinit (void)
{
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_TPM2);
}

/**
 * @}
 */

/* end of file */
