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
 * \brief KS16z TPM 定时功能用户配置文件
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
 * TPM0 设备
 ******************************************************************************/
/** \brief TPM0定时器功能的设备信息 */
extern amdr_tpm_timing_dev_t            g_tpm0_timing_dev;
    
/** \brief TPM0定时器功能设备 */
extern const amdr_tpm_timing_devinfo_t  g_tpm0_timing_devinfo;

/**
 * \brief TPM0定时器功能平台初始化
 * \return 无
 */
void amhw_plfm_tpm0_timing_init(void);
    
/**
 * \brief 解除TPM0定时器功能的平台初始化
 * \return 无
 */
void amhw_plfm_tpm0_timing_deinit(void);

/*******************************************************************************
 * TPM1 设备
 ******************************************************************************/
/** \brief TPM1定时器功能的设备信息 */
extern amdr_tpm_timing_dev_t            g_tpm1_timing_dev;

/** \brief TPM1定时器功能设备 */
extern const amdr_tpm_timing_devinfo_t  g_tpm1_timing_devinfo;

/**
 * \brief TPM1定时器功能平台初始化
 * \return 无
 */
void amhw_plfm_tpm1_timing_init(void);

/**
 * \brief 解除TPM1定时器功能的平台初始化
 * \return 无
 */
void amhw_plfm_tpm1_timing_deinit(void);

/*******************************************************************************
 * TPM2 设备
 ******************************************************************************/
/** \brief TPM2定时器功能的设备信息 */
extern amdr_tpm_timing_dev_t            g_tpm2_timing_dev;

/** \brief TPM2定时器功能设备 */
extern const amdr_tpm_timing_devinfo_t  g_tpm2_timing_devinfo;

/**
 * \brief TPM2定时器功能平台初始化
 * \return 无
 */
void amhw_plfm_tpm2_timing_init(void);

/**
 * \brief 解除TPM2定时器功能的平台初始化
 * \return 无
 */
void amhw_plfm_tpm2_timing_deinit(void);


#ifdef __cplusplus
}
#endif

#endif  /* __AMDR_HWCONFIG_TPM_TIMING_H */

/* end of file */
