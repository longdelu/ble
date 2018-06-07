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
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief ADC例程，利用查询方式获取软件触发的ADC转换结果，通过HW层接口实现
 *
 * - 操作步骤：
 *   1. PIOA_1引脚连接PC串口的TXD；
 *   2. PIOA_2引脚连接PC串口的RXD；
 *   3. J12跳线帽短接（此时参考电压为2.5v）；
 *   4. PIOE_20(ADC通道0) 连接模拟输入。
 *
 * - 实验现象：
 *   1. ADC模块以444.44kHz采样率(总线时钟为24MHz)，进行16位采样;
 *   2. 串口输出电压采样值。
 *
 * \par 源代码
 * \snippet demo_hw_adc_poll.c src_hw_adc_poll
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-26  xjc, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_hw_adc_poll
 * \copydoc demo_hw_adc_poll.c
 */

/** [src_hw_adc_poll] */
#include "ametal.h"
#include "am_board.h"

/**
 * \brief  ADC 配置函数
 * \param[in] 无
 * \return    无
 */
void adc_config (void)
{
    /* 设置ADC模块采样率或时钟 */
    amhw_adc_clkdiv_set (AMHW_ADC0, AMHW_ADC_CLK_DIV_2);
    amhw_adc_clksrc_set(AMHW_ADC0, AMHW_ADC_CLK_SRC_BUS);

    /* 设置分辨率 */
    amhw_adc_mode_set(AMHW_ADC0, AMHW_ADC_BITS_16);

    /* 使用软件触发 , 默认使用序列A*/
    amhw_adc_hwtrg_disable(AMHW_ADC0);

    /* 使能连续采样模式 */
    amhw_adc_continue_enable(AMHW_ADC0);

    /* 使能高速转换模式 */
    amhw_adc_hsc_enable(AMHW_ADC0);

    /* 不使能低功耗 */
    amhw_adc_lowpower_disable(AMHW_ADC0);

    /* 关闭硬件平均和比较功能 */
    amhw_adc_cmp_disable(AMHW_ADC0);
    amhw_adc_avg_disable(AMHW_ADC0);

    /* 配置成单端模式， 通道为0，不开中断，同时启动触发，先关闭通道 */
    amhw_adc_sc1_cfg(AMHW_ADC0,
                     AMHW_ADC_SEQ_A,
                     AMHW_ADC_SC1_CHAN_SEL_CFG(AMHW_ADC_CHAN_CLOSE));
}

/**
 * \brief  ADC 软件启动转换函数
 * \param[in] 无
 * \return    无
 */
void adc_start (amhw_adc_chan_t chan)
{
    amhw_adc_chan_set(AMHW_ADC0, AMHW_ADC_SEQ_A, chan);
}


int main (void) 
{
    uint32_t adc_mv;                /**< \brief 采样电压 */
    uint32_t adc_dat;               /**< \brief 采样值 */
    uint32_t cnt = 0;               /**< \brief 获取次数 */

    /* 板级初始化 */
    am_board_init();
    
    /* 配置ADC 通道0模拟输入引脚 */
    am_gpio_pin_cfg(PIOE_20, PIOE_20_ADC0_SE0);
    
    /* 使能ADC时钟                  */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_ADC0);

    /* ADC校准 */
    amhw_adc_calibrate(AMHW_ADC0);
    
    /* ADC配置初始化 */
    adc_config();
   
    printf("The ADC Poll Demo\r\n");
    amhw_adc_continue_enable(AMHW_ADC0);
    adc_start(AMHW_ADC_CHAN_0);

    while(1){

        while (!amhw_adc_complete_is(AMHW_ADC0, AMHW_ADC_SEQ_A)) {

        }
        am_led_toggle(0);
        adc_dat = amhw_adc_dat_get(AMHW_ADC0, AMHW_ADC_SEQ_A);
        cnt++;
        if (cnt >= 200000UL) { /* 延时，控制串口打印快慢  */
            cnt = 0;
            adc_mv = adc_dat * 2500 / ((1UL << 16) - 1);
            /* 串口输出采样电压值 */
            printf("Vol: %d mv\r\n", adc_mv);
        }
    }
}
/** [src_hw_adc_poll] */

/* end of file */
