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
 * \brief KS16 UART�û�����ͷ�ļ�
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
 * \brief UART0ƽ̨��ʼ����
 * \return �ޡ�
 */
void amhw_plfm_uart0_init(void);

/**
 * \brief ���UART0ƽ̨��ʼ����
 * \return �ޡ�
 */
void amhw_plfm_uart0_deinit(void);
    
/** \brief UART0�豸��Ϣ */    
extern const amdr_uart_devinfo_t g_uart0_devinfo;
    
/** \brief UART0�豸     */    
extern amdr_uart_dev_t           g_uart0_dev;     



/**
 * \brief UART1ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_uart1_init(void);

/**
 * \brief ���UART1ƽ̨��ʼ��
 * \return ��
 */
void amh_platform_uart1_deinit(void);

/** \brief UART1�豸��Ϣ */    
extern const amdr_uart_devinfo_t g_uart1_devinfo;
    
/** \brief UART1�豸     */    
extern amdr_uart_dev_t           g_uart1_dev; 


/**
 * \brief UART2ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_uart2_init(void);

/**
 * \brief ���UART2ƽ̨��ʼ��
 * \return ��
 */
void amh_platform_uart2_deinit(void);

/** \brief UART2�豸��Ϣ */    
extern const amdr_uart_devinfo_t g_uart2_devinfo;
    
/** \brief UART2�豸     */    
extern amdr_uart_dev_t           g_uart2_dev; 

#ifdef __cplusplus
}
#endif

#endif  /* __AMDR_HWCONFIG_UART_H */

/* end of file */
