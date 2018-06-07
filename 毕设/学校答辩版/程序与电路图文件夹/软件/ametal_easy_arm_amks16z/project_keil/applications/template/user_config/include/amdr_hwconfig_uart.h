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
 * \brief KS16 UART用户配置头文件
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-10-20  xym, first implementation.
 * \endinternal
 */
 
#ifndef __AMDR_HWCONFIG_UART_H
#define __AMDR_HWCONFIG_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
    
/**
 * \brief UART0平台初始化。
 * \return 无。
 */
void amhw_plfm_uart0_init(void);

/**
 * \brief 解除UART0平台初始化。
 * \return 无。
 */
void amhw_plfm_uart0_deinit(void);
    
/** \brief UART0设备信息 */    
extern const amdr_uart_devinfo_t g_uart0_devinfo;
    
/** \brief UART0设备     */    
extern amdr_uart_dev_t           g_uart0_dev;     



/**
 * \brief UART1平台初始化
 * \return 无
 */
void amhw_plfm_uart1_init(void);

/**
 * \brief 解除UART1平台初始化
 * \return 无
 */
void amh_platform_uart1_deinit(void);

/** \brief UART1设备信息 */    
extern const amdr_uart_devinfo_t g_uart1_devinfo;
    
/** \brief UART1设备     */    
extern amdr_uart_dev_t           g_uart1_dev; 


/**
 * \brief UART2平台初始化
 * \return 无
 */
void amhw_plfm_uart2_init(void);

/**
 * \brief 解除UART2平台初始化
 * \return 无
 */
void amh_platform_uart2_deinit(void);

/** \brief UART2设备信息 */    
extern const amdr_uart_devinfo_t g_uart2_devinfo;
    
/** \brief UART2设备     */    
extern amdr_uart_dev_t           g_uart2_dev; 

#ifdef __cplusplus
}
#endif

#endif  /* __AMDR_HWCONFIG_UART_H */

/* end of file */
