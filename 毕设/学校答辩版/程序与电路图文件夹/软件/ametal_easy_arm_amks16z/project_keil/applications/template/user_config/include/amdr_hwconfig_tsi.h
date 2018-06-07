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
 * \brief KS16XX TSI �û�����ͷ�ļ���
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  wxj, first implementation.
 * \endinternal
 */

#ifndef __AMDR_HWCONFIG_TSI_H
#define __AMDR_HWCONFIG_TSI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"

/**
 * \brief TSI ƽ̨��ʼ����
 * \return �ޡ�
 */
void amhw_plfm_tsi_init(void);

/**
 * \brief TSI ƽ̨ȥ��ʼ��
 * \return �ޡ�
 */
void amhw_plfm_tsi_deinit(void);

extern const amdr_tsi_devinfo_t g_tsi_devinfo; /**< \brief TSI�豸��Ϣ */
extern amdr_tsi_dev_t           g_tsi_dev;     /**< \brief TSI�豸  */

#ifdef __cplusplus
}
#endif

#endif  /* __AMDR_HWCONFIG_TSI_H */

/* end of file */
