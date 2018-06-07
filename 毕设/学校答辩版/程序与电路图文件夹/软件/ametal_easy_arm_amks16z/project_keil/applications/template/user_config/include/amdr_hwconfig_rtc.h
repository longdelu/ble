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
 * \brief RTC �û������ļ�
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
 *  \brief RTC ƽ̨��ʼ��
 *  \return ��
 */
void amhw_plfm_rtc_init(void);

/**
 *  \brief ��� RTC ƽ̨��ʼ��
 *  \return ��
 */
void amhw_plfm_rtc_deinit(void);

extern const struct amdr_rtc_devinfo g_rtc_devinfo; /**< \brief RTC�豸��Ϣ */
extern amdr_rtc_dev_t                g_rtc_dev;     /**< \brief RTC�豸     */


#ifdef __cplusplus
}
#endif

#endif

/** end of the file */
