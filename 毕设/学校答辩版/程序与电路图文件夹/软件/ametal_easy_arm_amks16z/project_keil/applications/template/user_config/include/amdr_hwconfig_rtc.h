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
 * \brief RTC 用户配置文件
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-19  zsp, first implementation.
 * \endinternal
 */

#ifndef __AMDR_HWCONFIG_RTC_H
#define __AMDR_HWCONFIG_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"

/**
 *  \brief RTC 平台初始化
 *  \return 无
 */
void amhw_plfm_rtc_init(void);

/**
 *  \brief 解除 RTC 平台初始化
 *  \return 无
 */
void amhw_plfm_rtc_deinit(void);

extern const struct amdr_rtc_devinfo g_rtc_devinfo; /**< \brief RTC设备信息 */
extern amdr_rtc_dev_t                g_rtc_dev;     /**< \brief RTC设备     */


#ifdef __cplusplus
}
#endif

#endif

/** end of the file */
