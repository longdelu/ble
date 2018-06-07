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
 * \brief KS16 DAC 用户配置文件。
 * \sa amdr_hwconfig_dac.c
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-07-14  xiongyanming, first implementation.
 * \endinternal
 */
 
#include "amdr_hwconfig_dac.h"

/**
 * \addtogroup amdr_if_hwconfig_src_dac
 * \copydoc amdr_hwconfig_dac.c
 * @{
 */

/** \brief DAC 设备信息 */
const amdr_dac_devinfo_t g_dac0_devinfo =
{
    AMHW_DAC0,                     /**< \brief 指向DAC0寄存器块的指针 */
    INUM_DAC0,                     /**< \brief DAC0中断号             */
    12,                            /**< \brief DAC转换精度            */
    3270,                          /**< \brief DAC参考电压，单位：mV  */
    AMHW_DAC_VREF_2                /**< \brief DAC电压源              */
};

/** \brief DAC0设备实例 */
amdr_dac_dev_t g_dac0_dev;

/** 
 * \brief DAC 平台初始化。
 */
void amhw_plfm_dac0_init (void)
{
    /* 配置PIOE_30为DAC0_OUT功能                 */
     am_gpio_pin_cfg (PIOE_30,PIOE_30_DAC0_OUT);

     /* 开启DAC时钟                  */
     amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_DAC0);
   
}

/** 
 * \brief DAC 平台去初始化。
 */
void amhw_plfm_dac0_deinit (void)
{
   /* 关闭DAC时钟                  */
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_DAC0);
}

/**
 * @}
 */

/* end of file */
