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
 * \brief KS16xx WDT �û������ļ�
 * \sa amdr_hwconfig_wdt.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-23  wxj, first implementation.
 * \endinternal
 */

#include "amdr_hwconfig_wdt.h"

/**
 * \addtogroup amdr_if_hwconfig_src_wdt
 * \copydoc amdr_hwconfig_wdt.c
 * @{
 */

/** \brief WDT �豸��Ϣ */
const amdr_wdt_devinfo_t  g_wdt_devinfo = {
        AMHW_SIM,                          /* ָ��SIM�Ĵ������ָ�� */
};

/** \brief WDT�豸ʵ��  */
amdr_wdt_dev_t            g_wdt_dev;

/** \brief WDT ƽ̨��ʼ�� */
void amhw_plfm_wdt_init (void)
{
    /* ����Ҫ���������� */
}

/** \brief ���WDT ƽ̨��ʼ�� */
void amhw_plfm_wdt_deinit (void)
{
    /* ����Ҫ���������� */
}

/**
 * @}
 */

/* end of file */
