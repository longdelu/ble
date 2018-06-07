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
 * \brief TPM驱动，输入捕获驱动层实现
 *
 * 1. TPM支持提供如下三种标准服务，本驱动提供的是服务"捕获"标准服务的驱动;
 *     - 定时
 *     - PWM输出
 *     - 捕获
 * 2. TPM0有6个通道可作为捕获功能，TPM1和TPM2各有2个通道可作为捕获功能.
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  cuishengen, first implementation.
 * \endinternal
 */

#include "amdr_tpm_cap.h"
#include "amdr_clk.h"
#include "am_gpio.h"
#include "am_int.h"

/*******************************************************************************
* 函数声明
*******************************************************************************/

/** \brief 捕获参数配置  */
static int __tpm_cap_config(void              *p_cookie,
                            int                chan,
                            unsigned int       flags,
                            am_cap_callback_t  pfn_callback,
                            void              *p_arg);

/** \brief 使能捕获通道 */
static int __tpm_cap_enable(void *p_drv, int chan);

/** \brief 禁能捕获通道 */
static int __tpm_cap_disable(void *p_drv, int chan);
                                 
/** \brief 复位捕获通道计数值*/
static int __tpm_cap_reset(void *p_drv, int chan);

static int __tpm_cap_count_to_time (void         *p_drv,
                                    int           chan,
                                    unsigned int  count1,
                                    unsigned int  count2,
                                    unsigned int *p_time_ns);
                                    
static void __tpm_cap_irq_handler(void *p_arg);

/** \brief 捕获服务驱动函数 */
static const struct am_cap_drv_funcs __g_tpm_cap_drv_funcs = {
    __tpm_cap_config,
    __tpm_cap_enable,
    __tpm_cap_disable,
    __tpm_cap_reset,
    __tpm_cap_count_to_time,
};

/******************************************************************************/

/** \brief 配置一个输入捕获通道  */
static int __tpm_cap_config (void              *p_drv,
                             int                chan,
                             unsigned int       options,
                             am_cap_callback_t  pfn_callback,
                             void              *p_arg)
{
    amdr_tpm_cap_dev_t *p_dev    = (amdr_tpm_cap_dev_t *)p_drv;
    amhw_tpm_t         *p_hw_tpm = p_dev->p_devinfo->p_hw_tpm;
    
    /* 有效通道号范围 0 ~ (channels_num - 1) */
    if (chan >= p_dev->p_devinfo->channels_num) {
        return -AM_EINVAL;
    }
    
    /*  选择上升/下降沿触发 */
    if ((options & AM_CAP_TRIGGER_BOTH_EDGES) == AM_CAP_TRIGGER_BOTH_EDGES) {
        amhw_tpm_ch_mode(p_hw_tpm, chan, AMHW_TPM_COUNTING_UP,
                         AMHW_TPM_CHSC_MS(0) | AMHW_TPM_CHSC_EL_SEL(3));
    }

    /*  选择上升沿触发 */
    if ((options & AM_CAP_TRIGGER_RISE) == AM_CAP_TRIGGER_RISE) {
        amhw_tpm_ch_mode(p_hw_tpm, chan, AMHW_TPM_COUNTING_UP,
                         AMHW_TPM_CHSC_MS(0) | AMHW_TPM_CHSC_EL_SEL(1));
    }
    
    /*  选择下降沿触发 */
    if ((options & AM_CAP_TRIGGER_FALL) == AM_CAP_TRIGGER_FALL) {
        amhw_tpm_ch_mode(p_hw_tpm, chan, AMHW_TPM_COUNTING_UP,
                         AMHW_TPM_CHSC_MS(0) | AMHW_TPM_CHSC_EL_SEL(2));
    }
    
    p_dev->callback_info[chan].callback_func = pfn_callback;
    p_dev->callback_info[chan].p_arg         = p_arg;
    
    return AM_OK;
}

/** \brief 捕获使能 */
static int __tpm_cap_enable (void *p_drv, int chan)
{
    amdr_tpm_cap_dev_t    *p_dev    = (amdr_tpm_cap_dev_t *)p_drv;
    amhw_tpm_t            *p_hw_tpm = p_dev->p_devinfo->p_hw_tpm;
    amdr_tpm_cap_ioinfo_t *p_ioinfo = p_dev->p_devinfo->p_ioinfo;
    
    /* 有效通道号范围 0 ~ (channels_num - 1) */
    if (chan >= p_dev->p_devinfo->channels_num) {
        return -AM_EINVAL;
    }
    
    /* 引脚配置 */
    am_gpio_pin_cfg(p_ioinfo[chan].gpio, p_ioinfo[chan].func);

    /* 使能通道中断 */
    amhw_tpm_ch_sc_set(p_hw_tpm, chan, AMHW_TPM_CHSC_IE);
    
    /* 中断连接并使能 */
    am_int_connect(p_dev->p_devinfo->inum, __tpm_cap_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);

    /* 打开时钟，启动计数 */
    amhw_tpm_clock_mode(p_hw_tpm, AMHW_TPM_CLK_SRC_MODULE);
    
    return AM_OK;
}

/** \brief 捕获禁能 */
static int __tpm_cap_disable (void *p_drv, int chan)
{
    amdr_tpm_cap_dev_t    *p_dev    = (amdr_tpm_cap_dev_t *)p_drv;
    amhw_tpm_t            *p_hw_tpm = p_dev->p_devinfo->p_hw_tpm;
    amdr_tpm_cap_ioinfo_t *p_ioinfo = p_dev->p_devinfo->p_ioinfo;
 
    /* 有效通道号范围 0 ~ (channels_num - 1) */
    if (chan >= p_dev->p_devinfo->channels_num) {
        return -AM_EINVAL;
    }
    
    /* 还原GPIO配置 */
    am_gpio_pin_cfg(p_ioinfo[chan].gpio, p_ioinfo[chan].dfunc);
    
    amhw_tpm_ch_sc_clear(p_hw_tpm, chan, AMHW_TPM_CHSC_IE);

    /* 关闭时钟，停止计数 */
    amhw_tpm_clock_mode(p_hw_tpm, AMHW_TPM_CLK_SRC_NONE);
    
    return AM_OK;
}

/**
  * \brief 复位捕获通道计数值
  */
static int __tpm_cap_reset (void *p_drv, int chan)
{
    amdr_tpm_cap_dev_t *p_dev    = (amdr_tpm_cap_dev_t *)p_drv;
    amhw_tpm_t         *p_hw_tpm = p_dev->p_devinfo->p_hw_tpm;
    
    /* 停止TPM计数 */
    amhw_tpm_clock_mode(p_hw_tpm, AMHW_TPM_CLK_SRC_NONE);
    
    return AM_OK;
}

/**
  * \brief 转换两次捕获值为时间值
  */
static int __tpm_cap_count_to_time (void         *p_drv,
                                    int           chan,
                                    unsigned int  count1,
                                    unsigned int  count2,
                                    unsigned int *p_time_ns)
{
    amdr_tpm_cap_dev_t *p_dev    = (amdr_tpm_cap_dev_t *)p_drv;
    amhw_tpm_t         *p_hw_tpm = p_dev->p_devinfo->p_hw_tpm;

    uint32_t clkfreq  = amdr_clk_periph_rate_get(p_dev->p_devinfo->p_hw_tpm);
    uint32_t count_err;
    uint64_t time_ns;
    uint8_t  reg_pre, pre;

    reg_pre = (uint8_t)amhw_tpm_prescale_get(p_hw_tpm);
    if (reg_pre == 0) {
        pre = 1;
    } else {
        pre = 2 << reg_pre;
    }
    
    count_err = count2 - count1;

    /* 将两次读取值的差转换成时间  */
    time_ns = (uint64_t)1000000000 * (uint64_t)count_err * pre / (uint64_t)clkfreq;
    
    *p_time_ns = time_ns;
    
    return AM_OK;
}

/**
  * \brief 中断服务函数
  */
static void __tpm_cap_irq_handler (void *p_arg)
{
    amdr_tpm_cap_dev_t *p_dev     = (amdr_tpm_cap_dev_t *)p_arg;
    amhw_tpm_t         *p_hw_tpm  = p_dev->p_devinfo->p_hw_tpm;
    uint8_t             chans_num = p_dev->p_devinfo->channels_num;
    
    uint8_t             i;
    am_cap_callback_t   callback_func;
    uint32_t            value;
    
    for (i = 0; i < chans_num; i++) {
        if (amhw_tpm_stat_flag_get(p_hw_tpm) & (AMHW_TPM_STAT_CH0_FLAG << i) ) {
            
            callback_func = p_dev->callback_info[i].callback_func;
            value = amhw_tpm_ch_value_get(p_hw_tpm, i);
            
            if (callback_func != NULL) {
                callback_func(p_dev->callback_info[i].p_arg, value);
            }
            
            /* 清除通道i标志 */
            amhw_tpm_stat_flag_clear(p_hw_tpm, (AMHW_TPM_STAT_CH0_FLAG << i));
        }
    }
}

/** 
  * \brief 捕获初始化
  */
void __tpm_cap_init (amhw_tpm_t *p_hw_tpm)
{
    /* 清零计数器 */
    amhw_tpm_count_clear(p_hw_tpm);
    amhw_tpm_prescale_set(p_hw_tpm, AMHW_TPM_DIVIDED_BY_1);
}

am_cap_handle_t amdr_tpm_cap_init (amdr_tpm_cap_dev_t           *p_dev,
                                   const amdr_tpm_cap_devinfo_t *p_devinfo)
{
    amhw_tpm_t *p_hw_tpm;
    uint8_t    i;
    
    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }
    
    p_hw_tpm                = p_devinfo->p_hw_tpm;
    p_dev->p_devinfo        = p_devinfo;
    p_dev->cap_serv.p_funcs = (struct am_cap_drv_funcs *)&__g_tpm_cap_drv_funcs;
    p_dev->cap_serv.p_drv   = p_dev;
    
    for (i = 0; i < p_devinfo->channels_num; i++) {
        p_dev->callback_info[i].callback_func = NULL;
    }
    
    /* 捕获初始化 */
    __tpm_cap_init(p_hw_tpm);
    
    return &(p_dev->cap_serv);
}

void amdr_tpm_cap_deinit (amdr_tpm_cap_dev_t *p_dev)
{
    amhw_tpm_t *p_hw_tpm  = p_dev->p_devinfo->p_hw_tpm;
 
    /* 关闭TPM模块 */
    amhw_tpm_clock_mode(p_hw_tpm, AMHW_TPM_CLK_SRC_NONE);
    
    am_int_disable(p_dev->p_devinfo->inum);
}

/* end of file */
