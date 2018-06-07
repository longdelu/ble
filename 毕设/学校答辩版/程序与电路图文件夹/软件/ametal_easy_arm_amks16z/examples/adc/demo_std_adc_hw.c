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
 * \brief ADC例程，硬件触发转换，DMA传输转换结果，通过标准接口实现
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
 * \snippet demo_std_adc_hw.c src_std_adc_hw
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-26  xjc, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_adc_hw
 * \copydoc demo_std_adc_hw.c
 */

/** [src_std_adc_hw] */
#include "ametal.h"
#include "am_board.h"

/** \brief ADC 设备信息 （硬件触发，无比较无硬件平均）*/
const amdr_adc_devinfo_t g_adc0_devinfo_hw =
{
    AMHW_ADC0,                     /**< \brief 指向ADC0寄存器块的指针 */
    2500,                          /**< \brief ADC参考电压，单位：mV  */
    12,                            /**< \brief ADC采样精度(位数)   */
    AMDR_ADC_DIFF_DIS,             /**< \brief ADC0单端模式        */
    INUM_ADC0,                     /**< \brief ADC中断向量号  */
    AMDR_ADC_MODE_TRG_HW,          /**< \brief ADC硬件触发模式      */
    AMHW_ADC_SEQ_A,                /**< \brief ADC序列索引            */
    DMA_CHAN_0,                    /**< \brief DMA通道0传输 ，值为-1时不使用DMA */
    NULL,                          /**< \brief 不使用硬件平均功能       */
    NULL,                          /**< \brief 不使用比较功能       */
};

/**
 * \brief  定时器0回调函数
 * \param[in] p_arg : 用户自定义参数，由am_timer_callback_set() 函数传递。
 * \return  无
 */
void tpm0_timing_callback (void *p_arg)
{
    am_led_toggle(LED0);
}
/**
 * \brief 主函数入口
 */
int main (void)
{
    uint32_t adc_mv[5];             /**< \brief 采样电压 */
    int chan = 0;                   /**< \brief 通道0 */
    int i;

    am_adc_handle_t   adc0_handle;          /**< \brief ADC标准服务操作句柄 */
    am_timer_handle_t tpm0_timing_handle;   /**< \brief TPM标准服务操作句柄 */
    am_timer_info_t   tpm0_timing_info;     /**< \brief 定时信息                     */


    /* 板级初始化 */
    am_board_init();

    /* DMA平台初始化 */
    amhw_plfm_dma_init();

    /* DMA初始化 */
    amdr_dma_init(&g_dma_dev, &g_dma_devinfo);

    /* ADC平台初始化 */
    amdr_plfm_adc0_init(); 
   
    /* TPM平台初始化 */
    amhw_plfm_tpm0_timing_init();

    /* 初始化定时器0为定时功能 */
    tpm0_timing_handle = amdr_tpm_timing_init(&g_tpm0_timing_dev,
                                              &g_tpm0_timing_devinfo);
    am_timer_connect(tpm0_timing_handle);

    am_timer_info_get(tpm0_timing_handle, &tpm0_timing_info);
    am_timer_callback_set(tpm0_timing_handle, 1, tpm0_timing_callback, NULL);

    /* 设定中断频率20Hz 1秒钟触发20次ADC转换 */
    am_timer_enable(tpm0_timing_handle, 1, tpm0_timing_info.clk_frequency / 20);

    /* 使能ADC硬件可选触发源，选择TPM0溢出触发 */
    amhw_sim_adc0_alter_trigger_enable();
    amhw_sim_adc0_trigger_set(AMHW_SIM_ADC0_TRIGGER_TPM0);

    /* 配置ADC 通道0模拟输入引脚 */
    am_gpio_pin_cfg(PIOE_20, PIOE_20_ADC0_SE0);
    
    /* 初始化ADC，并获取ADC标准服务操作句柄 */
    adc0_handle = amdr_adc_init(&g_adc0_dev, &g_adc0_devinfo_hw);

    printf("The ADC HW Demo\r\n");

    while (1) {
        
        /* 获取ADC采集电压，采样完成才返回 */
        am_adc_read_mv(adc0_handle, chan, adc_mv, 5);
        for (i = 1; i < 5; i++) {
            adc_mv[0] += adc_mv[i];
        }
        adc_mv[0] /= 5;
        printf("Vol: %d mv\r\n", adc_mv[0]);
    }
}

/** [src_std_adc_hw] */
 
/* end of file */
