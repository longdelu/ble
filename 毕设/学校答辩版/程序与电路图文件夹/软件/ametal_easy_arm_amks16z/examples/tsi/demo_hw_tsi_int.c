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
 * \brief   TSI触摸中断检测，硬件层实现
 *
 *  实验现象：
 *  1.当手不接触PIOA_1引脚时，输出检测到的基本值
 *  2.当触摸PIOA_1引脚后，电容检测大于阀值上限1000时，进入中断，LED1状态翻转
 *
 * \par 源代码
 * \snippet demo_hw_tsi_int.c src_hw_tsi_int
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-26  xjc, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_hw_tsi_int
 * \copydoc demo_hw_tsi_int.c
 */
 
/** [src_hw_tsi_int] */
#include "ametal.h"
#include "am_board.h"
    
void tsi_isr(void * p_arg)
{
    /* 清超阀值标志 */
    amhw_tsi_outrgf_clr(AMHW_TSI);
       
    /* 清扫描结束标志 */
    amhw_tsi_eosf_clr(AMHW_TSI);
    am_led_toggle(LED1);
}

/**
 * \brief 主函数入口
 */
int main (void)
{  
    uint16_t value = 0;
    
    /* 板级初始化  */
    am_board_init();
    
    /* 使能tsi相关外设时钟 */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_TSI);

    /* 配置PIOA_1引脚为TSI0_CH2 */
    am_gpio_pin_cfg(PIOA_1, PIOA_1_TSI0_CH2);

    /* 设置外部振荡器恒定电流为16uA */
    amhw_tsi_extchrg_val_set(AMHW_TSI, AMHW_TSI_EXTCHRG_16UA);

    /* 设置电压增量为600mV (Vp - Vm)*/
    amhw_tsi_dvolt_set (AMHW_TSI, AMHW_TSI_DV073);

    /* 扫描预分频设置为2分频 */
    amhw_tsi_oscfreq_div_set(AMHW_TSI, AMHW_TSI_OSCFRQ_DIV_2);

    /* 设置电极扫描次数为16 */
    amhw_tsi_scan_num_set(AMHW_TSI, 16);

    /* 设置参考振荡器充放电电流为32uA */
    amhw_tsi_refchrg_val_set(AMHW_TSI, AMHW_TSI_REFCHRG_32UA);

    /* 配置触发扫描模式 */
    amhw_tsi_scan_trig_set(AMHW_TSI, AMHW_TSI_TRIG_SOFTWARE);

    /* 使能TSI通道 */
    amhw_tsi_chan_set(AMHW_TSI, AMHW_TSI_CHAN2);

    /* 使能TSI模块 */
    amhw_tsi_mode_enable(AMHW_TSI);
    
    /* 连接TSI中断服务函数 */
    am_int_connect(INUM_TSI0, tsi_isr, NULL);
    
    /* TSI中断使能 */
    amhw_tsi_int_enable(AMHW_TSI);
    
    /* 系统中断使能 */
    am_int_enable(INUM_TSI0);
    
    /* 设置阀值上限 */
    amhw_tsi_wake_chan_thresholdh_write(AMHW_TSI, 1000);
    
    /* 设置阀值下限 */
    amhw_tsi_wake_chan_thresholdl_write(AMHW_TSI, 100);
       
    while (1) {

        /* 启动软件触发扫描 */
        amhw_tsi_soft_tirg_start(AMHW_TSI);  
        
        /* 读取检测结结果 */
        value = amhw_tsi_convert_value_get(AMHW_TSI);

        printf("%d\r\n",value);
        am_mdelay(100);

    }
}

/** [src_hw_tsi_int] */
 
/* end of file */

