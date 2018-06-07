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
 * \brief KS16XX CLK 用户配置头文件。
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-22 lnk, first implementation.
 * \endinternal
 */

#ifndef __AMDR_HWCONFIG_CLK_H
#define __AMDR_HWCONFIG_CLK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "amdr_clk.h"

/**
 * \brief CLK 平台初始化
 * \return 无
 */
void amhw_plfm_clk_init (void);

/**
 * \brief CLK 平台去初始化
 * \return 无
 */
void amhw_plfm_clk_deinit (void);

extern const amdr_clk_devinfo_t       g_clk_devinfo; /**< \brief CLK设备信息 */
extern       amdr_clk_dev_t           g_clk_dev;     /**< \brief CLK设备     */

#ifdef __cplusplus
}
#endif

#endif  /* __AMDR_HWCONFIG_CLK_H */

/* end of file */
