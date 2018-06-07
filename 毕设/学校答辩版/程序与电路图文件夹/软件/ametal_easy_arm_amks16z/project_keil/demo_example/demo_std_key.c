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

/**
 * \brief 主函数入口
 */
int main (void)
{    
    uint8_t i = 0;
    
    uint16_t key_stat   = 0;    
       
    /* 定义矩阵键盘的操作句柄    */
    am_matrix_key_handle_t  matrix_key_handle = NULL; 

    /* 定义矩阵键盘的设备结构体  */
    am_matrix_key_dev_t matrix_key_dev;    
    
     
    /* 板级初始化         */
    am_board_init();
    
    /* 按键初始化         */
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
