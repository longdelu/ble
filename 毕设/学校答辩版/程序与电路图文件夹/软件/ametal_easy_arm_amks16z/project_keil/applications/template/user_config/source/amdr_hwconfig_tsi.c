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
 * \brief KS16XX TSI �û������ļ���
 * \sa amdr_hwconfig_tsi.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  wxj, first implementation.
 * \endinternal
 */

#include "amdr_hwconfig_tsi.h"

/**
 * \addtogroup amdr_if_hwconfig_src_tsi
 * \copydoc amdr_hwconfig_tsi.c
 * @{
 */

/** \brief tsi�豸��Ϣ     */
const amdr_tsi_devinfo_t g_tsi_devinfo = {
    AMHW_TSI,       /**< \brief ָ��tsi�˿ڼĴ������ָ�� */
    FALSE,          /**< \brief �����õ͹����жϻ���MCU���� */
};

/** \brief tsi�豸ʵ��   */
amdr_tsi_dev_t g_tsi_dev;

/** \brief tsiƽ̨��ʼ�� */
void amhw_plfm_tsi_init (void)
{
    /* ʹ��tsi�������ʱ�� */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_TSI);

    /* ����PIOA_1����ΪTSI0_CH2 */
    am_gpio_pin_cfg(PIOA_1, PIOA_1_TSI0_CH2);
}

/** \brief tsiƽ̨ȥ��ʼ�� */
void amhw_plfm_tsi_deinit (void)
{
    /* ����tsi�������ʱ�� */
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_TSI);
}

/**
 * @}
 */

/* end of file */
