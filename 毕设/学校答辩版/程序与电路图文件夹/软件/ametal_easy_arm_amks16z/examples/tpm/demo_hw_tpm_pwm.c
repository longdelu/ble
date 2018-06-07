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
 * \brief TPM实现PWM功能，通过HW层的接口实现
 *
 * - 实验现象：
 *   1. PIOD_0(TPM_OUT1)输出4kHz的PWM，占空比为50%；
 *   2. PIOD_1(TPM_OUT2)输出4kHz的PWM，占空比为25%。
 *
 * \par 源代码
 * \snippet demo_hw_tpm_pwm.c src_hw_tpm_pwm
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  cuishengen, first implementation.
 * \endinternal
 */ 

/**
 * \addtogroup demo_if_hw_tpm_pwm
 * \copydoc demo_hw_tpm_pwm.c
 */
 
/** [src_hw_tpm_pwm] */
#include "ametal.h"
#include "am_board.h"


int tpm_pwm_config (amhw_tpm_t    *p_hw_tpm,
                    int            chan,
                    unsigned long  duty_ns,
                    unsigned long  period_ns)
{
    uint32_t clkfreq;
    uint32_t period_c, duty_c;
    uint8_t  prescale, write_pre = 0, temp;

    /* 参数不合法 */
    if ((period_ns == 0) || (duty_ns > period_ns)) {
        return -AM_EINVAL;
    }

    clkfreq = amhw_clock_tpm_rate_get();

    period_c = (uint64_t)(period_ns) * (clkfreq) / (uint64_t)1000000000;
    duty_c   = (uint64_t)(duty_ns)   * (clkfreq) / (uint64_t)1000000000;

    /* 16位定时器，最大128分频 */
    if (period_c > (0xffffu * 128)) {
        return -AM_EINVAL;
    }

    temp = period_c / 0xffffu + 1;          /* 计算分频大小 */

    /* 只支持分频大小1,2,4,8...128，求得大于分频数中最小的数2^n */
    for (prescale = 1; prescale < temp; ) {
        write_pre++;                        /* 计算写入寄存器的分频值0,1,2,... */
        prescale = prescale << 1;           /* 分频数2^n */
    }
    amhw_tpm_prescale_set(p_hw_tpm, (amhw_tpm_prescale_t)write_pre);

    period_c = period_c / prescale;
    /* 周期最少为1 */
    if (period_c == 0) {
        period_c = 1;
    }
    amhw_tpm_mod_set(p_hw_tpm, period_c - 1);

    if (duty_c == 0) {
        duty_c = 1;
    }
    duty_c = duty_c / prescale;
    amhw_tpm_ch_value_set(p_hw_tpm, AMHW_TPM_CH(chan), duty_c - 1);

    return AM_OK;
}

void tpm_pwm_enable (amhw_tpm_t *p_hw_tpm, int chan)
{
    /* 选择向上计数,PWM模式 */
    amhw_tpm_ch_mode(p_hw_tpm, chan, AMHW_TPM_COUNTING_UP,
                     AMHW_TPM_CHSC_MS(2) | AMHW_TPM_CHSC_EL_SEL(2));

    /* 开始计数 */
    amhw_tpm_clock_mode(p_hw_tpm, AMHW_TPM_CLK_SRC_MODULE);

    return;
}

/**
 * \brief 主函数入口
 */
int main (void)
{
    /* 板级初始化 */
    am_board_init();
    
    /* 平台初始化 */
    amhw_plfm_tpm0_pwm_init();
    
    AM_DBG_INFO("The demo for TPM output PWM:\r\n");
    AM_DBG_INFO("The PIOD_0(TPM_OUT0) PWM: freq is 4kHz, The duty ratio is 50% \r\n");
    AM_DBG_INFO("The PIOD_1(TPM_OUT1) PWM: freq is 4kHz, The duty ratio is 25% \r\n");
    
    am_gpio_pin_cfg(PIOD_0, PIOD_0_TPM0_CH0);
    am_gpio_pin_cfg(PIOD_1, PIOD_1_TPM0_CH1);

    tpm_pwm_config(AMHW_TPM0,
                   AMHW_TPM_CH(0),          /* 通道号 */
                   250000 / 2,              /* 占空比(ns) */
                   250000);                 /* 周期（ns） */
    tpm_pwm_config(AMHW_TPM0,
                   AMHW_TPM_CH(1),          /* 通道号 */
                   250000 / 4,              /* 占空比(ns) */
                   250000);                 /* 周期（ns） */

    tpm_pwm_enable(AMHW_TPM0, AMHW_TPM_CH(0));
    tpm_pwm_enable(AMHW_TPM0, AMHW_TPM_CH(1));
    
    while (1) {
        
        am_led_toggle(LED0);
        am_mdelay(500);
    }
}

/** [src_hw_tpm_pwm] */

/* end of file */
