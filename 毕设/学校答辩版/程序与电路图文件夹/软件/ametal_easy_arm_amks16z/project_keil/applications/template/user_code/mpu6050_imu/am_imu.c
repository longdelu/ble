/*******************************************************************************
*                                 Apollo
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
 * \brief mpu6050 imu implementation
 * 
 * \internal
 * \par Modification history
 * - 1.00 16-02-26  lgr, first implementation.
 * \endinternal
 */ 
 
 
#include "am_imu.h"
#include "am_mpu6050.h"
#include "am_i2c.h"
#include "am_delay.h"

int16_t g_buffer[7];            /**< \brief �������ݻ�����, ������  */

/** \brief MPU6050���ٶ��������ǽ������ݻ�����, �������ս�����   */
am_imu_sensor_data_t g_gyrobuf;         
am_imu_sensor_data_t g_accbuf;

/* ���ٶ��������ǵ�ƫ��ֵ */
am_imu_sensor_data_t g_gyrooffset;
am_imu_sensor_data_t g_accoffset;

/* ��ɽ��ٶ�����ٶ���ֵ��ʾ */
am_imu_data_t g_gyrofinal;
am_imu_data_t g_accfinal;

am_imu_angle_t g_angle;            /**< \brief�����̬�ǽṹ��          */

/** \brief ���ٶȻ��������˲����� */
int16_t    g_acc_x_buf[AM_IMU_FILTER_NUM],g_acc_y_buf[AM_IMU_FILTER_NUM],g_acc_z_buf[AM_IMU_FILTER_NUM]; 
            
/* ȡ�ü��ٶȵĵ�ԭʼֵ�����Ĵ�������������� */
void am_imu_data_get (void)
{
    am_mpu6050_xyz_data_read(mpu6050_handle, g_buffer, AM_MPU6050_DATA_ALL_SELECT); 
    
    g_accbuf.X   = g_buffer[0];          /**< \brief ��ȡX����ٶ����� */
    g_accbuf.Y   = g_buffer[1];          /**< \brief ��ȡY����ٶ����� */
    g_accbuf.Z   = g_buffer[2];          /**< \brief ��ȡZ����ٶ����� */
  
    g_gyrobuf.X  = g_buffer[4];          /**< \brief ��ȡX������������ */
    g_gyrobuf.Y  = g_buffer[5];          /**< \brief ��ȡY������������ */
    g_gyrobuf.Z  = g_buffer[6];          /**< \brief ��ȡZ������������ */
     
}

/* ƽ�����У����У������������尲װ�����ĵ����ƫ�*/
void am_imu_zero_correct (void)
{
    g_angle.Pitch=g_angle.Pitch-PITCH_OFFEST;     /**< \brief  pitch */
    g_angle.Roll=g_angle.Roll-ROLL_OFFEST;        /**< \brief  roll  */
}

/* ���ٶȼƻ�׼У��  */
void am_imu_acc_correct(void)
{
    uint16_t i = 0;
    uint16_t numAcc = 200;

    int32_t  Angleaccx = 0;
    int32_t  Angleaccy = 0;
    int32_t  Angleaccz = 0;                        /**< \brief ���ٶȼ�У���м���� */

    for (i = 0;i < numAcc; i++) {        
        am_imu_data_get();
        Angleaccx += g_accbuf.X;
        Angleaccy += g_accbuf.Y;
        
        /* ��ʼ�������������� */
        if (i  < AM_IMU_FILTER_NUM) {
            
            g_acc_x_buf[i] = g_accbuf.X;
            g_acc_y_buf[i] = g_accbuf.Y;
            g_acc_z_buf[i] = g_accbuf.Z;
            
        }
            
        am_mdelay(2);
    }    

    /** \brief ƽ����ֵ�õ����ٶȼƻ�׼ */
    g_accoffset.X = Angleaccx / numAcc;                       
    g_accoffset.Y = Angleaccy / numAcc;
    g_accoffset.Z = Angleaccz / numAcc;             
    
    AM_DBG_INFO("x_accoffset=%d y_accoffset=%d z_accoffset=%d\r\n", g_accoffset.X, g_accoffset.Y, g_accoffset.Z);
        
}


/*   �����ǻ�׼У��         */
void am_imu_gyro_correct (void)
{
    uint16_t i       = 0;
    uint16_t numGyro = 200;

    int32_t Gyrox = 0;
    int32_t Gyroy = 0;
    int32_t Gyroz = 0;                              /**< \brief ������У���м���� */

    for (i = 0; i < numGyro; i++) {
        am_imu_data_get();
        Gyrox += g_gyrobuf.X;
        Gyroy += g_gyrobuf.Y;
        Gyroz += g_gyrobuf.Z;
        am_mdelay(2);
    }    

    g_gyrooffset.X = Gyrox / numGyro;                       
    g_gyrooffset.Y = Gyroy / numGyro;
    g_gyrooffset.Z = Gyroz / numGyro;
    
    AM_DBG_INFO("x_gyrooffset=%d y_gyrooffset=%d z_gyrooffset=%d\r\n", g_gyrooffset.X, g_gyrooffset.Y, g_gyrooffset.Z);
}

    
/* ����ƽ��ֵ�˲����¼��ٶ�ֵ����ֹ���ٶ�ͻ�� */
void am_imu_data_prepare (void)
{  
    /* Ϊ��̬���������ﻬ�����ڵ����ֵ�����´����ȿ�ʼ */
    static uint8_t filter_cnt = 0;
    
    am_imu_sensor_data_t acc_avg;           /**< \brief ACC ������������      */  
    
    int32_t temp1 = 0, temp2 = 0, temp3 = 0;
    
    uint8_t i = 0;   
    
    /* �������� */
    am_imu_data_get();
    
    /* 
    ** ����2g�����£�0.061Ϊmg/LSB���㵥λ
    ** +-2g,16384sb/g--0.0610mg/lsb
    ** �˴�0.0098�ǣ�(9.8m/s^2)/1000,����mg��m/s^2    
    */
    acc_avg.X  = g_accbuf.X - g_accoffset.X;         
    acc_avg.Y  = g_accbuf.Y - g_accoffset.Y;        
    acc_avg.Z  = g_accbuf.Z - g_accoffset.Z;    
    
    /* ���»����������� */
    g_acc_x_buf[filter_cnt] = acc_avg.X;
    g_acc_y_buf[filter_cnt] = acc_avg.Y;
    g_acc_z_buf[filter_cnt] = acc_avg.Z;
    
    for (i = 0; i < AM_IMU_FILTER_NUM; i++) {
        temp1 += g_acc_x_buf[i];
        temp2 += g_acc_y_buf[i];
        temp3 += g_acc_z_buf[i];
    }
    
    g_accfinal.X = temp1 / AM_IMU_FILTER_NUM;
    g_accfinal.Y = temp2 / AM_IMU_FILTER_NUM;
    g_accfinal.Z = temp3 / AM_IMU_FILTER_NUM;
    
    filter_cnt++;
    
    if (filter_cnt == AM_IMU_FILTER_NUM) {
        filter_cnt=0;  
    }        
    
    /* 
     * 1/16.4=0.061��0.061Ϊ���ٶ�/LSB�Ļ��㵥λ,�˴���0.0174��3.14/180,
     *  ����ÿ���Ϊ����ÿ��.����ֵ��ȥ��׼ֵ���Ե�λ�����������ǽ��ٶȣ���λ����ÿ�� 
     */
    g_gyrofinal.X = (float)(g_gyrobuf.X-g_gyrooffset.X) * 0.061f * 0.0174f; 
    g_gyrofinal.Y = (float)(g_gyrobuf.Y-g_gyrooffset.Y) * 0.061f * 0.0174f;
    g_gyrofinal.Z = (float)(g_gyrobuf.Z-g_gyrooffset.Z) * 0.061f * 0.0174f;        
    
    g_accfinal.X  = (float)(g_accfinal.X * 0.061f) * 0.0098f;        
    g_accfinal.Y  = (float)(g_accfinal.Y * 0.061f) * 0.0098f;        
    g_accfinal.Z  = (float)(g_accfinal.Z * 0.061f) * 0.0098f;    

    /*
    AM_DBG_INFO("x_gyrfinal=%4.2f y_gyrfinal=%4.2f z_gyrfinal=%4.2f\r\n", g_gyrofinal.X, g_gyrofinal.Y, g_gyrofinal.Z);
    AM_DBG_INFO("x_accfinal=%4.2f y_accfinal=%4.2f z_accfinal=%4.2f\r\n", g_accfinal.X, g_accfinal.Y, g_accfinal.Z);
    */
    
}


#define Kp 10.0f         /**< \brief proportional gain governs rate of convergence to accelerometer/magnetometer */
#define Ki 0.008f        /**< \brief integral gain governs rate of convergence of gyroscope biases */
float   halfT = 0.0025;  /**< \brief ����ʱ���һ�룬Ϊ��ʱ��0�ж����ڵ�һ�� */

static float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;   
static float exInt = 0.0f, eyInt = 0.0f, ezInt = 0.0f;

/* ���ٶ�У�������� */
void am_imu_ag_updata (float gx, float gy, float gz, float ax, float ay, float az)
{
    float norm;
    float vx, vy, vz;
    float ex, ey, ez;

    float q0q0 = q0*q0;
    float q0q1 = q0*q1;
    float q0q2 = q0*q2;
    float q1q1 = q1*q1;
    float q1q3 = q1*q3;
    float q2q2 = q2*q2;
    float q2q3 = q2*q3;
    float q3q3 = q3*q3;
    
    if (ax * ay * az == 0) {
        
         return;
    }
    
    /* ���������� ��MPU6050���ٶȼƵ���ά����ת�ɵ�λ���� */
    arm_sqrt_f32(ax * ax + ay * ay + az * az, &norm);       
    ax = ax / norm;
    ay = ay / norm;
    az = az / norm;

    /*
     * ���ǰ���Ԫ������ɡ��������Ҿ����еĵ����е�����Ԫ�ء�
     * �������Ҿ����ŷ���ǵĶ��壬��������ϵ������������ת����������ϵ��������������Ԫ�ء�
     * ���������vx\y\z����ʵ���ǵ�ǰ��ŷ���ǣ�����Ԫ�����Ļ����������ϵ�ϣ����������������λ���� 
     * ���Ʒ��������,�õ����۵�ֵ�ļ��ٶ�    
     */
    vx = 2 * (q1q3 - q0q2);                                             
    vy = 2 * (q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;

    /*
     * ������ˣ��õ���������ϵ�ĺϼ��ٶȣ�������MPU6050�ļ��ٶȵĲ���ֵ���������ϵ
     * ����������ϵ�ļ��ٶ�����ֵ���Ĵ�С�뷽��, ��ΪMPU6050��������ֵ������������
     */
    
    /*
     * axyz�ǻ����������ϵ�ϣ����ٶȼƲ����������������Ҳ����ʵ�ʲ����������������
     * axyz�ǲ����õ�������������vxyz�����ݻ��ֺ����̬����������������������Ƕ��ǻ����������ϵ�ϵ�����������
     * ������֮�������������������ݻ��ֺ����̬�ͼӼƲ��������̬֮�����
     * ������������������������Ҳ�������������ˣ�����ʾ��exyz�����������������Ĳ����
     * �����������Ծ���λ�ڻ�������ϵ�ϵģ������ݻ������Ҳ���ڻ�������ϵ�����Ҳ���Ĵ�С�����ݻ����������� 
     * ���������������ݡ���������Լ��ö�������һ�£����������ǶԻ���ֱ�ӻ��֣����Զ����ݵľ�������ֱ��������
     * �Ի�������ϵ�ľ�����
     */
    ex = (ay * vz - az * vy);                                               
    ey = (az * vx - ax * vz);
    ez = (ax * vy - ay * vx);
    
    /*   �����Ӧ�û��ַ���                           */
    exInt = exInt + ex * Ki ;                     
    eyInt = eyInt + ey * Ki ;
    ezInt = ezInt + ez * Ki ;

    /* У�������ǲ���ֵ, �ò���������PI����������ƫ */
    gx = gx + Kp * ex + exInt;                                                
    gy = gy + Kp * ey + eyInt;
    gz = gz + Kp * ez + ezInt;         
          
    /*  ������Ԫ����, ��Ԫ��΢�ַ���, ��Ԫ�������㷨��һ�����ⷨ */                                           
    q0 = q0 + (-q1*gx - q2*gy - q3*gz)* halfT;                            
    q1 = q1 + (q0*gx + q2*gz - q3*gy) * halfT;
    q2 = q2 + (q0*gy - q1*gz + q3*gx) * halfT;
    q3 = q3 + (q0*gz + q1*gy - q2*gx) * halfT;                     
    
    /* ��Ԫ���淶�� */    
    arm_sqrt_f32(q0*q0 + q1*q1 + q2*q2 + q3*q3, &norm);        
    q0 = q0 / norm;
    q1 = q1 / norm;
    q2 = q2 / norm;
    q3 = q3 / norm;

    /* ת��Ϊŷ���� */
    g_angle.Pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3f;                                     /* pitch ������-y�� */
    g_angle.Roll  = atan2(2 * q2 * q3 + 2 * q0 * q1,  -2 * q1 * q1 - 2 * q2 * q2 + 1)* 57.3f;    /* roll �����-x��  */
    
#if 0
    g_angle.Yaw   = -atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1)* 57.3f;   /* yaw ƫ����-Z��   */
#endif /* 0 */

    g_angle.Yaw   += gz * 57.3 * 0.0025;
}

/* ���ٶ���ش�У�������� */
void am_imu_agm_updata (float gx, float gy, float gz, float ax, float ay, float az,float mx, float my, float mz)
{
    float norm;
    float hx, hy, hz, bx, bz;
    float vx, vy, vz, wx, wy, wz;
    float ex, ey, ez; 
    static float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;

    /*  �Ȱ���Щ�õõ���ֵ��� */
    float q0q0 = q0*q0;
    float q0q1 = q0*q1;
    float q0q2 = q0*q2;
    float q0q3 = q0*q3;
    float q1q1 = q1*q1;
    float q1q2 = q1*q2;
    float q1q3 = q1*q3;
    float q2q2 = q2*q2;   
    float q2q3 = q2*q3;
    float q3q3 = q3*q3;
 
    float delta_2=0;

    const static float FACTOR = 0.002;

    arm_sqrt_f32(ax * ax + ay * ay + az * az, &norm);       
    ax = ax / norm;
    ay = ay / norm;
    az = az / norm;

    norm = sqrt(mx * mx + my * my + mz * mz);          
    mx = mx / norm;
    my = my / norm;
    mz = mz / norm;

    /*  compute reference direction of flux */
    hx = 2*mx*(0.5f - q2q2 - q3q3) + 2*my*(q1q2 - q0q3) + 2*mz*(q1q3 + q0q2);
    hy = 2*mx*(q1q2 + q0q3) + 2*my*(0.5f - q1q1 - q3q3) + 2*mz*(q2q3 - q0q1);
    hz = 2*mx*(q1q3 - q0q2) + 2*my*(q2q3 + q0q1) + 2*mz*(0.5f - q1q1 - q2q2);         
    bx = sqrt((hx*hx) + (hy*hy));
    bz = hz;     
  
    /*  estimated direction of gravity and flux (v and w) */
    vx = 2*(q1q3 - q0q2);
    vy = 2*(q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;
    wx = (2*bx*(0.5f - q2q2 - q3q3) + 2*bz*(q1q3 - q0q2));
    wy = 2*bx*(q1q2 - q0q3) + 2*bz*(q0q1 + q2q3);
    wz = (2*bx*(q0q2 + q1q3) + 2*bz*(0.5f - q1q1 - q2q2));  
  
    /* 
     * error is sum of cross product between
     * reference direction of fields and direction measured by sensors 
     */
    ex = (ay*vz - az*vy) + (my*wz - mz*wy);
    ey = (az*vx - ax*vz) + (mz*wx - mx*wz);
    ez = (ax*vy - ay*vx) + (mx*wy - my*wx);


    /*  
     * halfT=GET_NOWTIME(),���μ����ʱ��������λ�� 
     * У�������ǲ���ֵ,�ò���������PI����������ƫ.û��ʹ��IУ�������ܴ��������� 
     */    
    
    gx = gx + ex * FACTOR/halfT;                                     
    gy = gy + ey * FACTOR/halfT; 
    gz = gz + ez * FACTOR/halfT;     
      
    delta_2=(2*halfT*gx)*(2*halfT*gx)+(2*halfT*gy)*(2*halfT*gy)+(2*halfT*gz)*(2*halfT*gz);    
 
    /* ������Ԫ����     ��Ԫ��΢�ַ���    ��Ԫ�������㷨�����ױϿ��� */
    q0 = (1-delta_2/8)*q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
    q1 = (1-delta_2/8)*q1 + (q0*gx + q2*gz - q3*gy)*halfT;
    q2 = (1-delta_2/8)*q2 + (q0*gy - q1*gz + q3*gx)*halfT;
    q3 = (1-delta_2/8)*q3 + (q0*gz + q1*gy - q2*gx)*halfT;             
   
    /* ������Ԫ����,��Ԫ��΢�ַ���,��Ԫ�������㷨��һ�����ⷨ */ 
    q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;                
    q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
    q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
    q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;             
      
    /* ��������Ԫ�� */                                       
    norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);                
    q0 = q0 / norm;
    q1 = q1 / norm;
    q2 = q2 / norm;
    q3 = q3 / norm;

    /* ת��Ϊŷ���� */
    g_angle.Pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3f;                                   /* pitch */
    g_angle.Roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3f;   /* roll */
    g_angle.Yaw   = -atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3 * q3 + 1)* 57.3f;   /* yaw */

}


/* end of file */
