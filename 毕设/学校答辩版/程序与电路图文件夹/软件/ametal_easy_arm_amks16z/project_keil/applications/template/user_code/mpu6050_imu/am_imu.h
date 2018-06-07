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
 * \brief mpu6050��̬������Ԫ����׼�ӿڲ�
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
 * \name �����������ǵ�ƫ��ϵ��
 * @{ 
 */  
    
#define AM_IMU_FILTER_NUM  20  /**< \brief���ٶȼƻ����˲��ĺ�                */ 

/** @} */     

/**
 * \name �����������ǵ�ƫ��ϵ��
 * @{ 
 */     
    
#define PITCH_OFFEST  -1.0
#define ROLL_OFFEST   -1.0
    
/** @} */  

/**
 * \brief MPU6050����Ԫ����̬����Ϣ�ṹ�� 
 */
typedef struct am_imu_angle{
	float Roll;                 /**< \brief ����� */
	float Pitch;                /**< \brief ������ */
	float Yaw;                  /**< \brief ������ */
}am_imu_angle_t;  

/**
 * \brief MPU6050�ļĴ���ԭʼֵ�ṹ�� 
 */
typedef struct am_imu_sensor_data{
	int16_t X;                  /**< \brief ������X��ԭʼֵ */
	int16_t Y;                  /**< \brief ������Y��ԭʼֵ */
	int16_t Z;                  /**< \brief ������Z��ԭʼֵ */
}am_imu_sensor_data_t;

/**
 * \brief MPU6050�ļĴ���ԭʼֵ��������õ�����ֵ�ṹ��
 */
typedef struct  am_imu_data{
	float X;                   /**< \brief ������X��ԭʼֵ */
	float Y;                   /**< \brief ������X��ԭʼֵ */
	float Z;                   /**< \brief ������X��ԭʼֵ */   
}am_imu_data_t;

extern am_imu_angle_t   g_angle;        /**<  �����̬�ǽṹ��                */
extern am_imu_data_t    g_gyrofinal;    /**<  �����̬�ǽṹ��                */
extern am_imu_data_t	g_accfinal;     /**<  �����̬�ǽṹ��                */

/**
 * \brief ȡ�ü��ٶ���������ԭʼֵ
 * \param[in] ��
 * \return    ��
 */                
void am_imu_data_get (void);
                
/**
 * \brief �����ǻ�׼У��
 * \param[in] ��
 * \return    ��
 */
void am_imu_gyro_correct (void);
                
                
/**
 * \brief ���ٶȼƻ�׼У��
 * \param[in] ��
 * \return    ��
 */
void am_imu_acc_correct (void);
                
/**
 * \brief ƽ�����У����У������������尲װ�����ĵ����ƫ�
 * \param[in] ��
 * \return    ��
 */
void am_imu_zero_correct (void);

/**
 * \brief ����ƽ��ֵ�˲����¼��ٶ�ֵ����ֹ���ٶ�ͻ��
 * \param[in] ��
 * \return    ��
 */                
void am_imu_data_prepare (void);


/**
 * \brief ��Ԫ��������²�ȡ����̬��
 * \param[in] gx��gy��gz�� �����������᷽����ٶ�ֵ
 * \param[in] ax��ay��az�� ���ٶ������᷽��ļ��ٶ�ֵ�����������ٶȷ�������ʾ
 * \return    ��
 */    
void am_imu_ag_updata (float gx, float gy, float gz, float ax, float ay, float az);			

/**
 * \brief ��Ԫ��������²�ȡ����̬��
 * \param[in] gx��gy��gz�� �����������᷽����ٶ�ֵ
 * \param[in] ax��ay��az�� ���ٶ������᷽��ļ��ٶ�ֵ�����������ٶȷ�������ʾ
 * \param[in] mx��my��mz�� �شŴ�������ֵ������У֤�����
 * \return    ��
 */
void am_imu_agm_updata (float gx, float gy, float gz, float ax, float ay, float az,float mx, float my, float mz);


/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __IMU_H */
