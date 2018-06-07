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
 * \brief mpu6050姿态解算四元数标准接口层
 *
 * \internal
 * \par Modification History
 * - 1.00 16-02-29  lgr, first implementation.
 * \endinternal
 */

#ifndef __AM_IMU_H
#define	__AM_IMU_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include <arm_math.h>
#include <stdint.h>

/**
 * \name 俯仰角与横滚角的偏移系数
 * @{ 
 */  
    
#define AM_IMU_FILTER_NUM  20  /**< \brief加速度计滑动滤波的宏                */ 

/** @} */     

/**
 * \name 俯仰角与横滚角的偏移系数
 * @{ 
 */     
    
#define PITCH_OFFEST  -1.0
#define ROLL_OFFEST   -1.0
    
/** @} */  

/**
 * \brief MPU6050的四元数姿态角信息结构体 
 */
typedef struct am_imu_angle{
	float Roll;                 /**< \brief 横滚角 */
	float Pitch;                /**< \brief 俯仰角 */
	float Yaw;                  /**< \brief 航各角 */
}am_imu_angle_t;  

/**
 * \brief MPU6050的寄存器原始值结构体 
 */
typedef struct am_imu_sensor_data{
	int16_t X;                  /**< \brief 传感器X轴原始值 */
	int16_t Y;                  /**< \brief 传感器Y轴原始值 */
	int16_t Z;                  /**< \brief 传感器Z轴原始值 */
}am_imu_sensor_data_t;

/**
 * \brief MPU6050的寄存器原始值经过处理得到的数值结构体
 */
typedef struct  am_imu_data{
	float X;                   /**< \brief 传感器X轴原始值 */
	float Y;                   /**< \brief 传感器X轴原始值 */
	float Z;                   /**< \brief 传感器X轴原始值 */   
}am_imu_data_t;

extern am_imu_angle_t   g_angle;        /**<  输出姿态角结构体                */
extern am_imu_data_t    g_gyrofinal;    /**<  输出姿态角结构体                */
extern am_imu_data_t	g_accfinal;     /**<  输出姿态角结构体                */

/**
 * \brief 取得加速度与陀螺仪原始值
 * \param[in] 无
 * \return    无
 */                
void am_imu_data_get (void);
                
/**
 * \brief 陀螺仪基准校正
 * \param[in] 无
 * \return    无
 */
void am_imu_gyro_correct (void);
                
                
/**
 * \brief 加速度计基准校正
 * \param[in] 无
 * \return    无
 */
void am_imu_acc_correct (void);
                
/**
 * \brief 平衡零点校正（校正传感器与机体安装产生的的相对偏差）
 * \param[in] 无
 * \return    无
 */
void am_imu_zero_correct (void);

/**
 * \brief 滑动平均值滤波更新加速度值，防止加速度突变
 * \param[in] 无
 * \return    无
 */                
void am_imu_data_prepare (void);


/**
 * \brief 四元数矩阵更新并取得姿态角
 * \param[in] gx，gy，gz： 陀螺仪三个轴方向角速度值
 * \param[in] ax，ay，az： 加速度三个轴方向的加速度值，以重力加速度方法来表示
 * \return    无
 */    
void am_imu_ag_updata (float gx, float gy, float gz, float ax, float ay, float az);			

/**
 * \brief 四元数矩阵更新并取得姿态角
 * \param[in] gx，gy，gz： 陀螺仪三个轴方向角速度值
 * \param[in] ax，ay，az： 加速度三个轴方向的加速度值，以重力加速度方法来表示
 * \param[in] mx，my，mz： 地磁传感器数值，用来校证航向角
 * \return    无
 */
void am_imu_agm_updata (float gx, float gy, float gz, float ax, float ay, float az,float mx, float my, float mz);


/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __IMU_H */
