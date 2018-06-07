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
 * \brief AMKS16 I2C_SLV 用户配置文件
 * \sa amdr_hwconfig_i2c_slv.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-12-21  mem , first implementation.
 * \endinternal
 */

#include "amdr_hwconfig_i2c_slv.h"

/**
 * \addtogroup amdr_if_hwconfig_src_i2c_slv
 * \copydoc amdr_hwconfig_i2c_slv.c
 * @{
 */

#define __I2C_BUS_SPEED_I2C_SLV0     (400000)  /**< \brief I2C_SLV0 总线速率 */
#define __I2C_SLV0_DEV_NUM              3      /**< \brief 从机设备数量      */

/**
 * \brief 存储设备地址内存
 */
uint32_t __g_i2c_slv0_devlist[__I2C_SLV0_DEV_NUM];

/**
 * \brief I2C_SLV0 设备信息
 */
const  struct amdr_i2c_slv_devinfo  g_i2c_slv0_devinfo = {

    /** \brief 指向I2C0寄存器块的指针 */
    AMHW_I2C0,

    /** \brief 总线速度 */
    __I2C_BUS_SPEED_I2C_SLV0,

    /** \brief 指向从机设备的指针 */
    __g_i2c_slv0_devlist,

    /** \brief 使用的从机设备 */
    __I2C_SLV0_DEV_NUM,

    /** \brief I2C0 中断编号 */
    INUM_I2C0,
};

amdr_i2c_slv_dev_t g_i2c_slv0_dev; /**< \brief I2C0 设备实例 */

/** \brief I2C_SLV0 平台初始化函数 */
void amhw_plfm_i2c_slv0_init (void)
{
    am_gpio_pin_cfg(PIOB_0, PIOB_0_I2C0_SCL);
    am_gpio_pin_cfg(PIOB_1, PIOB_1_I2C0_SDA);

    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_I2C0);
}

/** \brief 解除I2C_SLV0 平台初始化函数 */
void amhw_plfm_i2c_slv0_deinit (void)
{
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_I2C0);
}

#define __I2C_BUS_SPEED_I2C_SLV1     (400000)  /**< \brief I2C_SLV0 总线速率 */
#define __I2C_SLV1_DEV_NUM              3      /**< \brief 从机设备数量      */

/**
 * \brief 存储设备地址指针
 */
uint32_t __g_i2c_slv1_devlist[__I2C_SLV1_DEV_NUM];

/**
 * \brief I2C1 设备信息
 */
const  struct amdr_i2c_slv_devinfo  g_i2c_slv1_devinfo = {

    /** \brief 指向I2C1寄存器块的指针 */
    AMHW_I2C1,

    /** \brief 总线速度 */
    __I2C_BUS_SPEED_I2C_SLV1,

    /** \brief 指向从机设备的指针 */
    __g_i2c_slv1_devlist,

    /** \brief 使用的从机设备数量 */
    __I2C_SLV1_DEV_NUM,

    /** \brief I2C0 中断编号 */
    INUM_I2C1,
};

amdr_i2c_slv_dev_t g_i2c_slv1_dev; /**< \brief I2C1 设备实例 */

/** \brief I2C1 平台初始化函数 */
void amhw_plfm_i2c_slv1_init (void)
{
    am_gpio_pin_cfg (PIOE_0, PIOE_0_I2C1_SDA);
    am_gpio_pin_cfg (PIOE_1, PIOE_1_I2C1_SCL);

    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_I2C1);
}

/** \brief 解除I2C1 平台初始化函数 */
void amhw_plfm_i2c_slv1_deinit (void)
{
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_I2C1);
}

/** @} */

/* end of file */
