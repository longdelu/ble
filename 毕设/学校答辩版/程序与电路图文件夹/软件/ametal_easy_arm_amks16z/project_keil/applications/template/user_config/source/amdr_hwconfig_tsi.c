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
 * \brief KS16XX TSI 用户配置文件。
 * \sa amdr_hwconfig_tsi.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  wxj, first implementation.
 * \endinternal
 */

#include "amdr_hwconfig_tsi.h"

/**
 * \addtogroup amdr_if_hwconfig_src_tsi
 * \copydoc amdr_hwconfig_tsi.c
 * @{
 */

/** \brief tsi设备信息     */
const amdr_tsi_devinfo_t g_tsi_devinfo = {
    AMHW_TSI,       /**< \brief 指向tsi端口寄存器块的指针 */
    FALSE,          /**< \brief 不启用低功耗中断唤醒MCU功能 */
};

/** \brief tsi设备实例   */
amdr_tsi_dev_t g_tsi_dev;

/** \brief tsi平台初始化 */
void amhw_plfm_tsi_init (void)
{
    /* 使能tsi相关外设时钟 */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_TSI);

    /* 配置PIOA_1引脚为TSI0_CH2 */
    am_gpio_pin_cfg(PIOA_1, PIOA_1_TSI0_CH2);
}

/** \brief tsi平台去初始化 */
void amhw_plfm_tsi_deinit (void)
{
    /* 禁能tsi相关外设时钟 */
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_TSI);
}

/**
 * @}
 */

/* end of file */
