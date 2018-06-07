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
 * \brief KS16Z TPM PWM功能用户配置文件
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
 * TPM0 设备
 ******************************************************************************/

/** \brief TPM0 PWM功能的设备信息 */
extern amdr_tpm_pwm_dev_t            g_tpm0_pwm_dev;

/** \brief TPM0 PWM功能设备 */
extern const amdr_tpm_pwm_devinfo_t  g_tpm0_pwm_devinfo;

/**
 * \brief TPM0 PWM功能的平台初始化
 * \return 无
 */
void amhw_plfm_tpm0_pwm_init(void);

/**
 * \brief 解除TPM0 PWM功能的平台初始化
 * \return 无
 */
void amhw_plfm_tpm0_pwm_deinit(void);

/*******************************************************************************
 * TPM1 设备
 ******************************************************************************/
/** \brief TPM1 PWM功能的设备信息 */
extern amdr_tpm_pwm_dev_t            g_tpm1_pwm_dev;

/** \brief TPM1 PWM功能设备 */
extern const amdr_tpm_pwm_devinfo_t  g_tpm1_pwm_devinfo;

/**
 * \brief TPM1 PWM功能的平台初始化
 * \return 无
 */
void amhw_plfm_tpm1_pwm_init(void);

/**
 * \brief 解除TPM1 PWM功能的平台初始化
 * \return 无
 */
void amhw_plfm_tpm1_pwm_deinit(void);

/*******************************************************************************
 * TPM2 设备
 ******************************************************************************/
/** \brief TPM2 PWM功能的设备信息 */
extern amdr_tpm_pwm_dev_t            g_tpm2_pwm_dev;

/** \brief TPM2 PWM功能设备 */
extern const amdr_tpm_pwm_devinfo_t  g_tpm2_pwm_devinfo;

/**
 * \brief TPM2 PWM功能的平台初始化
 * \return 无
 */
void amhw_plfm_tpm2_pwm_init(void);

/**
 * \brief 解除TPM2 PWM功能的平台初始化
 * \return 无
 */
void amhw_plfm_tpm2_pwm_deinit(void);


#ifdef __cplusplus
}
#endif

#endif  /* __AMDR_HWCONFIG_TPM_PWM_H */

/* end of file */
