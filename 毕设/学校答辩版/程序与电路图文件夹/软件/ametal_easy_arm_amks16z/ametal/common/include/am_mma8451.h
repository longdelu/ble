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
 * \brief mma8451标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 15-11-19  lgr, first implementation.
 * \endinternal
 */
 
#ifndef __AM_MMA8451_H
#define __AM_MMA8451_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"   
#include "am_gpio.h"
#include "am_i2c.h"
#include "amdr_i2c.h" 
#include <math.h>    
#include <string.h>
#include <stdlib.h>
    
/**
 * @addtogroup am_if_mma8451
 * @copydoc am_mma8451.h
 * @{
 */    

/**
 * \name MMA8451从机设备地址（驱动当中会左移一位，变成0x38等,此时SA0 = 0)
 * @{ 
 */      
#define AM_MMA8451_ADDRESS       0x1c    /**< \brief MMA8451从机设备地址 */      
 
/** @} */       

/**
 * \name MMA8451设备各寄存器地址，为从机设备的子地址
 * @{ 
 */  

#define AM_MMA8451_F_STATUS      0x00
#define AM_MMA8451_OUT_X_MSB     0x01
#define AM_MMA8451_OUT_X_LSB     0x02
#define AM_MMA8451_OUT_Y_MSB     0x03
#define AM_MMA8451_OUT_Y_LSB     0x04
#define AM_MMA8451_OUT_Z_MSB     0x05
#define AM_MMA8451_OUT_Z_LSB     0x06
#define AM_MMA8451_F_SETUP       0x09
#define AM_MMA8451_TRIG_CFG      0x0A
#define AM_MMA8451_SYSMOD        0x0B
#define AM_MMA8451_INT_SOURCE    0x0C
#define AM_MMA8451_WHO_AM_I      0x0D
#define AM_MMA8451_XYZ_DATA_CFG  0x0E
#define AM_MMA8451_HPF_CUTOFF    0x0F
#define AM_MMA8451_PL_STATUS     0x10
#define AM_MMA8451_PL_CFG        0x11
#define AM_MMA8451_PL_COUNT      0x12
#define AM_MMA8451_PL_BF_ZCOMP   0x13
#define AM_MMA8451_PL_THS_REG    0x14
#define AM_MMA8451_FF_MT_CFG     0x15
#define AM_MMA8451_FF_MT_SRC     0x16
#define AM_MMA8451_FF_MT_THS     0x17
#define AM_MMA8451_FF_MT_COUNT   0x18
#define AM_MMA8451_TRANSIENT_CFG 0x1D
#define AM_MMA8451_TRANSIENT_SCR 0x1E
#define AM_MMA8451_TRANSIENT_THS 0x1F
#define AM_MMA8451_TRANSIENT_CNT 0x20
#define AM_MMA8451_PULSE_CFG     0x21
#define AM_MMA8451_PULSE_SRC     0x22
#define AM_MMA8451_PULSE_THSX    0x23
#define AM_MMA8451_PULSE_THSY    0x24
#define AM_MMA8451_PULSE_THSZ    0x25
#define AM_MMA8451_PULSE_TMLT    0x26
#define AM_MMA8451_PULSE_LTCY    0x27
#define AM_MMA8451_PULSE_WIND    0x28
#define AM_MMA8451_ASLP_COUNT    0x29
#define AM_MMA8451_CTRL_REG1     0x2A
#define AM_MMA8451_CTRL_REG2     0x2B
#define AM_MMA8451_CTRL_REG3     0x2C
#define AM_MMA8451_CTRL_REG4     0x2D
#define AM_MMA8451_CTRL_REG5     0x2E
#define AM_MMA8451_OFF_X         0x2F
#define AM_MMA8451_OFF_Y         0x30
#define AM_MMA8451_OFF_Z         0x31

/** @} */ 

/**
 * \name MMA8451在2g量程下小数位数代表
 * @{ 
 */ 

#define AM_MMA8451_FRAC_2D1           5000
#define AM_MMA8451_FRAC_2D2           2500
#define AM_MMA8451_FRAC_2D3           1250
#define AM_MMA8451_FRAC_2D4           625
#define AM_MMA8451_FRAC_2D5           313
#define AM_MMA8451_FRAC_2D6           156
#define AM_MMA8451_FRAC_2D7           78
#define AM_MMA8451_FRAC_2D8           39
#define AM_MMA8451_FRAC_2D9           20
#define AM_MMA8451_FRAC_2D10          10
#define AM_MMA8451_FRAC_2D11           5
#define AM_MMA8451_FRAC_2D12           2

/** @} */ 

/**
 * \name MMA8451选择数据输出时是否使用高通滤波
 * @{ 
 */
#define AM_MMA8451_RESET_ENALBE     (1UL << 6)  /**< \brief 允许复位  */
#define AM_MMA8451_RESET_DISALBE    (1UL << 6)  /**< \brief 禁止复位  */

/** @} */
/**
 * \name MMA8451选择备用模式或激活模式
 * @{ 
 */
#define AM_MMA8451_STANDBY_MODE_CFG     0x01  /**< \brief 备用模式  */
#define AM_MMA8451_ACTIVE_MODE_CFG      0x01  /**< \brief 激活模式  */

/** @} */

/**
 * \name MMA8451选择不同的量程范围
 * @{ 
 */
#define AM_MMA8451_G_RANGES(X)  ((X) & 0xff)          /**< \brief X的取值可以为0~2 */  

/** @} */


/**
 * \name MA8451正常(唤醒)模式下数据输出速率设置
 * @{ 
 */
#define AM_MMA8451_DATA_RATA_SET(X)  (X << 3)         /**< \brief X的取值可以为0~7 */  

/** @} */
 
/**
 * \name MMA8451睡眠模式下数据输出速率设置
 * @{ 
 */

#define AM_MMA8451_SLEEP_DATA_RATA_SET(X)  (X << 6)  /**< \brief X的取值可以为0~3 */ 

/** @} */

/**
 * \name MMA8451选择不同的采样模式
 * @{ 
 */

#define AM_MMA8451_OVERSAMPLING_MODE_SET(X)  (X)    /**< \brief X的取值可以为0~3 */ 

/** @} */

/** \brief MMA8451高通滤波截止频率选择 */ 
#define AM_MMA8451_HPF_FRQ_CUFOFF(X)  ((X) & 0xff)  /**< \brief X的取值可以为0~3 */  

/** @} */

/**
 * \name MMA8451选择数据输出时是否使用高通滤波
 * @{ 
 */
#define AM_MMA8451_DATA_HPF_ENALBE     (1UL << 4)  /**< \brief 数据使用高滤波     */
#define AM_MMA8451_DATA_HPF_DISALBE    (1UL << 4)  /**< \brief 数据不使用高通滤波 */

/** @} */

/**
 * \name MMA8451选择数据读写的位数方式
 * @{ 
 */
#define  AM_MMA8451_DATA_FAST_READ_CFG   (1UL << 1)  /**< \BRIEF 数据使用8位     */
#define  AM_MMA8451_DATA_NORMAL_READ_CFG (1UL << 1)  /**< \BRIEF 数据使用14位    */

/** @} */

/**
 * \name MMA8451中断配置，包括使能与引脚,极性与激活配置
 * @{ 
 */
#define AM_MMA8451_INT_ASLP_CFG        (1UL << 7)  /**< \brief 睡眠与唤醒中断  */
#define AM_MMA8451_INT_FIFO_CFG        (1UL << 6)  /**< \brief FIFO中断        */
#define AM_MMA8451_INT_TRANS_CFG       (1UL << 5)  /**< \brief 瞬态检测中断    */
#define AM_MMA8451_INT_LNDPRT_CFG      (1UL << 4)  /**< \brief 方向检测中断    */
#define AM_MMA8451_INT_PULSE_CFG       (1UL << 3)  /**< \brief 点击检测中断    */
#define AM_MMA8451_INT_FFMT_CFG        (1UL << 2)  /**< \brief 运动检测中断    */
#define AM_MMA8451_INT_DRDY_CFG        (1UL << 0)  /**< \brief 数据准备中断    */

#define AM_MMA8451_INT_POL_LOW_CFG     (0UL << 1)  /**< \brief 中断低电平激活  */
#define AM_MMA8451_INT_POL_HIG_CFG     (1UL << 1)  /**< \brief 中断高电平激活  */
#define AM_MMA8451_INT_PP_CFG          (0UL << 0)  /**< \brief 中断引脚上拉模式*/
#define AM_MMA8451_INT_OD_CFG          (1UL << 0)  /**< \brief 中断引脚开漏模式*/

/** @} */

/**
 * \name MMA8451中断状态标志位
 * @{ 
 */
#define AM_MMA8451_INT_ASLP_FLAG       (1UL << 7)  /**< \brief 睡眠唤醒中断标志 */
#define AM_MMA8451_INT_FIFO_FLAG       (1UL << 6)  /**< \brief FIFO中断标志     */
#define AM_MMA8451_INT_TRANS_FLAG      (1UL << 5)  /**< \brief 瞬态检测中断标志 */
#define AM_MMA8451_INT_LNDPRT_FLAG     (1UL << 4)  /**< \brief 方向检测中断标志 */
#define AM_MMA8451_INT_PULSE_FLAG      (1UL << 3)  /**< \brief 点击检测中断标志 */
#define AM_MMA8451_INT_FFMT_FLAG       (1UL << 2)  /**< \brief 运动检测中断标志 */
#define AM_MMA8451_INT_DRDY_FLAG       (1UL << 0)  /**< \brief 数据准备中断标志 */

/** @} */

/**
 * \name MMA8451各轴加速度值是否有更新 
 * @{ 
 */
#define AM_MMA8451_XYZ_DATA_CHECK      (1UL << 4)  /**< \brief 数据更新标志     */

/** @} */

/**
 * \name MMA8451低功耗配置，包括自动睡眠使能等
 * @{ 
 */
#define AM_MMA8451_AUTO_SLEEP          (1UL << 2)  /**< \brief 自动睡眠使能标志位 */
  
#define AM_MMA8451_GATA_CFG            (1UL << 7) /**< \brief FIFO 的GATA标志  */
#define AM_MMA8451_WAKE_TRANS_CFG      (1UL << 6) /**< \brief 瞬态检测唤醒标志 */
#define AM_MMA8451_WAKE_LNDPRT_CFG     (1UL << 5) /**< \brief 方向检测唤醒标志 */
#define AM_MMA8451_WAKE_PULSE_CFG      (1UL << 4) /**< \brief 点击检测唤醒标志 */
#define AM_MMA8451_WAKE_FFMT_CFG       (1UL << 3) /**< \brief 运动检测唤醒标志 */

#define AM_MMA8451_SLEEP_MODE_FLAG     (0x02)     /**< \brief 睡眠模式         */
#define AM_MMA8451_WAKE_MODE_FLAG      (0x01)     /**< \brief 唤醒模式         */

/** @} */

/**
 * \name MMA8451运动检测配置
 * @{ 
 */
#define AM_MMA8451_MOTION_CFG          (1UL << 6)  /**< \brief 运动检测标志位 */

/**
 * \name 运动检测标志使能保存到运动检测状态标志寄存器各个位宏定义
 * @{ 
 */
#define AM_MMA8451_MOTION_ELE_CFG      (1UL << 7) 
#define AM_MMA8451_MOTION_ZEFE_CFG     (1UL << 5) /**< \brief Z方向检测使能标志 */
#define AM_MMA8451_MOTION_YEFE_CFG     (1UL << 4) /**< \brief Y方向检测使能标志 */
#define AM_MMA8451_MOTION_XEFE_CFG     (1UL << 3) /**< \brief X方向检测使能标志 */

/** @} */

/**
 * \name 运动检测事件状态标志
 * @{ 
 */   
#define AM_MMA8451_MOTION_EA_FLAG      (1UL << 7) /**< \brief 至少有一个事件发生 */
#define AM_MMA8451_MOTION_ZHE_FLAG     (1UL << 5) /**< \brief Z方向检测标志 */
#define AM_MMA8451_MOTION_ZHP_FLAG     (1UL << 4) /**< \brief Z方向极性标志 */
#define AM_MMA8451_MOTION_YHE_FLAG     (1UL << 3) /**< \brief Y方向检测标志 */
#define AM_MMA8451_MOTION_YHP_FLAG     (1UL << 2) /**< \brief Y方向极性标志 */
#define AM_MMA8451_MOTION_XHE_FLAG     (1UL << 1) /**< \brief X方向检测标志 */
#define AM_MMA8451_MOTION_XHP_FLAG     (1UL << 0) /**< \brief X方向极性标志 */

/** @} */

/**
 * \name MMA8451FIFO 配置
 * @{ 
 */ 

#define AM_MMA8451_FIFO_LOW_MOTION_CFG      0      /**< \brief 使用低功耗配置下的FIFO */
#define AM_MMA8451_FIFO_DAT_COLLECT_CFG     1      /**< \brief 中断模式下的FIFO      */

/** @} */


/**
 * \brief MMA8451设备相关信息结构体
 * \note  只有开启相应的报警使能开启后，参数设定的值才会起作用
 */
typedef struct am_mma8451_device {     
    uint16_t  dev_addr;            /**< \brief MMA8451设备地址               */ 
    uint16_t  dev_flags;           /**< \brief MMA8451设备特性               */
    
    /** \brief 连接MMA8451设备的中断引脚1 */  
    uint8_t   interrupt_pin1;     

    /** \brief 连接MMA8451设备的中断引脚2 */  
    uint8_t   interrupt_pin2;      
                                             
} am_mma8451_device_t;

/**
 * \brief MMA8451设备结构体
 */
typedef struct am_mma8451_dev {    
    /** \brief MMA8451使用I2C标准服务操作句柄  */
    am_i2c_handle_t   handle;
    
    /**< \brief I2C设备信息结构体              */
    am_i2c_device_t     i2c_device;               
    
    /** \brief 指向MMA8451设备信息的指针       */
    const am_mma8451_device_t *p_devinfo;
} am_mma8451_dev_t;

/** \brief MMA8451设备操作句柄类型定义 */
typedef struct am_mma8451_dev *am_mma8451_handle_t;

/**
 * \brief MMA8451设备复位使能 
 * \param[in] handle : 指向MMA8451设备操作句柄的指针
 * \param[in] flag   : 复位标志，值为 AM_MMA8451_RESET_ENALBE                 
 * \return    无
 */
void am_mma8451_reset_enable(am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief MMA8451设备复位禁能 
 * \param[in] handle : 指向MMA8451设备操作句柄的指针
 * \param[in] flag   : 复位标志，值为 AM_MMA8451_RESET_DISALBE                  
 * \return    无
 */
void am_mma8451_reset_disable(am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief  MMA8451选择备用模式 
 * \param[in] handle : 指向MMA8451设备操作句柄的指针
 * \param[in] flag   : 值为 AM_MMA8451_STANDBY_MODE_CFG                  
 * \return    无
 */
void am_mma8451_standby_mode_select (am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief  MMA8451选择激活模式 
 * \param[in] handle : 指向MMA8451设备操作句柄的指针
 * \param[in] flag   : 值为 M_MMA8451_ACTIVE_MODE_CFG                
 * \return    无
 */
void am_mma8451_active_mode_select (am_mma8451_handle_t handle, uint32_t flag);



/**
 * \brief \brief  MMA8451读设备ID
 *
 * \param[in] handle    : 指向MPU6050设备操作句柄的指针
 * \param[in] p_id      : 用户保存ID的缓冲区指针
 *
 * \return   无
 */  
void am_mma8451_id_read (am_mma8451_handle_t handle, uint8_t *p_id);


/**
 * \brief MMA8451量程范围设置
 *
 * \param[in] handle    : 指向MPU6050设备操作句柄的指针
 * \param[in] value     : 默认值为0x00
 *
 * \return   无
 */
void am_mma8451_g_range_set (am_mma8451_handle_t handle, uint32_t value);


/**
 * \brief MMA8451数据输出速率设置
 *
 * \param[in] handle    : 指向MPU6050设备操作句柄的指针
 * \param[in] value     : 默认值为0x00
 *
 * \return   无
 */
void am_mma8451_data_rate_out (am_mma8451_handle_t handle, uint32_t value);


/**
 * \brief MMA8451非睡眠模式下采样模式设置
 *
 * \param[in] handle    : 指向MPU6050设备操作句柄的指针
 * \param[in] value     : 默认值为0x00
 *
 * \return   无
 */
void am_mma8451_oversampling_mode_set (am_mma8451_handle_t handle, uint32_t value);


/**
 * \brief MMA8451高通滤波的截止频率
 *
 * \param[in] handle    : 指向MPU6050设备操作句柄的指针
 * \param[in] value     : 默认值为0x00
 *
 * \return   无
 */
void am_mma8451_hpf_fra_cutoff (am_mma8451_handle_t handle, uint32_t value);


/**
 * \brief  设置数据输出时使用高通滤波 
 *
 * \param[in] handle    : 指向MPU6050设备操作句柄的指针
 * \param[in] flag      : 默认值为0x00
 *
 * \return   无
 */
void am_mma8451_data_hpf_enable (am_mma8451_handle_t handle, uint32_t flag);


/**
 * \brief 设置数据输出时不使用高通滤波 
 *
 * \param[in] handle    : 指向MPU6050设备操作句柄的指针
 * \param[in] flag      : 默认值为0x00
 *
 * \return   无
 */
void am_mma8451_data_hpf_disable (am_mma8451_handle_t handle, uint32_t flag);


/**
 * \brief 设置快速读，8位数据 
 *
 * \param[in] handle    : 指向MPU6050设备操作句柄的指针
 * \param[in] flag      : 默认值为0x00
 *
 * \return   无
 */
void am_mma8451_data_fast_read (am_mma8451_handle_t handle, uint32_t flag);
    

/**
 * \brief   设置正常读，14位数据
 *
 * \param[in] handle    : 指向MPU6050设备操作句柄的指针
 * \param[in] flag      : 默认值为0x00
 *
 * \return   无
 */
void am_mma8451_data_normal_read (am_mma8451_handle_t handle, uint32_t flag);


/**
 * \brief 8位或14位方式先择 
 * \param[in] handle : 指向MMA8451设备操作句柄的指针
 * \param[in] flag   : 数据格式(8位与14位) 值为 0 或 1; 0：数据为14位，1：数据为8位                    
 * \return    无
 */
void am_mma8451_data_read_way_select(am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief 读x,y,z各轴加速度的值,注意此时没有使用FIFO
 * \param[in] handle : 指向MMA8451设备操作句柄的指针                
 * \param[in] buffer : 指向保存加速度缓冲区的指针
 * \return    无
 */
void am_mma8451_xyz_data_read (am_mma8451_handle_t handle, int16_t *buffer);

/**
 * \brief FIFO读x,y,z各轴加速度的值
 * \param[in] handle : 指向MMA8451设备操作句柄的指针                
 * \param[in] buffer : 指向保存加速度缓冲区的指针
 * \return    无
 */
void am_mma8451_fifo_data_read (am_mma8451_handle_t handle, int16_t *buffer);

/**
 * \brief 查询读x,y,z各轴加速度的值
 * \param[in] handle : 指向MMA8451设备操作句柄的指针                 
 * \param[in] buffer : 指向保存加速度缓冲区的指针
 * \param[in] flag   : 数据更新标志，值为 AM_MMA8451_XYZ_DATA_CHECK    
 * \return    无
 */
void am_mma8451_xyz_data_poll_read (am_mma8451_handle_t handle, 
                                    int16_t *buffer, 
                                    uint32_t flag);
 
/**
 * \brief 中断激活极性配置 
 * \param[in] handle : 指向MMA8451设备操作句柄的指针
 * \param[in] flag   : 中断激活极性的宏，值为 AM_MMA8451_INT_POL_LOW_CFG 
 *                     或 AM_MMA8451_INT_POL_HIG_CFG                  
 * \return    无
 */
void am_mma8451_intterupt_pol_set (am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief 中断引脚模式配置
 * \param[in] handle : 指向MMA8451设备操作句柄的指针
 * \param[in] flag   : 中断引脚模式的宏，值为 AM_MMA8451_INT_OD_CFG 或 AM_MMA8451_INT_PP_CFG                       
 * \return    无
 */
void am_mma8451_int_pin_mod_set (am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief 使能加速度计相应中断 
 * \param[in] handle : 指向MMA8451设备操作句柄的指针
 * \param[in] flag   : 中断标志，值为 AM_MMA8451_INT_*_CFG 如AM_MMA8451_INT_DRDY_CFG                       
 * \return    无
 */
void am_mma8451_intterupt_enable (am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief 禁能加速度计相应中断 
 * \param[in] handle : 指向MMA8451设备操作句柄的指针
 * \param[in] flag   : 中断标志，值为 AM_MMA8451_INT_*_CFG 如 AM_MMA8451_INT_DRDY_CFG                    
 * \return    无
 */
void am_mma8451_intterupt_disable (am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief 设置连接的中断引脚  
 * \param[in] handle : 指向MMA8451设备操作句柄的指针
 * \param[in] flag   : 中断标志，值为 AM_MMA8451_INT_*_CFG 如 AM_MMA8451_INT_DRDY_CFG    
 * \param[in] pin    : 中断引脚 值为 0 或 1; 0：中断引脚0，1：中断引脚1                      
 * \param[in] buffer : 指向保存加速度缓冲区的指针
 * \return    无
 */
void am_mma8451_int_pin_rout (am_mma8451_handle_t handle, uint32_t flag, uint8_t pin);

/**
 * \brief 中断状态返回，返回值用于判断中断是否发生
 * \param[in] handle : 指向MMA8451设备操作句柄的指针                    
 * \return    返回中断标志寄存器的值，类似于总中断
 */
uint8_t am_mma8451_int_status_get (am_mma8451_handle_t handle);

/**
 * \brief 配置中断读x,y,z各轴加速度的值
 * \param[in] handle : 指向MMA8451设备操作句柄的指针                 
 * \return    无
 */
void am_mma8451_xyz_data_intterupt_cfg (am_mma8451_handle_t handle);

/**
 * \brief 自动睡眠使能或禁能
 * \param[in] handle : 指向MMA8451设备操作句柄的指针
 * \param[in] flag   : 自动睡眠标志位，值为                      
 * \param[in] val    : 值为0或1; 1: 使能，0：禁能
 * \note               设备要先调用 am_mma8451_standby_mode_select函数进入备用模式
 * \return    无
 */
void am_mma8451_auto_sleep_cfg (am_mma8451_handle_t handle, uint32_t flag, uint8_t var);

/**
 * \brief 睡眠模式下数据输出速率
 * \param[in] handle : 指向MMA8451设备操作句柄的指针
 * \param[in] flag   : 值为 AM_MMA8451_SLEEP_DATA_RATA_SET(X) 带宏的参数                
 * \note               设备要先调用 am_mma8451_standby_mode_select函数进入备用模式
 * \return    无
 */
void am_mma8451_dat_rat_sleep_mod_set (am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief MMA8451睡眠模式下采样模式设置
 * \param[in] handle : 指向MMA8451设备操作句柄的指针
 * \param[in] flag   : 值为 AM_MMA8451_OVERSAMPLING_MODE_SET 带宏的参数                
 * \note               设备要先调用 am_mma8451_standby_mode_select函数进入备用模式
 * \return    无
 */
void am_mma8451_sleep_mod_over_mode_set (am_mma8451_handle_t handle, uint32_t value);

/**
 * \brief 设置GATE为1，在缓冲区清空之前，数据不能流入FIFO,需要手动清除缓冲区 
 * \param[in] handle : 指向MMA8451设备操作句柄的指针
 * \param[in] flag   :  GATE标志宏，值为AM_MMA8451_GATA_CFG                
 * \note 设备要先调用 am_mma8451_standby_mode_select函数进入备用模式
 * \return    无
 */
void am_mma8451_gate_set (am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief 设置GATE为0，睡眠/唤醒模式变换的时候自动清除了缓冲区 
 * \param[in] handle : 指向MMA8451设备操作句柄的指针
 * \param[in] flag   : GATE标志宏，值为AM_MMA8451_GATA_CFG                
 * \note 设备要先调用 am_mma8451_standby_mode_select函数进入备用模式
 * \return    无
 */
void am_mma8451_gate_clr (am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief 设备唤醒事件使能
 * \param[in] handle : 指向MMA8451设备操作句柄的指针
 * \param[in] flag   : 值为AM_MMA8451_WAKE_*_CFG ,#AM_MMA8451_WAKE_TRANS_CFG                 
 * \note 设备要先调用 am_mma8451_standby_mode_select函数进入备用模式
 * \return    无
 */
void am_mma8451_wake_from_sleep_enable (am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief 设备唤醒事件禁能 
 * \param[in] handle : 指向MMA8451设备操作句柄的指针
 * \param[in] flag   : 值为AM_MMA8451_WAKE_*_CFG ,#AM_MMA8451_WAKE_TRANS_CFG     
 * 
 * \note 设备要先调用 am_mma8451_standby_mode_select函数进入备用模式
 *
 * \return    无
 */
void am_mma8451_wake_from_sleep_disable (am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief MMA8451睡眠时间设置
 *
 * \param[in] handle : 指向MMA8451设备操作句柄的指针
 * \param[in] value  : 计算公式为睡眠时间 / ASLP_COUNT Step 
 * 
 * \note 设备要先调用 am_mma8451_standby_mode_select函数进入备用模式
 *
 * \return    无
 */
void am_mma8451_sleep_time_set (am_mma8451_handle_t handle, uint32_t value);

/**
 * \brief 睡眠/唤醒，低功耗模式设置
 * \param[in] handle : 指向MMA8451设备操作句柄的指针
 * \return    无
 */
void am_mma8451_low_power_cfg (am_mma8451_handle_t handle);

/**
 * \brief 运动检测配置
 * \param[in] handle : 指向MMA8451设备操作句柄的指针
 * \return    无
 */
void am_mma8451_motion_detect_cfg (am_mma8451_handle_t handle);

/**
 * \brief 运动检测事件状态源寄存器返回
 * \param[in] handle : 指向MMA8451设备操作句柄的指针                    
 * \return    返回运动检测寄存器的值(即是从寄存器里面的值）
 */
uint8_t am_mma8451_motion_event_src_get (am_mma8451_handle_t handle);

/**
 * \brief 运动检测中断辅助函数
 * \param[in] handle : 指向MMA8451设备操作句柄的指针                    
 * \return    无
 */
void am_mma8451_int_motion_assist (am_mma8451_handle_t handle);

/**
 * \brief 睡眠/唤醒模式设状态返回
 * \param[in] handle : 指向MMA8451设备操作句柄的指针                    
 * \return    返回系统模式寄存器的值
 */
uint8_t am_mma8451_system_mode_get (am_mma8451_handle_t handle);

/**
 * \brief 睡眠/唤醒模式中断辅助函数 
 * \param[in] handle : 指向MMA8451设备操作句柄的指针  
 * \param[in] buffer : 指向保存加速度缓冲区的指针 
 * \return    返回中断标志寄存器的值
 */
void am_mma8451_system_mode_assist (am_mma8451_handle_t handle, int16_t *buffer);
 
/**
 * \brief FIFO中断辅助函数
 * \param[in] handle : 指向MMA8451设备操作句柄的指针  
 * \param[in] buffer : 指向保存加速度缓冲区的指针 
 * \return    返回中断标志寄存器的值
 */
void am_mma8451_fifo_mode_assist (am_mma8451_handle_t handle, int16_t *buffer);

/**
 * \brief 使用FIFO配置 
 * \param[in] handle : 指向MMA8451设备操作句柄的指针   
 * \param[in] flag   : 值为AM_MMA8451_WAKE_*_CFG ,#AM_MMA8451_WAKE_TRANS_CFG  
 * \note  使用FIFO配置时，数据为8位格式
 * \return    无
 */
void am_mma8451_fifo_cfg (am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief MMA8451设备中断配置
 *
 * \param[in] handle    : 指向MMA8451设备操作句柄的指针   
 * \param[in] pfn_isr   : 注册的中断回调函数
 * \param[in] p_arg     : 注册的中断回调函数的参数 
 *
 * \note  MMA8451的中断引脚程序都设为开漏状态，需要芯片内部上拉或外接上拉电阻
 *
 * \return    返回系统模式寄存器的值
 */
void am_mma8451_int_cfg (am_mma8451_handle_t handle, 
                         am_pfnvoid_t pfn_isr1, 
                         am_pfnvoid_t pfn_isr2, 
                         void *p_arg);

/**
 * \brief 数据从整型转化为字符串
 *
 * \param[in] handle    : 指向MMA8451设备操作句柄的指针   
 * \param[in] buffer    : 整形数据
 * \param[in] p_txbuf   : 字符串字针
 *
 * \note  MMA8451的中断引脚程序都设为开漏状态，需要芯片内部上拉或外接上拉电阻
 *
 * \return    返回系统模式寄存器的值
 */
void am_mma8451_data_change_to_char (am_mma8451_handle_t handle, 
                                     uint8_t *p_txbuf, 
                                     const int16_t *buffer); 

/**
 * \brief 数据从字符串转化整型
 *
 * \param[in] handle    : 指向MMA8451设备操作句柄的指针   
 * \param[in] buffer    : 整形数据
 * \param[in] p_txbuf   : 字符串字针
 *
 * \note  MMA8451的中断引脚程序都设为开漏状态，需要芯片内部上拉或外接上拉电阻
 *
 * \return    返回系统模式寄存器的值
 */
void am_mma8451_data_change_to_int (am_mma8451_handle_t handle, 
                                    const uint8_t *p_txbuf, 
                                    int16_t *buffer); 

/**
 * \brief MMA8451设备中断处理函数 
 * \param[in] handle : 指向MMA8451设备操作句柄的指针  
 * \param[in] buffer : 指向采集数据缓冲区的指针   
 * \return    无
 */
void am_mma8451_intterupt_handle (am_mma8451_handle_t handle, int16_t *buffer);

/**
 * \brief 数据处理，转换成重力加速度的表示方法
 * \param[in] handle : 指向MMA8451设备操作句柄的指针  
 * \param[in] buf    : 指向重力加速度表示数据的缓冲区指针
 * \param[in] buffer : 指向采集数据缓冲区的指针   
 * \return    无
 */
void am_mma8451_data_handle(am_mma8451_handle_t handle, float *buf, int16_t *buffer);

/** 
 * \brief 数据处理，转换成重力加速度的表示方法
 * \param[in] handle : 指向MMA8451设备操作句柄的指针  
 * \param[in] buf    : 指向角度的缓冲区指针
 * \param[in] buffer : 指向重力加速度的指针   
 * \return    无
 */
void am_mma8451_acc_to_angle(am_mma8451_handle_t handle, int16_t * buf, float *buffer);

/**
 * \brief 初始化mma8451
 *
 * \param[in] p_dev     : mma8451设备结构体指针
 * \param[in] handle    : I2C标准服务操作句柄
 * \param[in] p_devinfo : 设备信息结构体
 *
 * \return   mma8451的操作句柄，如果值为NULL，表明初始化失败
 */
am_mma8451_handle_t am_mma8451_init (am_mma8451_dev_t *p_dev,
                                     const am_mma8451_device_t *p_devinfo,
                                     am_i2c_handle_t   handle);
                                                                                                  
/**
 * \brief 解除mma8451初始化
 *
 * \param[in] p_dev  : mma8451设备结构体指针
 *
 * \return    无
 */
void am_mma8451_deinit (am_mma8451_dev_t *p_dev);


/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_MMA8451_H */

/* end of file */



