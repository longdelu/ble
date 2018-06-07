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
 * \brief LPC82X I2C 用户配置文件
 * \sa amdr_hwconfig_i2c.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-27  LXY , first implementation.
 * \endinternal
 */

#include "amdr_hwconfig_i2c.h"

/**
 * \addtogroup amdr_if_hwconfig_src_i2c
 * \copydoc amdr_hwconfig_i2c.c
 * @{
 */

/**
 * \name I2C0 控制器参数定义
 * @{
 */

#define BUS_SPEED_I2C0     (100000)        /**< \brief I2C0 总线速率参数定义 */

/** @} */

/**
 * \brief I2C0 设备信息
 */
const  struct amdr_i2c_devinfo  g_i2c0_devinfo = {
    AMHW_I2C0,
    BUS_SPEED_I2C0,                        /**< \brief 指向I2C0寄存器块的指针 */
    INUM_I2C0,                             /**< \brief I2C0 中断编号 */
};

amdr_i2c_dev_t g_i2c0_dev;                 /**< \brief I2C0 设备实例 */

/** \brief I2C0 平台初始化函数 */
void amhw_plfm_i2c0_init (void)
{
    am_gpio_pin_cfg(PIOC_9, PIOC_9_I2C0_SDA);
    am_gpio_pin_cfg(PIOC_8, PIOC_8_I2C0_SCL);

    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_I2C0);
}

/** \brief 解除I2C0 平台初始化函数 */
void amhw_plfm_i2c0_deinit (void)
{
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_I2C0);
}

/**
 * \name I2C1 控制器参数定义
 * @{
 */

#define BUS_SPEED_I2C1     (100000)     /**< \brief I2C1 总线速率参数定义 */

/** @} */

/**
 * \brief I2C1 设备信息
 */
const  struct amdr_i2c_devinfo  g_i2c1_devinfo = {
    AMHW_I2C1,                             /**< \brief 指向I2C1寄存器块的指针 */
    BUS_SPEED_I2C1,
    INUM_I2C1,                             /**< \brief I2C1 中断编号 */
};

amdr_i2c_dev_t g_i2c1_dev;                 /**< \brief I2C1 设备实例 */

/** \brief I2C1 平台初始化函数 */
void amhw_plfm_i2c1_init (void)
{
    am_gpio_pin_cfg (PIOE_0, PIOE_0_I2C1_SDA);
    am_gpio_pin_cfg (PIOE_1, PIOE_1_I2C1_SCL);

    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_I2C1);
}

/** \brief 解除I2C1 平台初始化函数 */
void amhw_plfm_i2c1_deinit (void)
{
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_I2C1);
}

/** @} */

/* end of file */
