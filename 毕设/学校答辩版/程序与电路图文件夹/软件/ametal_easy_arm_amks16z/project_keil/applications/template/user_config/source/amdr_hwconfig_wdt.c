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
 * \brief KS16xx WDT 用户配置文件
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

/** \brief WDT 设备信息 */
const amdr_wdt_devinfo_t  g_wdt_devinfo = {
        AMHW_SIM,                          /* 指向SIM寄存器块的指针 */
};

/** \brief WDT设备实例  */
amdr_wdt_dev_t            g_wdt_dev;

/** \brief WDT 平台初始化 */
void amhw_plfm_wdt_init (void)
{
    /* 不需要做其他操作 */
}

/** \brief 解除WDT 平台初始化 */
void amhw_plfm_wdt_deinit (void)
{
    /* 不需要做其他操作 */
}

/**
 * @}
 */

/* end of file */
