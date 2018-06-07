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
 * \brief LPC82X DMA �û������ļ�
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

/** \brief DMA �豸��Ϣ */
const amdr_dma_devinfo_t g_dma_devinfo = {
    AMHW_DMAMUX,                   /**< \brief ָ��DMAMUX�Ĵ������ָ��  */
    AMHW_DMA,                      /**< \brief ָ��DMA�Ĵ������ָ��          */
    INUM_DMA_CH0                   /**< \brief DMAͨ��0�ж�������                 */
};

/** \brief DMA�豸ʵ�� */
amdr_dma_dev_t g_dma_dev;

/**
 * \brief DMA ƽ̨��ʼ����
 */
void amhw_plfm_dma_init (void)
{
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_DMAMUX);
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_DMA);
//    AMHW_SIM->scgc[2] |= (1UL<<1);  /**< \brief ʹ��DMAMUXʱ��          */
//    AMHW_SIM->scgc[3] |= (1UL<<8);  /**< \brief ʹ��DMAʱ��                  */
}

/**
 * \brief DMA ƽ̨ȥ��ʼ����
 */
void amhw_plfm_dma_deinit (void)
{

}


/**
 * @}
 */

/* end of file */

