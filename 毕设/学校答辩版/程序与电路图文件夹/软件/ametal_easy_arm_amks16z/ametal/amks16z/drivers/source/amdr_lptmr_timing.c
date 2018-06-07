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
 * \brief KS16z LPTMR 定时功能驱动层实现
 *
 * 1. LPTMR低功耗定时器，本驱动实现其"定时"服务
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-20  cuishengen, first implementation.
 * \endinternal
 */

#include "amdr_lptmr_timing.h"
#include "am_int.h"
#include "amdr_clk.h"
#include "amhw_lptmr.h"

/*******************************************************************************
  定时器驱动函数声明
*******************************************************************************/

static int __lptmr_timing_info_get(void *p_drv, am_timer_info_t *p_info);

static int __lptmr_timing_count_get(void     *p_drv,
                                    uint8_t   chan,
                                    uint32_t *p_count);

static int __lptmr_timing_rollover_get(void     *p_drv,
                                       uint8_t   chan,
                                       uint32_t *p_rollover);

static int __lptmr_timing_disable(void *p_drv, uint8_t chan);

static int __lptmr_timing_enable(void     *p_drv,
                                 uint8_t   chan,
                                 uint32_t  max_lptmr_count);

static int __lptmr_timing_callback_set(void    *p_drv,
                                       uint8_t  chan,
                                       void   (*pfn_callback)(void *),
                                       void    *p_arg);

static int  __lptmr_timing_connect(void *p_drv);

static void __lptmr_irq_handler(void *p_arg);

/** \brief LPTMR定时器驱动函数 */
static const struct am_timer_drv_funcs __g_lptmr_timing_drv_funcs = {
    __lptmr_timing_info_get,
    __lptmr_timing_count_get,
    __lptmr_timing_rollover_get,
    __lptmr_timing_disable,
    __lptmr_timing_enable,
    __lptmr_timing_callback_set,
    NULL,
    NULL,
    NULL,
    __lptmr_timing_connect,
};

/******************************************************************************/
/**
 * \brief LPTMR 定时器中断连接
 */
int __lptmr_timing_connect (void *p_drv)
{
    amdr_lptmr_timing_dev_t *p_dev = (amdr_lptmr_timing_dev_t *)p_drv;

    am_int_connect(p_dev->p_devinfo->inum, __lptmr_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);

    return AM_OK;
}

void __lptmr_irq_handler (void *p_arg)
{
    amdr_lptmr_timing_dev_t *p_dev    = (amdr_lptmr_timing_dev_t *)p_arg;
    amhw_lptmr_t            *p_hw_lptmr =  p_dev->p_devinfo->p_hw_lptmr;

    if (amhw_lptmr_flag_check(p_hw_lptmr)) {

        if (p_dev->pfn_callback != NULL) {
            p_dev->pfn_callback(p_dev->p_arg);
        }

        /* 清除溢出标志，写1清零 */
        amhw_lptmr_ctl_set(p_hw_lptmr, AMHW_LPTMR_CSR_TCF);
    }
}

/** \brief 获取LPTMR信息 */
static int __lptmr_timing_info_get (void *p_drv, am_timer_info_t *p_info)
{
    amdr_lptmr_timing_dev_t *p_dev    = (amdr_lptmr_timing_dev_t *)p_drv;
    amhw_lptmr_t            *p_hw_lptmr =  p_dev->p_devinfo->p_hw_lptmr;

    /* 获取输入频率 */
    p_info->clk_frequency = amdr_clk_periph_rate_get(p_hw_lptmr);

    /* 可以不分频 */
    p_info->max_frequency = p_info->clk_frequency;
    /* 16位定时器，最大支持65536分频 */
    p_info->min_frequency = p_info->clk_frequency / (0xFFFFu * 65536) + 1;

    p_info->features  = AM_TIMER_SIZE(16)           |   /* 16位宽定时器 */
                        AM_TIMER_CHAN_NUM(1)        |   /* 支持1个通道 */
                        AM_TIMER_CAN_INTERRUPT      |   /* 可以产生中断 */
                     // AM_TIMER_INTERMEDIATE_COUNT |   /* 可以实时读出计数值 */
                        AM_TIMER_AUTO_RELOAD;           /* 自动装载 */

    return AM_OK;
}

/**
 * \brief 获取LPTMR当前计数值
 */
static int __lptmr_timing_count_get (void     *p_drv,
                                     uint8_t   chan,
                                     uint32_t *p_count)
{
    amdr_lptmr_timing_dev_t *p_dev      = (amdr_lptmr_timing_dev_t *)p_drv;
    amhw_lptmr_t            *p_hw_lptmr =  p_dev->p_devinfo->p_hw_lptmr;

    *p_count = amhw_lptmr_counter_get(p_hw_lptmr);

    return AM_OK;
}

/**
 * \brief 获取LPTMR翻滚值
 */
static int __lptmr_timing_rollover_get (void     *p_drv,
                                      uint8_t   chan,
                                      uint32_t *p_rollover)
{
    amdr_lptmr_timing_dev_t *p_dev    = (amdr_lptmr_timing_dev_t *)p_drv;
    amhw_lptmr_t            *p_hw_lptmr =  p_dev->p_devinfo->p_hw_lptmr;

    *p_rollover = 0xFFFFu / amdr_clk_periph_rate_get(p_hw_lptmr);

    return AM_OK;
}

/**
 * \brief LPTMR禁能
 */
static int __lptmr_timing_disable (void *p_drv, uint8_t chan)
{
    amdr_lptmr_timing_dev_t *p_dev    = (amdr_lptmr_timing_dev_t *)p_drv;
    amhw_lptmr_t            *p_hw_lptmr =  p_dev->p_devinfo->p_hw_lptmr;

    /* 清零使能位，禁能 */
    amhw_lptmr_ctl_clear(p_hw_lptmr, AMHW_LPTMR_CSR_TEN);

    return AM_OK;
}

/**
 * \brief LPTMR使能并设置计数值
 */
static int __lptmr_timing_enable (void     *p_drv,
                                  uint8_t   chan,
                                  uint32_t  max_lptmr_count)
{
    amdr_lptmr_timing_dev_t *p_dev    = (amdr_lptmr_timing_dev_t *)p_drv;
    amhw_lptmr_t            *p_hw_lptmr =  p_dev->p_devinfo->p_hw_lptmr;

    uint32_t pre_reg = 0, pre_real;
    uint32_t temp;

    if (max_lptmr_count > (0xFFFFu * 65536)) {
        return -AM_EINVAL;
    }

    /* 不需要分频，禁能分频器 */
    if (max_lptmr_count < 0xFFFF) {
        pre_real = 1;
        amhw_lptmr_prescaler_filter_set(p_hw_lptmr, TRUE);
    } else {
        temp = max_lptmr_count / 0xFFFFu + 1;
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

    max_lptmr_count = max_lptmr_count / pre_real;

    /* 设置装载值 */
    amhw_lptmr_compare_set(p_hw_lptmr, max_lptmr_count - 1);

    /* 使能LPTMR */
    amhw_lptmr_ctl_set(p_hw_lptmr, AMHW_LPTMR_CSR_TEN);

    return AM_OK;
}

/**
 * \brief 设置中断回调函数
 */
static int __lptmr_timing_callback_set (void     *p_drv,
                                        uint8_t   chan,
                                        void    (*pfn_callback)(void *),
                                        void     *p_arg)
{
    amdr_lptmr_timing_dev_t *p_dev     = (amdr_lptmr_timing_dev_t *)p_drv;
    amhw_lptmr_t            *p_hw_lptmr =  p_dev->p_devinfo->p_hw_lptmr;


    if (pfn_callback == NULL) {                     /* 不需要开启中断 */
        amhw_lptmr_ctl_clear(p_hw_lptmr, AMHW_LPTMR_CSR_TIE);
    } else {                                        /* 需要开启中断 */
        p_dev->pfn_callback = pfn_callback;
        p_dev->p_arg        = p_arg;
        amhw_lptmr_ctl_set(p_hw_lptmr, AMHW_LPTMR_CSR_TIE);
    }

    return AM_OK;
}

/**
 * \brief LPTMR初始化
 */
am_timer_handle_t amdr_lptmr_timing_init (amdr_lptmr_timing_dev_t           *p_dev,
                                        const amdr_lptmr_timing_devinfo_t *p_devinfo)
{
    amhw_lptmr_t *p_hw_lptmr;

    if (p_devinfo == NULL || p_devinfo->p_hw_lptmr == NULL) {
        return NULL;
    }

    p_hw_lptmr                = p_devinfo->p_hw_lptmr;
    p_dev->p_devinfo          = p_devinfo;
    p_dev->timer_serv.p_funcs = (struct am_timer_drv_funcs *)&__g_lptmr_timing_drv_funcs;
    p_dev->timer_serv.p_drv   = p_dev;

    p_dev->pfn_callback = NULL;

    /* 关闭LPTMR模块，LMTMR预分频寄存器只有在关闭状态下才允许修改 */
    amhw_lptmr_ctl_clear(p_hw_lptmr, AMHW_LPTMR_CSR_MASK);

    return &(p_dev->timer_serv);
}

/**
 * \brief LPTMR解初始化
 */
void amdr_lptmr_timing_deinit (amdr_lptmr_timing_dev_t *p_dev)
{
    amhw_lptmr_t *p_hw_lptmr = p_dev->p_devinfo->p_hw_lptmr;

    /* 关闭LPTMR模块 */
    amhw_lptmr_ctl_clear(p_hw_lptmr, AMHW_LPTMR_CSR_TEN);

    /* 关闭LPTMR中断 */
    am_int_disable(p_dev->p_devinfo->inum);
}

/* end of file */
