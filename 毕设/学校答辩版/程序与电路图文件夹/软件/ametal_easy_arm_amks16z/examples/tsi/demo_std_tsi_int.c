/*******************************************************************************
*                                 AMetal
*                       ---------------------------
*                       innovating embedded systems
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/
/**
 * \file
 * \brief   TSI触摸中断检测，驱动层实现
 *
 *  实验现象：
 *  1.实验配置PIOA_1引脚为TSI通道2功能，当手不接触PIOA_1引脚时不会触发中断
 *  2.当触摸PIOA_1引脚后，电容检测大于阀值上限1值时，进入中断，LED1状态翻转
 *
 *  注意：
 *  当TSI设置为软件触发模式时，需要执行启动软件扫描函数才能执行一次扫描，所以
 *  在使用时，当配置为软件触发模式时，需要一直或者定时执行启动扫描函数。
 *
 * \par 源代码
 * \snippet demo_std_tsi_int.c src_std_tsi_int
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-26  wangxiaojian, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_std_tsi_int
 * \copydoc demo_std_tsi_int.c
 */
 
/** [src_std_tsi_int] */
#include "ametal.h"
#include "am_board.h"

void tsi_isr(void * p_arg)
{
    /* TSI中断禁能 */
    amdr_tsi_int_disable();

    /* 清超阀值标志 */
    amhw_tsi_outrgf_clr(AMHW_TSI);

    am_led_toggle(LED1);
    am_mdelay(100);

    /* TSI中断使能 */
    amdr_tsi_int_enable();
}

/**
 * \brief 主函数入口
 */
int main (void)
{  
    /* 板级初始化  */
    am_board_init();
    
    /* tsi平台初始化 */
    amhw_plfm_tsi_init();

    /* TSI设备初始化 */
    am_tsi_init(&g_tsi_dev,&g_tsi_devinfo);

    /* TSI功能配置 */
    amdr_tsi_fun_cfg(AMHW_TSI_CHAN2,
                     AMHW_TSI_CAP_SENSING,
                     AMHW_TSI_TRIG_SOFTWARE);

     /* TSI中断触发模式为超阀值触发模式 */
    amdr_tsi_trigger_cfg(AMHW_TSI_OUT_RANGE);

    /* 连接TSI中断服务函数 */
    amdr_tsi_trigger_connect(&tsi_isr);

    /* 设置TSI灵敏度 */
    amdr_tsi_sens_set(500);

    /* TSI中断使能 */
    amdr_tsi_int_enable();

    while (1) {

        /* 启动软件触发扫描 */
        amhw_tsi_soft_tirg_start(AMHW_TSI);

    }
}

/** [src_std_tsi_int] */
 
/* end of file */

