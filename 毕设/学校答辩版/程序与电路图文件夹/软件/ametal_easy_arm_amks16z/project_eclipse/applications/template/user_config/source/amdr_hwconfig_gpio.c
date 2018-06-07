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
 * \brief KS16XX GPIO 用户配置文件。
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

/** \brief 引脚触发信息内存 */
static struct amdr_gpio_trigger_info g_gpio_triginfos[PIN_INT_MAX];

/** \brief 引脚触发信息映射 */
static uint8_t g_gpio_infomap[PIN_INT_MAX];

/** \brief GPIO设备信息     */
const amdr_gpio_devinfo_t g_gpio_devinfo = {
    AMHW_PORT,                /**< \brief 指向GPIO端口寄存器块的指针 */
    AMHW_GPIO,                /**< \brief 指向GPIO寄存器块的指针 */
    {
        INUM_PORTA,
        INUM_PORTC_PORTD,
    },
    PIN_INT_MAX,               /**< \brief GPIO支持的引脚中断号数量 */
    g_gpio_infomap,            /**< \brief 引脚触发信息映射         */
    g_gpio_triginfos           /**< \brief 引脚触发信息内存         */
};

/** \brief GPIO设备实例   */
amdr_gpio_dev_t g_gpio_dev;

/** \brief GPIO平台初始化 */
void amhw_plfm_gpio_init (void)
{
    /* 使能GPIO相关外设时钟 */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_PORTA);
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_PORTB);
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_PORTC);
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_PORTD);
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_PORTE);
}

/** \brief GPIO平台去初始化 */
void amhw_plfm_gpio_deinit (void)
{
    /* 禁能GPIO相关外设时钟 */
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
