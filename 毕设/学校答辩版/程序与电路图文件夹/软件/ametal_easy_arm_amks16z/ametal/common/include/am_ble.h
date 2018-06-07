/*******************************************************************************
*                                  Apollo
*                       ----------------------------
*                       innovating embedded platform
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
 * \brief �͹�������BLE��׼�ӿ�
 *
 * \internal
 * \par Modification History
 * - 1.00 15-11-25  lgr, first implementation.
 * \endinternal
 */
 
#ifndef __AM_BLE_H
#define __AM_BLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"   
#include "am_gpio.h"
#include "am_uart.h"
#include "am_uart_int.h"  
#include "amdr_uart.h"
    
/**
 * \name �͹�������BELָ��
 * \note ���ڷ��͵�ָ��ֻ��������ʾ����ʱ��ͬʱ��ָ��ʾβ����\r\n
 * @{ 
 */      
//#define AM_BLE_AT_TEST       AT                 /**< \brief BLE�Բ���ָ��           */  
//#define AM_BLE_AT_DEFAULT    AT+DEFAULT         /**< \brief BLE�ָ���������ָ��     */     
//#define AM_BLE_AT_RESET      AT+RESET           /**< \brief BLE��λָ��             */ 
//#define AM_BLE_AT_VERSION    AT+VERSION         /**< \brief BLE��ȡ����汾ָ��     */ 
//#define AM_BLE_AT_NAME       AT+NAME            /**< \brief BLE��ȡ�����豸����ָ�� */ 
//#define AM_BLE_AT_NAME(X)    AT+NAMEX           /**< \brief BLE���������豸����ָ�� */ 
//#define AM_BLE_AT_LADDR      AT+LADDR           /**< \brief BLE��ȡ����ģ���ַָ�� */   
//#define AM_BLE_AT_PIN(X)     AT+PINX            /**< \brief BLE�������������       */ 
//#define AM_BLE_AT_PIN(       AT+PIN             /**< \brief BLE��ȡ���������       */ 
//#define AM_BLE_AT_BAUD(X)    AT+BAUDX           /**< \brief BLE���ò�����ָ��       */ 
//#define AM_BLE_AT_BAUD       AT+BAUD            /**< \brief BLE��ȡ������ָ��       */ 
//#define AM_BLE_AT_STOP(X)    AT+STOPX           /**< \brief BLE���ò�����ֹͣλ     */ 
//#define AM_BLE_AT_STOP       AT+STOP            /**< \brief BLE��ȡ������ֹͣλ     */ 
//#define AM_BLE_AT_PARI(X)    AT+PARIX           /**< \brief BLE���ò�����У��λ     */
//#define AM_BLE_AT_PARI       AT+PARI            /**< \brief BLE��ȡ������У��λ     */
//#define AM_BLE_AT_PWRM(X)    AT+PWRMX           /**< \brief BEL�����Ƿ����͹���   */ 
//#define AM_BLE_AT_PWRM       AT+PWRM            /**< \brief BEL��ȡ�Ƿ����͹���   */
//#define AM_BLE_AT_SLEEP      AT+SLEEP           /**< \brief BLE����͹���ģʽָ��   */     
//#define AM_BLE_AT_ROLE(X)    AT+ROLEX           /**< \brief BEL��������ģʽָ��     */ 
//#define AM_BLE_AT_ROLE       AT+ROLE            /**< \brief BEL��ȡ������ģʽ״̬   */ 
//#define AM_BLE_AT_INQ        AT+INQ             /**< \brief BLE��ģʽ���������豸   */ 
//#define AM_BLE_AT_SHOW       AT+SHOW            /**< \brief BLE��ģʽ��ʾ�������������豸*/
//#define AM_BLE_AT_CONN(X)    AT+CONNX           /**< \brief BLE��ģʽ������Զ���豸 */ 
//#define AM_BLE_AT_POWE(X)    AT+POWEX           /**< \brief BLE����ģ�鹦��         */ 
//#define AM_BLE_AT_POWE       AT+POWE            /**< \brief BLE��ȡ����״̬         */      
//#define AM_BLE_AT_HELP       AT+HELP            /**< \brief BLE����ָ��             */ 

#define AM_BLE_MASTER_MODE_SET       1            /**< \brief BLE��ģʽ             */      
#define AM_BLE_SLAVE_MODE_SET        0            /**< \brief BLE��ģʽ             */ 
    
/** @} */       
    
/**
 * \brief BLE�豸�����Ϣ�ṹ��
 * \note  ֻ�п�����Ӧ�ı���ʹ�ܿ����󣬲����趨��ֵ�Ż�������
 */
typedef struct am_ble_device {  
    uint8_t  ble_master_slave;            /**< \brief BLE�豸����ģʽ����             */ 
    uint8_t  *ble_pin;                    /**< \brief BLE�豸���������               */  
    uint8_t  interrupt_pin;               /** \brief ����BLE�豸���ж�����            */                                            
} am_ble_device_t;
  
/**
 * \brief BLE�豸�ṹ��
 */
typedef struct am_ble_dev {
    
    /** \brief BLEʹ��I2C��׼����������  */
    am_uart_int_handle_t   handle;
    
    /** \brief ָ��BLE�豸��Ϣ��ָ��       */
    const am_ble_device_t *p_devinfo;
    
    
} am_ble_dev_t;

/** \brief BLE�豸����������Ͷ��� */
typedef struct am_ble_dev *am_ble_handle_t;    

/**
 * \brief BLE�Բ�������
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT\r\n"                       
 * \return    ��
 */
void am_ble_at_selftest (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE�ָ���������
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+DEFAULT\r\n"                       
 * \return    ��
 */
void am_ble_at_default_set (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE��λ
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+RESET\r\n"                       
 * \return    ��
 */
void am_ble_at_reset (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE��ȡ����汾
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+VERSION\r\n"                       
 * \return    ��
 */
void am_ble_at_version_get (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE��ȡ�����豸����ַ
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+LADDR\r\n"                     
 * \return    ��
 */
void am_ble_at_laddr_set (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE��ȡ�����豸����
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+NAME\r\n"                       
 * \return    ��
 */
void am_ble_at_name_get (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE���������豸����
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+NAME<x>\r\n" ������xΪ�����豸������                      
 * \return    ��
 */
void am_ble_at_name_set (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE��ȡ�����豸�����
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+PIN\r\n"                     
 * \return    ��
 */
void am_ble_at_pin_get (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE���������豸�����
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+PIN<x>\r\n" ������xΪ�����豸�������,�� 000000                     
 * \return    ��
 */
void am_ble_at_pin_set (am_ble_handle_t handle, const uint8_t *p_txbuf); 


/**
 * \brief BLE��ȡ�����豸������
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+BAUD\r\n"                     
 * \return    ��
 */
void am_ble_at_baud_get (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE���������豸�Ĳ�����
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+BAUD<x>\r\n" ������xֵΪ0��1��2��3��4��5��6��7��8
 *                      4: 9600������ ��8: 115200������ 
 * \return    ��
 */
void am_ble_at_baud_set (am_ble_handle_t handle, const uint8_t *p_txbuf); 


/**
 * \brief BLE��ȡ�����豸�����ʵ�ֹͣλ
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+STOP\r\n"                     
 * \return    ��
 */
void am_ble_at_stop_get (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE���������豸�Ĳ����ʵ�ֹͣλ
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+STOP<x>\r\n" ������xֵΪ0��1
 *                      0: 1λֹͣλ ��1: 2λֹͣλ 
 * \return    ��
 */
void am_ble_at_stop_set (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE��ȡ�����豸�����ʵ�У�鷽ʽ
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+PARI\r\n"                     
 * \return    ��
 */
void am_ble_at_pari_get (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE���������豸�Ĳ����ʵ�ֹͣλ
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+PARI<x>\r\n" ������xֵΪ0��1��2
 *                      0: ��У�� ��1: ��У�� ��2��żУ�� 
 * \return    ��
 */
void am_ble_at_pari_set (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE��ȡ�����豸�Ƿ���빦��ģʽ
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+PWRM\r\n"                     
 * \return    ��
 */
void am_ble_at_pwrm_get (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE���������ϵ��Ƿ����͹���ģʽ
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+PWRM<x>\r\n" ������xֵΪ0��1
 *                      0: ����͹��ģ�1: ��������
 * \return    ��
 */
void am_ble_at_pwrm_set (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE������������͹���ģʽ
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+SLEEP\r\n" 
 * \return    ��
 */
void am_ble_at_sleep_set (am_ble_handle_t handle, const uint8_t *p_txbuf); 
    
/**
 * \brief BLE��ȡ�����豸����ģʽ
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+ROLE\r\n"                     
 * \return    ��
 */
void am_ble_at_role_get (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE��������������ģʽ
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+ROLE<x>\r\n" ������xֵΪ0��1
 *                      0: ���豸��1�����豸
 * \return    ��
 */
void am_ble_at_role_set (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE��������ģʽ�����������豸
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+INQ\r\n" 
 * \return    ��
 */
void am_ble_at_inq (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE��������ģʽ����ʾ�������������豸
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+SHOW\r\n" 
 * \return    ��
 */
void am_ble_at_show (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE��������ģʽ�����������豸
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+CONN<x>\r\n" ����xֵΪ0��1�� 2�ȣ�
 *                      �����������������豸
 * \return    ��
 */
void am_ble_at_conn (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE��ȡ�����豸�Ĺ���ģʽ
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+POWE\r\n"                     
 * \return    ��
 */
void am_ble_at_powe_get (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE���������豸�Ĺ���ģʽ
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+POWE<x>\r\n" ������xֵΪ0��1, 2, 3
 *                      0: -23dB��1��-6dB, 2: 0dB, 3: 4dB
 * \return    ��
 */
void am_ble_at_powe_set (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE�����鿴������Ϣ
 * \param[in] handle  : ָ��BLE�豸���������ָ��
 * \param[in] p_txbuf : ��Ҫ���͵����ݣ�ֵΪ "AT+HELP\r\n" 
 * \return    ��
 */
void am_ble_at_help (am_ble_handle_t handle, const uint8_t *p_txbuf); 


/** \brief BLE������ģʽ����  */
void am_ble_master_set (am_ble_handle_t handle);

/** \brief BLE������ģʽ����  */
void am_ble_slave_set (am_ble_handle_t handle);

/**
 * \brief BLE�ж�����
 * \param[in] handle : ָ��MMA8451�豸���������ָ��   
 * \param[in] pfn_isr   : ע����жϻص�����
 * \param[in] p_arg     : ע����жϻص������Ĳ��� 
 * \return    ����ϵͳģʽ�Ĵ�����ֵ
 */
void am_ble_int_cfg (am_ble_handle_t handle, am_pfnvoid_t pfn_isr, void *p_arg);


/**
 * \brief ��ʼ��BLE
 *
 * \param[in] p_dev     : BLE�豸�ṹ��ָ��
 * \param[in] p_devinfo : BLE�豸��Ϣ�ṹ�� 
 * \param[in] handle    : UART�жϱ�׼����������
 *
 * \return    BLE�Ĳ�����������ֵΪNULL��������ʼ��ʧ��
 */
am_ble_handle_t am_ble_init (am_ble_dev_t *p_dev,
                             const am_ble_device_t *p_devinfo,
                             am_uart_int_handle_t   handle);
                                                                  
                                 
/**
 * \brief ���BLE��ʼ��
 *
 * \param[in] p_dev  : BLE�豸�ṹ��ָ��
 *
 * \return    None
 */
void am_ble_deinit (am_ble_dev_t *p_dev);


    
/**
 * @addtogroup am_if_ble
 * @copydoc am_ble.h
 * @{
 */    

/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_BLE_H */

/* end of file */















