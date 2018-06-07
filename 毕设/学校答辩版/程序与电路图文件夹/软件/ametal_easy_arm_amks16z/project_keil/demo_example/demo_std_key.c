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

/**
 * \brief ���������
 */
int main (void)
{    
    uint8_t i = 0;
    
    uint16_t key_stat   = 0;    
       
    /* ���������̵Ĳ������    */
    am_matrix_key_handle_t  matrix_key_handle = NULL; 

    /* ���������̵��豸�ṹ��  */
    am_matrix_key_dev_t matrix_key_dev;    
    
     
    /* �弶��ʼ��         */
    am_board_init();
    
    /* ������ʼ��         */
    matrix_key_handle = am_matrix_key_init(&matrix_key_dev, &g_matrix_key_devinfo);

    while (1) {       
        key_stat = am_matrix_key_scan_repeat(matrix_key_handle, 4);
        
        am_matrix_key_value_def(matrix_key_handle, key_stat);
        
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
