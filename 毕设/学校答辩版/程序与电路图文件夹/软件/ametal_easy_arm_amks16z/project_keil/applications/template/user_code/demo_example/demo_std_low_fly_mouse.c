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
 * \brief ������ʾ����
 *
 * - �������裺
 *  1. ��PCB���ϵ磻
 *  2. ��PCB��������ñ������������оƬ�Ĵ��ڽ����� 
 *  3. ��������ȴ�Լ5�룬������У׼���
 *
 * - ʵ������:
 *  1. 10S���ް������£����������ݱ仯�������������״̬������ϵͳ��������״̬
 *
 * \par Դ����
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

/** \brief mpu6050�豸��Ϣ�ṹ�� */
const am_mpu6050_device_t g_mpu6050_devinfo = {
    AM_MPU6050_ADDRESS,
    AM_I2C_M_7BIT | AM_I2C_SUBADDR_1BYTE,
    PIOD_6,
};

/** \brief ��������豸��Ϣ�ṹ�� */
const am_matrix_key_device_t g_matrix_key_devinfo = {
    INUM_PORTC_PORTD,
    am_int_wake_up_cal,
    PIOC_0,
    PIOC_4,
};

/** \brief ����I2C�豸�ṹ���������ڷ�����ؿռ�           */  
am_i2c_handle_t      i2c_handle     = NULL;
   
/** \brief ���崮��1����ѯģʽ���豸��������ڷ�����ؿռ�   */    
am_uart_handle_t     uart1_handle     = NULL;

/** \brief ���崮��1���ж�ģʽ���豸��������ڷ�����ؿռ�   */  
am_uart_int_handle_t uart1_int_handle = NULL;

/** \brief ���崮��1���ж�ģʽ���豸�ṹ�壬���ڷ�����ؿռ� */  
am_uart_int_dev_t    g_uart1_int_dev;

float  g_bufferr[3] = {0.0f, 0.0f, 0.0f};          /**< \brief ��һ֡����̬����     */
float  g_last_bufferr[3] = {0.0f, 0.0f, 0.0f};     /**< \brief ��ǰ֡������         */

int8_t g_pst[3] = {0, 0, 0};                       /**< \brief ��̬������ʱ����     */

uint16_t g_key_stat       = 0;                     /**< \brief ����״̬��Ϣ         */ 
uint16_t g_last_key_stat  = 0;                     /**< \brief ��һ�ΰ���״̬��Ϣ    */ 

/**
 * \brief ���ݱ�дЭ��
 * 
 * \param[in] p_pst�� ���λ�ƻ�����ָ����ָ�� g_pst
 */
void am_data_send (int8_t *p_pst)
{   
    uint8_t i = 0; 
    
    uint8_t _cnt=0;    
    
    uint8_t sum = 0;
       
    uint8_t data_to_send[14];   
                
    /* ֡ͷ          */
    data_to_send[_cnt++] = data_to_send[7]  = 0xA5;
    data_to_send[_cnt++] = data_to_send[8]  = 0xA5;

    /* y��������̬���� */
    data_to_send[_cnt++] = data_to_send[9]  = (uint8_t)(-p_pst[1]);    
       
    /* Z��������̬���� */   
    data_to_send[_cnt++] = data_to_send[10] = (uint8_t)(-p_pst[2]); 
      
    /* ����״̬��Ϣ   */      
    data_to_send[_cnt++] = data_to_send[11] = (uint8_t)(g_key_stat >> 8);
    data_to_send[_cnt++] = data_to_send[12] = (uint8_t)g_key_stat ;                        
      
    /* ���У��       */      
    for(i=0; i < _cnt; i++) {
        sum += data_to_send[i];
    }
        
    data_to_send[_cnt++] = data_to_send[13] = sum;
   
    /* ���ڷ����������ݣ�������ɶ��ֽ� */     
    am_uart_int_send(uart1_int_handle, data_to_send, 14); 
            
}

/**
 * \brief ������
 */
int main(void)
{        
    uint16_t i = 0;
    
    /* �͹���״̬���뷧ֵʱ�����  */
    volatile uint16_t stat = 0; 
    
    /* ��������ʱ���걣��Ϊ0��ʱ�� */
    uint16_t time_cnt = 40;
    
    /* �������µ�״̬��־  */
    uint8_t key_flag = 0;   

    /* DMP�⻺��������    */
    int16_t acc[3]  = {0, 0, 0}; 
    int16_t gro[3]  = {0, 0, 0};       
       
    /* ���������̵Ĳ������    */
    am_matrix_key_handle_t  matrix_key_handle = NULL; 

    /* ���������̵��豸�ṹ��  */
    am_matrix_key_dev_t matrix_key_dev;    
    
     
    /* �弶��ʼ��         */
    am_board_init();
    
    /* ������ʼ��         */
    matrix_key_handle = am_matrix_key_init(&matrix_key_dev, &g_matrix_key_devinfo);
    
    /* �жϻ������ų�ʼ�� */
    am_int_wake_up_cfg(matrix_key_handle, am_int_wake_up_cal);    
         
    /* UART1ƽ̨��ʼ��    */
    amhw_plfm_uart1_init();        
    
    /* I2C0 Ӳ��ƽ̨��ʼ�� */
    amhw_plfm_i2c0_init();         

    /* UART1��ʼ��        */
    uart1_handle     = amdr_uart_init(&g_uart1_dev, &g_uart1_devinfo);

    /* UART1�ж�ģʽ��ʼ�� */
    uart1_int_handle = am_uart_int_init(&g_uart1_int_dev, uart1_handle);       
    
    /* I2C ��ʼ��         */
    i2c_handle = amdr_i2c_init(&g_i2c0_dev, &g_i2c0_devinfo);
    
    /* I2C �ж�����       */
    am_i2c_connect(i2c_handle);   
        
    /* MPU6050��ʼ��     */
    mpu6050_handle = am_mpu6050_init(&g_mpu6050_dev, &g_mpu6050_devinfo, i2c_handle);
       
     /* MPU6050DMP��ʼ�� */          
    while(am_mpu_dmp_init(mpu6050_handle)) {
       ;
    }          
              
    /* �õ���ǰ����̬���� */     
    while (am_mpu_dmp_get_data(mpu6050_handle, &acc[0], &gro[0], &g_last_bufferr[0])) {
        ; 
    }
           
    while (1) {              
        /* ɨ��õ�����״̬      */
        g_key_stat = am_matrix_key_scan_repeat(matrix_key_handle, 5); 
               
        /* ֧������У׼��������� */
        if (g_key_stat & 0x2000) {           
            while(am_mpu_dmp_init(mpu6050_handle)) {
               ;   
            } 
        }
        
        /* ˢ������           */     
        while(am_mpu_dmp_get_data(mpu6050_handle, &acc[0], &gro[0], &g_bufferr[0]))
                          ;    
        for (i = 0; i < 3; i++) {               
             g_pst[i] = (g_bufferr[i] * 100 - g_last_bufferr[i] * 100) / 4 ;
             g_last_bufferr[i] = g_bufferr[i];                       
        }       

        /* ������һ���ķ�ֵ��û�б仯ʱ�밴��û�а���ʱ */        
        if (abs(g_pst[0]) > 3 || 
            abs(g_pst[1]) > 3 || 
            abs(g_pst[2]) > 3 ||
            g_key_stat != 0   || 
            g_last_key_stat   != 0 ) {
                                       
            /* ��������µĻ���������Լ100ms���겻�仯 */                
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
        
        /* �а�����ʱ�����ʱһ��ʱ��������е����λ�� */ 
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
             
        /* �͹�������                              */        
        if (stat >= 3000) {           
            /* ����������͹���                    */            
            am_gpio_set(PIOD_4, 0);            
            
            /* ʹ���жϻ�������                    */ 
            am_int_wake_up_enable(matrix_key_handle);         
           
            /* �ر�DMP                             */
            while(mpu_set_dmp_state(0));
            
            /* MPU6050����͹��ģ��������ٶȷ�������Ի���MPU65050 */
            while(mpu_lp_motion_interrupt(40, 2, 5));
            
            am_udelay(300);            

            AM_DBG_INFO("sleep...\r\n");
            
            /* ����˯��ģʽ���ȴ��жϻ���            */
            if(amdr_pmu_mode_into(AMDR_PMU_MODE_VLPS) != AM_OK) {
               AM_DBG_INFO("Enter VLPS_MODE Failed !\r\n");
            };  

            am_udelay(1000); 

            AM_DBG_INFO("wake up...\r\n");   

            am_int_wake_up_disable(matrix_key_handle);           

            /* ��������                               */            
            am_gpio_set(PIOD_4, 1);                                     
                         
            /* �˳��͹��ģ�MPU6050��������            */
            while(mpu_lp_motion_interrupt(50, 1, 0));
            
            /* ��DMP                                */
            while( mpu_set_dmp_state(1));  
                                                                               
            stat = 0;
            
            am_mdelay(400); 
        }                                           
    }  
}

/** [src_std_low_fly_mouse] */

/* end of file */  
