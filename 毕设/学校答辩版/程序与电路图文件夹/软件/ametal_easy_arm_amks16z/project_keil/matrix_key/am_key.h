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
 * \brief am_matrix_key标准接口
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
 * \name 键盘按键值定义                                           
 * @{                                                                   
 */                                                                     
#define AM_KEY_NONE                          0x00   /**< \brief 按键释放      */                                                 
#define AM_KEY_ENTER                         0x28   /**< \brief 回车键        */ 
#define AM_KEY_ESCAPE                        0x29   /**< \brief 返回键        */ 
#define AM_KEY_BACKSPACE                     0x2A   /**< \brief 清除键        */                                
#define AM_KEY_SPACEBAR                      0x2C   /**< \brief 空格键        */
#define AM_KEY_RIGHTARROW                    0x4F   /**< \brief 方向右键      */               
#define AM_KEY_LEFTARROW                     0x50   /**< \brief 方向左键      */               
#define AM_KEY_DOWNARROW                     0x51   /**< \brief 方向下键      */ 
#define AM_KEY_UPARROW                       0x52   /**< \brief 方向上键      */
#define AM_MODIFERKEYS_LEFT_CTRL             0x01   /**< \brief 左CTRL键      */                 
#define AM_MODIFERKEYS_LEFT_SHIFT            0x02   /**< \brief 左SHIFT键     */                
#define AM_MODIFERKEYS_LEFT_ALT              0x04   /**< \brief 左ALT键       */                                 
                                                                      
/** @} */ 

/**                                                                     
 * \name 鼠标按键值定义                                           
 * @{                                                                   
 */ 
#define  AM_MOUSE_LEFT_CLICK                 0x01  /**< \brief Mouse Left click  */
#define  AM_MOUSE_RIGHT_CLICK                0x02  /**< \brief Mouse Right click */

/** @} */

/**                                                                     
 * \name 鼠标与键盘切换标志                                           
 * @{                                                                   
 */ 
#define  AM_KEYBOARD_FLAG                    0xA0  /**< \brief 选择功能为键盘  */
#define  AM_MOUSE_FLAG                       0xA1  /**< \brief 选择功能为鼠标  */

/** @} */

/**
 * \brief 矩阵键盘设备相关信息结构体
 * \note  当选确定的一个引脚作为行与列的的引脚时，接下来与之连号的三个引脚分别
 *        对应相应的行与列之间的引脚 
 */
typedef struct am_matrix_key_device { 
    
    uint8_t  inum;               /**< \brief 矩阵键盘中断引脚中断号  */ 
    
    am_pfnvoid_t pfn_isr;        /**< \brief 矩阵键盘中断回调函数    */
       
    int32_t  row_pin;            /**< \brief 矩阵键盘列对应于 PTC0  */ 
    int32_t  col_pin;            /**< \brief 矩阵键盘行对应于 PTC4  */ 
          
} am_matrix_key_device_t;

/**
 * \brief 矩阵键盘设备结构体
 */
typedef struct am_matrix_key_dev {
    
    /** \brief 使能中断标志位                  */
    uint8_t int_status; 
    
    /** \brief 矩阵键盘中断回调函数            */
    am_pfnvoid_t pfn_isr;        
      
    /** \brief 指向矩阵键盘设备信息的指针       */
    const am_matrix_key_device_t *p_devinfo;
    
} am_matrix_key_dev_t;

/** \brief 矩阵键盘设备操作句柄类型定义 */
typedef struct am_matrix_key_dev *am_matrix_key_handle_t;

extern uint8_t g_mouse_or_key_flag ; /**< \brief 选择鼠标与键盘标志，默认为鼠标 */

extern uint8_t g_mouse_button[2];    /**< \brief 鼠标键值 */
extern uint8_t g_key_board[8];       /**< \brief 键盘键值 */
  
/**
 * \brief  矩阵按键初始化
 *
 * \param[in] p_dev     : 矩阵键盘设备结构体指针
 * \param[in] p_devinfo : 矩阵键盘设备信息结构体
 *
 * \return  矩阵键盘设备的操作句柄，如果值为NULL，表明初始化失败
 */
am_matrix_key_handle_t am_matrix_key_init (am_matrix_key_dev_t           *p_dev, 
                                           const am_matrix_key_device_t  *p_devinfo);
                                                                                   
/**
 * \brief 解除矩阵按键初始化
 *
 * \param[in] p_dev  : 矩阵键盘设备结构体指针
 *
 * \return    None
 */
void am_matrix_key_deinit (am_matrix_key_dev_t  *p_dev);

/**
 * \brief  扫描得到按键状态
 *
 * \param[in] handle : 矩阵键盘设备操作句柄的指针  
 *
 * \return 返回扫描得到按键状态，若无按键，返回0x0 
 */
uint16_t am_matrix_key_scan (am_matrix_key_handle_t  handle);

/**
 * \brief  中断唤醒引脚配置
 *
 * \param[in] handle : 矩阵键盘设备操作句柄的指针  
 *
 * \return 无 
 */
void am_int_wake_up_cfg (am_matrix_key_handle_t  handle, am_pfnvoid_t  pfn_isr) ; 

/**
 * \brief  使能中断唤醒引脚配置
 *
 * \param[in] handle : 矩阵键盘设备操作句柄的指针  
 *
 * \return 无 
 */
void am_int_wake_up_enable(am_matrix_key_handle_t handle);

/**
 * \brief  禁能中断唤醒引脚
 *
 * \param[in] handle : 矩阵键盘设备操作句柄的指针  
 *
 * \return 无 
 */
void am_int_wake_up_disable(am_matrix_key_handle_t handle);

/** 
 * \brief  唤醒引脚中断回调用函数
 * \param  p_arg : 空类型指针
 *
 * \return 无 
 */
void am_int_wake_up_cal (void *p_arg); 

/**
 * \brief  多次扫描得到键值，消除抖动
 *
 * \param[in] handle : 矩阵键盘设备操作句柄的指针  
 * \param[in] count  : 按键重复扫描的次数，一般取值为2 ~ 10
 *
 * \return 返回扫描到的键值，若是有抖动，则返回值为-AM_ERROR 
 */

uint16_t am_matrix_key_scan_repeat (am_matrix_key_handle_t  handle, uint8_t  count);

/**
 * \brief   解释按键状态为定义的相应键值  
 *
 * \param[in] handle  :  矩阵键盘设备操作句柄的指针  
 * \param[in] key_avl :  按键状态代表的值
 *
 * \return 无 
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
