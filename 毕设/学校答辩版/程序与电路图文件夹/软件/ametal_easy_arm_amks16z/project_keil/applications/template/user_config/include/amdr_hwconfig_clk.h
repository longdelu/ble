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
 * \brief KS16XX CLK �û�����ͷ�ļ���
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
 * \brief CLK ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_clk_init (void);

/**
 * \brief CLK ƽ̨ȥ��ʼ��
 * \return ��
 */
void amhw_plfm_clk_deinit (void);

extern const amdr_clk_devinfo_t       g_clk_devinfo; /**< \brief CLK�豸��Ϣ */
extern       amdr_clk_dev_t           g_clk_dev;     /**< \brief CLK�豸     */

#ifdef __cplusplus
}
#endif

#endif  /* __AMDR_HWCONFIG_CLK_H */

/* end of file */
