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
 * \brief KS16z LPTMR 定时功能用户配置文件
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  cuishengen, first implementation.
 * \endinternal
 */
 
#ifndef __AMDR_HWCONFIG_LPTMR_TIMING_H
#define __AMDR_HWCONFIG_LPTMR_TIMING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"

/** \brief LPTMR定时器功能的设备信息 */
extern amdr_lptmr_timing_dev_t            g_lptmr_timing_dev;
    
/** \brief LPTMR定时器功能设备 */
extern const amdr_lptmr_timing_devinfo_t  g_lptmr_timing_devinfo;

/**
 * \brief LPTMR0定时器功能平台初始化
 * \return 无
 */
void amhw_plfm_lptmr_timing_init(void);
    
/**
 * \brief 解除LPTMR0定时器功能的平台初始化
 * \return 无
 */
void amhw_plfm_lptmr_timing_deinit(void);

#ifdef __cplusplus
}
#endif

#endif  /* __AMDR_HWCONFIG_LPTMR_TIMING_H */

/* end of file */
