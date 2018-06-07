/*******************************************************************************
*                                 Apollo
*                       ---------------------------
*                       innovating embedded systems
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief 系统初始化设置
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 14-11-01  tee, first implementation.
 * \endinternal
 */

#include "am_system.h"
#include "am_prj_config.h"

/**
 * \brief 系统初始化函数
 *
 * 使用PLL得到更高的时钟频率
 */
void am_system_init(void)
{
    /* PMU相关初始化 */
    amhw_plfm_pmu_init();
    amdr_pmu_init(&g_pmu_dev, &g_pmu_devinfo);

    /* CLK相关初始化*/
    amhw_plfm_clk_init();
    amdr_clk_init(&g_clk_dev, &g_clk_devinfo);
}

/* end of file */
