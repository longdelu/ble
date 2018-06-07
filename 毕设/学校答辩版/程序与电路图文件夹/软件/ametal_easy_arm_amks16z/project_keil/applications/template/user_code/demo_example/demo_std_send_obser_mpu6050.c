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
 *  3. PTE0连接到PC串口的RXD,PTE1连接到PC串口的TXD。 
 *
 * - 实验现象:
 *  1. 发送姿态信息到匿名上位机
 *
 *
 * \par 源代码
 * \snippet demo_std_mpu6050_obser.c src_std_mpu6050_obser 
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-11-20  lgr, first implementation.
 * \endinternal
 */ 
 
/**
 * \addtogroup demo_if_std_mpu6050_obser
 * \copydoc demo_std_mpu6050_obser.c
 */

/** [src_std_mpu6050_obser] */

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

/** \brief 定义I2C设备结构体句柄，用于分配相关空间           */  
am_i2c_handle_t      i2c_handle     = NULL;
   
/** \brief 定义串口1（查询模式）设备句柄，用于分配相关空间   */    
am_uart_handle_t     uart_handle     = NULL;

/** \brief 定义串口1（中断模式）设备句柄，用于分配相关空间   */  
am_uart_int_handle_t uart_int_handle = NULL;

/** \brief 定义串口1（中断模式）设备结构体，用于分配相关空间 */  
am_uart_int_dev_t    g_uart1_int_dev;

/* 数据发送编码 */
void am_data_send(float *p_fpst)
{
    uint8_t _cnt=0;    
    
    uint8_t sum = 0;
    
    uint8_t i = 0,j = 0;
    
    int16_t pst[3] = {0};
    
    uint8_t data_to_send[23];   
    
    
    for (j = 0; j < 3; j++) {
        pst[j] = p_fpst[j] * 100;
    }
          
    /* 帧头     */
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0x01;
    data_to_send[_cnt++]=0;
    
    /* MPU6050y姿态数据0-90 */
    data_to_send[_cnt++]=(uint8_t)(pst[0]>>8);  
    data_to_send[_cnt++]=(uint8_t)pst[0];      
       
    /* MPU6050X姿态数据0-180 */
    data_to_send[_cnt++]=(uint8_t)(pst[1]>>8);
    data_to_send[_cnt++]=(uint8_t)pst[1];

    /* MPU6050z姿态数据 */    
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
    
    for(i=0;i < _cnt;i++) {

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
    
    /* 姿态数据缓冲区      */
    float  bufferr[3] = {0.0f, 0.0f, 0.0f};    
    

    /* 板级初始化           */
    am_board_init();
    
    /* UART1平台初始化      */
    amhw_plfm_uart1_init();     
    
    /* I2C0 硬件平台初始化  */
    amhw_plfm_i2c0_init();         

    /* UART1初始化          */
    uart_handle     = amdr_uart_init(&g_uart1_dev, &g_uart1_devinfo);

    /* UART0初始化为中断模式 */
    uart_int_handle = am_uart_int_init(&g_uart1_int_dev, uart_handle);       
    
    /* I2C 初始化           */
    i2c_handle = amdr_i2c_init(&g_i2c0_dev, &g_i2c0_devinfo);
    
    /* I2C 中断连接         */
    am_i2c_connect(i2c_handle);   
    
    /* MPU6050初始化        */
    mpu6050_handle = am_mpu6050_init(&g_mpu6050_dev, &g_mpu6050_devinfo, i2c_handle);
        
    while(am_mpu_dmp_init(mpu6050_handle)) {
       ;
    } 
      
    while (1) {         
        if (am_mpu_dmp_get_data(mpu6050_handle, &acc[0], &gro[0], &bufferr[0])==0) {                        
            am_data_send(bufferr);
        }               
    }
}

/** [src_std_mpu6050_obser] */

/* end of file */  
