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
 * \brief usbcore标准接口
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
 * \name USB协议相关定义
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



void UsbDisconnect(void); //USB断开连接
void UsbConnect(void);    //USB连接
void UsbBusSuspend(void); //总线挂起中断处理
void UsbBusReset(void);   //总线复位中断处理
void UsbEp0Out(void);     //端点0输出中断处理
void UsbEp0In(void);      //端点0输入中断处理
void UsbEp1Out(void);     //端点1输出中断处理
void UsbEp1In(void);      //端点1输入中断处理
void UsbEp2Out(void);     //端点2输出中断处理
void UsbEp2In(void);      //端点2输入中断处理

extern uint8_t ConfigValue;  //当前配置值
extern uint8_t Ep1InIsBusy;  //端点1输入是否忙


/**
 * \brief 设备中断配置
 * \param[in] pfn_isr   : 注册的中断回调函数
 * \param[in] p_arg     : 注册的中断回调函数的参数
 * \param[in] pin       : 中断引脚，值为D12_INT  
 * \note  D12的中断引脚为开漏状态，需要芯片内部上拉或外拉上拉电阻
 * \return    无
 */
void am_usbcore_int_cfg (am_pfnvoid_t pfn_isr, void *p_arg,int pin);

/**
 * \brief 设备中断处理函数 
 * \param[in] flag   : 中断标志位，值为FLASE或TRUE，FLASE：中断未发生，TRUE：中断已发生   
 * \return    无
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




