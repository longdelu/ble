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
 * \brief DAC数模转换例程，通过HW接口实现
 *
 * - 实验现象：
 *   1. 采集数模转换的输出口PE30的电压值
 *   2. 串口输出PE30的电压值。
 *
 *
 * \par 源代码
 * \snippet demo_hw_dac.c src_hw_dac 
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-22  xym, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_hw_dac
 * \copydoc demo_hw_dac.c
 */
 
 /** [src_hw_dac] */
#include "ametal.h"
#include "am_board.h"

/**
 * \brief 主函数入口
 */

void __dac_init (void)
{

    /* 配置PIOE_30为DAC0_OUT功能                 */
    am_gpio_pin_cfg (PIOE_30,PIOE_30_DAC0_OUT);

    /* 开启DAC时钟                               */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_DAC0);

    /*
     * 配置DAC相关功能
     */
    amhw_dac_c0_cfg(AMHW_DAC0,AMHW_DAC_C0_EN                        |
                              AMHW_DAC_C0_VREF_SEL(AMHW_DAC_VREF_2) |
                              AMHW_DAC_C0_TRG_SEL(AMHW_DAC_TRG_SOFT)|
                              AMHW_DAC_C0_POWER_SEL(AMHW_DAC_POWER_LOW));
}

void  __dac_trigger (uint16_t data)
{
     /*  DAC寄存器赋值*/
    amhw_dac_dat_set(AMHW_DAC0,AMHW_DAC_DAT_0,data);
     /* 使能DAC软件触发            */
    amhw_dac_soft_trg_enable(AMHW_DAC0);
}

int main (void)
{
    /* 计算转换值              */
    uint16_t v_out = 1.823 * 4096 / 3.3;
    /* 板级初始化  */
    am_board_init();
    __dac_init();
    while (1) {
        __dac_trigger(v_out);

    }
}


 /** [src_hw_dac] */
 
/* end of file */


