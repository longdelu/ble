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
 * \brief LPC82XX ADC �û������ļ���
 * \sa amdr_hwconfig_adc.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-15  xjc, first implementation.
 * \endinternal
 */

#include "amdr_hwconfig_adc.h"

/**
 * \addtogroup amdr_if_hwconfig_src_adc
 * \copydoc amdr_hwconfig_adc.c
 * @{
 */

/** \brief ADC �豸��Ϣ ������������ޱȽ���Ӳ��ƽ����*/
const amdr_adc_devinfo_t g_adc0_devinfo =
{
    AMHW_ADC0,                     /**< \brief ָ��ADC0�Ĵ������ָ�� */
    2500,                          /**< \brief ADC�ο���ѹ����λ��mV  */
    12,                            /**< \brief ADC��������(λ��)   */
    AMDR_ADC_DIFF_DIS,             /**< \brief ADC0����ģʽ        */
    INUM_ADC0,                     /**< \brief ADC�ж�������  */
    AMDR_ADC_MODE_TRG_SF,          /**< \brief ADC�������ģʽ      */
    AMHW_ADC_SEQ_A,                /**< \brief ADC��������            */
    DMA_CHAN_0,                    /**< \brief DMAͨ��0���䣬ֵΪ-1ʱ��ʹ��DMA */
    NULL,                          /**< \brief ��ʹ��Ӳ��ƽ������       */
    NULL,                          /**< \brief ��ʹ�ñȽϹ���       */
};

/** \brief ADC0�豸ʵ�� */
amdr_adc_dev_t g_adc0_dev;

/**
 * \brief ADC ƽ̨��ʼ����
 */
void amdr_plfm_adc0_init (void)
{
    /* ʹ��ADCʱ��                  */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_ADC0);

    /* ��ADC����У׼                */
    amhw_adc_calibrate(AMHW_ADC0);

}

/**
 * \brief ADC ƽ̨ȥ��ʼ����
 */
void amdr_plfm_adc0_deinit (void)
{
    /* �ر�ADCģ���ϵ�                */
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_ADC0);
}

/**
 * @}
 */

/* end of file */
