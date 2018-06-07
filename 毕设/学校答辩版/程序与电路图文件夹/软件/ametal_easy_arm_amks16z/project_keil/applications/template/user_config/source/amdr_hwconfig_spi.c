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
 * \brief KS16 SPI 用户配置文件
 * \sa amdr_hwconfig_spi.c
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-10-23  liqing, first implementation.
 * \endinternal
 */

#include "amdr_hwconfig_spi.h"

/**
 * \addtogroup amdr_if_hwconfig_src_spi
 * \copydoc amdr_hwconfig_spi.c
 * @{
 */

/**
 * \brief SPI0 设备信息
 */
const  struct amdr_spi_devinfo  g_spi0_devinfo = {
    AMHW_SPI0,                                  /**< \brief SPI0寄存器指针   */
    AM_SPI_MODE_0,                              /**< \brief 初始化模式       */
    3000000,                                    /**< \brief 初始速度3MHz     */
    AMDR_SPI_CFG_WIAT_EN,                       /**< \brief Wiat模式+DMA传输 */
    INUM_SPI0                                   /**< \brief SPI0中断号       */
};

/** \brief SPI0 设备实例 */
amdr_spi_dev_t g_spi0_dev;

/** \brief SPI0 平台初始化 */
void amhw_plfm_spi0_init (void)
{
    am_gpio_pin_cfg(PIOE_17, PIOE_17_SPI0_SCK  | AM_GPIO_PULLUP);
    am_gpio_pin_cfg(PIOE_18, PIOE_18_SPI0_MOSI | AM_GPIO_PULLUP);
    am_gpio_pin_cfg(PIOE_19, PIOE_19_SPI0_MISO | AM_GPIO_PULLUP);
    
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_SPI0);
}

/** \brief 解除SPI0 平台初始化 */
void amhw_plfm_spi0_deinit (void)
{
    /* 释放引脚为输入模式 */
    am_gpio_pin_cfg(PIOE_17, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOE_18, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOE_19, AM_GPIO_INPUT);

    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_SPI0);
}

/**
 * \brief SPI1 设备信息
 */
const  struct amdr_spi_devinfo  g_spi1_devinfo = {
    AMHW_SPI1,                                  /**< \brief SPI0寄存器指针   */
    AM_SPI_MODE_0,                              /**< \brief 初始化模式       */
    3000000,                                    /**< \brief 初始速度3MHz     */
    AMDR_SPI_CFG_WIAT_EN,                       /**< \brief Wiat模式+DMA传输 */
    INUM_SPI1                                   /**< \brief SPI0中断号       */
};

/** \brief SPI1 设备实例 */
amdr_spi_dev_t g_spi1_dev;

/** \brief SPI1 平台初始化 */
void amhw_plfm_spi1_init (void)
{
    am_gpio_pin_cfg(PIOD_5, PIOD_5_SPI1_SCK  | AM_GPIO_PULLUP);
    am_gpio_pin_cfg(PIOD_6, PIOD_6_SPI1_MISO | AM_GPIO_PULLUP);
    am_gpio_pin_cfg(PIOD_7, PIOD_7_SPI1_MOSI | AM_GPIO_PULLUP);
    
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_SPI1);
}

/** \brief 解除SPI1 平台初始化 */
void amhw_plfm_spi1_deinit (void)
{
    am_gpio_pin_cfg(PIOD_5, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOD_6, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOD_7, AM_GPIO_INPUT);

    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_SPI1);
}

/**
 * @}
 */

/* end of file */
