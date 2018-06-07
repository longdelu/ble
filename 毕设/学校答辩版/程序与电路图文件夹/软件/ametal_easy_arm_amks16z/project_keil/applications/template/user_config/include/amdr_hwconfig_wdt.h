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
 * \brief KS16xx WDT 用户配置头文件
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-23  wxj, first implementation.
 * \endinternal
 */

#ifndef __AMDR_HWCONFIG_WDT_H
#define __AMDR_HWCONFIG_WDT_H

#include "ametal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief WDT 平台初始化。
 * \return 无.
 */
void amhw_plfm_wdt_init(void);

/**
 * \brief 解除 WDT平台初始化
 * \return 无.
 */
void amhw_plfm_wdt_deinit(void);

/** \brief WDT 设备信息 */
extern const amdr_wdt_devinfo_t  g_wdt_devinfo;

/** \brief WDT 设备 */
extern amdr_wdt_dev_t            g_wdt_dev;

#ifdef __cplusplus
}
#endif

#endif  /* __AMDR_HWCONFIG_WDT_H */

/* end of file */

