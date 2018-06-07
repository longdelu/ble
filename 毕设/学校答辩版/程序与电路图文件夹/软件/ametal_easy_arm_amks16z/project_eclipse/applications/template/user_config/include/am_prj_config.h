/*******************************************************************************
*                                 AMetal
*                       ---------------------------
*                       innovating embedded systems
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief 工程配置文件，包括系统，板级和硬件配置
 * \sa am_prj_config.h
 * \internal
 * \par Modification history
 * - 1.00 15-10-23  link, first implementation.
 * \endinternal
 */

#ifndef __AM_PRJ_CONFIG_H
#define __AM_PRJ_CONFIG_H

#include "ametal.h"

/**
 * \addtogroup am_if_prj_config
 * \copydoc am_prj_config.h
 * @{
 */

/**
 * \name 板级初始化配置
 * 如果不使用任何的板上资源，可以将下列宏定义的值设置为0
 * @{
 */

/** \brief 如果为1，则初始化延时函数，初始化后可以使用am_mdelay() 和 am_udelay()延时函数 */
#define AM_CFG_DELAY_ENABLE          1

/** \brief 如果为1，将初始化中断系统，初始化后可以使用am_int_connect()等中断标准接口函数 */
#define AM_CFG_INT_ENABLE            1

/** \brief 如果为1，则初始化串口调试                */
#define AM_CFG_DEBUG_ENABLE          1

/** \brief 调试串口选择，0 - USART0, 1 - USART1...  */
#define AM_CFG_DEBUG_USART           1

/** \brief 调试串口波特率设置                */
#define AM_CFG_DEBUG_BAUDRATE       (115200)

/** \brief 如果为1，则初始化led的相关功能    */
#define AM_CFG_LED_ENABLE            1

/** \brief 如果为1，则初始化蜂鸣器的相关功能 */
#define AM_CFG_BUZZER_ENABLE         0

/** \brief 蜂鸣器的PWM频率              */
#define AM_CFG_BUZZER_PWM_FREQ       2500

/** \brief PWM的占空比，50代表50%       */
#define AM_CFG_BUZZER_PWM_DUTY       50

/** \brief 如果为1，使能看门狗功能 */
#define AM_CFG_SIM_COP               0

/** @} */

#include "amdr_hwconfig_clk.h"
#include "amdr_hwconfig_int.h"
#include "amdr_hwconfig_uart.h"
#include "amdr_hwconfig_gpio.h"
#include "amdr_hwconfig_dac.h"
#include "amdr_hwconfig_adc.h"
#include "amdr_hwconfig_tpm_timing.h"
#include "amdr_hwconfig_tpm_pwm.h"
#include "amdr_hwconfig_tpm_cap.h"
#include "amdr_hwconfig_spi.h"
#include "amdr_hwconfig_wdt.h"
#include "amdr_hwconfig_dma.h"
#include "amdr_hwconfig_rtc.h"
#include "amdr_hwconfig_pit_timing.h"
#include "amdr_hwconfig_tsi.h"
#include "amdr_hwconfig_lptmr_timing.h"
#include "amdr_hwconfig_pmu.h"
#include "amdr_hwconfig_i2c.h"
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* __AM_PRJ_CONFIG_H */

/* end of file */
