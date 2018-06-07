/*******************************************************************************
*                                 Apollo
*                       ---------------------------
*                       innovating embedded systems
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
 * \brief MPU6050演示例程，通过AMetal的I2C标准接口实现
 *
 * - 操作步骤：
 *  1. 给mpu6050上电；
 *  2. 把控制器芯片I2C外设引脚PTC8 SCL,PTC9 SDA接到MPU6050的I2C接口相对应的引脚上;
 *  3. PTA2连接到PC串口的RXD,PTA2连接到PC串口的TXD。 
 *
 * - 实验现象:
 *  1. 串口打印姿态信息
 *
 *
 * \par 源代码
 * \snippet demo_std_imu_mpu6050.c src_std_imu_mpu6050 
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-11-20  lgr, first implementation.
 * \endinternal
 */ 
 
/**
 * \addtogroup demo_if_std_imu_mpu6050
 * \copydoc demo_std_imu_mpu6050.c
 */

/** [src_std_imu_mpu6050] */

#include "ametal.h"
#include "am_board.h"
#include "am_mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

volatile bool_t g_mpu6050_flag  = FALSE;          /**< \brief mpu6050中断标志位定义    */

/** \brief mpu6050设备信息结构体 */
const am_mpu6050_device_t g_mpu6050_devinfo = {
    AM_MPU6050_ADDRESS,
    AM_I2C_M_7BIT | AM_I2C_SUBADDR_1BYTE,
    PIOA_5,
};

/* 定义I2C设备结构体，用于分配相关空间 */  
am_i2c_handle_t      i2c_handle     = NULL;  


/* 定义串口（中断模式）设备结构体，用于分配相关空间 */    
am_uart_handle_t     uart_handle     = NULL;
am_uart_int_handle_t uart_int_handle = NULL;
am_uart_int_dev_t    g_uart0_int_dev;


volatile bool_t g_tpmtimer0_flag  = FALSE;          /**< \brief 定时器0中断标志位定义    */

/** 
 * \brief  定时器0回调函数
 * \param[in] p_arg : 用户自定义参数，由am_timer_callback_set() 函数传递。
 * \return  无
 */
void tpm0_timing_callback (void *p_arg)
{
    g_tpmtimer0_flag = TRUE;
    
    am_led_toggle(LED0);  /* LED0反转 */
}


/* 数据发送编码 */
void am_data_send(void)
{
    uint8_t _cnt=0;    
    
    uint8_t sum = 0;
    
    uint8_t i = 0;
    
    int16_t pst[3] = {0};
    
    uint8_t data_to_send[23];                  
    

    pst[0] = g_angle.Pitch * 100;
    pst[1] = g_angle.Roll * 100;
    pst[2] = g_angle.Yaw * 100;
 
    
    /* 帧头     */
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0x01;
    data_to_send[_cnt++]=0;
    
    /* 姿态数据 */
    data_to_send[_cnt++]=(uint8_t)(pst[0]>>8);  
    data_to_send[_cnt++]=(uint8_t)pst[0];       
    data_to_send[_cnt++]=(uint8_t)(pst[1]>>8);
    data_to_send[_cnt++]=(uint8_t)pst[1];
    data_to_send[_cnt++]=(uint8_t)(pst[2]>>8);
    data_to_send[_cnt++]=(uint8_t)pst[2];
    
    /* 飞行高度 */
    data_to_send[_cnt++]=0;
    data_to_send[_cnt++]=0;
    data_to_send[_cnt++]=0;
    data_to_send[_cnt++]=10;
    
    /* 飞控解锁 */
    data_to_send[_cnt++]=0xA1;
    
    /* 定义用户自定义发送了多少个字节 */
    data_to_send[3] = _cnt-4;                     
      
    /* 求和校验 */      
    for(i=0; i < _cnt; i++) {
        sum += data_to_send[i];
    }
        
    data_to_send[_cnt++] = sum;
    
    for(i=0; i < _cnt; i++) {

      /* 串口发送一个字节函数 */
      am_uart_int_send(uart_int_handle, &data_to_send[i], 1);   
    }
  
}

/**
 * \brief MPU6050引脚中断服务函数
 */
void mpu6050_gpio_isr (void *p_arg)
{
    /* 标志中断标志 */
    g_mpu6050_flag = TRUE;
    
    am_led_toggle(LED0);
}

/**
 * \brief 主函数
 */
int main(void)
{   
    int16_t buffer[7] = {0, 0};
    
    am_timer_handle_t tpm0_timing_handle;
    am_timer_info_t   tpm0_timing_info;
    
    /* 板级初始化          */
    am_board_init();
    
    /* UART0平台初始化     */
    amhw_plfm_uart0_init();
    
    /* 定时器0平台初始化   */
    amhw_plfm_tpm0_timing_init();    
    
    /* I2C0 硬件平台初始化 */
    amhw_plfm_i2c0_init();    
    
   /* UART0初始化 */
    uart_handle     = amdr_uart_init(&g_uart0_dev, &g_uart0_devinfo);

    /* UART0初始化为中断模式 */
    uart_int_handle = am_uart_int_init(&g_uart0_int_dev, uart_handle); 

    
    /* I2C 初始化         */
    i2c_handle = amdr_i2c_init(&g_i2c0_dev, &g_i2c0_devinfo);
    
    /* I2C 中断连接       */
    am_i2c_connect(i2c_handle);   
    
    /* MPU6050初始化      */
    mpu6050_handle = am_mpu6050_init(&g_mpu6050_dev, &g_mpu6050_devinfo, i2c_handle);
    
    am_mdelay(500);
    am_mpu6050_xyz_data_read(mpu6050_handle, buffer, AM_MPU6050_DATA_ALL_SELECT);  
    am_mpu6050_xyz_data_read(mpu6050_handle, buffer, AM_MPU6050_DATA_ALL_SELECT);  
    
    AM_DBG_INFO("X_ACC=%d Y_ACC=%d Z_ACC=%d\r\n", buffer[0], buffer[1], buffer[2]); 
    AM_DBG_INFO("X_GRO=%d Y_GRO=%d Z_GRO=%d\r\n", buffer[4], buffer[5], buffer[6]);

    am_imu_acc_correct();      
    am_imu_gyro_correct();
    
    
    /* 初始化定时器0为定时功能      */
    tpm0_timing_handle = amdr_tpm_timing_init(&g_tpm0_timing_dev, 
                                              &g_tpm0_timing_devinfo); 
    am_timer_connect(tpm0_timing_handle);
    
    am_timer_info_get(tpm0_timing_handle, &tpm0_timing_info);
    
    am_timer_callback_set(tpm0_timing_handle, 0, tpm0_timing_callback, NULL);
    
    /* 设定定时器0中断频率为200Hz    */
    am_timer_enable(tpm0_timing_handle, 0, tpm0_timing_info.clk_frequency / 200);
       
    
    while (1) { 
               
        if (g_tpmtimer0_flag == TRUE) {
            
            
            am_imu_data_prepare();
            
            am_imu_ag_updata(g_gyrofinal.X, g_gyrofinal.Y, g_gyrofinal.Z, g_accfinal.X, g_accfinal.Y, g_accfinal.Z);
            AM_DBG_INFO("pitch=%4.2f roll=%4.2f yaw=%4.2f\r\n\r\n",g_angle.Pitch, g_angle.Roll, g_angle.Yaw);
             
            am_data_send();
            
            g_tpmtimer0_flag = FALSE;
                
        }
    }
}

/** [src_std_mmpu6050] */

/* end of file */  


