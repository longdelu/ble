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
 * \brief TPM驱动，PWM驱动层实现
 *
 * 1. TPM支持提供如下三种标准服务，本驱动提供的是服务"PWM输出"标准服务的驱动
 *     - 定时
 *     - PWM输出
 *     - 捕获
 *
 * \note 一个TPM的所有通道共用一个周期值，也就是说同一个TPM不同的通道的PWM周期相同
 *          （TPM0有6个通道，TPM1和TPM1各有2个通道）
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-21  cuishengen, first implementation.
 * \endinternal
 */

#include "amdr_tpm_pwm.h"
#include "am_int.h"
#include "amdr_clk.h"
#include "am_gpio.h"

/*******************************************************************************
* 函数声明
*******************************************************************************/

/** \brief 配置PWM  */
static int __tpm_pwm_config (void          *p_drv,
                             int            chan,
                             unsigned long  duty_ns,
                             unsigned long  period_ns);

/** \brief 使能PWM输出 */
static int __tpm_pwm_enable(void *p_drv, int chan);

/** \brief 禁能PWM输出 */
static int __tpm_pwm_disable(void *p_drv, int chan);

/** \brief PWM驱动函数 */
static const struct am_pwm_drv_funcs __g_tpm_pwm_drv_funcs = {
    __tpm_pwm_config,
    __tpm_pwm_enable,
    __tpm_pwm_disable,
};

/******************************************************************************/
/** \brief 配置PWM  */
static int __tpm_pwm_config (void          *p_drv,
                             int            chan,
                             unsigned long  duty_ns,
                             unsigned long  period_ns)
{
    amdr_tpm_pwm_dev_t *p_dev    = (amdr_tpm_pwm_dev_t *)p_drv;
    amhw_tpm_t         *p_hw_tpm = p_dev->p_devinfo->p_hw_tpm;

    uint32_t clkfreq;
    uint32_t period_c, duty_c;
    uint8_t  pre_real, pre_reg = 0, temp;

    /* 参数不合法 */
    if ((period_ns == 0) || (duty_ns > period_ns)) {
        return -AM_EINVAL;
    }

    /* 有效通道范围 0 ~ (channels_num - 1) */
    if (chan >= p_dev->p_devinfo->channels_num) {
        return -AM_EINVAL;
    }

    clkfreq = amdr_clk_periph_rate_get(p_dev->p_devinfo->p_hw_tpm);

    period_c = (uint64_t)(period_ns) * (clkfreq) / (uint64_t)1000000000;
    duty_c   = (uint64_t)(duty_ns)   * (clkfreq) / (uint64_t)1000000000;

    /* 16位定时器，最大128分频 */
    if (period_c > (0xffffu * 128)) {
        return -AM_EINVAL;
    }

    temp = period_c / 0xffffu + 1;          /* 计算分频大小 */

    /* 只支持分频大小1,2,4,8...128，求得大于分频数中最小的数2^n */
    for (pre_real = 1; pre_real < temp; ) {
        pre_reg++;                        /* 计算写入寄存器的分频值0,1,2,... */
        pre_real = pre_real << 1;           /* 分频数2^n */
    }
    amhw_tpm_prescale_set(p_hw_tpm, (amhw_tpm_prescale_t)pre_reg);

    period_c = period_c / pre_real;
    /* 周期最少为1 */
    if (period_c == 0) {
        period_c = 1;
    }
    amhw_tpm_mod_set(p_hw_tpm, period_c - 1);

    if (duty_c == 0) {
        duty_c = 1;
    }
    duty_c = duty_c / pre_real;
    amhw_tpm_ch_value_set(p_hw_tpm, AMHW_TPM_CH(chan), duty_c - 1);

    return AM_OK;
}

/**
 * \brief 使能PWM通道输出
 */
static int __tpm_pwm_enable (void *p_drv, int chan)
{
    amdr_tpm_pwm_dev_t    *p_dev    = (amdr_tpm_pwm_dev_t *)p_drv;
    amhw_tpm_t            *p_hw_tpm = p_dev->p_devinfo->p_hw_tpm;
    amdr_tpm_pwm_ioinfo_t *p_ioinfo = p_dev->p_devinfo->p_ioinfo;

    /* 有效通道范围 0 ~ (channels_num - 1) */
    if (chan >= p_dev->p_devinfo->channels_num) {
        return -AM_EINVAL;
    }

    /* 选择向上计数 */
    amhw_tpm_ch_mode(p_hw_tpm, chan, AMHW_TPM_COUNTING_UP,
                     AMHW_TPM_CHSC_MS(2) | AMHW_TPM_CHSC_EL_SEL(2));

    am_gpio_pin_cfg(p_ioinfo[chan].gpio, p_ioinfo[chan].func);

    amhw_tpm_clock_mode(p_hw_tpm, AMHW_TPM_CLK_SRC_MODULE);

    return AM_OK;
}

/**
 * \brief 禁能PWM通道输出
 */
static int __tpm_pwm_disable (void *p_drv, int chan)
{
    amdr_tpm_pwm_dev_t    *p_dev      = (amdr_tpm_pwm_dev_t *)p_drv;
    amhw_tpm_t            *p_hw_tpm   = p_dev->p_devinfo->p_hw_tpm;
    amdr_tpm_pwm_ioinfo_t *p_ioinfo   = p_dev->p_devinfo->p_ioinfo;

    /* 有效通道范围 0 ~ (channels_num - 1) */
    if (chan >= p_dev->p_devinfo->channels_num) {
        return -AM_EINVAL;
    }

    amhw_tpm_clock_mode(p_hw_tpm, AMHW_TPM_CLK_SRC_NONE);
    am_gpio_pin_cfg(p_ioinfo[chan].gpio, p_ioinfo[chan].dfunc);

    return AM_OK;
}

/**
  * \brief pwm初始化
  */
void __tpm_pwm_init (amhw_tpm_t *p_hw_tpm)
{
    /* 暂停计数器 */
    amhw_tpm_clock_mode(p_hw_tpm, AMHW_TPM_CLK_SRC_NONE);

    /* 使用系统时钟，不进行分频 */
    amhw_tpm_prescale_set(p_hw_tpm, AMHW_TPM_DIVIDED_BY_1);
}

/**
  * \brief tpm pwm服务初始化
  */
am_pwm_handle_t amdr_tpm_pwm_init (amdr_tpm_pwm_dev_t           *p_dev,
                                   const amdr_tpm_pwm_devinfo_t *p_devinfo)
{
    amhw_tpm_t  *p_hw_tpm;

    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    p_hw_tpm                = p_devinfo->p_hw_tpm;
    p_dev->p_devinfo        = p_devinfo;
    p_dev->pwm_serv.p_funcs = (struct am_pwm_drv_funcs *)&__g_tpm_pwm_drv_funcs;
    p_dev->pwm_serv.p_drv   = p_dev;

    __tpm_pwm_init(p_hw_tpm);

    return &(p_dev->pwm_serv);
}


void amdr_tpm_pwm_deinit (amdr_tpm_pwm_dev_t *p_dev)
{
    amhw_tpm_t *p_hw_tpm;

    if (p_dev == NULL ) {
        return;
    }

    p_hw_tpm = p_dev->p_devinfo->p_hw_tpm;
    amhw_tpm_clock_mode(p_hw_tpm, AMHW_TPM_CLK_SRC_NONE);
}

/* end of file */
