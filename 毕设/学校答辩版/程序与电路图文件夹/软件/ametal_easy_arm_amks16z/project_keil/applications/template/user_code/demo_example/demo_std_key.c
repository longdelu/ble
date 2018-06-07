/*******************************************************************************
*                                 AMetal
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
 * \brief ���󰴼�����������ͨ����׼��ʵ��
 *
 * - �������裺
 *   1. PTC0 ~ PTC3�Ӿ�����̵���
 *   2. PTC4 ~ PTC7�Ӿ�����̵���
 *   3. PTA1��PC���ڵ�TXD,PTA2��PC���ڵ�RXD
 *
 * - ʵ������
 *   1. ���ذ���ֵ��������״̬����ļ�ֵ
 *
 * \par Դ����
 * \snippet demo_std_key.c src_std_key
 *
 * \internal
 * \par Modification History
 * - 1.00 16-3-21 lgr, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_std_key
 * \copydoc demo_std_key.c
 */
 
/** [src_std_key] */
#include "ametal.h"
#include "am_board.h"
#include "am_key.h"

/** \brief ��������豸��Ϣ�ṹ�� */
const am_matrix_key_device_t g_matrix_key_devinfo = {
    INUM_PORTC_PORTD,
    am_int_wake_up_cal,
    PIOC_0,
    PIOC_4,
};

/* ת����ʮ����������ʾ */
void am_data_change_to_char (uint8_t *p_txbuf,  uint16_t buffer)
{
    uint8_t i = 0;
    
    for (i = 0; i < 16; i ++) {
        
        if ( buffer & 0x0001) {
            p_txbuf[i] = '1';
        } else {
            p_txbuf[i] = '0';
        }
         buffer >>= 1;
    }        
        
}

/**
 * \brief ���������
 */
int main (void)
{  
    /* ���崮�ڣ��ж�ģʽ���豸�ṹ�壬���ڷ�����ؿռ� */    
    am_uart_handle_t     uart_handle     = NULL;
    am_uart_int_handle_t uart_int_handle = NULL;
    am_uart_int_dev_t    g_uart1_int_dev;    
    
    /** \brief ���������̵Ĳ������ */
    am_matrix_key_handle_t  matrix_key_handle = NULL; 

    /** \brief ���������̵��豸�ṹ��  */
    am_matrix_key_dev_t matrix_key_dev;   
  
    uint16_t key_stat     = 0;  
    
    uint8_t key_array[16] = {0, 0};
    
    uint8_t i = 0;
    
    /* �弶��ʼ��  */
    am_board_init();
    
    /* UART1ƽ̨��ʼ��    */
    amhw_plfm_uart1_init();       
    
    /* UART1��ʼ�� */
    uart_handle     = amdr_uart_init(&g_uart1_dev, &g_uart1_devinfo);

    /* UART1��ʼ��Ϊ�ж�ģʽ */
    uart_int_handle = am_uart_int_init(&g_uart1_int_dev, uart_handle);     
    
    
    /* ������ʼ��         */
    matrix_key_handle = am_matrix_key_init(&matrix_key_dev, &g_matrix_key_devinfo);
    
    /* �жϻ������ų�ʼ�� */
    am_int_wake_up_cfg(am_int_wake_up_cal);

    while (1) {
        
        g_key_stat = am_matrix_key_scan_repeat(matrix_key_handle, 4); ;
        
        am_data_change_to_char(key_array, key_stat);
               
        am_matrix_key_value_def(matrix_key_handle, key_stat);
        
        am_uart_int_send(uart_int_handle, key_array, 16); 
        am_uart_int_send(uart_int_handle, "\r\n", 2); 
        
        
        AM_DBG_INFO("key_stat = 0x%x\r\n", key_stat);
        
        AM_DBG_INFO("g_mouse_or_key_flag = 0x%x\r\n", g_mouse_or_key_flag);
        
        AM_DBG_INFO("g_mouse_button[0] = 0x%x\r\n", g_mouse_button[0]);
        AM_DBG_INFO("g_mouse_button[1] = 0x%x\r\n", g_mouse_button[1]);
        
        for (i = 0; i < 8; i++) {
            
            AM_DBG_INFO("g_key_board[%d] = 0x%x\r\n", i, g_key_board[i]);
                         
        }
              
    }
}

/** [src_std_key] */
 
/* end of file */
