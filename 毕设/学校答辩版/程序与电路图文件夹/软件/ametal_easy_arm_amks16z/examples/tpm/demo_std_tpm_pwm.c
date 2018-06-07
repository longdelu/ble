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
 * \brief TPM实现PWM标准接口例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. PIOD_0(TPM_OUT1)输出4kHz的PWM，占空比为50%；
 *   2. PIOD_1(TPM_OUT2)输出4kHz的PWM，占空比为25%。
 *
 * \par 源代码
 * \snippet demo_std_tpm_pwm.c src_std_tpm_pwm
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  cuishengen, first implementation.
 * \endinternal
 */ 

/**
 * \addtogroup demo_if_std_tpm_pwm
 * \copydoc demo_std_tpm_pwm.c
 */
 
/** [src_std_tpm_pwm] */
#include "ametal.h"
#include "am_board.h"


/**
 * \brief 主函数入口
 */
int main (void)
{
    am_pwm_handle_t tpm0_pwm_handle;
    
    /* 板级初始化 */
    am_board_init();
    
    /* 平台初始化 */
    amhw_plfm_tpm0_pwm_init();
    
    AM_DBG_INFO("The TPM demo for standard pwm service\r\n");
    AM_DBG_INFO("The PIOD_0(TPM_OUT0) PWM: freq is 4kHz, The duty ratio is 50% \r\n");
    AM_DBG_INFO("The PIOD_1(TPM_OUT1) PWM: freq is 4kHz, The duty ratio is 25% \r\n");
    
    /* 初始化定时器0为PWM功能 */
    tpm0_pwm_handle = amdr_tpm_pwm_init(&g_tpm0_pwm_dev, 
                                        &g_tpm0_pwm_devinfo);

    /* 通道0使用PIOD_0，在 amdr_hwconfig_tpm_pwm.c 中配置 */
    am_pwm_config(tpm0_pwm_handle, 0, 250000 / 2, 250000);
    am_pwm_enable(tpm0_pwm_handle, 0);

    /* 通道1使用PIOD_1，在 amdr_hwconfig_tpm_pwm.c 中配置 */
    am_pwm_config(tpm0_pwm_handle, 1, 250000 / 4, 250000);
    am_pwm_enable(tpm0_pwm_handle, 1);
    
    while (1) {
        
        am_led_toggle(LED0);
        am_mdelay(500);
    }
}

/** [src_std_tpm_pwm] */

/* end of file */
