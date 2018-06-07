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
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  cuishengen, first implementation.
 * \endinternal
 */
 
#ifndef __AMDR_HWCONFIG_TPM_TIMING_H
#define __AMDR_HWCONFIG_TPM_TIMING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"

/*******************************************************************************
 * TPM0 �豸
 ******************************************************************************/
/** \brief TPM0��ʱ�����ܵ��豸��Ϣ */
extern amdr_tpm_timing_dev_t            g_tpm0_timing_dev;
    
/** \brief TPM0��ʱ�������豸 */
extern const amdr_tpm_timing_devinfo_t  g_tpm0_timing_devinfo;

/**
 * \brief TPM0��ʱ������ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_tpm0_timing_init(void);
    
/**
 * \brief ���TPM0��ʱ�����ܵ�ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_tpm0_timing_deinit(void);

/*******************************************************************************
 * TPM1 �豸
 ******************************************************************************/
/** \brief TPM1��ʱ�����ܵ��豸��Ϣ */
extern amdr_tpm_timing_dev_t            g_tpm1_timing_dev;

/** \brief TPM1��ʱ�������豸 */
extern const amdr_tpm_timing_devinfo_t  g_tpm1_timing_devinfo;

/**
 * \brief TPM1��ʱ������ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_tpm1_timing_init(void);

/**
 * \brief ���TPM1��ʱ�����ܵ�ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_tpm1_timing_deinit(void);

/*******************************************************************************
 * TPM2 �豸
 ******************************************************************************/
/** \brief TPM2��ʱ�����ܵ��豸��Ϣ */
extern amdr_tpm_timing_dev_t            g_tpm2_timing_dev;

/** \brief TPM2��ʱ�������豸 */
extern const amdr_tpm_timing_devinfo_t  g_tpm2_timing_devinfo;

/**
 * \brief TPM2��ʱ������ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_tpm2_timing_init(void);

/**
 * \brief ���TPM2��ʱ�����ܵ�ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_tpm2_timing_deinit(void);


#ifdef __cplusplus
}
#endif

#endif  /* __AMDR_HWCONFIG_TPM_TIMING_H */

/* end of file */
