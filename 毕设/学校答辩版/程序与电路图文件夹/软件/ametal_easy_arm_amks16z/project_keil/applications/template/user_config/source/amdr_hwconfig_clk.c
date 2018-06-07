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
 * \brief KS16XX CLK 用户配置文件。
 * \sa amdr_hwconfig_clk.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  lnk, first implementation.
 * \endinternal
 */

#include "amdr_hwconfig_clk.h"

/**
 * \addtogroup amdr_if_hwconfig_src_clk
 * \copydoc amdr_hwconfig_clk.c
 * @{
 */

/** \brief 时钟工作模式
 *
 *   可定义为AMDR_CLK_MODE_FEI 或 AMDR_CLK_MODE_PEE
 *
 *  - FEI mode: 使用内部慢速 IRC(32.768KHz)作FLL参考时钟 ，经FLL倍频后作主时钟，
 *              Core clock = 43.97MHz，
 *              Bus clock  = 23.97152MHz
 *
 *  - PEE mode: 使用外部时钟XTAL作PLL参考时钟 ，经PLL倍频后作主时钟，
 *              Core clock = 48MHz，
 *              Bus clock  = 24Hz
 *
 */
#define __CLK_MODE    AMDR_CLK_MODE_FEI


/** \brief CLK设备信息     */
const amdr_clk_devinfo_t g_clk_devinfo = {
    
    /* 时钟工作模式 */
    __CLK_MODE,

    /* AMDR_CLK_MODE_PEE模式的外部晶振频率，可用8-32MHz, 4的整数倍 */
    (8*1000*1000)
};

/** \brief CLK设备实例   */
amdr_clk_dev_t g_clk_dev;

/** \brief CLK平台初始化 */
void amhw_plfm_clk_init (void)
{
    /** \brief  设置外部晶振OSC引脚功能 */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_PORTA);
    amhw_port_pin_func_cfg(AMHW_PORT,PIOA_18,PIOA_18_EXTAL0);
    amhw_port_pin_func_cfg(AMHW_PORT,PIOA_19,PIOA_19_XTAL0);
}

/* \brief CLK 平台去初始化 */
void amhw_plfm_clk_deinit (void)
{
    /* 时钟无去初始化操作*/
    return;
}

/**
 * @}
 */

/* end of file */
