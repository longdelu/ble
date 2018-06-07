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
 * \brief 矩阵按键测试用例，通过标准层实现
 *
 * - 操作步骤：
 *   1. PTC0 ~ PTC3接矩阵键盘的列
 *   2. PTC4 ~ PTC7接矩阵键盘的行
 *   3. PTA1接PC串口的TXD,PTA2接PC串口的RXD
 *
 * - 实验现象：
 *   1. 返回按键值及按键该状态代表的键值
 *
 * \par 源代码
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

/** \brief 矩阵键盘设备信息结构体 */
const am_matrix_key_device_t g_matrix_key_devinfo = {
    INUM_PORTC_PORTD,
    am_int_wake_up_cal,
    PIOC_0,
    PIOC_4,
};

/* 转换成十六进制来显示 */
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
 * \brief 主函数入口
 */
int main (void)
{  
    /* 定义串口（中断模式）设备结构体，用于分配相关空间 */    
    am_uart_handle_t     uart_handle     = NULL;
    am_uart_int_handle_t uart_int_handle = NULL;
    am_uart_int_dev_t    g_uart1_int_dev;    
    
    /** \brief 定义矩阵键盘的操作句柄 */
    am_matrix_key_handle_t  matrix_key_handle = NULL; 

    /** \brief 定义矩阵键盘的设备结构体  */
    am_matrix_key_dev_t matrix_key_dev;   
  
    uint16_t key_stat     = 0;  
    
    uint8_t key_array[16] = {0, 0};
    
    uint8_t i = 0;
    
    /* 板级初始化  */
    am_board_init();
    
    /* UART1平台初始化    */
    amhw_plfm_uart1_init();       
    
    /* UART1初始化 */
    uart_handle     = amdr_uart_init(&g_uart1_dev, &g_uart1_devinfo);

    /* UART1初始化为中断模式 */
    uart_int_handle = am_uart_int_init(&g_uart1_int_dev, uart_handle);     
    
    
    /* 按键初始化         */
    matrix_key_handle = am_matrix_key_init(&matrix_key_dev, &g_matrix_key_devinfo);
    
    /* 中断唤醒引脚初始化 */
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
