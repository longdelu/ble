/*******************************************************************************
*                                 Apollo
*                       ---------------------------
*                       innovating embedded systems
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
 * \brief MPU6050演示例程，通过AMetal的I2C标准接口实现
 *
 * - 操作步骤：
 *  1. 给mpu6050上电；
 *  2. 把控制器芯片I2C外设引脚PTC8 SCL,PTC9 SDA接到MPU6050的I2C接口相对应的引脚上;
 *  3. PTA2连接到PC串口的RXD,PTA1连接到PC串口的TXD 
 *
 * - 实验现象:
 *  1. 利用匿名上位机观察姿态信息
 *
 *
 * \par 源代码
 * \snippet demo_std_mpu6050.c src_std_mpu6050 
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-11-20  lgr, first implementation.
 * \endinternal
 */ 
 
/**
 * \addtogroup demo_if_std_mpu6050
 * \copydoc demo_std_mpu6050.c
 */

/** [src_std_mpu6050] */

#include "ametal.h"
#include "am_board.h"
#include "am_mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

volatile bool_t g_mpu6050_flag  = FALSE;       /**< \brief mpu6050中断标志位定义    */

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

/* 数据发送编码 */
void am_data_send(int16_t *p_pst)
{
    uint8_t _cnt=0;    
    
    uint8_t sum = 0;
    
    uint8_t i = 0;
        
    uint8_t data_to_send[23];                       
    
    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0xAA;
    data_to_send[_cnt++] = 0x02;
    data_to_send[_cnt++] = 0;
    data_to_send[_cnt++] = (uint8_t)(p_pst[0]>>8);  /* 高8位 */
    data_to_send[_cnt++] = (uint8_t)p_pst[0];       /* 低8位 */
    data_to_send[_cnt++] = (uint8_t)(p_pst[1]>>8);
    data_to_send[_cnt++] = (uint8_t)p_pst[1];
    data_to_send[_cnt++] = (uint8_t)(p_pst[2]>>8);
    data_to_send[_cnt++] = (uint8_t)p_pst[2];
    
    data_to_send[3] = _cnt-4;
    
    sum = 0;
        
    for(i = 0; i < _cnt; i++)
        sum += data_to_send[i];
        
    data_to_send[_cnt++] = sum;
    
    for (i = 0;i < _cnt; i++) {

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
    int16_t acc[3]  = {0, 0, 0}; 
    int16_t gro[3]  = {0, 0, 0}; 
    
    float  buf[7]= {0.0f, 0.0f, 0.0f};    
    float  bufferr[3] = {0.0f, 0.0f, 0.0f};    
     
    int16_t buffer[7] = {0, 0};

    
    /* 板级初始化   */
    am_board_init();
    
    /* 平台初始化   */
    amhw_plfm_uart0_init();

    /* UART0初始化 */
    uart_handle     = amdr_uart_init(&g_uart0_dev, &g_uart0_devinfo);

    /* UART0初始化为中断模式 */
    uart_int_handle = am_uart_int_init(&g_uart0_int_dev, uart_handle);        
    
    /* I2C0 硬件平台初始化 */
    amhw_plfm_i2c0_init();                          
    
    /* I2C 初始化         */
    i2c_handle = amdr_i2c_init(&g_i2c0_dev, &g_i2c0_devinfo);
    
    /* I2C 中断连接       */
    am_i2c_connect(i2c_handle);   
    
    /* MPU6050初始化      */
    mpu6050_handle = am_mpu6050_init(&g_mpu6050_dev, &g_mpu6050_devinfo, i2c_handle);
     
    while(am_mpu_dmp_init(mpu6050_handle)) {
       ;
    } 
       
    while (1) {    
        
        am_mpu6050_xyz_data_read(mpu6050_handle, buffer, AM_MPU6050_DATA_ALL_SELECT);  
        am_mpu6050_data_handle(mpu6050_handle, buf, buffer);  
        
        AM_DBG_INFO("normal read and write mpu6050\r\n");
        AM_DBG_INFO("X_ACC=%4.2f  Y_ACC=%4.2f  Z_ACC=%4.2f\r\n", buf[0], buf[1], buf[2]); 
        AM_DBG_INFO("X_GRO=%4.2f  Y_GRO=%4.2f  Z_GRO=%4.2f\r\n\r\n", buf[4], buf[4], buf[6]);        
        
        if (am_mpu_dmp_get_data(mpu6050_handle, &acc[0], &gro[0], &bufferr[0])==0) {                    
            AM_DBG_INFO("DMP read and write mpu6050\r\n");
            
            AM_DBG_INFO("X_ACC=%d Y_ACC=%d Z_ACC=%d\r\n", acc[0], acc[1], acc[2]); 
               
            AM_DBG_INFO("X_GRO=%d Y_GRO=%d Z_GRO=%d\r\n", gro[0], gro[1], gro[2]);

            AM_DBG_INFO("pitch=%4.2f roll=%4.2f yaw=%4.2f\r\n\r\n", bufferr[0], bufferr[1], bufferr[2]);                     
        }        
                    
        am_led_toggle(LED0) ;/* LED0反转 */
        am_mdelay(500);
    }
}

/** [src_std_mmpu6050] */

/* end of file */  

