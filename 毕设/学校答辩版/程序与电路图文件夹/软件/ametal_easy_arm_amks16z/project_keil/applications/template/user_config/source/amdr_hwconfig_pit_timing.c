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
 * \brief KS16z PIT ��ʱ�����û������ļ�
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

/** \brief PIT �豸��Ϣ */
const amdr_pit_timing_devinfo_t  g_pit0_timing_devinfo = {
    AMHW_PIT0,                  /**< \brief ָ��PIT�Ĵ������ָ��  */
    AMDR_PIT_TIMING_2_32BIT,    /**< \brief ����Ϊ2ͨ����32λ��ʱ�� */
    INUM_PIT,                   /**< \brief PIT0�жϱ��           */
};

/** \brief PIT �豸����  */
amdr_pit_timing_dev_t  g_pit0_timing_dev;

/** \brief PIT0 ƽ̨��ʼ�� */
void amhw_plfm_pit0_timing_init (void)
{
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_PIT);
}

/** \brief ��� PIT0 ƽ̨��ʼ�� */
void amhw_plfm_pit0_timing_deinit (void)
{
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_PIT);
}

/**
 * @}
 */

/* end of file */
