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
 * \brief KS16z 用于捕获功能的用户配置文件
 * \sa amdr_hwconfig_tpm_cap.c
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  tee, first implementation.
 * \endinternal
 */

#include "amdr_hwconfig_tpm_cap.h"

/**
 * \addtogroup amdr_if_hwconfig_src_tpm_cap
 * \copydoc amdr_hwconfig_tpm_cap.c
 * @{
 */
 
/*******************************************************************************
 * TPM0 配置
 ******************************************************************************/

/** \brief TPM0用于捕获功能的引脚配置信息列表 */
amdr_tpm_cap_ioinfo_t __g_tpm0_cap_ioinfo_list[] = {
    {PIOD_0, PIOD_0_TPM0_CH0, PIOD_0_GPIO | AM_GPIO_INPUT}, /**< \brief 通道 0 */
    {PIOD_1, PIOD_1_TPM0_CH1, PIOD_1_GPIO | AM_GPIO_INPUT}, /**< \brief 通道 1 */
    {PIOD_2, PIOD_2_TPM0_CH2, PIOD_2_GPIO | AM_GPIO_INPUT}, /**< \brief 通道 2 */
    {PIOD_3, PIOD_3_TPM0_CH3, PIOD_3_GPIO | AM_GPIO_INPUT}, /**< \brief 通道 3 */
    {PIOD_4, PIOD_4_TPM0_CH4, PIOD_4_GPIO | AM_GPIO_INPUT}, /**< \brief 通道 4 */
    {PIOD_5, PIOD_5_TPM0_CH5, PIOD_5_GPIO | AM_GPIO_INPUT}, /**< \brief 通道 5 */
};

/** \brief TPM0用于捕获功能的设备信息 */
const amdr_tpm_cap_devinfo_t  g_tpm0_cap_devinfo = {
    AMHW_TPM0,                 /**< \brief 指向TPM0寄存器块的指针 */
    INUM_TPM0,                 /**< \brief TPM0中断编号 */
    6,                         /**< \brief 6个捕获通道  */
    &__g_tpm0_cap_ioinfo_list[0],
};

/** \brief TPM0用于捕获功能的设备定义 */
amdr_tpm_cap_dev_t  g_tpm0_cap_dev;

/** \brief TPM0用于捕获功能的平台初始化 */
void amhw_plfm_tpm0_cap_init (void)
{
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_TPM0);
    amhw_sim_tpm_src_set(AMHW_SIM_TPMSRC_PLLFLLCLK);
}

/** \brief  解除TPM0平台初始化 */
void amhw_plfm_tpm0_cap_deinit (void)
{
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_TPM0);
}

/*******************************************************************************
 * TPM1 配置
 ******************************************************************************/

/** \brief TPM1用于捕获功能的引脚配置信息列表 */
amdr_tpm_cap_ioinfo_t __g_tpm1_cap_ioinfo_list[] = {
    {PIOE_20, PIOE_20_TPM1_CH0, PIOE_20_GPIO | AM_GPIO_INPUT},  /**< \brief 通道0 */
    {PIOE_21, PIOE_21_TPM1_CH1, PIOE_21_GPIO | AM_GPIO_INPUT},  /**< \brief 通道1 */
};

/** \brief TPM1用于捕获功能的设备信息 */
const amdr_tpm_cap_devinfo_t  g_tpm1_cap_devinfo = {
    AMHW_TPM1,                 /**< \brief 指向TPM1寄存器块的指针 */
    INUM_TPM1,                 /**< \brief TPM1中断编号 */
    2,                         /**< \brief 2个捕获通道  */
    &__g_tpm1_cap_ioinfo_list[0],
};

/** \brief TPM1用于捕获功能的设备定义 */
amdr_tpm_cap_dev_t  g_tpm1_cap_dev;

/** \brief TPM1用于捕获功能的平台初始化 */
void amhw_plfm_tpm1_cap_init (void)
{
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_TPM1);
    amhw_sim_tpm_src_set(AMHW_SIM_TPMSRC_PLLFLLCLK);
}

/** \brief  解除TPM1平台初始化 */
void amhw_plfm_tpm1_cap_deinit (void)
{
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_TPM1);
}

/*******************************************************************************
 * TPM2 配置
 ******************************************************************************/

/** \brief TPM2用于捕获功能的引脚配置信息列表 */
amdr_tpm_cap_ioinfo_t __g_tpm2_cap_ioinfo_list[] = {
    {PIOE_22, PIOE_22_TPM2_CH0, PIOE_22_GPIO | AM_GPIO_INPUT},  /**< \brief 通道0 */
    {PIOE_23, PIOE_23_TPM2_CH1, PIOE_23_GPIO | AM_GPIO_INPUT},  /**< \brief 通道1 */
};

/** \brief TPM2用于捕获功能的设备信息 */
const amdr_tpm_cap_devinfo_t  g_tpm2_cap_devinfo = {
    AMHW_TPM2,                 /**< \brief 指向TPM2寄存器块的指针 */
    INUM_TPM2,                 /**< \brief TPM2中断编号 */
    2,                         /**< \brief 2个捕获通道  */
    &__g_tpm2_cap_ioinfo_list[0],
};

/** \brief TPM2用于捕获功能的设备定义 */
amdr_tpm_cap_dev_t  g_tpm2_cap_dev;

/** \brief TPM2用于捕获功能的平台初始化 */
void amhw_plfm_tpm2_cap_init (void)
{
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_TPM2);
    amhw_sim_tpm_src_set(AMHW_SIM_TPMSRC_PLLFLLCLK);
}

/** \brief  解除TPM2平台初始化 */
void amhw_plfm_tpm2_cap_deinit (void)
{
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_TPM2);
}

/**
 * @}
 */

/* end of file */
