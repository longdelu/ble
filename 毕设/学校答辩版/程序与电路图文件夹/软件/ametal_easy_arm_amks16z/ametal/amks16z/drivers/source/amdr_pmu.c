/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
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
 * \brief PMU drivers implementation
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-10-29  liqing, first implementation.
 * \endinternal
 */

#include "am_common.h"
#include "am_int.h"
#include "amhw_int.h"
#include "amks16z_inum.h"
#include "amhw_amks16z.h"
#include "amdr_clk.h"
#include "amdr_pmu.h"

/******************************************************************************
 * 其他模块的操作                                                             *
 ******************************************************************************/

/* 关闭检测OSC时钟是否丢失 */
#define __PMU_CLOCK_MONITOR_CLOSE()    

/* 开启检测OSC时钟是否丢失 */
#define __PMU_CLOCK_MONITOR_OPEN()    
              

/* 进入VLPx模式前时钟配置（PEE->BLPE 或者 FEI->BLPI） */
#define __PMU_CLOCK_VLPX_MODE_INTO()                             \
{                                                                \
    if (amdr_clk_mode_get() == AMDR_CLK_MODE_PEE) {          \
        amdr_clk_mode_pee2blpe();                              \
    } else if (amdr_clk_mode_get() == AMDR_CLK_MODE_FEI) {   \
        amdr_clk_mode_fei2blpi();                              \
    }                                                            \
    __PMU_CLOCK_MONITOR_CLOSE();                                 \
    amhw_mcg_irc_src_set(AMHW_MCG_IRC_SRC_FAST);                 \
}

/* 退出VLPx模式后时钟配置（BLPE->PEE 或者 BLPI->FEI） */
#define __PMU_CLOCK_VLPX_MODE_EXIT()                             \
{                                                                \
    if (amdr_clk_mode_get() == AMDR_CLK_MODE_BLPE) {         \
        amdr_clk_mode_blpe2pee();                              \
        __PMU_CLOCK_MONITOR_OPEN();                              \
    } else if (amdr_clk_mode_get() == AMDR_CLK_MODE_BLPI) {  \
        amdr_clk_mode_blpi2fei();                              \
    }                                                            \
}

/* 内核睡眠函数 */
void __pmu_cpu_wif(bool_t deep)
{
    if (deep) {
        AMHW_INT->scr |= 0x04;
    } else {
        AMHW_INT->scr &= ~0x04;
    }
    
    AMHW_INT->scr &= ~0x02;

#ifndef __CC_ARM
    asm("WFI");
#else
    __wfi();
#endif
}


/******************************************************************************
 * 内部宏定义，全局变量定义，静态函数声明
 ******************************************************************************/
#define __PMU_P_SMC    __gp_pmu_dev->p_devinfo->p_hw_smc
#define __PMU_P_PMC    __gp_pmu_dev->p_devinfo->p_hw_pmc
#define __PMU_P_RCM    __gp_pmu_dev->p_devinfo->p_hw_rcm
#define __PMU_P_LLWU   __gp_pmu_dev->p_devinfo->p_hw_llwu
#define __PMU_P_MCM    __gp_pmu_dev->p_devinfo->p_hw_mcm

static amdr_pmu_dev_t *__gp_pmu_dev = NULL;  /* 单一全局设备指针        */

void __pmu_lvw_irq_handle(void*);            /* 掉电报警回调函数        */
                                             
void __pmu_vlpr_exit(void);                  /* 退出VLPR模式            */
void __pmu_vlpr_into(void);                  /* 进入VLPR模式            */
void __pmu_wait_into(void);                  /* 进入WAIT模式            */
void __pmu_stop_into(uint8_t level);         /* 进入STOP(level)模式     */
void __pmu_vlps_into(void);                  /* 进入VLPS模式            */
void __pmu_lls_into(void);                   /* 进入LLS模式             */
void __pmu_vlls_into(uint8_t level);         /* 进入VLLSx模式           */


/******************************************************************************
 * 以下为函数实现
 ******************************************************************************/

 /**
 * \brief 掉电报警回调函数
 */
void __pmu_lvw_irq_handle(void *p_arg)
{
        
    /* 清除报警标志 */
    if (amhw_pmc_int_flags_get(__PMU_P_PMC) & AMHW_PMC_FLAG_LVWF) {
        amhw_pmc_ack_set(__PMU_P_PMC, AMHW_PMC_ACK_LVW);
    }

    if (__gp_pmu_dev->p_warn_func != NULL) {
        __gp_pmu_dev->p_warn_func(__gp_pmu_dev->p_warn_arg);
    }
}

 /**
 * \brief LLWU唤醒中断函数
 */
void amdr_pmu_llwu_irq_handle(void)
{
    amhw_llwu_wuf_t llwu_flag = (amhw_llwu_wuf_t)amhw_llwu_wuflags_get(AMHW_LLWU);

    /* 清除引脚唤醒标志，外设模块有对应的模块清除 */
    amhw_llwu_wuflags_clr(AMHW_LLWU, llwu_flag);

    /* 关闭LLWU中断 */
    amhw_int_disable(AMHW_INT, INUM_LLWU);
}

/**
 * \brief PMU 初始化
 */
int amdr_pmu_init (amdr_pmu_dev_t *p_dev, const amdr_pmu_devinfo_t *p_devinfo)
{
    amhw_smc_t  *p_hw_smc;
    amhw_pmc_t  *p_hw_pmc;
    amhw_rcm_t  *p_hw_rcm;
    amhw_llwu_t *p_hw_llwu;

    /* 复位引脚滤波周期 */
    uint8_t filt_count = AM_BITS_GET(p_devinfo->flags, 1, 5);

    __gp_pmu_dev     = p_dev;
    p_dev->p_devinfo = p_devinfo;
    
    p_hw_smc         = p_devinfo->p_hw_smc;
    p_hw_pmc         = p_devinfo->p_hw_pmc;
    p_hw_rcm         = p_devinfo->p_hw_rcm;
    p_hw_llwu        = p_devinfo->p_hw_llwu;

    if (p_hw_smc  == NULL  ||
        p_hw_pmc  == NULL  ||
        p_hw_rcm  == NULL  ||
        p_hw_llwu == NULL  ||
        filt_count > 32) {
        return -AM_EINVAL;
    }

    p_dev->p_warn_func = NULL;

    /* 停止模式复位引脚滤波使能 */
    if (p_devinfo->flags & AMDR_PMU_ARG_REST_PIN_STOP_FILT_ENABLE) {
        amhw_rcm_respin_filt_stop_cfg(p_hw_rcm, AMHW_RCM_RESPIN_FILT_STOP_LPO);
    }

    /* 运行模式复位引脚滤波配置 */
    if ( filt_count == 0) {
        amhw_rcm_respin_filt_runw_cfg(p_hw_rcm, AMHW_RCM_RESPIN_FILT_RUNW_DISABLE);
    } else {
        amhw_rcm_respin_filt_runw_cfg(p_hw_rcm, AMHW_RCM_RESPIN_FILT_RUNW_BUS);
        amhw_rcm_respin_filt_busc_cfg(p_hw_rcm, filt_count & 0x1F);
    }

    /* 使能VLPx模式能带隙 */
    if (p_devinfo->flags & AMDR_PMU_ARG_VLPx_BANDGAP_ENABLE) {
        amhw_pmc_bandgap_enable(p_hw_pmc);
    } else {
        amhw_pmc_bandgap_disable(p_hw_pmc);
    }

    /* 使能能带隙缓冲 */
    if (p_devinfo->flags & AMDR_PMU_ARG_BANDGAP_BUF_ENABLE) {
        amhw_pmc_bandgap_buf_enable(p_hw_pmc);
    } else {
        amhw_pmc_bandgap_buf_disable(p_hw_pmc);
    }

    /* VLSS0模式禁能电源检测电路 */
    if (p_devinfo->flags & AMDR_PMU_ARG_VLLS0_POR_DISABLE) {
        amhw_smc_vlls0_por_disable(p_hw_smc);
    } else {
        amhw_smc_vlls0_por_enable(p_hw_smc);
    }

    /* 清除唤醒复位标志，释放IO引脚 */
    if ((p_devinfo->flags & AMDR_PMU_ARG_VLLSx_ISOACK_CLEAR) &&
        (p_hw_pmc->regsc & 0x08)) {
        amhw_pmc_ack_set(p_hw_pmc, AMHW_PMC_ACK_VLLS);
    }

    amhw_pmc_int_disable(p_hw_pmc, AMHW_PMC_IRQ_LVW | AMHW_PMC_IRQ_LVD);
    amhw_pmc_lvdrest_disable(p_hw_pmc);

    /* 可以进入所有系统模式 */
    amhw_smc_mode_allow_cfg(p_hw_smc, AMHW_SMC_PROTECT_AVLP_ALLOW |
                                      AMHW_SMC_PROTECT_ALLS_ALLOW |
                                      AMHW_SMC_PROTECT_AVLLS_ALLOW);

    return AM_OK;
}


/**
 * \brief 解除PMU初始化
 */
void amdr_pmu_deinit (amdr_pmu_dev_t *p_dev)
{
    __gp_pmu_dev       = NULL;
    p_dev->p_warn_func = NULL;

    am_int_disable(INUM_PMC);
    am_int_disconnect(INUM_PMC, __pmu_lvw_irq_handle, (void*)0);
}


/**
 * \brief 配置系统模式
 */
int amdr_pmu_mode_into (amdr_pmu_mode_t mode)
{
    amdr_pmu_mode_t cur_mode = amdr_pmu_mode_get();

    if (cur_mode == mode) {
        return AM_OK;
    }

    switch (mode) {

    case AMDR_PMU_MODE_RUN:    /* 运行模式 */
        if (cur_mode == AMDR_PMU_MODE_VLPR) {
            __pmu_vlpr_exit();
            __PMU_CLOCK_VLPX_MODE_EXIT();

            if (amdr_pmu_mode_get() != AMDR_PMU_MODE_RUN) {
                return AM_ERROR;
            }
        }

        break;

    case AMDR_PMU_MODE_VLPR:  /* 超低功耗运行模式 */
        if (cur_mode == AMDR_PMU_MODE_RUN) {
            __PMU_CLOCK_VLPX_MODE_INTO();
            __pmu_vlpr_into();
            if (amdr_pmu_mode_get() != AMDR_PMU_MODE_VLPR) {
                __PMU_CLOCK_VLPX_MODE_EXIT();
                return AM_ERROR;
            }
        }

        break;

    case AMDR_PMU_MODE_WAIT:  /* 等待模式 */
        if (cur_mode != AMDR_PMU_MODE_RUN) {
            return -AM_EPERM;
        }
        __pmu_wait_into();
        break;

    case AMDR_PMU_MODE_VLPW:  /* 超低功耗等待模式 */
        if (cur_mode != AMDR_PMU_MODE_VLPR) {
            return -AM_EPERM;
        }
        __pmu_wait_into();
        
        break;

    case AMDR_PMU_MODE_STOP2: /* 停止模式2 */
        if (cur_mode != AMDR_PMU_MODE_RUN) {
            return -AM_EPERM;
        }

        __PMU_CLOCK_MONITOR_CLOSE();
        __pmu_stop_into(2);
        __PMU_CLOCK_MONITOR_OPEN();

        break;

    case AMDR_PMU_MODE_STOP1: /* 停止模式1 */
        if (cur_mode != AMDR_PMU_MODE_RUN) {
            return -AM_EPERM;
        }

        __PMU_CLOCK_MONITOR_CLOSE();
        __pmu_stop_into(1);
        __PMU_CLOCK_MONITOR_OPEN();
        
        break;

    case AMDR_PMU_MODE_STOP:  /* 正常停止模式 */
        if (cur_mode != AMDR_PMU_MODE_RUN) {
            return -AM_EPERM;
        }
        __PMU_CLOCK_MONITOR_CLOSE();
        __pmu_stop_into(0);
        __PMU_CLOCK_MONITOR_OPEN();
        
        break;

    case AMDR_PMU_MODE_VLPS:  /* 超低功耗停止模式 */
        //__PMU_CLOCK_MONITOR_CLOSE();
        __pmu_vlps_into();
        //__PMU_CLOCK_MONITOR_OPEN();
        
        break;

    case AMDR_PMU_MODE_LLS:   /* 低漏电停止模式 */
        am_int_enable(INUM_LLWU); 
        
       __PMU_CLOCK_MONITOR_CLOSE();
        __pmu_lls_into();
        __PMU_CLOCK_MONITOR_OPEN();
        
        break;

    case AMDR_PMU_MODE_VLLS3: /* 超低漏电停止模式3 */
        __PMU_CLOCK_MONITOR_CLOSE();
        __pmu_vlls_into(3);
        
        break;

    case AMDR_PMU_MODE_VLLS1: /* 超低漏电停止模式1 */
        __PMU_CLOCK_MONITOR_CLOSE();
        __pmu_vlls_into(1);
        
        break;

    case AMDR_PMU_MODE_VLLS0: /* 超低漏电停止模式0 */
        __PMU_CLOCK_MONITOR_CLOSE();
        __pmu_vlls_into(0);
        
        break;

    default:
        break;
    }

    return AM_OK;
}

/**
 * \brief 获取系统模式
 */
amdr_pmu_mode_t amdr_pmu_mode_get (void)
{
    /* 低功耗运行模式 */
    if (amhw_smc_mode_get(__PMU_P_SMC) == AMHW_SMC_MODE_VLPR) {
        return AMDR_PMU_MODE_VLPR;

    /* 运行模式 */
    } else {
        return AMDR_PMU_MODE_RUN;
    }
}

/**
 * \brief 配置掉电检测信息
 */
int amdr_pmu_brownout_cfg (amdr_pmu_brownout_info_t *p_info)
{
    amdr_pmu_mode_t cur_mode = amdr_pmu_mode_get();

    if(p_info == NULL) {
        return -AM_EINVAL;
    }

    /* 低功耗模式不支持掉电检测 */
    if (cur_mode == AMDR_PMU_MODE_VLPR) {
        return AM_ERROR;
    }

    amhw_pmc_lvw_cfg(__PMU_P_PMC, (amhw_pmc_lvw_sel_t)p_info->warn_v);
    amhw_pmc_lvd_cfg(__PMU_P_PMC, (amhw_pmc_lvd_sel_t)p_info->rest_v);

    if (p_info->enable) {

        amhw_pmc_int_enable(__PMU_P_PMC, AMHW_PMC_IRQ_LVW);
        amhw_pmc_lvdrest_enable(__PMU_P_PMC);


        if (__gp_pmu_dev->p_warn_func != NULL) {
            am_int_disable(INUM_PMC);
            am_int_disconnect(INUM_PMC, __gp_pmu_dev->p_warn_func, (void*)0);
        }

        am_int_connect(INUM_PMC, p_info->p_callfunc, (void*)0);

        __gp_pmu_dev->p_warn_func = p_info->p_callfunc;
        __gp_pmu_dev->p_warn_arg  = p_info->p_arg;

        am_int_enable(INUM_PMC);

    } else {
        amhw_pmc_int_disable(__PMU_P_PMC, AMHW_PMC_IRQ_LVW);
        amhw_pmc_lvdrest_disable(__PMU_P_PMC);

        am_int_disable(INUM_PMC);
        am_int_disconnect(INUM_PMC, __gp_pmu_dev->p_warn_func, (void*)0);

        __gp_pmu_dev->p_warn_func =NULL;
    }

    return AM_OK;
}

/**
 * \brief 配置唤醒源
 */
int amdr_pmu_llwu_src_cfg (amdr_pmu_llwu_src_info_t *p_info)
{
    uint8_t val;
    uint8_t src;
    if (p_info == NULL) {
        return -AM_EINVAL;
    }

    src = AM_BITS_GET(p_info->src, 0, 24);
    val = AM_BITS_GET(p_info->extra_info, 24, 2);
    
    switch (val) {
    case 00: /* 配置唤醒引脚 */
        amhw_llwu_pin_cfg(__PMU_P_LLWU,
                             (amhw_llwu_pin_t)src,
                             (amhw_llwu_pin_arg_t)p_info->extra_info);
        break;

    case 01: /* 配置唤醒模块 */
        amhw_llwu_module_cfg(__PMU_P_LLWU,
                            (amhw_llwu_module_t)src,
                            (amhw_llwu_module_arg_t)p_info->extra_info);
        break;

    case 02: /* 配置唤醒滤波通道 */
        amhw_llwu_filt_cfg(__PMU_P_LLWU,
                           (amhw_llwu_filt_chan_t)src,
                           (amhw_llwu_pin_t)(p_info->extra_info & 0xFFFF),
                           (amhw_llwu_filt_chan_arg_t)(p_info->extra_info >> 16));
        break;

    default:
        return -AM_EINVAL;
    }

    return AM_OK;
}

/**
 * \brief 退出低功耗运行模式
 */
void __pmu_vlpr_exit()
{
    uint8_t i;

    amhw_smc_run_mdoe_sel(__PMU_P_SMC, AMHW_SMC_RUNMODE_RUN);

    /* 等待调整器过渡完成 */
    for (i = 0; i < 0xff; i++) {
        if (amdr_pmu_mode_get() == AMDR_PMU_MODE_RUN) {
            if (amhw_pmc_regulator_wstat_get(__PMU_P_PMC) == TRUE) {
                break;
            }
        }
    }
}

/**
 * \brief 进入低功耗运行模式
 */
void __pmu_vlpr_into()
{
    uint32_t i;
    
    amhw_smc_run_mdoe_sel(__PMU_P_SMC, AMHW_SMC_RUNMODE_VLPR);

    /* 等待调整器过渡完成(调整器变0) */
    for (i=0; i < 10000; i++) {
        if (amdr_pmu_mode_get() == AMDR_PMU_MODE_RUN) {
            if (amhw_pmc_regulator_wstat_get(__PMU_P_PMC) == FALSE) { //?
                break;
            }
        }
    }
}

/**
 * \brief 进入等待模式
 */
void __pmu_wait_into()
{
    __pmu_cpu_wif(FALSE);
}

/**
 * \brief 进入停止模式
 */
void __pmu_stop_into(uint8_t level)
{
    switch (level) {
    case 2:
    case 1:
    case 0:
        amhw_smc_stop_mdoe_sel(__PMU_P_SMC, AM_SBF(0, 8) | AM_SBF(level, 6));
        break;
    default:
        return;
    }

    /* 读取寄存器，保证进入停止之前数据写入 */
    amhw_smc_stop_mdoe_isabort(__PMU_P_SMC);

    __pmu_cpu_wif(TRUE);
}

void __pmu_vlps_into()
{
    amhw_smc_stop_mdoe_sel(__PMU_P_SMC, AMHW_SMC_STOPMODE_VLPS);

    /* 读取寄存器，保证进入停止之前数据写入 */
    amhw_smc_stop_mdoe_isabort(__PMU_P_SMC);

    __pmu_cpu_wif(TRUE);
}

/**
 * \brief 进入低漏电停止模式
 */
void __pmu_lls_into()
{
    /* 清除唤醒标志 */
    amhw_llwu_wuflags_clr(__PMU_P_LLWU, AMHW_LLWU_WUF_P5_PTB0  |
                                        AMHW_LLWU_WUF_P6_PTC1  |
                                        AMHW_LLWU_WUF_P7_PTC3  |
                                        AMHW_LLWU_WUF_P8_PTC4  |
                                        AMHW_LLWU_WUF_P9_PTC5  |
                                        AMHW_LLWU_WUF_P10_PTC6 |
                                        AMHW_LLWU_WUF_P14_PTD4 |
                                        AMHW_LLWU_WUF_P15_PTD6 |
                                        AMHW_LLWU_WUF_F1       |
                                        AMHW_LLWU_WUF_F2);
    
    amhw_smc_stop_mdoe_sel(__PMU_P_SMC, AMHW_SMC_STOPMODE_LLS);

    /* 读取寄存器，保证进入停止之前数据写入 */
    amhw_smc_stop_mdoe_isabort(__PMU_P_SMC);

    __pmu_cpu_wif(TRUE);
}

/**
 * \brief 进入超低漏电停止模式
 */
void __pmu_vlls_into(uint8_t level)
{
    switch (level) {
    case 3:
    case 1:
    case 0:
        /* 清除唤醒标志 */
        amhw_llwu_wuflags_clr(__PMU_P_LLWU, AMHW_LLWU_WUF_P5_PTB0  |
                                            AMHW_LLWU_WUF_P6_PTC1  |
                                            AMHW_LLWU_WUF_P7_PTC3  |
                                            AMHW_LLWU_WUF_P8_PTC4  |
                                            AMHW_LLWU_WUF_P9_PTC5  |
                                            AMHW_LLWU_WUF_P10_PTC6 |
                                            AMHW_LLWU_WUF_P14_PTD4 |
                                            AMHW_LLWU_WUF_P15_PTD6 |
                                            AMHW_LLWU_WUF_F1       |
                                            AMHW_LLWU_WUF_F2);
    
        amhw_smc_stop_mdoe_sel(__PMU_P_SMC, AM_SBF(4, 8) | AM_SBF(level, 0));
        break;
    default:
        return;
    }

    /* 读取寄存器，保证进入停止之前数据写入 */
    amhw_smc_stop_mdoe_isabort(__PMU_P_SMC);

    __pmu_cpu_wif(TRUE);

}

/* end of file */
