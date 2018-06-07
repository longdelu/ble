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
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief KS16 PMU�û�����ͷ�ļ�
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-10-30  liqing, first implementation.
 * \endinternal
 */
 
#ifndef __AMDR_HWCONFIG_PMU_H
#define __AMDR_HWCONFIG_PMU_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"


extern const amdr_pmu_devinfo_t  g_pmu_devinfo; /**< \brief PMU �豸��Ϣ */
extern amdr_pmu_dev_t            g_pmu_dev;     /**< \brief PMU �豸     */

/**
 * \brief PMU ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_pmu_init(void);

/**
 * \brief ���PMU0 ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_pmu_deinit(void);

#ifdef __cplusplus
}
#endif

#endif  /* __AMDR_HWCONFIG_PMU_H */

/* end of file */
