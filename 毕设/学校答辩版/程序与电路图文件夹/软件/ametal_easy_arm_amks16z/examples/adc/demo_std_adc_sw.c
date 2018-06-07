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
 * \brief ADC例程，软件触发转换，DMA传输转换结果，通过标准接口实现
 *
 * - 操作步骤：
 *   1. PIOA_1引脚连接PC串口的TXD；
 *   2. PIOA_2引脚连接PC串口的RXD；
 *   3. J12跳线帽短接（此时参考电压为2.5v）；
 *   4. PIOE_20(ADC通道0) 连接模拟输入。
 *
 * - 实验现象：
 *   1. 串口输出电压采样值。
 *
 * \par 源代码
 * \snippet demo_std_adc_sw.c src_std_adc_sw
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-26  xjc, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_adc_sw
 * \copydoc demo_std_adc_sw.c
 */

/** [src_std_adc_sw] */
#include "ametal.h"
#include "am_board.h"

/**
 * \brief 主函数入口
 */
int main (void)
{
    uint32_t adc_bits = 0;          /**< \brief ADC位数 */
    uint32_t adc_vref = 0;          /**< \brief ADC参考电压*/
    uint32_t adv_val0[10];          /**< \brief ADC采样原始值 */
    uint32_t adc_mv;                /**< \brief 采样电压 */
    uint32_t cnt = 0;               /**< \brief 获取次数 */
    int chan = 0;                   /**< \brief 通道0 */
    int i;

    am_adc_handle_t adc0_handle; /**< \brief ADC标准服务操作句柄 */

    /* 板级初始化 */
    am_board_init();

    /* 平台初始化 */
    amdr_plfm_adc0_init(); 

    /* 配置ADC 通道0模拟输入引脚 */
    am_gpio_pin_cfg(PIOE_20, PIOE_20_ADC0_SE0);
    
    /* 平台初始化 */
      amhw_plfm_dma_init();

      /* DMA初始化 */
      amdr_dma_init(&g_dma_dev, &g_dma_devinfo);

    /* 初始化ADC，并获取ADC标准服务操作句柄 */
    adc0_handle = amdr_adc_init(&g_adc0_dev, &g_adc0_devinfo);

    /* 获取ADC转换精度 */
    adc_bits = am_adc_bits_get(adc0_handle, chan);
    
    /* 获取ADC参考电压，单位：mV */
    adc_vref = am_adc_vref_get(adc0_handle, chan);
    printf("The ADC SW Demo\r\n");

    while (1) {
        
        /* 获取ADC转换结果，采样完成才返回 */
        am_adc_read(adc0_handle, chan, adv_val0, 10);
        cnt++;
        if (cnt >= 200UL) { /* 延时，控制串口打印快慢  */
            cnt = 0;
            for (i = 1; i < 10; i++) {
                adv_val0[0] += adv_val0[i];
            }
            adv_val0[0] /= 10;
            adc_mv = adv_val0[0] * adc_vref / ((1UL << adc_bits) - 1);
            /* 串口输出采样电压值 */
            printf("Vol: %d mv\r\n", adc_mv);
        }
    }
}

/** [src_std_adc_sw] */
 
/* end of file */
