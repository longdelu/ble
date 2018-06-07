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
 * \brief PDIUSBD12��׼�ӿ�
 *
 * \internal
 * \par Modification History
 * - 1.00 15-11-28  lgr, first implementation.
 * \endinternal
 */
 
#ifndef __AM_PDIUSBD12_H
#define __AM_PDIUSBD12_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"   
#include "am_soc.h"
#include "am_delay.h"    
#include "am_gpio.h"
#include "am_uart.h"
#include "am_uart_int.h"  
#include "amdr_usart.h"
      
/**
 * @addtogroup am_if_pdiusbd12
 * @copydoc am_pdiusbd12.h
 * @{
 */    

/**
 * \name PDIUSBD12ָ��
 * @{ 
 */      

/* D12�Ķ�ID���� */
#define Read_ID                   0xFD

/* D12�Ķ�ID���� */
#define Read_ID                   0xFD

/* D12������ģʽ���� */
#define D12_SET_MODE              0xF3

/* D12�Ķ��жϼĴ������� */
#define READ_INTERRUPT_REGISTER   0xF4

/* D12���˵㻺���������� */
#define D12_READ_BUFFER           0xF0

/* D12д�˵㻺���������� */
#define D12_WRITE_BUFFER          0xF0

/* D12������ն˵㻺���������� */
#define D12_CLEAR_BUFFER          0xF2

/* D12ʹ�ܷ��Ͷ˵㻺���������� */
#define D12_VALIDATE_BUFFER       0xFA

/* D12��Ӧ�����ð����� */
#define D12_ACKNOWLEDGE_SETUP     0xF1

/* D12�����õ�ַ/ʹ������ */
#define D12_SET_ADDRESS_ENABLE    0xD0

/* D12��ʹ�ܶ˵����� */
#define D12_SET_ENDPOINT_ENABLE   0xD8

/** @} */       
    
/**
 * \name PDIUSBD12������������
 * @{ 
 */      

/* �����ַ�����ݵ�ַ    */
#define D12_COMMAND_ADD           1
#define D12_DATA_ADD              0

/* PDIUSBD12оƬ�������� */
#define D12_DATA                  ((amhw_gpio_port_get(AMHW_GPIO, AMHW_GPIO_PORT_0))>>20)
#define D12_A0                    PIO0_19
#define D12_WR                    PIO0_13
#define D12_RD                    PIO0_17
#define D12_INT                   PIO0_6

/* ѡ����������ݵ�ַ */
#define D12SetCommandAddr() am_gpio_set(D12_A0, D12_COMMAND_ADD)
#define D12SetDataAddr()    am_gpio_set(D12_A0, D12_DATA_ADD)

/* WR����       */
#define D12SetWr() am_gpio_set(D12_WR, 1)
#define D12ClrWr() am_gpio_set(D12_WR, 0)
/* RD����       */
#define D12SetRd() am_gpio_set(D12_RD, 1)
#define D12ClrRd() am_gpio_set(D12_RD, 0)

/* ��ȡ�ж�״̬ */
#define D12GetIntPin()  am_gpio_get(D12_INT)


/* ��д����     */
#define D12GetData()       D12_DATA
#define D12SetData(Value)  amhw_gpio_port_set(AMHW_GPIO, AMHW_GPIO_PORT_0, Value << 20)

/* �����ݿ�����Ϊ����״̬��51��Ƭ���˿�д1����Ϊ����״̬ */
#define D12SetPortIn()    //amhw_gpio_port_in(AMHW_GPIO, AMHW_GPIO_PORT_0, 0x0ff00000)

/* �����ݿ�����Ϊ���״̬������51��Ƭ����׼˫��IO�ڣ����Բ����л���Ϊ�պ� */
#define D12SetPortOut()   amhw_gpio_port_out(AMHW_GPIO, AMHW_GPIO_PORT_0, 0x0ff00000)

/** @} */    

/* �������� */
void D12WriteCommand (uint8_t Command);


uint8_t D12ReadByte (void);


uint16_t D12ReadID (void);

/**
 * \brief PDIUSBD12��ʾID���� 
 * \param[in] handle : ��                
 * \return    ��
 */
void am_pdiusbd12_id_show(void);


/*************************************************************************** 
  ȦȦ����ѧUSB������ֵ
***************************************************************************/

void D12WriteByte(uint8_t Value);

void DelayXms(uint16_t x);

uint8_t D12ReadEndpointBuffer(uint8_t Endp, uint8_t Len, uint8_t *Buf);

uint8_t D12WriteEndpointBuffer(uint8_t Endp, uint8_t Len, uint8_t *Buf);

void D12ClearBuffer(void);

uint8_t D12ReadEndpointLastStatus(uint8_t Endp);

void D12AcknowledgeSetup(void);

void D12SetAddress(uint8_t Addr);

void D12SetEndpointEnable(uint8_t Enable);




/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_PDIUSBD12_H */

/* end of file */    
