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
 * \brief KS16 DAC 用户配置头文件。
 * 
 * \internal
 * \par Modification history
 * - 1.00 14-12-23  hbt, first implementation.
 * \endinternal
 */
 
#ifndef __AMDR_HWCONFIG_DAC_H
#define __AMDR_HWCONFIG_DAC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"

/**
 * \brief DAC 平台初始化。
 * \return 无。 
 */
void amhw_plfm_dac0_init(void);

/**
 * \brief 解除DAC0 平台初始化。
 * \return 无。
 */
void amhw_plfm_dac0_deinit(void);


extern const amdr_dac_devinfo_t g_dac0_devinfo; /**< \brief DAC 设备信息 */
extern amdr_dac_dev_t           g_dac0_dev;     /**< \brief DAC0设备      */

#ifdef __cplusplus
}
#endif

#endif  /* __AMDR_HWCONFIG_DAC_H */

/* end of file */
