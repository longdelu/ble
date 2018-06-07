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
 * \brief ���������ļ�������ϵͳ���弶��Ӳ������
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
 * \name �弶��ʼ������
 * �����ʹ���κεİ�����Դ�����Խ����к궨���ֵ����Ϊ0
 * @{
 */

/** \brief ���Ϊ1�����ʼ����ʱ��������ʼ�������ʹ��am_mdelay() �� am_udelay()��ʱ���� */
#define AM_CFG_DELAY_ENABLE          1

/** \brief ���Ϊ1������ʼ���ж�ϵͳ����ʼ�������ʹ��am_int_connect()���жϱ�׼�ӿں��� */
#define AM_CFG_INT_ENABLE            1

/** \brief ���Ϊ1�����ʼ�����ڵ���                */
#define AM_CFG_DEBUG_ENABLE          1

/** \brief ���Դ���ѡ��0 - USART0, 1 - USART1...  */
#define AM_CFG_DEBUG_USART           1

/** \brief ���Դ��ڲ���������                */
#define AM_CFG_DEBUG_BAUDRATE       (115200)

/** \brief ���Ϊ1�����ʼ��led����ع���    */
#define AM_CFG_LED_ENABLE            1

/** \brief ���Ϊ1�����ʼ������������ع��� */
#define AM_CFG_BUZZER_ENABLE         0

/** \brief ��������PWMƵ��              */
#define AM_CFG_BUZZER_PWM_FREQ       2500

/** \brief PWM��ռ�ձȣ�50����50%       */
#define AM_CFG_BUZZER_PWM_DUTY       50

/** \brief ���Ϊ1��ʹ�ܿ��Ź����� */
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
