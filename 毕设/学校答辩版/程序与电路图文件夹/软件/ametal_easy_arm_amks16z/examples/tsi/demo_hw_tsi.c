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
 * \brief   TSI触摸检测，硬件层实现
 *
 *  实验现象：
 *  1.当手不接触PIOA_1引脚时，输出检测到的基本值
 *  2.当触摸PIOA_1引脚后，检测到的值变大，并串口输出
 *
 * \par 源代码
 * \snippet demo_hw_tsi.c src_hw_tsi
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-26  xjc, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_hw_tsi
 * \copydoc demo_hw_tsi.c
 */
 
/** [src_hw_tsi] */
#include "ametal.h"
#include "am_board.h"
    
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

    /* 中断禁能 */
    amhw_tsi_int_disable(AMHW_TSI);

    /* 配置触发扫描模式 */
    amhw_tsi_scan_trig_set(AMHW_TSI, AMHW_TSI_TRIG_SOFTWARE);

    /* 使能TSI通道 */
    amhw_tsi_chan_set(AMHW_TSI, AMHW_TSI_CHAN2);

    /* 使能TSI模块 */
    amhw_tsi_mode_enable(AMHW_TSI);
    
    while (1) {

        /* 启动软件触发扫描 */
        amhw_tsi_soft_tirg_start(AMHW_TSI);

        /* 获取扫描结束标志,等待扫描结束 */
        while ( !amhw_tsi_eosf_get(AMHW_TSI));

        /* 读取检测结结果 */
        value = amhw_tsi_convert_value_get(AMHW_TSI);

        /* 清扫描结束标志 */
        amhw_tsi_eosf_clr(AMHW_TSI);

        printf("%d\r\n",value);
        am_led_toggle (LED1);
        am_mdelay(1000);

    }
}

/** [src_hw_tsi] */
 
/* end of file */

