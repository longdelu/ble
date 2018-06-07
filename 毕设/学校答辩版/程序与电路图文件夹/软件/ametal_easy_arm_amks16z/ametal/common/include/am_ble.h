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
 * \brief 低功耗蓝牙BLE标准接口
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
 * \name 低功耗蓝牙BEL指令
 * \note 串口发送的指令只能在蓝牙示连接时，同时在指令示尾加上\r\n
 * @{ 
 */      
//#define AM_BLE_AT_TEST       AT                 /**< \brief BLE自测试指令           */  
//#define AM_BLE_AT_DEFAULT    AT+DEFAULT         /**< \brief BLE恢复出厂设置指令     */     
//#define AM_BLE_AT_RESET      AT+RESET           /**< \brief BLE复位指令             */ 
//#define AM_BLE_AT_VERSION    AT+VERSION         /**< \brief BLE获取软件版本指令     */ 
//#define AM_BLE_AT_NAME       AT+NAME            /**< \brief BLE获取蓝牙设备名称指令 */ 
//#define AM_BLE_AT_NAME(X)    AT+NAMEX           /**< \brief BLE设置蓝牙设备名称指令 */ 
//#define AM_BLE_AT_LADDR      AT+LADDR           /**< \brief BLE获取蓝牙模块地址指令 */   
//#define AM_BLE_AT_PIN(X)     AT+PINX            /**< \brief BLE设置蓝牙配对码       */ 
//#define AM_BLE_AT_PIN(       AT+PIN             /**< \brief BLE获取蓝牙配对码       */ 
//#define AM_BLE_AT_BAUD(X)    AT+BAUDX           /**< \brief BLE设置波特率指令       */ 
//#define AM_BLE_AT_BAUD       AT+BAUD            /**< \brief BLE获取波特率指令       */ 
//#define AM_BLE_AT_STOP(X)    AT+STOPX           /**< \brief BLE设置波特率停止位     */ 
//#define AM_BLE_AT_STOP       AT+STOP            /**< \brief BLE获取波特率停止位     */ 
//#define AM_BLE_AT_PARI(X)    AT+PARIX           /**< \brief BLE设置波特率校验位     */
//#define AM_BLE_AT_PARI       AT+PARI            /**< \brief BLE获取波特率校验位     */
//#define AM_BLE_AT_PWRM(X)    AT+PWRMX           /**< \brief BEL设置是否进入低功耗   */ 
//#define AM_BLE_AT_PWRM       AT+PWRM            /**< \brief BEL获取是否进入低功耗   */
//#define AM_BLE_AT_SLEEP      AT+SLEEP           /**< \brief BLE进入低功耗模式指令   */     
//#define AM_BLE_AT_ROLE(X)    AT+ROLEX           /**< \brief BEL设置主从模式指令     */ 
//#define AM_BLE_AT_ROLE       AT+ROLE            /**< \brief BEL获取是主从模式状态   */ 
//#define AM_BLE_AT_INQ        AT+INQ             /**< \brief BLE主模式搜索蓝牙设备   */ 
//#define AM_BLE_AT_SHOW       AT+SHOW            /**< \brief BLE主模式显示搜索到的蓝牙设备*/
//#define AM_BLE_AT_CONN(X)    AT+CONNX           /**< \brief BLE主模式下连接远端设备 */ 
//#define AM_BLE_AT_POWE(X)    AT+POWEX           /**< \brief BLE设置模块功率         */ 
//#define AM_BLE_AT_POWE       AT+POWE            /**< \brief BLE获取功率状态         */      
//#define AM_BLE_AT_HELP       AT+HELP            /**< \brief BLE帮助指令             */ 

#define AM_BLE_MASTER_MODE_SET       1            /**< \brief BLE主模式             */      
#define AM_BLE_SLAVE_MODE_SET        0            /**< \brief BLE从模式             */ 
    
/** @} */       
    
/**
 * \brief BLE设备相关信息结构体
 * \note  只有开启相应的报警使能开启后，参数设定的值才会起作用
 */
typedef struct am_ble_device {  
    uint8_t  ble_master_slave;            /**< \brief BLE设备主从模式设置             */ 
    uint8_t  *ble_pin;                    /**< \brief BLE设备配对连接码               */  
    uint8_t  interrupt_pin;               /** \brief 连接BLE设备的中断引脚            */                                            
} am_ble_device_t;
  
/**
 * \brief BLE设备结构体
 */
typedef struct am_ble_dev {
    
    /** \brief BLE使用I2C标准服务操作句柄  */
    am_uart_int_handle_t   handle;
    
    /** \brief 指向BLE设备信息的指针       */
    const am_ble_device_t *p_devinfo;
    
    
} am_ble_dev_t;

/** \brief BLE设备操作句柄类型定义 */
typedef struct am_ble_dev *am_ble_handle_t;    

/**
 * \brief BLE自测试命令
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT\r\n"                       
 * \return    无
 */
void am_ble_at_selftest (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE恢复出厂设置
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+DEFAULT\r\n"                       
 * \return    无
 */
void am_ble_at_default_set (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE复位
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+RESET\r\n"                       
 * \return    无
 */
void am_ble_at_reset (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE获取软件版本
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+VERSION\r\n"                       
 * \return    无
 */
void am_ble_at_version_get (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE获取蓝牙设备地名址
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+LADDR\r\n"                     
 * \return    无
 */
void am_ble_at_laddr_set (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE获取蓝牙设备名称
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+NAME\r\n"                       
 * \return    无
 */
void am_ble_at_name_get (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE设置蓝牙设备名称
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+NAME<x>\r\n" ，其中x为蓝牙设备的名字                      
 * \return    无
 */
void am_ble_at_name_set (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE获取蓝牙设备配对码
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+PIN\r\n"                     
 * \return    无
 */
void am_ble_at_pin_get (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE设置蓝牙设备配对码
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+PIN<x>\r\n" ，其中x为蓝牙设备的配对码,如 000000                     
 * \return    无
 */
void am_ble_at_pin_set (am_ble_handle_t handle, const uint8_t *p_txbuf); 


/**
 * \brief BLE获取蓝牙设备波特率
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+BAUD\r\n"                     
 * \return    无
 */
void am_ble_at_baud_get (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE设置蓝牙设备的波特率
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+BAUD<x>\r\n" ，其中x值为0，1，2，3，4，5，6，7，8
 *                      4: 9600波特率 ，8: 115200波特率 
 * \return    无
 */
void am_ble_at_baud_set (am_ble_handle_t handle, const uint8_t *p_txbuf); 


/**
 * \brief BLE获取蓝牙设备波特率的停止位
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+STOP\r\n"                     
 * \return    无
 */
void am_ble_at_stop_get (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE设置蓝牙设备的波特率的停止位
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+STOP<x>\r\n" ，其中x值为0，1
 *                      0: 1位停止位 ，1: 2位停止位 
 * \return    无
 */
void am_ble_at_stop_set (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE获取蓝牙设备波特率的校验方式
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+PARI\r\n"                     
 * \return    无
 */
void am_ble_at_pari_get (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE设置蓝牙设备的波特率的停止位
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+PARI<x>\r\n" ，其中x值为0，1，2
 *                      0: 无校验 ，1: 奇校验 ，2：偶校验 
 * \return    无
 */
void am_ble_at_pari_set (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE获取蓝牙设备是否进入功耗模式
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+PWRM\r\n"                     
 * \return    无
 */
void am_ble_at_pwrm_get (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE设置蓝牙上电是否进入低功耗模式
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+PWRM<x>\r\n" ，其中x值为0，1
 *                      0: 进入低功耗，1: 正常工作
 * \return    无
 */
void am_ble_at_pwrm_set (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE设置蓝牙进入低功耗模式
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+SLEEP\r\n" 
 * \return    无
 */
void am_ble_at_sleep_set (am_ble_handle_t handle, const uint8_t *p_txbuf); 
    
/**
 * \brief BLE获取蓝牙设备主从模式
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+ROLE\r\n"                     
 * \return    无
 */
void am_ble_at_role_get (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE设置蓝牙的主从模式
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+ROLE<x>\r\n" ，其中x值为0，1
 *                      0: 从设备，1：主设备
 * \return    无
 */
void am_ble_at_role_set (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE蓝牙在主模式下搜索蓝牙设备
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+INQ\r\n" 
 * \return    无
 */
void am_ble_at_inq (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE蓝牙在主模式下显示搜索到的蓝牙设备
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+SHOW\r\n" 
 * \return    无
 */
void am_ble_at_show (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE蓝牙在主模式下连接蓝牙设备
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+CONN<x>\r\n" 其中x值为0，1， 2等，
 *                      代表搜索到的蓝牙设备
 * \return    无
 */
void am_ble_at_conn (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE获取蓝牙设备的功率模式
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+POWE\r\n"                     
 * \return    无
 */
void am_ble_at_powe_get (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE设置蓝牙设备的功率模式
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+POWE<x>\r\n" ，其中x值为0，1, 2, 3
 *                      0: -23dB，1：-6dB, 2: 0dB, 3: 4dB
 * \return    无
 */
void am_ble_at_powe_set (am_ble_handle_t handle, const uint8_t *p_txbuf); 

/**
 * \brief BLE蓝牙查看帮助信息
 * \param[in] handle  : 指向BLE设备操作句柄的指针
 * \param[in] p_txbuf : 需要发送的内容：值为 "AT+HELP\r\n" 
 * \return    无
 */
void am_ble_at_help (am_ble_handle_t handle, const uint8_t *p_txbuf); 


/** \brief BLE蓝牙主模式设置  */
void am_ble_master_set (am_ble_handle_t handle);

/** \brief BLE蓝牙从模式设置  */
void am_ble_slave_set (am_ble_handle_t handle);

/**
 * \brief BLE中断配置
 * \param[in] handle : 指向MMA8451设备操作句柄的指针   
 * \param[in] pfn_isr   : 注册的中断回调函数
 * \param[in] p_arg     : 注册的中断回调函数的参数 
 * \return    返回系统模式寄存器的值
 */
void am_ble_int_cfg (am_ble_handle_t handle, am_pfnvoid_t pfn_isr, void *p_arg);


/**
 * \brief 初始化BLE
 *
 * \param[in] p_dev     : BLE设备结构体指针
 * \param[in] p_devinfo : BLE设备信息结构体 
 * \param[in] handle    : UART中断标准服务操作句柄
 *
 * \return    BLE的操作句柄，如果值为NULL，表明初始化失败
 */
am_ble_handle_t am_ble_init (am_ble_dev_t *p_dev,
                             const am_ble_device_t *p_devinfo,
                             am_uart_int_handle_t   handle);
                                                                  
                                 
/**
 * \brief 解除BLE初始化
 *
 * \param[in] p_dev  : BLE设备结构体指针
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















