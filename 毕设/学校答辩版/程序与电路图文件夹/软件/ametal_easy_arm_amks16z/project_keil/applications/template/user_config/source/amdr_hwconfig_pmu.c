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
 * \brief KS16 PMU �û������ļ�
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
 * \brief PMU �豸��Ϣ
 */
const  struct amdr_pmu_devinfo  g_pmu_devinfo = {
    AMHW_SMC,                           /**< \brief SMC�Ĵ���ָ��       */
    AMHW_PMC,                           /**< \brief PMC�Ĵ���ָ��       */
    AMHW_RCM,                           /**< \brief RCM�Ĵ���ָ��       */
    AMHW_LLWU,                          /**< \brief LLWU�Ĵ���ָ��      */
    AMDR_PMU_ARG_VLLS0_POR_DISABLE |    /**< \brief VLLS0���ܵ�Դ���   */
    AMDR_PMU_ARG_VLLSx_ISOACK_CLEAR     /**< \brief �Զ��������ACK��־ */
};

/** \brief PMU �豸ʵ�� */
amdr_pmu_dev_t g_pmu_dev;

/** \brief PMU ƽ̨��ʼ�� */
void amhw_plfm_pmu_init (void)
{
    amhw_int_enable(AMHW_INT, INUM_LLWU);
}

/** \brief ���PMU ƽ̨��ʼ�� */
void amhw_plfm_pmu_deinit (void)
{
    amhw_int_disable(AMHW_INT, INUM_LLWU);
}

/**
 * @}
 */

/* end of file */
