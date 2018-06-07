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
 * \brief KS16z LPTMR ��ʱ�����û������ļ�
 * \sa amdr_hwconfig_lptmr_timing.c
 * 
 * \note LPTMֻ��һ����ʱͨ������׼�ӿڲ����ͨ���������塣
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

/** \brief LPTMR �豸��Ϣ */
const amdr_lptmr_timing_devinfo_t  g_lptmr_timing_devinfo = {
    AMHW_LPTMR,                     /**< \brief ָ��LPTMR�Ĵ������ָ�� */
    INUM_LPTMR0,                    /**< \brief LPTMR0�жϱ�� */
};

/** \brief LPTMR �豸����  */
amdr_lptmr_timing_dev_t  g_lptmr_timing_dev;

/** \brief LPTMR ƽ̨��ʼ�� */
void amhw_plfm_lptmr_timing_init (void)
{
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_LPTMR);
    
    /* �ر�LPTMRģ�� */
    amhw_lptmr_ctl_clear(AMHW_LPTMR, AMHW_LPTMR_CSR_TEN);
    /* ʱ��Դѡ�� */
    amhw_lptmr_clock_set(AMHW_LPTMR, AMHW_LPTMR_CLOCK_SRC_MCGIRCLK);
}

/** \brief ��� LPTMR ƽ̨��ʼ�� */
void amhw_plfm_lptmr_timing_deinit (void)
{
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_LPTMR);
}

/**
 * @}
 */

/* end of file */
