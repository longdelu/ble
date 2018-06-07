/*******************************************************************************
*                                 Apollo
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
 * \brief LPC82X DMA 用户配置文件
 * \sa amdr_hwconfig_dma.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-09  zhouziwei, first implementation.
 * \endinternal
 */

#include "amdr_hwconfig_dma.h"

/**
 * \addtogroup amdr_if_hwconfig_src_dma
 * \copydoc amdr_hwconfig_dma.c
 * @{
 */

/** \brief DMA 设备信息 */
const amdr_dma_devinfo_t g_dma_devinfo = {
    AMHW_DMAMUX,                   /**< \brief 指向DMAMUX寄存器块的指针  */
    AMHW_DMA,                      /**< \brief 指向DMA寄存器块的指针          */
    INUM_DMA_CH0                   /**< \brief DMA通道0中断向量号                 */
};

/** \brief DMA设备实例 */
amdr_dma_dev_t g_dma_dev;

/**
 * \brief DMA 平台初始化。
 */
void amhw_plfm_dma_init (void)
{
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_DMAMUX);
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_DMA);
//    AMHW_SIM->scgc[2] |= (1UL<<1);  /**< \brief 使能DMAMUX时钟          */
//    AMHW_SIM->scgc[3] |= (1UL<<8);  /**< \brief 使能DMA时钟                  */
}

/**
 * \brief DMA 平台去初始化。
 */
void amhw_plfm_dma_deinit (void)
{

}


/**
 * @}
 */

/* end of file */

