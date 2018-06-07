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
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief KS16 PMU 用户配置文件
 * \sa amdr_hwconfig_pmu.c
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-10-30  liqing, first implementation.
 * \endinternal
 */

#include "amdr_hwconfig_pmu.h"

/**
 * \addtogroup amdr_if_hwconfig_src_pmu
 * \copydoc amdr_hwconfig_pmu.c
 * @{
 */

/**
 * \brief PMU 设备信息
 */
const  struct amdr_pmu_devinfo  g_pmu_devinfo = {
    AMHW_SMC,                           /**< \brief SMC寄存器指针       */
    AMHW_PMC,                           /**< \brief PMC寄存器指针       */
    AMHW_RCM,                           /**< \brief RCM寄存器指针       */
    AMHW_LLWU,                          /**< \brief LLWU寄存器指针      */
    AMDR_PMU_ARG_VLLS0_POR_DISABLE |    /**< \brief VLLS0禁能电源检测   */
    AMDR_PMU_ARG_VLLSx_ISOACK_CLEAR     /**< \brief 自动清除唤醒ACK标志 */
};

/** \brief PMU 设备实例 */
amdr_pmu_dev_t g_pmu_dev;

/** \brief PMU 平台初始化 */
void amhw_plfm_pmu_init (void)
{
    amhw_int_enable(AMHW_INT, INUM_LLWU);
}

/** \brief 解除PMU 平台初始化 */
void amhw_plfm_pmu_deinit (void)
{
    amhw_int_disable(AMHW_INT, INUM_LLWU);
}

/**
 * @}
 */

/* end of file */
