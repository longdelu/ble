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
 * \brief 飞鼠演示例程
 *
 * - 操作步骤：
 *  1. 给PCB板上电；
 *  2. 在PCB板上用跳帽把主控与蓝牙芯片的串口接起来 
 *  3. 防置桌面等待约5秒，传感器校准完毕
 *
 * - 实验现象:
 *  1. 10S后无按键按下，传感器数据变化不大，则进入休眠状态，否则系统处于运行状态
 *
 * \par 源代码
 * \snippet demo_std_low_fly_mouse.c src_std_low_fly_mouse 
 * 
 * \internal
 * \par Modification history
 * - 1.00 16-04-10  lgr, first implementation.
 * \endinternal
 */ 
 
/**
 * \addtogroup demo_if_std_low_fly_mouse
 * \copydoc demo_std_low_fly_mouse.c
 */

/** [src_std_low_fly_mouse] */

#include "ametal.h"
#include "am_board.h"
#include "am_mpu6050.h"
#include "am_key.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

/** \brief mpu6050设备信息结构体 */
const am_mpu6050_device_t g_mpu6050_devinfo = {
    AM_MPU6050_ADDRESS,
    AM_I2C_M_7BIT | AM_I2C_SUBADDR_1BYTE,
    PIOD_6,
};

/** \brief 矩阵键盘设备信息结构体 */
const am_matrix_key_device_t g_matrix_key_devinfo = {
    INUM_PORTC_PORTD,
    am_int_wake_up_cal,
    PIOC_0,
    PIOC_4,
};

/** \brief 定义I2C设备结构体句柄，用于分配相关空间           */  
am_i2c_handle_t      i2c_handle     = NULL;
   
/** \brief 定义串口1（查询模式）设备句柄，用于分配相关空间   */    
am_uart_handle_t     uart1_handle     = NULL;

/** \brief 定义串口1（中断模式）设备句柄，用于分配相关空间   */  
am_uart_int_handle_t uart1_int_handle = NULL;

/** \brief 定义串口1（中断模式）设备结构体，用于分配相关空间 */  
am_uart_int_dev_t    g_uart1_int_dev;

float  g_bufferr[3] = {0.0f, 0.0f, 0.0f};          /**< \brief 上一帧的姿态数据     */
float  g_last_bufferr[3] = {0.0f, 0.0f, 0.0f};     /**< \brief 当前帧的数据         */

int8_t g_pst[3] = {0, 0, 0};                       /**< \brief 姿态数据临时变量     */

uint16_t g_key_stat       = 0;                     /**< \brief 按键状态信息         */ 
uint16_t g_last_key_stat  = 0;                     /**< \brief 上一次按键状态信息    */ 

/**
 * \brief 数据编写协议
 * 
 * \param[in] p_pst： 光标位移缓冲区指区，指向 g_pst
 */
void am_data_send (int8_t *p_pst)
{   
    uint8_t i = 0; 
    
    uint8_t _cnt=0;    
    
    uint8_t sum = 0;
       
    uint8_t data_to_send[14];   
                
    /* 帧头          */
    data_to_send[_cnt++] = data_to_send[7]  = 0xA5;
    data_to_send[_cnt++] = data_to_send[8]  = 0xA5;

    /* y轴坐标姿态数据 */
    data_to_send[_cnt++] = data_to_send[9]  = (uint8_t)(-p_pst[1]);    
       
    /* Z轴坐标姿态数据 */   
    data_to_send[_cnt++] = data_to_send[10] = (uint8_t)(-p_pst[2]); 
      
    /* 按键状态信息   */      
    data_to_send[_cnt++] = data_to_send[11] = (uint8_t)(g_key_stat >> 8);
    data_to_send[_cnt++] = data_to_send[12] = (uint8_t)g_key_stat ;                        
      
    /* 求和校验       */      
    for(i=0; i < _cnt; i++) {
        sum += data_to_send[i];
    }
        
    data_to_send[_cnt++] = data_to_send[13] = sum;
   
    /* 串口发送三次数据，以免造成丢字节 */     
    am_uart_int_send(uart1_int_handle, data_to_send, 14); 
            
}

/**
 * \brief 主函数
 */
int main(void)
{        
    uint16_t i = 0;
    
    /* 低功耗状态进入阀值时间计数  */
    volatile uint16_t stat = 0; 
    
    /* 按键按下时坐标保持为0的时间 */
    uint16_t time_cnt = 40;
    
    /* 按键按下的状态标志  */
    uint8_t key_flag = 0;   

    /* DMP库缓冲区数据    */
    int16_t acc[3]  = {0, 0, 0}; 
    int16_t gro[3]  = {0, 0, 0};       
       
    /* 定义矩阵键盘的操作句柄    */
    am_matrix_key_handle_t  matrix_key_handle = NULL; 

    /* 定义矩阵键盘的设备结构体  */
    am_matrix_key_dev_t matrix_key_dev;    
    
     
    /* 板级初始化         */
    am_board_init();
    
    /* 按键初始化         */
    matrix_key_handle = am_matrix_key_init(&matrix_key_dev, &g_matrix_key_devinfo);
    
    /* 中断唤醒引脚初始化 */
    am_int_wake_up_cfg(matrix_key_handle, am_int_wake_up_cal);    
         
    /* UART1平台初始化    */
    amhw_plfm_uart1_init();        
    
    /* I2C0 硬件平台初始化 */
    amhw_plfm_i2c0_init();         

    /* UART1初始化        */
    uart1_handle     = amdr_uart_init(&g_uart1_dev, &g_uart1_devinfo);

    /* UART1中断模式初始化 */
    uart1_int_handle = am_uart_int_init(&g_uart1_int_dev, uart1_handle);       
    
    /* I2C 初始化         */
    i2c_handle = amdr_i2c_init(&g_i2c0_dev, &g_i2c0_devinfo);
    
    /* I2C 中断连接       */
    am_i2c_connect(i2c_handle);   
        
    /* MPU6050初始化     */
    mpu6050_handle = am_mpu6050_init(&g_mpu6050_dev, &g_mpu6050_devinfo, i2c_handle);
       
     /* MPU6050DMP初始化 */          
    while(am_mpu_dmp_init(mpu6050_handle)) {
       ;
    }          
              
    /* 得到当前的姿态数据 */     
    while (am_mpu_dmp_get_data(mpu6050_handle, &acc[0], &gro[0], &g_last_bufferr[0])) {
        ; 
    }
           
    while (1) {              
        /* 扫描得到按键状态      */
        g_key_stat = am_matrix_key_scan_repeat(matrix_key_handle, 5); 
               
        /* 支持重新校准传感器零点 */
        if (g_key_stat & 0x2000) {           
            while(am_mpu_dmp_init(mpu6050_handle)) {
               ;   
            } 
        }
        
        /* 刷新数据           */     
        while(am_mpu_dmp_get_data(mpu6050_handle, &acc[0], &gro[0], &g_bufferr[0]))
                          ;    
        for (i = 0; i < 3; i++) {               
             g_pst[i] = (g_bufferr[i] * 100 - g_last_bufferr[i] * 100) / 4 ;
             g_last_bufferr[i] = g_bufferr[i];                       
        }       

        /* 当其在一定的阀值内没有变化时与按键没有按下时 */        
        if (abs(g_pst[0]) > 3 || 
            abs(g_pst[1]) > 3 || 
            abs(g_pst[2]) > 3 ||
            g_key_stat != 0   || 
            g_last_key_stat   != 0 ) {
                                       
            /* 是左键按下的话让它保持约100ms坐标不变化 */                
            if (g_key_stat != 0) {               
                 key_flag = 1;                 
            } else {             
                if (key_flag == 0) {
                    time_cnt = 40;
                }
            }
            g_last_key_stat  =  g_key_stat;              
            
            stat = 0;              

        } else {
                  
            stat++;
        }
        
        /* 有按键的时候的延时一段时间清除所有的鼠标位移 */ 
        if (key_flag != 0) {            
            if (time_cnt > 0 ) {                
                time_cnt--;
                
                for (i = 0; i < 3; i++) {                             
                    g_pst[i] = 0;            
                } 
            } else {                
                key_flag = 0;          
            }                            
        }
                  
        am_data_send(g_pst);                
                    
        am_mdelay(2);            
             
        /* 低功耗设置                              */        
        if (stat >= 3000) {           
            /* 让蓝牙进入低功耗                    */            
            am_gpio_set(PIOD_4, 0);            
            
            /* 使能中断唤醒引脚                    */ 
            am_int_wake_up_enable(matrix_key_handle);         
           
            /* 关闭DMP                             */
            while(mpu_set_dmp_state(0));
            
            /* MPU6050进入低功耗，三个加速度方向轴可以唤醒MPU65050 */
            while(mpu_lp_motion_interrupt(40, 2, 5));
            
            am_udelay(300);            

            AM_DBG_INFO("sleep...\r\n");
            
            /* 进入睡眠模式，等待中断唤醒            */
            if(amdr_pmu_mode_into(AMDR_PMU_MODE_VLPS) != AM_OK) {
               AM_DBG_INFO("Enter VLPS_MODE Failed !\r\n");
            };  

            am_udelay(1000); 

            AM_DBG_INFO("wake up...\r\n");   

            am_int_wake_up_disable(matrix_key_handle);           

            /* 唤醒蓝牙                               */            
            am_gpio_set(PIOD_4, 1);                                     
                         
            /* 退出低功耗，MPU6050正常工作            */
            while(mpu_lp_motion_interrupt(50, 1, 0));
            
            /* 打开DMP                                */
            while( mpu_set_dmp_state(1));  
                                                                               
            stat = 0;
            
            am_mdelay(400); 
        }                                           
    }  
}

/** [src_std_low_fly_mouse] */

/* end of file */  
