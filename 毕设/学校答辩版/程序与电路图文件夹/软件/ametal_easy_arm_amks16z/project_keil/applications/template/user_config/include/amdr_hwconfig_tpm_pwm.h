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
 * \brief KS16Z TPM PWM�����û������ļ�
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-21  cuishengen, first implementation.
 * \endinternal
 */

#ifndef __AMDR_HWCONFIG_TPM_PWM_H
#define __AMDR_HWCONFIG_TPM_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"


/*******************************************************************************
 * TPM0 �豸
 ******************************************************************************/

/** \brief TPM0 PWM���ܵ��豸��Ϣ */
extern amdr_tpm_pwm_dev_t            g_tpm0_pwm_dev;

/** \brief TPM0 PWM�����豸 */
extern const amdr_tpm_pwm_devinfo_t  g_tpm0_pwm_devinfo;

/**
 * \brief TPM0 PWM���ܵ�ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_tpm0_pwm_init(void);

/**
 * \brief ���TPM0 PWM���ܵ�ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_tpm0_pwm_deinit(void);

/*******************************************************************************
 * TPM1 �豸
 ******************************************************************************/
/** \brief TPM1 PWM���ܵ��豸��Ϣ */
extern amdr_tpm_pwm_dev_t            g_tpm1_pwm_dev;

/** \brief TPM1 PWM�����豸 */
extern const amdr_tpm_pwm_devinfo_t  g_tpm1_pwm_devinfo;

/**
 * \brief TPM1 PWM���ܵ�ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_tpm1_pwm_init(void);

/**
 * \brief ���TPM1 PWM���ܵ�ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_tpm1_pwm_deinit(void);

/*******************************************************************************
 * TPM2 �豸
 ******************************************************************************/
/** \brief TPM2 PWM���ܵ��豸��Ϣ */
extern amdr_tpm_pwm_dev_t            g_tpm2_pwm_dev;

/** \brief TPM2 PWM�����豸 */
extern const amdr_tpm_pwm_devinfo_t  g_tpm2_pwm_devinfo;

/**
 * \brief TPM2 PWM���ܵ�ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_tpm2_pwm_init(void);

/**
 * \brief ���TPM2 PWM���ܵ�ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_tpm2_pwm_deinit(void);


#ifdef __cplusplus
}
#endif

#endif  /* __AMDR_HWCONFIG_TPM_PWM_H */

/* end of file */
