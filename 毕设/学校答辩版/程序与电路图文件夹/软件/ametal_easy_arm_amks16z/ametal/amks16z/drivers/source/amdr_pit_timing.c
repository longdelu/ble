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
 * \brief KS16z PIT 定时功能驱动层实现
 *
 * 1. PIT周期性中断定时器，本驱动实现其"定时"服务
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-20  cuishengen, first implementation.
 * \endinternal
 */

#include "amdr_pit_timing.h"
#include "am_int.h"
#include "amdr_clk.h"

/*******************************************************************************
  定时器驱动函数声明
*******************************************************************************/

static int __pit_timing_info_get(void *p_drv, am_timer_info_t *p_info);

static int __pit_timing_count_get(void     *p_drv,
                                  uint8_t   chan,
                                  uint32_t *p_count);

static int __pit_timing_rollover_get(void     *p_drv,
                                     uint8_t   chan,
                                     uint32_t *p_rollover);

static int __pit_timing_disable(void *p_drv, uint8_t chan);

static int __pit_timing_enable(void     *p_drv,
                               uint8_t   chan,
                               uint32_t  max_pit_count);

static int __pit_timing_callback_set(void    *p_drv,
                                     uint8_t  chan,
                                     void   (*pfn_callback)(void *),
                                     void    *p_arg);

static int  __pit_timing_connect(void *p_drv);

static void __pit_irq_handler(void *p_arg);

static int __pit_timing_count_get64 (void     *p_drv,
                                     uint8_t   chan,
                                     uint64_t *p_count);
static int __pit_timing_rollover_get64 (void    *p_drv,
                                        uint8_t  chan,
                                        uint64_t *p_rollover);

static int __pit_timing_enable64 (void     *p_drv,
                                  uint8_t   chan,
                                  uint64_t  max_timer_count);

/** \brief SCT for timing driver functions */
static const struct am_timer_drv_funcs __g_pit_timing_drv_funcs = {
    __pit_timing_info_get,
    __pit_timing_count_get,
    __pit_timing_rollover_get,
    __pit_timing_disable,
    __pit_timing_enable,
    __pit_timing_callback_set,
    __pit_timing_count_get64,
    __pit_timing_rollover_get64,
    __pit_timing_enable64,
    __pit_timing_connect,
};

/******************************************************************************/
/**
 * \brief PIT 定时器中断连接
 */
int __pit_timing_connect (void *p_drv)
{
    amdr_pit_timing_dev_t *p_dev = (amdr_pit_timing_dev_t *)p_drv;

    am_int_connect(p_dev->p_devinfo->inum, __pit_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);

    return AM_OK;
}

void __pit_irq_handler (void *p_arg)
{
    amdr_pit_timing_dev_t *p_dev    = (amdr_pit_timing_dev_t *)p_arg;
    amhw_pit_t            *p_hw_pit =  p_dev->p_devinfo->p_hw_pit;

    /*
     * 判断是否是定时器溢出中断
     */
    if (amhw_pit_timerout_flag_check(p_hw_pit, AMHW_PIT_CH(0))) {
        if (p_dev->pfn_ch1_callback != NULL) {
            p_dev->pfn_ch1_callback(p_dev->p_arg_ch1);
            amhw_pit_timerout_flag_clr(p_hw_pit, 0);
        }
    }
    if(amhw_pit_timerout_flag_check(p_hw_pit, AMHW_PIT_CH(1))) {
        if (p_dev->pfn_ch2_callback != NULL) {
            p_dev->pfn_ch2_callback(p_dev->p_arg_ch2);
             amhw_pit_timerout_flag_clr(p_hw_pit, 1);
        }
    }
}

/** \brief 获取PIT信息 */
static int __pit_timing_info_get (void *p_drv, am_timer_info_t *p_info)
{
    amdr_pit_timing_dev_t *p_dev    = (amdr_pit_timing_dev_t *)p_drv;
    amhw_pit_t            *p_hw_pit =  p_dev->p_devinfo->p_hw_pit;

    /* 获取输入频率 */
    p_info->clk_frequency = amdr_clk_periph_rate_get(p_hw_pit);

    p_info->max_frequency = p_info->clk_frequency;
    p_info->min_frequency = p_info->clk_frequency / (0xFFFFFFFFu) + 1;

    p_info->features  = AM_TIMER_SIZE(32)           |   /* 32位宽定时器 */
                        AM_TIMER_CHAN_NUM(2)        |   /* 支持2个通道 */
                        AM_TIMER_CAN_INTERRUPT      |   /* 可以产生中断 */
                        AM_TIMER_INTERMEDIATE_COUNT |   /* 可以实时读出计数值 */
                        AM_TIMER_AUTO_RELOAD;           /* 自动装载 */

    return AM_OK;
}

/**
 * \brief 获取PIT当前计数值
 */
static int __pit_timing_count_get (void     *p_drv,
                                   uint8_t   chan,
                                   uint32_t *p_count)
{
    amdr_pit_timing_dev_t *p_dev    = (amdr_pit_timing_dev_t *)p_drv;
    amhw_pit_t            *p_hw_pit =  p_dev->p_devinfo->p_hw_pit;

    *p_count = amhw_pit_ldval_get(p_hw_pit, AMHW_PIT_CH(chan));

    return AM_OK;
}

/**
 * \brief 获取PIT翻滚值
 */
static int __pit_timing_rollover_get (void     *p_drv,
                                      uint8_t   chan,
                                      uint32_t *p_rollover)
{
    amdr_pit_timing_dev_t *p_dev    = (amdr_pit_timing_dev_t *)p_drv;
    amhw_pit_t            *p_hw_pit =  p_dev->p_devinfo->p_hw_pit;

    *p_rollover = 0xFFFFFFFFu / amdr_clk_periph_rate_get(p_hw_pit);


    return AM_OK;
}

/**
 * \brief PIT禁能
 */
static int __pit_timing_disable (void *p_drv, uint8_t chan)
{
    amdr_pit_timing_dev_t *p_dev    = (amdr_pit_timing_dev_t *)p_drv;
    amhw_pit_t            *p_hw_pit =  p_dev->p_devinfo->p_hw_pit;

    /* 清零使能位 */
    amhw_pit_timer_ctrl_clear(p_hw_pit,
                              AMHW_PIT_CH(chan),
                              AMHW_PIT_TCTL_TMR_EN);

    return AM_OK;
}

/**
 * \brief PIT使能并设置计数值
 */
static int __pit_timing_enable (void     *p_drv,
                                uint8_t   chan,
                                uint32_t  max_pit_count)
{
    amdr_pit_timing_dev_t *p_dev    = (amdr_pit_timing_dev_t *)p_drv;
    amhw_pit_t            *p_hw_pit =  p_dev->p_devinfo->p_hw_pit;


    amhw_pit_module_enable(p_hw_pit);           /* 打开PIT模块 */

    /* 设置装载值 */
    amhw_pit_ldval_set(p_hw_pit, AMHW_PIT_CH(chan), max_pit_count);
    /* 打开中断并使能通道 */
    amhw_pit_timer_ctrl_set(p_hw_pit,
                            AMHW_PIT_CH(chan),
                            AMHW_PIT_TCTL_TI_EN | AMHW_PIT_TCTL_TMR_EN);

    return AM_OK;
}

/**
 * \brief 设置中断回调函数
 */
static int __pit_timing_callback_set (void     *p_drv,
                                      uint8_t   chan,
                                      void    (*pfn_callback)(void *),
                                      void     *p_arg)
{
    amdr_pit_timing_dev_t *p_dev    = (amdr_pit_timing_dev_t *)p_drv;
    amhw_pit_t            *p_hw_pit =  p_dev->p_devinfo->p_hw_pit;


    /* 不需要开启中断 */
    if (pfn_callback == NULL) {
        amhw_pit_timer_ctrl_clear(p_hw_pit,
                                  AMHW_PIT_CH(chan),
                                  AMHW_PIT_TCTL_TI_EN);
        return AM_OK;

    }
    if(chan == 0) {
        p_dev->pfn_ch1_callback = pfn_callback;
        p_dev->p_arg_ch1   = p_arg;
        if (p_dev->p_devinfo->flag == AMDR_PIT_TIMING_2_32BIT) {
            amhw_pit_timer_ctrl_set(p_hw_pit,
                                    AMHW_PIT_CH(chan),
                                    AMHW_PIT_TCTL_TI_EN);
        }
    } 
    
    if(chan == 1) {
        p_dev->pfn_ch2_callback = pfn_callback;
        p_dev->p_arg_ch2   = p_arg;
        if (p_dev->p_devinfo->flag == AMDR_PIT_TIMING_2_32BIT) {
            amhw_pit_timer_ctrl_set(p_hw_pit,
                                    AMHW_PIT_CH(chan),
                                    AMHW_PIT_TCTL_TI_EN);
        }
    }

    return AM_OK;
}

/**
 * \brief 64位定时器计数值获取
 */
static int __pit_timing_count_get64 (void     *p_drv,
                                     uint8_t   chan,
                                     uint64_t *p_count)
{
    amdr_pit_timing_dev_t *p_dev    = (amdr_pit_timing_dev_t *)p_drv;
    amhw_pit_t            *p_hw_pit =  p_dev->p_devinfo->p_hw_pit;

    *p_count = amhw_pit_ltmr64h_get(p_hw_pit);             /* 必须先获取高32位 */
    *p_count = (*p_count << 32) | amhw_pit_ltmr64l_get(p_hw_pit);

    return AM_OK;
}

static int __pit_timing_rollover_get64 (void     *p_drv,
                                        uint8_t   chan,
                                        uint64_t *p_rollover)
{
    amdr_pit_timing_dev_t *p_dev    = (amdr_pit_timing_dev_t *)p_drv;
    amhw_pit_t            *p_hw_pit =  p_dev->p_devinfo->p_hw_pit;

    *p_rollover = (~(uint64_t)0) / amdr_clk_periph_rate_get(p_hw_pit);

    return 0;
}

static int __pit_timing_enable64 (void    *p_drv,
                                  uint8_t  chan,
                                  uint64_t max_timer_count)
{
    amdr_pit_timing_dev_t *p_dev;
    amhw_pit_t            *p_hw_pit;

    uint32_t high_val, low_val;

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    p_dev    = (amdr_pit_timing_dev_t *)p_drv;
    p_hw_pit =  p_dev->p_devinfo->p_hw_pit;

    high_val = max_timer_count >> 32;
    low_val  = max_timer_count / (high_val + 1) - 1;

    amhw_pit_module_enable(p_hw_pit);
    amhw_pit_ldval_set(p_hw_pit, 1, high_val);
    /* 使能连接模式，使能中断，开始计数 */
    amhw_pit_timer_ctrl_set(p_hw_pit, 1, AMHW_PIT_TCTL_MASK);

    amhw_pit_ldval_set(p_hw_pit, 0, low_val);
    
    amhw_pit_timer_ctrl_clear(p_hw_pit,
                              0,
                              AMHW_PIT_TCTL_TI_EN | AMHW_PIT_TCTL_CHAIN_MODE);
    amhw_pit_timer_ctrl_set(p_hw_pit, 0, AMHW_PIT_TCTL_TMR_EN); /* 不能开启中断 */

    return AM_OK;
}

/**
 * \brief PIT初始化
 */
am_timer_handle_t amdr_pit_timing_init (amdr_pit_timing_dev_t           *p_dev,
                                        const amdr_pit_timing_devinfo_t *p_devinfo)
{
    if (p_devinfo == NULL || p_devinfo->p_hw_pit == NULL) {
        return NULL;
    }

    p_dev->p_devinfo          = p_devinfo;
    p_dev->timer_serv.p_funcs = (struct am_timer_drv_funcs *)&__g_pit_timing_drv_funcs;
    p_dev->timer_serv.p_drv   = p_dev;

    p_dev->pfn_ch1_callback = NULL;
    p_dev->p_arg_ch1        = NULL;
    
    p_dev->pfn_ch2_callback = NULL;
    p_dev->p_arg_ch1        = NULL;

    if (p_devinfo->flag == AMDR_PIT_TIMING_2_32BIT) {

    } else {

    }

    return &(p_dev->timer_serv);
}

/**
 * \brief PIT解初始化
 */
void amdr_pit_timing_deinit (amdr_pit_timing_dev_t *p_dev)
{
    amhw_pit_t *p_hw_pit = p_dev->p_devinfo->p_hw_pit;

    /* 关闭PIT模块 */
    amhw_pit_module_disable(p_hw_pit);

    am_int_disable(p_dev->p_devinfo->inum);
}

/* end of file */
