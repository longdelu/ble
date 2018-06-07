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
 * \brief AMKS16 I2C_SLV 用户配置头文件
 *
 * \internal
 * \par Modification history
 * - 1.00 15-12-21  mem , first implementation.
 * \endinternal
 */

#ifndef __AMDR_HWCONFIG_I2C_SLV_H
#define __AMDR_HWCONFIG_I2C_SLV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"

/**
 * \brief I2C_SLV0 平台初始化
 * \return 无
 */
void amhw_plfm_i2c_slv0_init(void);

/**
 * \brief 解除I2C_SLV0 平台初始化
 * \return 无
 */
void amhw_plfm_i2c_slv0_deinit(void);

/**< \brief I2C_SLV0 设备信息 */
extern const struct amdr_i2c_slv_devinfo g_i2c_slv0_devinfo;

/**< \brief I2C_SLV0 设备 */
extern amdr_i2c_slv_dev_t                 g_i2c_slv0_dev;

/**
 * \brief I2C_SLV1 平台初始化
 * \return 无
 */
void amhw_plfm_i2c_slv1_init(void);

/**
 * \brief 解除I2C_SLV1 平台初始化
 * \return 无
 */
void amhw_plfm_i2c_slv1_deinit(void);

/** \brief I2C_SLV1 设备信息     */
extern const struct amdr_i2c_slv_devinfo g_i2c_slv1_devinfo;

/** \brief I2C_SLV1 设备 */
extern amdr_i2c_slv_dev_t                 g_i2c_slv1_dev;

#ifdef __cplusplus
}
#endif

#endif  /* __AMDR_HWCONFIG_I2C_SLV_H */

/* end of file */
