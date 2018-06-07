/*******************************************************************************
*                                  Apollo
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2016 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief am_matrix_key��׼�ӿ�
 *
 * \internal
 * \par Modification History
 * - 1.00 16-03-19  lgr, first implementation.
 * \endinternal
 */
 
#ifndef __AM_KEY_H
#define __AM_KEY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"   
#include "am_gpio.h"
#include "amdr_gpio.h"
 
/**
 * @addtogroup am_if_am_matrix_key
 * @copydoc am_matrix_key.h
 * @{
 */
   
/**                                                                     
 * \name ���̰���ֵ����                                           
 * @{                                                                   
 */                                                                     
#define AM_KEY_NONE                          0x00   /**< \brief �����ͷ�      */                                                 
#define AM_KEY_ENTER                         0x28   /**< \brief �س���        */ 
#define AM_KEY_ESCAPE                        0x29   /**< \brief ���ؼ�        */ 
#define AM_KEY_BACKSPACE                     0x2A   /**< \brief �����        */                                
#define AM_KEY_SPACEBAR                      0x2C   /**< \brief �ո��        */
#define AM_KEY_RIGHTARROW                    0x4F   /**< \brief �����Ҽ�      */               
#define AM_KEY_LEFTARROW                     0x50   /**< \brief �������      */               
#define AM_KEY_DOWNARROW                     0x51   /**< \brief �����¼�      */ 
#define AM_KEY_UPARROW                       0x52   /**< \brief �����ϼ�      */
#define AM_MODIFERKEYS_LEFT_CTRL             0x01   /**< \brief ��CTRL��      */                 
#define AM_MODIFERKEYS_LEFT_SHIFT            0x02   /**< \brief ��SHIFT��     */                
#define AM_MODIFERKEYS_LEFT_ALT              0x04   /**< \brief ��ALT��       */                                 
                                                                      
/** @} */ 

/**                                                                     
 * \name ��갴��ֵ����                                           
 * @{                                                                   
 */ 
#define  AM_MOUSE_LEFT_CLICK                 0x01  /**< \brief Mouse Left click  */
#define  AM_MOUSE_RIGHT_CLICK                0x02  /**< \brief Mouse Right click */

/** @} */

/**                                                                     
 * \name ���������л���־                                           
 * @{                                                                   
 */ 
#define  AM_KEYBOARD_FLAG                    0xA0  /**< \brief ѡ����Ϊ����  */
#define  AM_MOUSE_FLAG                       0xA1  /**< \brief ѡ����Ϊ���  */

/** @} */

/**
 * \brief ��������豸�����Ϣ�ṹ��
 * \note  ��ѡȷ����һ��������Ϊ�����еĵ�����ʱ����������֮���ŵ��������ŷֱ�
 *        ��Ӧ��Ӧ��������֮������� 
 */
typedef struct am_matrix_key_device { 
    
    uint8_t  inum;               /**< \brief ��������ж������жϺ�  */ 
    
    am_pfnvoid_t pfn_isr;        /**< \brief ��������жϻص�����    */
       
    int32_t  row_pin;            /**< \brief ��������ж�Ӧ�� PTC0  */ 
    int32_t  col_pin;            /**< \brief ��������ж�Ӧ�� PTC4  */ 
          
} am_matrix_key_device_t;

/**
 * \brief ��������豸�ṹ��
 */
typedef struct am_matrix_key_dev {
    
    /** \brief ʹ���жϱ�־λ                  */
    uint8_t int_status; 
    
    /** \brief ��������жϻص�����            */
    am_pfnvoid_t pfn_isr;        
      
    /** \brief ָ���������豸��Ϣ��ָ��       */
    const am_matrix_key_device_t *p_devinfo;
    
} am_matrix_key_dev_t;

/** \brief ��������豸����������Ͷ��� */
typedef struct am_matrix_key_dev *am_matrix_key_handle_t;

extern uint8_t g_mouse_or_key_flag ; /**< \brief ѡ���������̱�־��Ĭ��Ϊ��� */

extern uint8_t g_mouse_button[2];    /**< \brief ����ֵ */
extern uint8_t g_key_board[8];       /**< \brief ���̼�ֵ */
  
/**
 * \brief  ���󰴼���ʼ��
 *
 * \param[in] p_dev     : ��������豸�ṹ��ָ��
 * \param[in] p_devinfo : ��������豸��Ϣ�ṹ��
 *
 * \return  ��������豸�Ĳ�����������ֵΪNULL��������ʼ��ʧ��
 */
am_matrix_key_handle_t am_matrix_key_init (am_matrix_key_dev_t           *p_dev, 
                                           const am_matrix_key_device_t  *p_devinfo);
                                                                                   
/**
 * \brief ������󰴼���ʼ��
 *
 * \param[in] p_dev  : ��������豸�ṹ��ָ��
 *
 * \return    None
 */
void am_matrix_key_deinit (am_matrix_key_dev_t  *p_dev);

/**
 * \brief  ɨ��õ�����״̬
 *
 * \param[in] handle : ��������豸���������ָ��  
 *
 * \return ����ɨ��õ�����״̬�����ް���������0x0 
 */
uint16_t am_matrix_key_scan (am_matrix_key_handle_t  handle);

/**
 * \brief  �жϻ�����������
 *
 * \param[in] handle : ��������豸���������ָ��  
 *
 * \return �� 
 */
void am_int_wake_up_cfg (am_matrix_key_handle_t  handle, am_pfnvoid_t  pfn_isr) ; 

/**
 * \brief  ʹ���жϻ�����������
 *
 * \param[in] handle : ��������豸���������ָ��  
 *
 * \return �� 
 */
void am_int_wake_up_enable(am_matrix_key_handle_t handle);

/**
 * \brief  �����жϻ�������
 *
 * \param[in] handle : ��������豸���������ָ��  
 *
 * \return �� 
 */
void am_int_wake_up_disable(am_matrix_key_handle_t handle);

/** 
 * \brief  ���������жϻص��ú���
 * \param  p_arg : ������ָ��
 *
 * \return �� 
 */
void am_int_wake_up_cal (void *p_arg); 

/**
 * \brief  ���ɨ��õ���ֵ����������
 *
 * \param[in] handle : ��������豸���������ָ��  
 * \param[in] count  : �����ظ�ɨ��Ĵ�����һ��ȡֵΪ2 ~ 10
 *
 * \return ����ɨ�赽�ļ�ֵ�������ж������򷵻�ֵΪ-AM_ERROR 
 */

uint16_t am_matrix_key_scan_repeat (am_matrix_key_handle_t  handle, uint8_t  count);

/**
 * \brief   ���Ͱ���״̬Ϊ�������Ӧ��ֵ  
 *
 * \param[in] handle  :  ��������豸���������ָ��  
 * \param[in] key_avl :  ����״̬�����ֵ
 *
 * \return �� 
 */
void am_matrix_key_value_def (am_matrix_key_handle_t  handle, uint16_t  key_avl);
    
    
/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_KEY_H */

/* end of file */
