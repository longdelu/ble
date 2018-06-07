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
 * \brief KS16XX GPIO �û������ļ���
 * \sa amdr_hwconfig_gpio.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  wxj, first implementation.
 * \endinternal
 */

#include "amdr_hwconfig_gpio.h"

/**
 * \addtogroup amdr_if_hwconfig_src_gpio
 * \copydoc amdr_hwconfig_gpio.c
 * @{
 */

/** \brief ���Ŵ�����Ϣ�ڴ� */
static struct amdr_gpio_trigger_info g_gpio_triginfos[PIN_INT_MAX];

/** \brief ���Ŵ�����Ϣӳ�� */
static uint8_t g_gpio_infomap[PIN_INT_MAX];

/** \brief GPIO�豸��Ϣ     */
const amdr_gpio_devinfo_t g_gpio_devinfo = {
    AMHW_PORT,                /**< \brief ָ��GPIO�˿ڼĴ������ָ�� */
    AMHW_GPIO,                /**< \brief ָ��GPIO�Ĵ������ָ�� */
    {
        INUM_PORTA,
        INUM_PORTC_PORTD,
    },
    PIN_INT_MAX,               /**< \brief GPIO֧�ֵ������жϺ����� */
    g_gpio_infomap,            /**< \brief ���Ŵ�����Ϣӳ��         */
    g_gpio_triginfos           /**< \brief ���Ŵ�����Ϣ�ڴ�         */
};

/** \brief GPIO�豸ʵ��   */
amdr_gpio_dev_t g_gpio_dev;

/** \brief GPIOƽ̨��ʼ�� */
void amhw_plfm_gpio_init (void)
{
    /* ʹ��GPIO�������ʱ�� */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_PORTA);
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_PORTB);
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_PORTC);
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_PORTD);
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_PORTE);
}

/** \brief GPIOƽ̨ȥ��ʼ�� */
void amhw_plfm_gpio_deinit (void)
{
    /* ����GPIO�������ʱ�� */
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_PORTA);
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_PORTB);
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_PORTC);
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_PORTD);
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_PORTE);
}

/**
 * @}
 */

/* end of file */
