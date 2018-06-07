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
 * \brief KS16z TPM捕获功能用户配置
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
 * TPM0 设备
 ******************************************************************************/
/**
 * \brief TPM0捕获功能平台初始化
 * \return 无
 */
void amhw_plfm_tpm0_cap_init(void);
    
/**
 * \brief 解除TPM0捕获功能平台初始化
 * \return 无
 */
void amhw_plfm_tpm0_cap_deinit(void);

/** \brief TPM0捕获功能设备信息 */
extern amdr_tpm_cap_dev_t            g_tpm0_cap_dev;
    
/** \brief TPM0捕获功能设备 */
extern const amdr_tpm_cap_devinfo_t  g_tpm0_cap_devinfo;

/*******************************************************************************
 * TPM1 设备
 ******************************************************************************/
/**
 * \brief TPM1捕获功能平台初始化
 * \return 无
 */
void amhw_plfm_tpm1_cap_init(void);

/**
 * \brief 解除TPM1捕获功能平台初始化
 * \return 无
 */
void amhw_plfm_tpm1_cap_deinit(void);

/** \brief TPM1捕获功能设备信息 */
extern amdr_tpm_cap_dev_t            g_tpm1_cap_dev;

/** \brief TPM1捕获功能设备 */
extern const amdr_tpm_cap_devinfo_t  g_tpm1_cap_devinfo;

/*******************************************************************************
 * TPM2 设备
 ******************************************************************************/
/**
 * \brief TPM2捕获功能平台初始化
 * \return 无
 */
void amhw_plfm_tpm2_cap_init(void);

/**
 * \brief 解除TPM1捕获功能平台初始化
 * \return 无
 */
void amhw_plfm_tpm2_cap_deinit(void);

/** \brief TPM2捕获功能设备信息 */
extern amdr_tpm_cap_dev_t            g_tpm2_cap_dev;

/** \brief TPM2捕获功能设备 */
extern const amdr_tpm_cap_devinfo_t  g_tpm2_cap_devinfo;

#ifdef __cplusplus
}
#endif

#endif  /* __AMDR_HWCONFIG_TPM_CAP_H */

/* end of file */
