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
 * \brief KS16 DAC �û������ļ���
 * \sa amdr_hwconfig_dac.c
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-07-14  xiongyanming, first implementation.
 * \endinternal
 */
 
#include "amdr_hwconfig_dac.h"

/**
 * \addtogroup amdr_if_hwconfig_src_dac
 * \copydoc amdr_hwconfig_dac.c
 * @{
 */

/** \brief DAC �豸��Ϣ */
const amdr_dac_devinfo_t g_dac0_devinfo =
{
    AMHW_DAC0,                     /**< \brief ָ��DAC0�Ĵ������ָ�� */
    INUM_DAC0,                     /**< \brief DAC0�жϺ�             */
    12,                            /**< \brief DACת������            */
    3270,                          /**< \brief DAC�ο���ѹ����λ��mV  */
    AMHW_DAC_VREF_2                /**< \brief DAC��ѹԴ              */
};

/** \brief DAC0�豸ʵ�� */
amdr_dac_dev_t g_dac0_dev;

/** 
 * \brief DAC ƽ̨��ʼ����
 */
void amhw_plfm_dac0_init (void)
{
    /* ����PIOE_30ΪDAC0_OUT����                 */
     am_gpio_pin_cfg (PIOE_30,PIOE_30_DAC0_OUT);

     /* ����DACʱ��                  */
     amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_DAC0);
   
}

/** 
 * \brief DAC ƽ̨ȥ��ʼ����
 */
void amhw_plfm_dac0_deinit (void)
{
   /* �ر�DACʱ��                  */
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_DAC0);
}

/**
 * @}
 */

/* end of file */
