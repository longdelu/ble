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
 * \brief MPU6050��ʾ���̣�ͨ��AMetal��I2C��׼�ӿ�ʵ��
 *
 * - �������裺
 *  1. ��mpu6050�ϵ磻
 *  2. �ѿ�����оƬI2C��������PTC8 SCL,PTC9 SDA�ӵ�MPU6050��I2C�ӿ����Ӧ��������;
 *  3. PTA2���ӵ�PC���ڵ�RXD,PTA1���ӵ�PC���ڵ�TXD 
 *
 * - ʵ������:
 *  1. ����������λ���۲���̬��Ϣ
 *
 *
 * \par Դ����
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

volatile bool_t g_mpu6050_flag  = FALSE;       /**< \brief mpu6050�жϱ�־λ����    */

/** \brief mpu6050�豸��Ϣ�ṹ�� */
const am_mpu6050_device_t g_mpu6050_devinfo = {
    AM_MPU6050_ADDRESS,
    AM_I2C_M_7BIT | AM_I2C_SUBADDR_1BYTE,
    PIOA_5,
};

/* ����I2C�豸�ṹ�壬���ڷ�����ؿռ� */  
am_i2c_handle_t      i2c_handle     = NULL;
   

/* ���崮�ڣ��ж�ģʽ���豸�ṹ�壬���ڷ�����ؿռ� */    
am_uart_handle_t     uart_handle     = NULL;
am_uart_int_handle_t uart_int_handle = NULL;
am_uart_int_dev_t    g_uart0_int_dev;

/* ���ݷ��ͱ��� */
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
    data_to_send[_cnt++] = (uint8_t)(p_pst[0]>>8);  /* ��8λ */
    data_to_send[_cnt++] = (uint8_t)p_pst[0];       /* ��8λ */
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

        /* ���ڷ���һ���ֽں��� */
        am_uart_int_send(uart_int_handle, &data_to_send[i], 1);   
    }
              
}

/**
 * \brief MPU6050�����жϷ�����
 */
void mpu6050_gpio_isr (void *p_arg)
{
    /* ��־�жϱ�־ */
    g_mpu6050_flag = TRUE;
    
    am_led_toggle(LED0);
}

/**
 * \brief ������
 */
int main(void)
{
    int16_t acc[3]  = {0, 0, 0}; 
    int16_t gro[3]  = {0, 0, 0}; 
    
    float  buf[7]= {0.0f, 0.0f, 0.0f};    
    float  bufferr[3] = {0.0f, 0.0f, 0.0f};    
     
    int16_t buffer[7] = {0, 0};

    
    /* �弶��ʼ��   */
    am_board_init();
    
    /* ƽ̨��ʼ��   */
    amhw_plfm_uart0_init();

    /* UART0��ʼ�� */
    uart_handle     = amdr_uart_init(&g_uart0_dev, &g_uart0_devinfo);

    /* UART0��ʼ��Ϊ�ж�ģʽ */
    uart_int_handle = am_uart_int_init(&g_uart0_int_dev, uart_handle);        
    
    /* I2C0 Ӳ��ƽ̨��ʼ�� */
    amhw_plfm_i2c0_init();                          
    
    /* I2C ��ʼ��         */
    i2c_handle = amdr_i2c_init(&g_i2c0_dev, &g_i2c0_devinfo);
    
    /* I2C �ж�����       */
    am_i2c_connect(i2c_handle);   
    
    /* MPU6050��ʼ��      */
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
                    
        am_led_toggle(LED0) ;/* LED0��ת */
        am_mdelay(500);
    }
}

/** [src_std_mmpu6050] */

/* end of file */  

