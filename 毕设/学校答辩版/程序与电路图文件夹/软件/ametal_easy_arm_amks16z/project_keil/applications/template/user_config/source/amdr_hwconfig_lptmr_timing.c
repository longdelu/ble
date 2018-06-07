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
 * \brief KS16z LPTMR 定时功能用户配置文件
 * \sa amdr_hwconfig_lptmr_timing.c
 * 
 * \note LPTM只有一个定时通道，标准接口层参数通道号无意义。
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  cuishengen, first implementation.
 * \endinternal
 */

#include "amdr_hwconfig_lptmr_timing.h"
#include "amdr_lptmr_timing.h"

/**
 * \addtogroup amdr_if_hwconfig_src_lptmr_timing
 * \copydoc amdr_hwconfig_lptmr_timing.c
 * @{
 */

/** \brief LPTMR 设备信息 */
const amdr_lptmr_timing_devinfo_t  g_lptmr_timing_devinfo = {
    AMHW_LPTMR,                     /**< \brief 指向LPTMR寄存器块的指针 */
    INUM_LPTMR0,                    /**< \brief LPTMR0中断编号 */
};

/** \brief LPTMR 设备定义  */
amdr_lptmr_timing_dev_t  g_lptmr_timing_dev;

/** \brief LPTMR 平台初始化 */
void amhw_plfm_lptmr_timing_init (void)
{
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_LPTMR);
    
    /* 关闭LPTMR模块 */
    amhw_lptmr_ctl_clear(AMHW_LPTMR, AMHW_LPTMR_CSR_TEN);
    /* 时钟源选择 */
    amhw_lptmr_clock_set(AMHW_LPTMR, AMHW_LPTMR_CLOCK_SRC_MCGIRCLK);
}

/** \brief 解除 LPTMR 平台初始化 */
void amhw_plfm_lptmr_timing_deinit (void)
{
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_LPTMR);
}

/**
 * @}
 */

/* end of file */
