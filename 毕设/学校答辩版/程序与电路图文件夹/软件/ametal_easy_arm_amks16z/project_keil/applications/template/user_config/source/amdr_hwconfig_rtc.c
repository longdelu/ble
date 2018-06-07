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
 * \brief KS16 SPI �û������ļ�
 * \sa amdr_hwconfig_spi.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-24  zsp, first implementation.
 * \endinternal
 */

#include "amdr_hwconfig_rtc.h"

/**
 * \addtogroup amdr_if_hwconfig_src_rtc
 * \copydoc amdr_hwconfig_rtc.c
 * @{
 */

/**
 *  \brief RTC ƽ̨��ʼ��
 *  \return ��
 */
void amhw_plfm_rtc_init(void)
{
    /** RTC ʱ��Դѡ��Ϊ�ⲿRTCʱ��Դ */
    amhw_sim_ercclk32k_src_set(AMHW_SIM_OSC32KSEL_CLKIN);

    /** RTC ģ��ʱ��ʹ�� */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_RTC);

    /** PORTC ʱ������ʹ�� */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_PORTC);

    /** PTC1 ����λ RTC_CLK_INPUT */
    amhw_port_pin_func_cfg(AMHW_PORT, PIOC_1, 1);
}

/**
 *  \brief ��� RTC ƽ̨��ʼ��
 *  \return ��
 */
void amhw_plfm_rtc_deinit(void)
{
    /** RTC ģ��ʱ��ʹ�� */
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_RTC);
}

/** \brief RTC�豸��Ϣ */
const struct amdr_rtc_devinfo g_rtc_devinfo = {
    AMHW_RTC
};

/**< \brief RTC�豸     */
amdr_rtc_dev_t                g_rtc_dev;

/**
 * @}
 */

/** end of file */

