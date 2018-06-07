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
 * \brief KS16z PIT 定时功能用户配置文件
 * \sa amdr_hwconfig_pit_timing.c
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  cuishengen, first implementation.
 * \endinternal
 */

#include "amdr_hwconfig_pit_timing.h"
#include "amdr_pit_timing.h"

/**
 * \addtogroup amdr_if_hwconfig_src_pit_timing
 * \copydoc amdr_hwconfig_pit_timing.c
 * @{
 */

/** \brief PIT 设备信息 */
const amdr_pit_timing_devinfo_t  g_pit0_timing_devinfo = {
    AMHW_PIT0,                  /**< \brief 指向PIT寄存器块的指针  */
    AMDR_PIT_TIMING_2_32BIT,    /**< \brief 配置为2通道的32位定时器 */
    INUM_PIT,                   /**< \brief PIT0中断编号           */
};

/** \brief PIT 设备定义  */
amdr_pit_timing_dev_t  g_pit0_timing_dev;

/** \brief PIT0 平台初始化 */
void amhw_plfm_pit0_timing_init (void)
{
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_PIT);
}

/** \brief 解除 PIT0 平台初始化 */
void amhw_plfm_pit0_timing_deinit (void)
{
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_PIT);
}

/**
 * @}
 */

/* end of file */
