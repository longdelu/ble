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
 * \brief LPTMR实现定时器例程，通过HW层接口实现
 *
 * - 实验现象：
 *   1. LED0以10Hz的频率闪烁。
 *
 * \note LPTMR时钟源有4种选择，在amdr_hwconfig_lptmr_timing.c中进行配置，选择不同大小
 *       的时钟源，定时的范围和精度存在差别。
 *
 * \par 源代码
 * \snippet demo_hw_lptmr_timer.c src_hw_lptmr_timer
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  cuishengen, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_hw_lptmr_timer
 * \copydoc demo_hw_lptmr_timer.c
 */
 
/** [src_hw_lptmr_timer] */
#include "ametal.h"
#include "am_board.h"

/** 
 * \brief  定时器0回调函数
 * \param[in] p_arg : 用户自定义参数，由am_timer_callback_set() 函数传递。
 * \return  无
 */
void lptmr_timing_callback (void *p_arg)
{
    if (amhw_lptmr_flag_check(AMHW_LPTMR)) {
        am_led_toggle(LED0);
        /* 清除溢出标志，写1清零 */
        amhw_lptmr_ctl_set(AMHW_LPTMR, AMHW_LPTMR_CSR_TCF);
    }
}

/**
 * \brief 初始化LPTMR
 *
 * \param[in] p_hw_lptmr : 指向LPTMR寄存器块的指针
 * \param[in] freq       : 中断频率。
 *
 * \retval  AM_OK      : 初始化成功
 * \retval -AM_EINVAL  : 参数有误
 */
int lptmr_timer_init(amhw_lptmr_t *p_hw_lptmr, uint32_t count)
{
    uint32_t pre_reg = 0, pre_real;
    uint32_t temp;

    if (count > (0xFFFFu * 65536)) {
        return -AM_EINVAL;
    }

    /* 不需要分频，禁能分频器 */
    if (count < 0xFFFF) {
        pre_real = 1;
        amhw_lptmr_prescaler_filter_set(p_hw_lptmr, TRUE);
    } else {
        temp = count / 0xFFFFu + 1;
        /* 计算分频值 2^n */
        for (pre_real = 2; pre_real < temp;) {
            pre_reg++;
            pre_real = pre_real << 1;
        }
        /* 分频器有效 */
        amhw_lptmr_prescaler_filter_set(p_hw_lptmr, FALSE);
        /* 设置预分频值 */
        amhw_lptmr_prescaler_set(p_hw_lptmr, (amhw_lptmr_ps_t)pre_reg);
    }

    count = count / pre_real;

    /* 设置装载值 */
    amhw_lptmr_compare_set(p_hw_lptmr, count - 1);

    amhw_lptmr_ctl_set(p_hw_lptmr, AMHW_LPTMR_CSR_TIE);        /* 开中断 */
    am_int_connect(INUM_LPTMR0, lptmr_timing_callback, (void *)0);
    am_int_enable(INUM_LPTMR0);

    return AM_OK;
}

/**
 * \brief 使能LPTMR定时器(开始计数)
 * \param[in] p_hw_lptmr : 指向LPTMR寄存器块的指针
 * \return 无
 */
void lptmr_timer_enable (amhw_lptmr_t *p_hw_lptmr)
{
    /* 关闭时钟，LPTMR开始运行 */
    amhw_lptmr_ctl_set(p_hw_lptmr, AMHW_LPTMR_CSR_TEN);
}

/**
 * \brief  禁止LPTMR定时器(停止计数)
 * \param[in] p_hw_lptmr : 指向LPTMR寄存器块的指针
 * \return  无
 */
void sct_timer_disable (amhw_lptmr_t *p_hw_lptmr)
{
    /* 关闭时钟，TPM停止运行 */
    amhw_lptmr_ctl_clear(p_hw_lptmr, AMHW_LPTMR_CSR_TEN);
}

/**
 * \brief 主函数入口
 */
int main (void)
{
    uint32_t lptmr_clock;

    /* 板级初始化 */
    am_board_init();
    
    /* 平台初始化 */
    amhw_plfm_lptmr_timing_init();
    
    AM_DBG_INFO("The LPTMR demo for standard timing service\r\n");
    AM_DBG_INFO("The led toggle in 10Hz \r\n");
    
    /* 获取输入频率 */
    lptmr_clock = amhw_lptmr_clock_get(AMHW_LPTMR);

    /* 中断频率20Hz */
    lptmr_timer_init(AMHW_LPTMR, lptmr_clock / 20);
    
    /* 使能LPTMR,开始计数 */
    lptmr_timer_enable(AMHW_LPTMR);
    
    while (1) {

    }
}

/** [src_hw_lptmr_timer] */

/* end of file */
