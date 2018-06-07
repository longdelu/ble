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
 * \brief KS16z TPM ��ʱ�����û������ļ�
 * \sa amdr_hwconfig_tpm_timing.c
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  cuishengen, first implementation.
 * \endinternal
 */

#include "amdr_hwconfig_tpm_timing.h"

/**
 * \addtogroup amdr_if_hwconfig_src_tpm_timing
 * \copydoc amdr_hwconfig_tpm_timing.c
 * @{
 */

/*******************************************************************************
 * TPM0 ����
 ******************************************************************************/

/** \brief TPM0 �豸��Ϣ */
const amdr_tpm_timing_devinfo_t  g_tpm0_timing_devinfo = {
    AMHW_TPM0,                  /**< \brief ָ��TPM�Ĵ������ָ��  */
    INUM_TPM0,                  /**< \brief TPM0�жϱ��           */
};

/** \brief TPM0 �豸����  */
amdr_tpm_timing_dev_t  g_tpm0_timing_dev;

/** \brief TPM0 ƽ̨��ʼ�� */
void amhw_plfm_tpm0_timing_init (void)
{
    amhw_sim_tpm_src_set(AMHW_SIM_TPMSRC_PLLFLLCLK);
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_TPM0);
}

/** \brief ��� TPM0 ƽ̨��ʼ�� */
void amhw_plfm_tpm0_timing_deinit (void)
{
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_TPM0);
}

/*******************************************************************************
 * TPM1 ����
 ******************************************************************************/

/** \brief TPM1 �豸��Ϣ */
const amdr_tpm_timing_devinfo_t  g_tpm1_timing_devinfo = {
    AMHW_TPM1,                  /**< \brief ָ��TPM�Ĵ������ָ��  */
    INUM_TPM1,                  /**< \brief TPM1�жϱ��           */
};

/** \brief TPM1 �豸����  */
amdr_tpm_timing_dev_t  g_tpm1_timing_dev;

/** \brief TPM1 ƽ̨��ʼ�� */
void amhw_plfm_tpm1_timing_init (void)
{
    amhw_sim_tpm_src_set(AMHW_SIM_TPMSRC_PLLFLLCLK);
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_TPM1);
}

/** \brief ��� TPM1 ƽ̨��ʼ�� */
void amhw_plfm_tpm1_timing_deinit (void)
{
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_TPM1);
}

/*******************************************************************************
 * TPM2 ����
 ******************************************************************************/

/** \brief TPM2 �豸��Ϣ */
const amdr_tpm_timing_devinfo_t  g_tpm2_timing_devinfo = {
    AMHW_TPM2,                  /**< \brief ָ��TPM�Ĵ������ָ��  */
    INUM_TPM2,                  /**< \brief TPM2�жϱ��           */
};

/** \brief TPM2 �豸����  */
amdr_tpm_timing_dev_t  g_tpm2_timing_dev;

/** \brief TPM2 ƽ̨��ʼ�� */
void amhw_plfm_tpm2_timing_init (void)
{
    amhw_sim_tpm_src_set(AMHW_SIM_TPMSRC_PLLFLLCLK);
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_TPM2);
}

/** \brief ��� TPM1 ƽ̨��ʼ�� */
void amhw_plfm_tpm2_timing_deinit (void)
{
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_TPM2);
}

/**
 * @}
 */

/* end of file */
