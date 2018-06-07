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
 * \brief TPM实现捕获功能，通过HW层的接口实现
 *
 * - 实验现象：
 *   1. 输出利用捕获功能得到的PWM信号的周期和频率。
 *
 * - 注意：
 *   1. TPM1通过PIOE_20引脚输出PWM；
 *   2. TPM0捕获输入通道0使用PIOD_0引脚；
 *   3. 关联PIOE_20引脚和PIOD_0引脚，使用捕获测量PWM频率。
 *
 * \par 源代码
 * \snippet demo_hw_tpm_cap.c src_hw_tpm_cap
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  cuishengen, first implementation.
 * \endinternal
 */ 
 
/**
 * \addtogroup demo_if_hw_tpm_cap
 * \copydoc demo_hw_tpm_cap.c
 */
 
/** [src_hw_tpm_cap] */
#include "ametal.h"
#include "am_board.h"

/** \brief 上升沿捕获 */
#define TPM_CAP_TRIGGER_RISE  1

/** \brief 下降沿捕获 */
#define TPM_CAP_TRIGGER_FALL  2

/** \brief TPM通道号 */
#define TPM_CAP_CH_NUM        AMHW_TPM_CH(0)

volatile bool_t   g_flag    = FALSE;    /**< \brief 捕获标志，是否捕获到新的事件   */
volatile uint32_t g_time_ns = 0;        /**< \brief 捕获计数值 */
uint32_t          g_tpm_clock;          /**< \brief tpm采用的时钟源大小 */
uint32_t          g_freq;               /**< \brief 捕获到的频率 */

void tpm_cap_isr (void *p_arg)
{
    static uint32_t count1;
    static bool_t first    = TRUE;
    amhw_tpm_t   *p_hw_tpm = (amhw_tpm_t *)p_arg;
    uint32_t      cap_val;
    
    if (amhw_tpm_stat_flag_get(p_hw_tpm) & AMHW_TPM_STAT_FLAG(TPM_CAP_CH_NUM)) {
        cap_val = amhw_tpm_ch_value_get(p_hw_tpm, TPM_CAP_CH_NUM);
        if (g_flag == FALSE) {
            if (first == TRUE) {
                count1 = cap_val;
                first = FALSE;
            } else {
                if(cap_val > count1) {
                    g_time_ns = (uint64_t)1000000000 * (cap_val - count1) / g_tpm_clock;
                }
                first = TRUE;
                /* 置标志为真，表明捕获完成 */
                g_flag = TRUE;
            } 
        }

        /* 清除通道标志 */
        amhw_tpm_stat_flag_clear(p_hw_tpm, AMHW_TPM_STAT_FLAG(TPM_CAP_CH_NUM));
    }
}

/**
 * \brief 捕获初始化
 * \param[in] p_tpm : 指向TPM寄存器块的指针
 * \return 无
 * \note 默认配置TPM0的通道0引脚，如选择其它通道则需在此修改
 */
void tpm_cap_init (amhw_tpm_t *p_hw_tpm)
{
    /* 关闭时钟，停止计数 */
    amhw_tpm_clock_mode(p_hw_tpm, AMHW_TPM_CLK_SRC_NONE);

    /* 清零计数器 */
    amhw_tpm_count_clear(p_hw_tpm);

    /* 预分频设置 */
    amhw_tpm_prescale_set(p_hw_tpm, AMHW_TPM_DIVIDED_BY_1);

    /* 获取tpm时钟源频率，便于后面计算 */
    g_tpm_clock = amhw_clock_tpm_rate_get();

    /* 捕获引脚设置 */
    am_gpio_pin_cfg(PIOD_0, PIOD_0_TPM0_CH0);
}

/**
 * \brief 初始化一路PWM输出。
 *
 * \param[in] p_tpm    : 指向TPM寄存器块的指针。
 * \param[in] cap_num  : 捕获编号。
 * \param[in] cap_edge : 捕获边沿选择 (#TPM_CAP_TRIGGER_RISE) 或 (#TPM_CAP_TRIGGER_FALL)
 *
 * \return  无
 */
void tpm_cap_chan_config (amhw_tpm_t *p_hw_tpm, uint8_t cap_num, uint8_t cap_edge)
{
    if (cap_edge == TPM_CAP_TRIGGER_RISE) {
        amhw_tpm_ch_mode(p_hw_tpm, cap_num, AMHW_TPM_COUNTING_UP,
                           AMHW_TPM_CHSC_MS(0) | AMHW_TPM_CHSC_EL_SEL(1));
    } else {
        amhw_tpm_ch_mode(p_hw_tpm, cap_num, AMHW_TPM_COUNTING_UP,
                           AMHW_TPM_CHSC_MS(0) | AMHW_TPM_CHSC_EL_SEL(2));
    }
}

void tpm_cap_chan_enable (amhw_tpm_t *p_hw_tpm, uint32_t cap_num)
{
    /* 使能通道中断 */
    amhw_tpm_ch_sc_set(p_hw_tpm, cap_num, AMHW_TPM_CHSC_IE);

    /* 打开时钟，启动计数 */
    amhw_tpm_clock_mode(p_hw_tpm, AMHW_TPM_CLK_SRC_MODULE);
}

/**
 * \brief 主函数入口
 */
int main (void)
{
    am_pwm_handle_t tpm1_pwm_handle;
    
    /* 板级初始化 */
    am_board_init();

    /* 平台初始化，打开外设时钟 */
    amhw_plfm_tpm1_pwm_init();

    /* 平台初始化，打开外设时钟 */
    amhw_plfm_tpm0_cap_init();
    
    AM_DBG_INFO("The TPM demo for CAP service\r\n");
    
    tpm1_pwm_handle = amdr_tpm_pwm_init(&g_tpm1_pwm_dev,
                                           &g_tpm1_pwm_devinfo);
    
    /* TPM1通道0使用PIOE_20，在 amhw_timer_cap_config.c 中配置 */
    am_pwm_config(tpm1_pwm_handle, 0, 500000 / 2, 500000); /* T频率为2KHz */
    am_pwm_enable(tpm1_pwm_handle, 0);
    
    am_int_connect(INUM_TPM0, tpm_cap_isr, (void *)AMHW_TPM0);
    am_int_enable(INUM_TPM0);

    tpm_cap_init(AMHW_TPM0);
    tpm_cap_chan_config(AMHW_TPM0, TPM_CAP_CH_NUM, TPM_CAP_TRIGGER_RISE);
    tpm_cap_chan_enable(AMHW_TPM0, TPM_CAP_CH_NUM);

    while (1) {
        if (g_flag == TRUE) {
            
            g_freq = 1000000000 / g_time_ns;           /* 转换成频率 */
            AM_DBG_INFO("The period is %d ns, The freq is %d Hz \r\n", g_time_ns, g_freq);
            g_flag = FALSE;   
        }
        am_mdelay(200);
        am_led_toggle(LED0);
    }
}
 
/** [src_hw_tpm_cap] */

/* end of file */
