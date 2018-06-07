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
 * \brief LPC82XX ADC 用户配置文件。
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

/** \brief ADC 设备信息 （软件触发，无比较无硬件平均）*/
const amdr_adc_devinfo_t g_adc0_devinfo =
{
    AMHW_ADC0,                     /**< \brief 指向ADC0寄存器块的指针 */
    2500,                          /**< \brief ADC参考电压，单位：mV  */
    12,                            /**< \brief ADC采样精度(位数)   */
    AMDR_ADC_DIFF_DIS,             /**< \brief ADC0单端模式        */
    INUM_ADC0,                     /**< \brief ADC中断向量号  */
    AMDR_ADC_MODE_TRG_SF,          /**< \brief ADC软件触发模式      */
    AMHW_ADC_SEQ_A,                /**< \brief ADC序列索引            */
    DMA_CHAN_0,                    /**< \brief DMA通道0传输，值为-1时不使用DMA */
    NULL,                          /**< \brief 不使用硬件平均功能       */
    NULL,                          /**< \brief 不使用比较功能       */
};

/** \brief ADC0设备实例 */
amdr_adc_dev_t g_adc0_dev;

/**
 * \brief ADC 平台初始化。
 */
void amdr_plfm_adc0_init (void)
{
    /* 使能ADC时钟                  */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_ADC0);

    /* 对ADC进行校准                */
    amhw_adc_calibrate(AMHW_ADC0);

}

/**
 * \brief ADC 平台去初始化。
 */
void amdr_plfm_adc0_deinit (void)
{
    /* 关闭ADC模块上电                */
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_ADC0);
}

/**
 * @}
 */

/* end of file */
