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
 * \brief usbcore��׼�ӿ�
 *
 * \internal
 * \par Modification History
 * - 1.00 15-11-28  lgr, first implementation.
 * \endinternal
 */
 
#ifndef __AM_USBCORE_H
#define __AM_USBCORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_pdiusbd12.h"   
    
/**
 * @addtogroup am_if_usbcore
 * @copydoc am_usbcore.h
 * @{
 */
 
    
/**
 * \name USBЭ����ض���
 * @{ 
 */      

#define GET_STATUS         0
#define CLEAR_FEATURE      1
#define SET_FEATURE        3
#define SET_ADDRESS        5
#define GET_DESCRIPTOR     6
#define SET_DESCRIPTOR     7
#define GET_CONFIGURATION  8
#define SET_CONFIGURATION  9
#define GET_INTERFACE      10
#define SET_INTERFACE      11
#define SYNCH_FRAME        12

#define DEVICE_DESCRIPTOR         1
#define CONFIGURATION_DESCRIPTOR  2
#define STRING_DESCRIPTOR         3
#define INTERFACE_DESCRIPTOR      4
#define ENDPOINT_DESCRIPTOR       5
#define REPORT_DESCRIPTOR         0x22

#define SET_IDLE 0x0A

/** @} */   



void UsbDisconnect(void); //USB�Ͽ�����
void UsbConnect(void);    //USB����
void UsbBusSuspend(void); //���߹����жϴ���
void UsbBusReset(void);   //���߸�λ�жϴ���
void UsbEp0Out(void);     //�˵�0����жϴ���
void UsbEp0In(void);      //�˵�0�����жϴ���
void UsbEp1Out(void);     //�˵�1����жϴ���
void UsbEp1In(void);      //�˵�1�����жϴ���
void UsbEp2Out(void);     //�˵�2����жϴ���
void UsbEp2In(void);      //�˵�2�����жϴ���

extern uint8_t ConfigValue;  //��ǰ����ֵ
extern uint8_t Ep1InIsBusy;  //�˵�1�����Ƿ�æ


/**
 * \brief �豸�ж�����
 * \param[in] pfn_isr   : ע����жϻص�����
 * \param[in] p_arg     : ע����жϻص������Ĳ���
 * \param[in] pin       : �ж����ţ�ֵΪD12_INT  
 * \note  D12���ж�����Ϊ��©״̬����ҪоƬ�ڲ�������������������
 * \return    ��
 */
void am_usbcore_int_cfg (am_pfnvoid_t pfn_isr, void *p_arg,int pin);

/**
 * \brief �豸�жϴ����� 
 * \param[in] flag   : �жϱ�־λ��ֵΪFLASE��TRUE��FLASE���ж�δ������TRUE���ж��ѷ���   
 * \return    ��
 */
void am_usbcore_intterupt_handle (void);



/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_USBCORE_H */

/* end of file */




