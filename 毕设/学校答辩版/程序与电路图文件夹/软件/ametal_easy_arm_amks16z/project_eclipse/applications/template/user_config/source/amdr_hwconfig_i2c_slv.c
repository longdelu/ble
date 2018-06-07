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
 * \brief AMKS16 I2C_SLV �û������ļ�
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

#define __I2C_BUS_SPEED_I2C_SLV0     (400000)  /**< \brief I2C_SLV0 �������� */
#define __I2C_SLV0_DEV_NUM              3      /**< \brief �ӻ��豸����      */

/**
 * \brief �洢�豸��ַ�ڴ�
 */
uint32_t __g_i2c_slv0_devlist[__I2C_SLV0_DEV_NUM];

/**
 * \brief I2C_SLV0 �豸��Ϣ
 */
const  struct amdr_i2c_slv_devinfo  g_i2c_slv0_devinfo = {

    /** \brief ָ��I2C0�Ĵ������ָ�� */
    AMHW_I2C0,

    /** \brief �����ٶ� */
    __I2C_BUS_SPEED_I2C_SLV0,

    /** \brief ָ��ӻ��豸��ָ�� */
    __g_i2c_slv0_devlist,

    /** \brief ʹ�õĴӻ��豸 */
    __I2C_SLV0_DEV_NUM,

    /** \brief I2C0 �жϱ�� */
    INUM_I2C0,
};

amdr_i2c_slv_dev_t g_i2c_slv0_dev; /**< \brief I2C0 �豸ʵ�� */

/** \brief I2C_SLV0 ƽ̨��ʼ������ */
void amhw_plfm_i2c_slv0_init (void)
{
    am_gpio_pin_cfg(PIOB_0, PIOB_0_I2C0_SCL);
    am_gpio_pin_cfg(PIOB_1, PIOB_1_I2C0_SDA);

    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_I2C0);
}

/** \brief ���I2C_SLV0 ƽ̨��ʼ������ */
void amhw_plfm_i2c_slv0_deinit (void)
{
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_I2C0);
}

#define __I2C_BUS_SPEED_I2C_SLV1     (400000)  /**< \brief I2C_SLV0 �������� */
#define __I2C_SLV1_DEV_NUM              3      /**< \brief �ӻ��豸����      */

/**
 * \brief �洢�豸��ַָ��
 */
uint32_t __g_i2c_slv1_devlist[__I2C_SLV1_DEV_NUM];

/**
 * \brief I2C1 �豸��Ϣ
 */
const  struct amdr_i2c_slv_devinfo  g_i2c_slv1_devinfo = {

    /** \brief ָ��I2C1�Ĵ������ָ�� */
    AMHW_I2C1,

    /** \brief �����ٶ� */
    __I2C_BUS_SPEED_I2C_SLV1,

    /** \brief ָ��ӻ��豸��ָ�� */
    __g_i2c_slv1_devlist,

    /** \brief ʹ�õĴӻ��豸���� */
    __I2C_SLV1_DEV_NUM,

    /** \brief I2C0 �жϱ�� */
    INUM_I2C1,
};

amdr_i2c_slv_dev_t g_i2c_slv1_dev; /**< \brief I2C1 �豸ʵ�� */

/** \brief I2C1 ƽ̨��ʼ������ */
void amhw_plfm_i2c_slv1_init (void)
{
    am_gpio_pin_cfg (PIOE_0, PIOE_0_I2C1_SDA);
    am_gpio_pin_cfg (PIOE_1, PIOE_1_I2C1_SCL);

    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_I2C1);
}

/** \brief ���I2C1 ƽ̨��ʼ������ */
void amhw_plfm_i2c_slv1_deinit (void)
{
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_I2C1);
}

/** @} */

/* end of file */
