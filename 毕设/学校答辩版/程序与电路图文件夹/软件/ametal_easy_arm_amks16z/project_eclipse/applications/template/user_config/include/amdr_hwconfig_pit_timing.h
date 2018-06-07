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
 * \brief KS16z PIT ��ʱ�����û������ļ�
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  cuishengen, first implementation.
 * \endinternal
 */
 
#ifndef __AMDR_HWCONFIG_PIT_TIMING_H
#define __AMDR_HWCONFIG_PIT_TIMING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"

/** \brief PIT0��ʱ�����ܵ��豸��Ϣ */
extern amdr_pit_timing_dev_t            g_pit0_timing_dev;
    
/** \brief PIT0��ʱ�������豸 */
extern const amdr_pit_timing_devinfo_t  g_pit0_timing_devinfo;

/**
 * \brief PIT0��ʱ������ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_pit0_timing_init(void);
    
/**
 * \brief ���PIT0��ʱ�����ܵ�ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_pit0_timing_deinit(void);

#ifdef __cplusplus
}
#endif

#endif  /* __AMDR_HWCONFIG_PIT_TIMING_H */

/* end of file */
