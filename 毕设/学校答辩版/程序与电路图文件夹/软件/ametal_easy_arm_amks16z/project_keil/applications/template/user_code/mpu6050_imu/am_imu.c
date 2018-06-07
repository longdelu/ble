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

int16_t g_buffer[7];            /**< \brief 接收数据缓存区, 过程量  */

/** \brief MPU6050加速度与陀螺仪接收数据缓存区, 数据最终接收量   */
am_imu_sensor_data_t g_gyrobuf;         
am_imu_sensor_data_t g_accbuf;

/* 加速度与陀螺仪的偏移值 */
am_imu_sensor_data_t g_gyrooffset;
am_imu_sensor_data_t g_accoffset;

/* 变成角速度与加速度数值显示 */
am_imu_data_t g_gyrofinal;
am_imu_data_t g_accfinal;

am_imu_angle_t g_angle;            /**< \brief输出姿态角结构体          */

/** \brief 加速度滑动窗口滤波数组 */
int16_t    g_acc_x_buf[AM_IMU_FILTER_NUM],g_acc_y_buf[AM_IMU_FILTER_NUM],g_acc_z_buf[AM_IMU_FILTER_NUM]; 
            
/* 取得加速度的的原始值，即寄存器里面读出来的 */
void am_imu_data_get (void)
{
    am_mpu6050_xyz_data_read(mpu6050_handle, g_buffer, AM_MPU6050_DATA_ALL_SELECT); 
    
    g_accbuf.X   = g_buffer[0];          /**< \brief 读取X轴加速度数据 */
    g_accbuf.Y   = g_buffer[1];          /**< \brief 读取Y轴加速度数据 */
    g_accbuf.Z   = g_buffer[2];          /**< \brief 读取Z轴加速度数据 */
  
    g_gyrobuf.X  = g_buffer[4];          /**< \brief 读取X轴陀螺仪数据 */
    g_gyrobuf.Y  = g_buffer[5];          /**< \brief 读取Y轴陀螺仪数据 */
    g_gyrobuf.Z  = g_buffer[6];          /**< \brief 读取Z轴陀螺仪数据 */
     
}

/* 平衡零点校正（校正传感器与机体安装产生的的相对偏差）*/
void am_imu_zero_correct (void)
{
    g_angle.Pitch=g_angle.Pitch-PITCH_OFFEST;     /**< \brief  pitch */
    g_angle.Roll=g_angle.Roll-ROLL_OFFEST;        /**< \brief  roll  */
}

/* 加速度计基准校正  */
void am_imu_acc_correct(void)
{
    uint16_t i = 0;
    uint16_t numAcc = 200;

    int32_t  Angleaccx = 0;
    int32_t  Angleaccy = 0;
    int32_t  Angleaccz = 0;                        /**< \brief 加速度计校正中间变量 */

    for (i = 0;i < numAcc; i++) {        
        am_imu_data_get();
        Angleaccx += g_accbuf.X;
        Angleaccy += g_accbuf.Y;
        
        /* 初始化滑动窗口数组 */
        if (i  < AM_IMU_FILTER_NUM) {
            
            g_acc_x_buf[i] = g_accbuf.X;
            g_acc_y_buf[i] = g_accbuf.Y;
            g_acc_z_buf[i] = g_accbuf.Z;
            
        }
            
        am_mdelay(2);
    }    

    /** \brief 平均求值得到加速度计基准 */
    g_accoffset.X = Angleaccx / numAcc;                       
    g_accoffset.Y = Angleaccy / numAcc;
    g_accoffset.Z = Angleaccz / numAcc;             
    
    AM_DBG_INFO("x_accoffset=%d y_accoffset=%d z_accoffset=%d\r\n", g_accoffset.X, g_accoffset.Y, g_accoffset.Z);
        
}


/*   陀螺仪基准校正         */
void am_imu_gyro_correct (void)
{
    uint16_t i       = 0;
    uint16_t numGyro = 200;

    int32_t Gyrox = 0;
    int32_t Gyroy = 0;
    int32_t Gyroz = 0;                              /**< \brief 陀螺仪校正中间变量 */

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

    
/* 滑动平均值滤波更新加速度值，防止加速度突变 */
void am_imu_data_prepare (void)
{  
    /* 为静态变量，到达滑动窗口的最大值后重新从最先开始 */
    static uint8_t filter_cnt = 0;
    
    am_imu_sensor_data_t acc_avg;           /**< \brief ACC 滑动窗口数据      */  
    
    int32_t temp1 = 0, temp2 = 0, temp3 = 0;
    
    uint8_t i = 0;   
    
    /* 更新数据 */
    am_imu_data_get();
    
    /* 
    ** 正负2g量程下，0.061为mg/LSB换算单位
    ** +-2g,16384sb/g--0.0610mg/lsb
    ** 此处0.0098是：(9.8m/s^2)/1000,乘以mg得m/s^2    
    */
    acc_avg.X  = g_accbuf.X - g_accoffset.X;         
    acc_avg.Y  = g_accbuf.Y - g_accoffset.Y;        
    acc_avg.Z  = g_accbuf.Z - g_accoffset.Z;    
    
    /* 更新滑动窗口数组 */
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
     * 1/16.4=0.061，0.061为角速度/LSB的换算单位,此处的0.0174是3.14/180,
     *  将度每秒变为弧度每秒.读出值减去基准值乘以单位，计算陀螺仪角速度，单位弧度每秒 
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
float   halfT = 0.0025;  /**< \brief 采样时间的一半，为定时器0中断周期的一半 */

static float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;   
static float exInt = 0.0f, eyInt = 0.0f, ezInt = 0.0f;

/* 加速度校正陀螺仪 */
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
    
    /* 测量正常化 把MPU6050加速度计的三维向量转成单位向量 */
    arm_sqrt_f32(ax * ax + ay * ay + az * az, &norm);       
    ax = ax / norm;
    ay = ay / norm;
    az = az / norm;

    /*
     * 这是把四元数换算成《方向余弦矩阵》中的第三列的三个元素。
     * 根据余弦矩阵和欧拉角的定义，地理坐标系的重力向量，转到机体坐标系，正好是这三个元素。
     * 所以这里的vx\y\z，其实就是当前的欧拉角（即四元数）的机体坐标参照系上，换算出来的重力单位向量 
     * 估计方向的重力,得到理论的值的加速度    
     */
    vx = 2 * (q1q3 - q0q2);                                             
    vy = 2 * (q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;

    /*
     * 向量叉乘，得到机体坐标系的合加速度（这里是MPU6050的加速度的测量值与地理坐标系
     * 到机体坐标系的加速度理论值）的大小与方向, 因为MPU6050测量的数值不包括重力的
     */
    
    /*
     * axyz是机体坐标参照系上，加速度计测出来的重力向量，也就是实际测出来的重力向量。
     * axyz是测量得到的重力向量，vxyz是陀螺积分后的姿态来推算出的重力向量，它们都是机体坐标参照系上的重力向量。
     * 那它们之间的误差向量，就是陀螺积分后的姿态和加计测出来的姿态之间的误差。
     * 向量间的误差，可以用向量叉积（也叫向量外积、叉乘）来表示，exyz就是两个重力向量的叉积。
     * 这个叉积向量仍旧是位于机体坐标系上的，而陀螺积分误差也是在机体坐标系，而且叉积的大小与陀螺积分误差成正比 
     * 正好拿来纠正陀螺。（你可以自己拿东西想象一下）由于陀螺是对机体直接积分，所以对陀螺的纠正量会直接体现在
     * 对机体坐标系的纠正。
     */
    ex = (ay * vz - az * vy);                                               
    ey = (az * vx - ax * vz);
    ez = (ax * vy - ay * vx);
    
    /*   计算和应用积分反馈                           */
    exInt = exInt + ex * Ki ;                     
    eyInt = eyInt + ey * Ki ;
    ezInt = ezInt + ez * Ki ;

    /* 校正陀螺仪测量值, 用叉积误差来做PI修正陀螺零偏 */
    gx = gx + Kp * ex + exInt;                                                
    gy = gy + Kp * ey + eyInt;
    gz = gz + Kp * ez + ezInt;         
          
    /*  整合四元数率, 四元数微分方程, 四元数更新算法，一阶龙库法 */                                           
    q0 = q0 + (-q1*gx - q2*gy - q3*gz)* halfT;                            
    q1 = q1 + (q0*gx + q2*gz - q3*gy) * halfT;
    q2 = q2 + (q0*gy - q1*gz + q3*gx) * halfT;
    q3 = q3 + (q0*gz + q1*gy - q2*gx) * halfT;                     
    
    /* 四元数规范化 */    
    arm_sqrt_f32(q0*q0 + q1*q1 + q2*q2 + q3*q3, &norm);        
    q0 = q0 / norm;
    q1 = q1 / norm;
    q2 = q2 / norm;
    q3 = q3 / norm;

    /* 转换为欧拉角 */
    g_angle.Pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3f;                                     /* pitch 俯仰角-y轴 */
    g_angle.Roll  = atan2(2 * q2 * q3 + 2 * q0 * q1,  -2 * q1 * q1 - 2 * q2 * q2 + 1)* 57.3f;    /* roll 横滚角-x轴  */
    
#if 0
    g_angle.Yaw   = -atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1)* 57.3f;   /* yaw 偏航角-Z轴   */
#endif /* 0 */

    g_angle.Yaw   += gz * 57.3 * 0.0025;
}

/* 加速度与地磁校正陀螺仪 */
void am_imu_agm_updata (float gx, float gy, float gz, float ax, float ay, float az,float mx, float my, float mz)
{
    float norm;
    float hx, hy, hz, bx, bz;
    float vx, vy, vz, wx, wy, wz;
    float ex, ey, ez; 
    static float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;

    /*  先把这些用得到的值算好 */
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
     * halfT=GET_NOWTIME(),两次计算的时间间隔，单位秒 
     * 校正陀螺仪测量值,用叉积误差来做PI修正陀螺零偏.没有使用I校正，可能存在有问题 
     */    
    
    gx = gx + ex * FACTOR/halfT;                                     
    gy = gy + ey * FACTOR/halfT; 
    gz = gz + ez * FACTOR/halfT;     
      
    delta_2=(2*halfT*gx)*(2*halfT*gx)+(2*halfT*gy)*(2*halfT*gy)+(2*halfT*gz)*(2*halfT*gz);    
 
    /* 整合四元数率     四元数微分方程    四元数更新算法，二阶毕卡法 */
    q0 = (1-delta_2/8)*q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
    q1 = (1-delta_2/8)*q1 + (q0*gx + q2*gz - q3*gy)*halfT;
    q2 = (1-delta_2/8)*q2 + (q0*gy - q1*gz + q3*gx)*halfT;
    q3 = (1-delta_2/8)*q3 + (q0*gz + q1*gy - q2*gx)*halfT;             
   
    /* 整合四元数率,四元数微分方程,四元数更新算法，一阶龙库法 */ 
    q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;                
    q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
    q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
    q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;             
      
    /* 正常化四元数 */                                       
    norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);                
    q0 = q0 / norm;
    q1 = q1 / norm;
    q2 = q2 / norm;
    q3 = q3 / norm;

    /* 转换为欧拉角 */
    g_angle.Pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3f;                                   /* pitch */
    g_angle.Roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3f;   /* roll */
    g_angle.Yaw   = -atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3 * q3 + 1)* 57.3f;   /* yaw */

}


/* end of file */
