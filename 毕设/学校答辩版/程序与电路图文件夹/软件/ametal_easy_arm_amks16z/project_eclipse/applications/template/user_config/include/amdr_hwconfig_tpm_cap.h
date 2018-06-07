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
 * \brief KS16z TPM�������û�����
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  cuishengen, first implementation.
 * \endinternal
 */
 
#ifndef __AMDR_HWCONFIG_TPM_CAP_H
#define __AMDR_HWCONFIG_TPM_CAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"

/*******************************************************************************
 * TPM0 �豸
 ******************************************************************************/
/**
 * \brief TPM0������ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_tpm0_cap_init(void);
    
/**
 * \brief ���TPM0������ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_tpm0_cap_deinit(void);

/** \brief TPM0�������豸��Ϣ */
extern amdr_tpm_cap_dev_t            g_tpm0_cap_dev;
    
/** \brief TPM0�������豸 */
extern const amdr_tpm_cap_devinfo_t  g_tpm0_cap_devinfo;

/*******************************************************************************
 * TPM1 �豸
 ******************************************************************************/
/**
 * \brief TPM1������ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_tpm1_cap_init(void);

/**
 * \brief ���TPM1������ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_tpm1_cap_deinit(void);

/** \brief TPM1�������豸��Ϣ */
extern amdr_tpm_cap_dev_t            g_tpm1_cap_dev;

/** \brief TPM1�������豸 */
extern const amdr_tpm_cap_devinfo_t  g_tpm1_cap_devinfo;

/*******************************************************************************
 * TPM2 �豸
 ******************************************************************************/
/**
 * \brief TPM2������ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_tpm2_cap_init(void);

/**
 * \brief ���TPM1������ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_tpm2_cap_deinit(void);

/** \brief TPM2�������豸��Ϣ */
extern amdr_tpm_cap_dev_t            g_tpm2_cap_dev;

/** \brief TPM2�������豸 */
extern const amdr_tpm_cap_devinfo_t  g_tpm2_cap_devinfo;

#ifdef __cplusplus
}
#endif

#endif  /* __AMDR_HWCONFIG_TPM_CAP_H */

/* end of file */
