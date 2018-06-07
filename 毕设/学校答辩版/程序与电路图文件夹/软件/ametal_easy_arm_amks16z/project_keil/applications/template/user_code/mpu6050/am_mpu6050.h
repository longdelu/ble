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
 * \brief mpu6050标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 15-11-19  lgr, first implementation.
 * \endinternal
 */
 
#ifndef __AM_MPU6050_H
#define __AM_MPU6050_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "am_common.h"   
#include "am_gpio.h"
#include "am_i2c.h"
#include "amdr_i2c.h" 
#include "amdr_uart.h"
#include "am_imu.h"
#include <math.h>    
#include <string.h>
#include <stdlib.h>
    
/**
 * @addtogroup am_if_mpu6050
 * @copydoc am_mpu6050.h
 * @{
 */   

/**
 * \name MPU6050从机设备地址（驱动当中会左移一位，变成0xD0等,此时AD0 = 0)
 * @{ 
 */      
#define AM_MPU6050_ADDRESS             0x68    /**< \brief MPU6050从机设备地址              */      
 
/** @} */  

/**
 * \name  MPU6050设备各寄存器地址，为从机设备的子地址
 * @{ 
 */
#define AM_MPU6050_SMPLRT_DIV          0x19    /**< \brief 陀螺仪采样率                      */
#define AM_MPU6050_CONFIG_MPU          0x1A    /**< \brief 低通滤波频率                      */
#define AM_MPU6050_GYRO_CONFIG         0x1B    /**< \brief 陀螺仪自检及测量范围；             */
#define AM_MPU6050_ACCEL_CONFIG        0x1C    /**< \brief 加速计自检、测量范围及高通滤波频率 */
#define AM_MPU6050_MOTIONTHR_CONFIG    0x1F    /**< \brief 运动检测阀值寄存器                 */
#define AM_MPU6050_FIFO_EN             0x23    /**< \brief FIFO使能寄存器                     */
#define AM_MPU6050_I2C_MASTER          0x24    /**< \brief MPU6050作为I2C主机的控制           */ 
#define AM_MPU6050_PINFUC_ENABLE       0x37    /**< \brief MPU6050作为I2C主机的控制           */ 
#define AM_MPU6050_INTSRC_ENABLE       0x38    /**< \brief MPU6050作为I2C主机的控制           */ 
#define AM_MPU6050_INT_STATUS          0x3A    /**< \brief MPU6050中断状态寄存器              */ 
#define AM_MPU6050_ACCEL_XOUT_H        0x3B    /**< \brief MPU6050X轴加速度数据输出高字节     */
#define AM_MPU6050_ACCEL_XOUT_L        0x3C    /**< \brief MPU6050X轴加速度数据输出低字节     */
#define AM_MPU6050_ACCEL_YOUT_H        0x3D    /**< \brief MPU6050Y轴加速度数据输出高字节     */
#define AM_MPU6050_ACCEL_YOUT_L        0x3E    /**< \brief MPU6050Y轴加速度数据输出低字节     */
#define AM_MPU6050_ACCEL_ZOUT_H        0x3F    /**< \brief MPU6050Z轴加速度数据输出高字节     */
#define AM_MPU6050_ACCEL_ZOUT_L        0x40    /**< \brief MPU6050Z轴加速度数据输出低字节     */
#define AM_MPU6050_TEMP_OUT_H          0x41    /**< \brief MPU6050温度数据输出高字节          */
#define AM_MPU6050_TEMP_OUT_L          0x42    /**< \brief MPU6050温度数据输出低字节          */
#define AM_MPU6050_GYRO_XOUT_H         0x43    /**< \brief MPU6050X轴陀螺仪数据输出高字节     */
#define AM_MPU6050_GYRO_XOUT_L         0x44    /**< \brief MPU6050X轴陀螺仪数据输出低字节     */
#define AM_MPU6050_GYRO_YOUT_H         0x45    /**< \brief MPU6050Y轴陀螺仪数据输出高字节     */
#define AM_MPU6050_GYRO_YOUT_L         0x46    /**< \brief MPU6050Y轴陀螺仪数据输出低字节     */
#define AM_MPU6050_GYRO_ZOUT_H         0x47    /**< \brief MPU6050Z轴陀螺仪数据输出高字节     */
#define AM_MPU6050_GYRO_ZOUT_L         0x48    /**< \brief MPU6050Z轴陀螺仪数据输出低字节     */
#define AM_MPU6050_USER_CTRL           0x6A    /**< \brief 用户控制寄存器                     */   
#define AM_MPU6050_PWR_MGMT_1          0x6B    /**< \brief 电源管理寄存器1                    */
#define AM_MPU6050_PWR_MGMT_2          0x6C    /**< \brief 电源管理寄存器2                    */
#define AM_MPU6050_WHO_AM_I            0x75    /**< \brief 地址寄存器(默认数值0x68，只读)     */

/** @} */

/**
 * \name MPU6050复位与唤醒与否
 * @{ 
 */
#define AM_MPU6050_RESET_ENALBE       (1UL << 7)  /**< \brief 使能复位                 */
#define AM_MPU6050_WAKE_ENALBE        (1UL << 6)  /**< \brief 唤醒使能                 */

/** @} */

/**
 * \name MPU6050中断使能
 * @{ 
 */
#define AM_MPU6050_MOT_EN_ENALBE      (1UL << 6)  /**< \brief 使能运动检测中断          */
#define AM_MPU6050_FIFO_OVER_ENALBE   (1UL << 4)  /**< \brief 使能FIFO益出中断          */
#define AM_MPU6050_I2C_MASINT_ENALBE  (1UL << 3)  /**< \brief 使能模块I2C主机功能中断   */
#define AM_MPU6050_DATA_READY_ENALBE  (1UL << 0)  /**< \brief 使能数据准备好中断        */

/** @} */

/**
 * \name MPU6050引脚功能与中断状态设置 
 * @{ 
 */
/** \brief 中断引脚触发时的电平,为1，激活时为低电平，0为低电平  */
#define AM_MPU6050_INT_LEVEL_SET      (1UL << 7)

/** \brief 中断引脚的上下拉模式，为1，开漏模式，0时，上拉模式   */
#define AM_MPU6050_INT_OPEN_SET       (1UL << 6) 

/** \brief 中断引脚触发后的电平状态 为0，中断电平保持一段时间，1中断电平保持直到清除中断标志      */
#define AM_MPU6050_LATCH_INT_SET      (1UL << 5)  

#define AM_MPU6050_INT_RD_CLEAR       (1UL << 4)  /**< \brief 设定只能通过清除中断标志清除中断 */
#define AM_MPU6050_FSYNC_INT_LEVEL    (1UL << 3)  /**< \brief 使能FSYNC引脚电平极              */
#define AM_MPU6050_FSYNC_INT_EN       (1UL << 2)  /**< \brief 使能FSYNC                        */
#define AM_MPU6050_I2C_BYPASS_EN      (1UL << 1)  /**< \brief 使能模块I2C主机                  */

/** @} */

/**
 * \name MPU6050中断状态
 * @{ 
 */
#define AM_MPU6050_MOT_INT_CHECK      (1UL << 6)  /**< \brief 运动检测中断标志位            */
#define AM_MPU6050_FIFO_OV_INT_CHECK  (1UL << 4)  /**< \brief FIFO益出中断标志              */
#define AM_MPU6050_I2C_MST_INT_CHECK  (1UL << 3)  /**< \brief 模块I2C主机中断标志           */
#define AM_MPU6050_DATA_RDY_INT_CHECK (1UL << 0)  /**< \brief 数扰准备好中断标志            */

/** @} */

/**
 * \name MPU6050数值类型读取选择
 * @{ 
 */
#define AM_MPU6050_DATA_ACC_SELECT      0          /**< \brief 运动检测中断标志位            */
#define AM_MPU6050_DATA_GRO_SELECT      1          /**< \brief FIFO益出中断标志              */
#define AM_MPU6050_DATA_TMP_SELECT      2          /**< \brief 模块I2C主机中断标志           */
#define AM_MPU6050_DATA_ALL_SELECT      3          /**< \brief 数扰准备好中断标志            */

/** @} */

/**
 * \name MPU6050加速度与角度范围
 * @{ 
 */
#define AM_MPU6050_ACC_RANGE(X)    (X << 3)  /**< \brief 加速度范围     */
#define AM_MPU6050_GRO_RANGE(X)    (X << 3)  /**< \brief 角度范围       */

/** @} */

/**
 * \name MPU6050的时钟源选择
 * @{ 
 */
#define AM_MPU6050_CLK_SECTLECT(X)    (X)   /**< \brief 时钟源选择      */

/** @} */

/**
 * \name  MPU6050数据计算
 * @{ 
 */
#define AM_MPU6050MAX_AVE       5          /**< \brief 均值滤波         */
#define AM_MPU6050PI          (3.1415926f) /**< \brief 圆周率           */

/** @} */

/**
 * \brief MPU6050设备相关信息结构体
 * \note  
 */
typedef struct am_mpu6050_device {  
    uint16_t  dev_addr;                    /**< \brief MPU6050设备地址  */ 
    uint16_t  dev_flags;                   /**< \brief MPU6050设备特性  */
    
    /** \brief 连接MPU6050设备的中断引脚 */  
    uint8_t   interrupt_pin;     
              
} am_mpu6050_device_t;

/**
 * \brief MPU6050设备结构体
 */
typedef struct am_mpu6050_dev {
    
    /** \brief MPU6050使用I2C标准服务操作句柄  */
    am_i2c_handle_t   handle;
    
    /**< \brief I2C设备信息结构体              */
    am_i2c_device_t     i2c_device;  
    
    /** \brief 指向MPU6050设备信息的指针       */
    const am_mpu6050_device_t *p_devinfo;
} am_mpu6050_dev_t;

/** \brief MPU6050设备操作句柄类型定义 */
typedef struct am_mpu6050_dev *am_mpu6050_handle_t;

/**
 * \brief MPU6050设备复位使能 
 * \param[in] handle : 指向MPU6050设备操作句柄的指针
 * \param[in] flag   : 复位标志，值为AM_MPU6050_RESET_ENALBE                 
 * \return    无
 */
void am_mpu6050_reset_enable (am_mpu6050_handle_t  handle, uint32_t  flag);

/**
 * \brief MPU6050设备复位禁能 
 * \param[in] handle : 指向MPU6050设备操作句柄的指针
 * \param[in] flag   : 唤醒使能，值为AM_MPU6050_WAKE_ENALBE                 
 * \return    无
 */
void am_mpu6050_wake_enable (am_mpu6050_handle_t  handle, uint32_t  flag);

/**    
 * \brief 设置MPU6050的时钟源 
 * \param[in] handle : 指向MPU6050设备操作句柄的指针
 * \param[in] flag   : 时钟源选择的宏，值为AM_MPU6050_CLK_SECTLECT，
 *                     其中宏参分别可取0 ~ 7，分别代表选择不同的时钟源，
 *                     如1就代表以X轴为参考的陀螺仪的PLL               
 * \return    无
 */
void am_mpu6050_clk_select (am_mpu6050_handle_t  handle, uint32_t  flag);

/**
 * \brief 设置MPU6050加速度范围值 
 * \param[in] handle : 指向MPU6050设备操作句柄的指针
 * \param[in] flag   : 加速度范围选择，值为AM_MPU6050_ACC_RANGE这个带参的宏，
 *                     其中宏参分别可取0，1，2，3，分别范围为2g, 4g, 8g,16g                 
 * \return    无
 */
void am_mpu6050_acc_range_set (am_mpu6050_handle_t  handle, uint32_t  flag);

/**
 * \brief 设置MPU6050陀螺仪角度范围值 
 * \param[in] handle : 指向MPU6050设备操作句柄的指针
 * \param[in] flag   : 角度范围选择，值为AM_MPU6050_GROP_RANGE这个带参的宏， 
 *                     其中宏参分别可取0，1，2，3，分别范围为 
 *                      250 °/s,  500 °/s,  1000 °/s, 2000 °/s                 
 * \return    无
 */
void am_mpu6050_gro_range_set (am_mpu6050_handle_t  handle, uint32_t  flag);

/**
 * \brief 设置MPU6050的输出速率 flag = 0x04 为 fs=1024 ,此时低通滤波为20HZ 
 * \param[in] handle : 指向MPU6050设备操作句柄的指针
 * \param[in] flag   : flag经验值一般取值为0x04 ，代表此时fs=1024 ,此时低通滤波为20HZ 
 *                                
 * \return    无
 */
void am_mpu6050_fs_set (am_mpu6050_handle_t  handle, uint32_t  flag);

/**
 * \brief 设置MPU6050采样率 
 * \param[in] handle : 指向MPU6050设备操作句柄的指针
 * \param[in] flag   : flag经验值一般取值为0x13 ，此时代表采样率为50Hz
 *                                
 * \return    无
 */
void am_mpu6050_sample_rate_set (am_mpu6050_handle_t  handle, uint32_t  flag);

/**
 * \brief 设置运动检测的阀值 
 * \param[in] handle : 指向MPU6050设备操作句柄的指针
 * \param[in] flag   ：阀值单位为1MSB/MG,          
 * \return    无
 */
void am_mpu6050_motion_thr_set (am_mpu6050_handle_t  handle, uint8_t  flag);

/**
 * \brief 使能MPU6050的中断  
 * \param[in] handle : 指向MPU6050设备操作句柄的指针
 * \param[in] flag   : AM_MPU6050_MOT_EN_ENALBE 一类的宏，可以组合                 
 * \return    无
 */
void am_mpu6050_int_enable (am_mpu6050_handle_t  handle, uint32_t  flags);

/**
 * \brief 禁能MPU6050的中断   
 * \param[in] handle : 指向MPU6050设备操作句柄的指针
 * \param[in] flag   : AM_MPU6050_MOT_EN_ENALBE一类的宏，可以组合                
 * \return    无
 */
void am_mpu6050_int_disable (am_mpu6050_handle_t  handle, uint32_t  flags);

/**
 * \brief 查询MPU6050的中断状态 
 * \param[in] handle : 指向MPU6050设备操作句柄的指针               
 * \return    返回各中断状态，一般与AM_MPU6050_MOT_INT_CHECK一类的宏相与
 */
uint8_t am_mpu6050_int_staus_get (am_mpu6050_handle_t  handle);

/**
 * \brief 设置MPU6050的引脚功能及中断引脚状态  
 * \param[in] handle : 指向MPU6050设备操作句柄的指针
 * \param[in] flags  : AM_MPU6050_INT_LEVEL_SET一类的宏及其这一类的宏值取反组合，
 *                     组合后典型值为0x80                 
 * \return    无
 */
void am_mpu6050_int_pin_set (am_mpu6050_handle_t  handle, uint32_t  flags);

/**
 * \brief 选择读x,y,z各轴加速度与角度的值   
 * \param[in] handle : 指向MPU6050设备操作句柄的指针
 * \param[in] buffer : 加速度与传感器的数值   
 * \param[in] flag   : 选择返回的加速度或角速度的数值，为AM_MPU6050_DATA_ACC_SELECT一类的宏值 
 * \return    无
 */
void am_mpu6050_xyz_data_read (am_mpu6050_handle_t  handle, int16_t  *buffer, uint8_t  flag);

/**   
 * \brief 单独读每个轴的值 
 * \param[in] handle   : 指向MPU6050设备操作句柄的指针
 * \param[in] sub_addr : AM_MPU6050_ACCEL_*_H 的宏 如# AM_MPU6050_ACCEL_XOUT_H   
 *
 * \return    返回每个轴的原始数值
 */
int16_t am_mpu6050_sigle_data_read (am_mpu6050_handle_t  handle, uint8_t  sub_addr);

/**   
 * \brief MPU6050数据处理 ，把加速度变成重力加速度G的表示方法，把陀螺仪变成角速度表示
 * \param[in] handle   : 指向MPU6050设备操作句柄的指针
 * \param[in] buffer   : 指向MPU6050原始数据的缓冲区指针
 * \param[in] buf      : 指向转换数据缓冲区指针
 *
 * \return    返回每个轴的原始数值
 */
void am_mpu6050_data_handle (am_mpu6050_handle_t  handle, float  *buf, int16_t  *buffer);

/**
 * \brief 初始化mpu6050
 *
 * \param[in] p_dev     : mpu6050设备结构体指针
 * \param[in] handle    : I2C标准服务操作句柄
 * \param[in] p_devinfo : mpu6050设备信息结构体
 *
 * \return   mpu6050的操作句柄，如果值为NULL，表明初始化失败
 */
am_mpu6050_handle_t am_mpu6050_init (am_mpu6050_dev_t  *p_dev,
                                     const am_mpu6050_device_t  *p_devinfo,
                                     am_i2c_handle_t    handle);
                                                                                                  
/**
 * \brief 解除mpu6050初始化
 *
 * \param[in] p_dev  : mpu6050设备结构体指针
 *
 * \return    None
 */
void am_mpu6050_deinit (am_mpu6050_dev_t  *p_dev);
                                                                         
/**
 * \brief MPU6050读设备ID
 *
 * \param[in] handle    : 指向MPU6050设备操作句柄的指针
 * \param[in] p_id      : 用户保存ID的缓冲区指针
 *
 * \return   无
 */                                                                    
void am_mpu6050_id_read (am_mpu6050_handle_t  handle, uint8_t  *p_id);

/**
 * \brief 打开MPU6050的I2C主模式
 *
 * \param[in] handle    : 指向MPU6050设备操作句柄的指针
 * \param[in] flag      : 默认值为0
 *
 * \return   无
 */ 
void am_mpu6050_i2c_master_open (am_mpu6050_handle_t  handle, uint8_t  flag);

/**
 * \brief 关闭MPU6050的I2C主模式
 *
 * \param[in] handle    : 指向MPU6050设备操作句柄的指针
 * \param[in] flag      : 默认值为0
 *
 * \return   无
 */ 
void am_mpu6050_i2c_master_close (am_mpu6050_handle_t  handle, uint8_t  flag);

/**
 * \brief 设置MPU6050的FIFO
 *
 * \param[in] handle    : 指向MPU6050设备操作句柄的指针
 * \param[in] flag      : 默认值为0
 *
 * \return   无
 */ 
void am_mpu6050_fifo_set (am_mpu6050_handle_t  handle, uint8_t  flags);                                     
                                                                        
                                     
#ifdef MPU6050_DMP                                       
/**
 * \brief DMP库通过I2C总线连续读
 *
 * \param[in] addr     : 器件地址 
 * \param[in] reg      : 寄存器地址
 * \param[in] len      : 写入长度
 * \param[in] p_buf    : 数据区
 *
 * \note 此函数仅用于DMP库对MPU6050进行I2C读
 * \retval  AM_OK      : 读数据完成;
 * \return  其他       : 返回错误代码
 */                   
int am_mpu_read_len (uint8_t  addr, uint8_t  reg, uint8_t  len, uint8_t  *p_buf);

/**
 * \brief DMP库通过I2C总线连续写
 *
 * \param[in] addr     : 器件地址 
 * \param[in] reg      : 寄存器地址
 * \param[in] len      : 写入长度
 * \param[in] buf      : 数据区
 *
 * \note 此函数仅用于DMP库对MPU6050进行I2C写
 * \retval  AM_OK      : 读数据完成 
 * \return  其他       ：错误代码
 */                   
int am_mpu_write_len (uint8_t  addr, uint8_t  reg, uint8_t  len, uint8_t  *p_buf);
  
extern am_i2c_handle_t      i2c_handle     ;  /**< \brief 引用全局变量，用于DMP与姿态解算 */ 
extern am_mpu6050_handle_t  mpu6050_handle ;  /**< \brief 在am_mpu6050.c文件中定义       */  
extern am_mpu6050_dev_t     g_mpu6050_dev  ;  /**< \brief 在am_mpu6050.c文件中定义       */ 

#endif  /* MPU6050_DMP */                                   
 
/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_MPU6050_H */

/* end of file */     
