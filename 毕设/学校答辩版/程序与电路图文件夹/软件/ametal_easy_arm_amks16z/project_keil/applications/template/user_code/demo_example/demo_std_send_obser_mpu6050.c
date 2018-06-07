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
 *  3. PTE0���ӵ�PC���ڵ�RXD,PTE1���ӵ�PC���ڵ�TXD�� 
 *
 * - ʵ������:
 *  1. ������̬��Ϣ��������λ��
 *
 *
 * \par Դ����
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

volatile bool_t g_mpu6050_flag  = FALSE;       /**< \brief mpu6050�жϱ�־λ����    */

/** \brief mpu6050�豸��Ϣ�ṹ�� */
const am_mpu6050_device_t g_mpu6050_devinfo = {
    AM_MPU6050_ADDRESS,
    AM_I2C_M_7BIT | AM_I2C_SUBADDR_1BYTE,
    PIOA_5,
};

/** \brief ����I2C�豸�ṹ���������ڷ�����ؿռ�           */  
am_i2c_handle_t      i2c_handle     = NULL;
   
/** \brief ���崮��1����ѯģʽ���豸��������ڷ�����ؿռ�   */    
am_uart_handle_t     uart_handle     = NULL;

/** \brief ���崮��1���ж�ģʽ���豸��������ڷ�����ؿռ�   */  
am_uart_int_handle_t uart_int_handle = NULL;

/** \brief ���崮��1���ж�ģʽ���豸�ṹ�壬���ڷ�����ؿռ� */  
am_uart_int_dev_t    g_uart1_int_dev;

/* ���ݷ��ͱ��� */
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
          
    /* ֡ͷ     */
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0x01;
    data_to_send[_cnt++]=0;
    
    /* MPU6050y��̬����0-90 */
    data_to_send[_cnt++]=(uint8_t)(pst[0]>>8);  
    data_to_send[_cnt++]=(uint8_t)pst[0];      
       
    /* MPU6050X��̬����0-180 */
    data_to_send[_cnt++]=(uint8_t)(pst[1]>>8);
    data_to_send[_cnt++]=(uint8_t)pst[1];

    /* MPU6050z��̬���� */    
    data_to_send[_cnt++]=(uint8_t)(pst[2]>>8);
    data_to_send[_cnt++]=(uint8_t)pst[2];
    
    /* ���и߶� */
    data_to_send[_cnt++]=0;
    data_to_send[_cnt++]=0;
    data_to_send[_cnt++]=0;
    data_to_send[_cnt++]=10;
    
    /* �ɿؽ��� */
    data_to_send[_cnt++]=0xA1;
    
    /* �����û��Զ��巢���˶��ٸ��ֽ� */
    data_to_send[3] = _cnt-4;                     
      
    /* ���У�� */      
    for(i=0; i < _cnt; i++) {
        sum += data_to_send[i];
    }
        
    data_to_send[_cnt++] = sum;
    
    for(i=0;i < _cnt;i++) {

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
    
    /* ��̬���ݻ�����      */
    float  bufferr[3] = {0.0f, 0.0f, 0.0f};    
    

    /* �弶��ʼ��           */
    am_board_init();
    
    /* UART1ƽ̨��ʼ��      */
    amhw_plfm_uart1_init();     
    
    /* I2C0 Ӳ��ƽ̨��ʼ��  */
    amhw_plfm_i2c0_init();         

    /* UART1��ʼ��          */
    uart_handle     = amdr_uart_init(&g_uart1_dev, &g_uart1_devinfo);

    /* UART0��ʼ��Ϊ�ж�ģʽ */
    uart_int_handle = am_uart_int_init(&g_uart1_int_dev, uart_handle);       
    
    /* I2C ��ʼ��           */
    i2c_handle = amdr_i2c_init(&g_i2c0_dev, &g_i2c0_devinfo);
    
    /* I2C �ж�����         */
    am_i2c_connect(i2c_handle);   
    
    /* MPU6050��ʼ��        */
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
