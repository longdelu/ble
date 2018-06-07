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
 * \brief KS16z LPTMR ��ʱ�����û������ļ�
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

/** \brief LPTMR��ʱ�����ܵ��豸��Ϣ */
extern amdr_lptmr_timing_dev_t            g_lptmr_timing_dev;
    
/** \brief LPTMR��ʱ�������豸 */
extern const amdr_lptmr_timing_devinfo_t  g_lptmr_timing_devinfo;

/**
 * \brief LPTMR0��ʱ������ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_lptmr_timing_init(void);
    
/**
 * \brief ���LPTMR0��ʱ�����ܵ�ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_lptmr_timing_deinit(void);

#ifdef __cplusplus
}
#endif

#endif  /* __AMDR_HWCONFIG_LPTMR_TIMING_H */

/* end of file */
