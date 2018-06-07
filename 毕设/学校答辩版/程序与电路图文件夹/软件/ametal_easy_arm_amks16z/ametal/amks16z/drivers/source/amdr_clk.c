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
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief CLK implementation
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-12-01  lnk, first implementation.
 * \endinternal
 */
 
#include "ametal.h"
#include "am_clk.h"
#include "amks16z_clk_id.h"
#include "amhw_mcg.h"
#include "amhw_sim.h"
#include "amhw_lptmr.h"
#include "amhw_amks16z.h"

#include "amdr_clk.h"


/**
 * \name 频率值定义
 * @{
 */

#define __AMDR_CLK_RATE_LPO         1000UL       /**< \brief PMC输出LPO频率        */
#define __AMDR_CLK_RATE_IRC_SLOW    32768UL      /**< \brief 内部慢速IRC频率       */
#define __AMDR_CLK_RATE_IRC_FAST    4000000UL    /**< \brief 内部快速IRC频率       */
#define __AMDR_CLK_RATE_RTC_CLKIN   32768UL      /**< \brief RTC外部时钟输入必须使用32768UL */
/** @} */


/******************************************************************************
  全局变量
******************************************************************************/

/** \brief 指向GPIO设备的指针 */
amdr_clk_dev_t *__gp_clk_dev;

/**
 * \brief 得到MCGIRCLK时钟频率
 * \return MCGIRCLK时钟频率（单位：Hz）
 */
uint32_t amdr_clk_mcgirclk_rate_get (void)
{
    uint32_t fcrdiv = 0;
    uint32_t clk = 0;

    if (amhw_mcg_irc_src_get() == AMHW_MCG_IRC_SRC_SLOW) {
        /** \brief  内部慢速时钟 */
        clk = __AMDR_CLK_RATE_IRC_SLOW;
    } else {
        /** \brief  内部快速时钟 */
        fcrdiv = amhw_mcg_irc_fast_div_get();
        clk = __AMDR_CLK_RATE_IRC_FAST / (1UL << fcrdiv);
    }

    return clk;
}

/**
 * \brief 得到OSCCLK/OSCERCLK时钟频率
 * \return OSCCLK/OSCERCLK时钟频率（单位：Hz）
 */
uint32_t amdr_clk_oscclk_rate_get (void)
{
    uint32_t clk = 0;

    if (amhw_mcg_osc_src_get() == AMHW_MCG_OSC_SRC_OSC) {
        /** \brief  晶振时钟输入 */
        clk = __gp_clk_dev->p_devinfo->xtal_rate;
    } else {
        /** \brief  外部时钟引脚输入 */
        clk = __gp_clk_dev->p_devinfo->xtal_rate;
    }

    return clk;
}

/**
 * \brief 得到MCGFLLCLK时钟频率
 * \return MCGFLLCLK时钟频率（单位：Hz）
 */
uint32_t amdr_clk_mcgfllclk_rate_get (void)
{
    uint32_t frdiv  = 0;
    uint32_t dcodiv = 0;
    uint32_t clk = 0;

    switch (amhw_mcg_fll_dco_div_get()) {

    case AMHW_MCG_DOC_DIV_640:  /**< \brief ref:31-39KHZ,out:20-25MHZ */
        dcodiv = 640;
        break;
    case AMHW_MCG_DOC_DIV_732:  /**< \brief ref:32KHZ,out:24MHZ */
        dcodiv = 732;
        break;
    case AMHW_MCG_DOC_DIV_1280: /**< \brief ref:31-39KHZ,out:40-50MHZ  */
        dcodiv = 1280;
        break;
    case AMHW_MCG_DOC_DIV_1464: /**< \brief ref:32KHZ out:48MHZ */
        dcodiv = 1464;
        break;
    case AMHW_MCG_DOC_DIV_1920: /**< \brief ref:31-39KHZ,out:60-75MHZ */
        dcodiv = 1920;
        break;
    case AMHW_MCG_DOC_DIV_2197: /**< \brief ref:32.768KHZ,out:72MHZ */
        dcodiv = 2197;
        break;
    case AMHW_MCG_DOC_DIV_2560: /**< \brief ref:31-39KHZ,out:80-100MHZ */
        dcodiv = 2560;
        break;
    case AMHW_MCG_DOC_DIV_2929: /**< \brief ref:32KHZ,out:96MHZ  */
        dcodiv = 2929;
        break;
    default : break;
    }

    if (amhw_mcg_fll_src_get() == AMHW_MCG_FLL_SRC_IRC_SLOW) {
        /** \brief  内部慢速时钟做FLL时钟源 */
        clk = __AMDR_CLK_RATE_IRC_SLOW * dcodiv;
    } else {
        /** \brief  外部时钟做FLL时钟源 */
        frdiv = amhw_mcg_fll_erc_div_get();

        if (amhw_mcg_osc_range_get() == AMHW_MCG_OSC_RANGE_LOW) {
            clk   = amdr_clk_oscclk_rate_get() / (1UL << frdiv) * dcodiv;
        } else {
            clk = amdr_clk_oscclk_rate_get() / (32UL << frdiv) * dcodiv;
        }
    }

    return clk;
}

/**
 * \brief 得到MCGPLLCLK时钟频率
 * \return MCGPLLCLK时钟频率（单位：Hz）
 */
uint32_t amdr_clk_mcgpllclk_rate_get (void)
{
    uint32_t prdiv  = 0;
    uint32_t vcodiv = 0;
    uint32_t clk    = 0;

    prdiv  = amhw_mcg_pll_erc_div_get();
    vcodiv = amhw_mcg_pll_vco_div_get();

    clk = amdr_clk_oscclk_rate_get() / (prdiv + 1) * (vcodiv + 24);

    return clk;
}

/**
 * \brief 得到MCGOUTCLK时钟频率
 * \return MCGOUTCLK时钟频率（单位：Hz）
 */
uint32_t amdr_clk_mcgoutclk_rate_get (void)
{
    uint32_t clk = 0;

    if (amhw_mcg_outclk_src_get() == AMHW_MCG_OUTCLK_SRC_IRC) {
        /** \brief  内部IRC时钟作MCGOUTCLK时钟源 */
        clk = amdr_clk_mcgirclk_rate_get();
    } else if (amhw_mcg_outclk_src_get() == AMHW_MCG_OUTCLK_SRC_ERC) {
        /** \brief  外部OSC时钟作MCGOUTCLK时钟源 */
        clk = amdr_clk_oscclk_rate_get();
    } else {
        /** \brief  MCGPLLCLK/MCGFLLCLK作MCGOUTCLK时钟源 */
        if (amhw_mcg_plls_src_get() == AMHW_MCG_PLLS_SRC_FLL) {
            clk = amdr_clk_mcgfllclk_rate_get();
        } else {
            clk = amdr_clk_mcgpllclk_rate_get();
        }
    }

    return clk;
}

/**
 * \brief 得到ERCLK32K时钟频率
 * \return ERCLK32K时钟频率（单位：Hz）
 */
uint32_t amdr_clk_erclk32k_rate_get (void)
{
    uint32_t clk;

    if (amhw_sim_ercclk32k_src_get() == AMHW_SIM_OSC32KSEL_CLKIN) {
        /** \brief  外部RTC_CLKIN输入作ERCLK32K的时钟源 */
        clk = __AMDR_CLK_RATE_RTC_CLKIN;
    } else if (amhw_sim_ercclk32k_src_get() == AMHW_SIM_OSC32KSEL_LPO) {
        /** \brief  LPO输出时钟作ERCLK32K的时钟源 */
        clk = __AMDR_CLK_RATE_LPO;
    } else {
        /** \brief  OSCCLK32K时钟作ERCLK32K的时钟源 */
        if (amhw_mcg_osc_range_get() == AMHW_MCG_OSC_RANGE_LOW &&
            amhw_mcg_osc_gain_get() == AMHW_MCG_OSC_GAIN_LOW) {
            /** \brief  仅当RANGE=0,HCG0 = 0时，OSCCLK32K有效 */
            clk = amdr_clk_oscclk_rate_get();
        }
    }

    return clk;
}

/**
 * \brief 得到RTC_CLKOUT时钟频率
 * \return RTC_CLKOUT时钟频率（单位：Hz）
 */
uint32_t amdr_clk_rtc_clkout_rate_get (void)
{
    uint32_t clk = 0;

    if (amhw_sim_rtc_clkout_get() == AMHW_SIM_RTC_CLKOUT_OSCERCLK) {
        /** \brief  OSCERCLK作RTC_OUT时钟源 */
        clk = amdr_clk_oscclk_rate_get();
    } else {
        /** \brief  RTC OUT 1HZ */
        clk = 1UL;
    }

    return clk;
}


/**
 * \brief 得到core/platform/system时钟频率
 * \return core/platform/system时钟频率
 */
uint32_t amdr_clk_core_clkrate_get(void)
{
    uint32_t outdiv1 = 0;
    uint32_t clk     = 0;

    outdiv1 = amhw_sim_clkdiv1_outdiv1_get();
    clk = amdr_clk_mcgoutclk_rate_get() / (1 + outdiv1);

    return clk;
}

/**
 * \brief 得到bus/flash时钟频率
 * \return bus/flash时钟频率
 */
uint32_t amdr_clk_bus_clkrate_get(void)
{
    uint32_t outdiv4 = 0;
    uint32_t clk     = 0;

    outdiv4 = amhw_sim_clkdiv1_outdiv4_get();
    
    clk = amdr_clk_core_clkrate_get() / (1 + outdiv4);

    return clk;
}

/**
 * \brief 得到TPM的时钟频率
 * \return  TPM的时钟频率（单位：Hz）
 */
uint32_t amdr_clk_tpm_rate_get (void)
{
    uint32_t clk = 0;

    if (amhw_sim_tpm_src_get() == AMHW_SIM_TPMSRC_DISABLED) {
        /** \brief TPM时钟禁能 */
    } else if (amhw_sim_tpm_src_get() == AMHW_SIM_TPMSRC_MCGIRCLK) {
        /** \brief  MCGIRCLK作TPM时钟源 */
        clk = amdr_clk_mcgirclk_rate_get();
    } else if (amhw_sim_tpm_src_get() == AMHW_SIM_TPMSRC_OSCERCLK) {
        /** \brief  OSCERCLK作TPM时钟源 */
        clk = amdr_clk_oscclk_rate_get();
    } else {
        /** \brief  MCGPLLCLK/MCGFLLCLK作TPM时钟源 */
        if (amhw_sim_pllfll_sel_get() == AMHW_SIM_PLLFLLSEL_FLL) {
            clk = amdr_clk_mcgfllclk_rate_get();
        } else {
            clk = amdr_clk_mcgpllclk_rate_get() / 2;
        }
    }

    return clk;
}

/**
 * \brief 得到RTC的时钟频率
 * \return  RTC的时钟频率（单位：Hz）
 */
uint32_t amdr_clk_rtc_rate_get (void)
{
    return amdr_clk_erclk32k_rate_get();
}

/**
 * \brief 得到UART0的时钟频率
 * \return  UART0的时钟频率（单位：Hz）
 */
uint32_t amdr_clk_uart0_rate_get (void)
{
    uint32_t clk = 0;

    if (amhw_sim_uart0_src_get() == AMHW_SIM_UART0SRC_DISABLED) {
        /** \brief TPM时钟禁能 */
    } else if (amhw_sim_uart0_src_get() == AMHW_SIM_UART0SRC_MCGIRCLK) {
        /** \brief  MCGIRCLK作TPM时钟源 */
        clk = amdr_clk_mcgirclk_rate_get();
    } else if (amhw_sim_uart0_src_get() == AMHW_SIM_UART0SRC_OSCERCLK) {
        /** \brief  OSCERCLK作TPM时钟源 */
        clk = amdr_clk_oscclk_rate_get();
    } else {
        /** \brief  MCGPLLCLK/MCGFLLCLK作TPM时钟源 */
        if (amhw_sim_pllfll_sel_get() == AMHW_SIM_PLLFLLSEL_FLL) {
            clk = amdr_clk_mcgfllclk_rate_get();
        } else {
            clk = amdr_clk_mcgpllclk_rate_get() / 2;
        }
    }

    return clk;
}

/**
 * \brief 得到wdt的时钟频率
 * \return  wdt的时钟频率（单位：Hz）
 */
uint32_t amdr_clk_wdt_rate_get (void)
{
    uint32_t clk = 0;
    if (amhw_sim_cop_src_get() == AMHW_SIM_COP_CLK_INTERNAL) {
        clk = __AMDR_CLK_RATE_LPO;
    } else {
        clk = amdr_clk_bus_clkrate_get();
    }

    return clk;
}

/**
 * \brief 获取lptmr时钟源大小
 * \param[in] p_hw_lptmr : 指向LPTMR寄存器块的指针
 * \return 时钟源大小
 */
uint32_t amdr_clk_lptmr_rate_get (void)
{
    uint32_t               clk;
    amhw_lptmr_clock_src_t lptmr_clk;

    lptmr_clk = (amhw_lptmr_clock_src_t)
                (amhw_lptmr_ps_reg_get(AMHW_LPTMR) & 0x3);

    switch (lptmr_clk) {

    case AMHW_LPTMR_CLOCK_SRC_MCGIRCLK:
        clk = amdr_clk_mcgirclk_rate_get();
        break;
    case AMHW_LPTMR_CLOCK_SRC_LPOCLK:
        clk = __AMDR_CLK_RATE_LPO;
        break;
    case AMHW_LPTMR_CLOCK_SRC_ER32KCLK:
        clk = amdr_clk_erclk32k_rate_get();
        break;
    case AMHW_LPTMR_CLOCK_SRC_OSCERCLK:
        clk = amdr_clk_oscclk_rate_get();
        break;
    default:
        clk = 0;
        break;
    }

    return clk;
}
/**
 * \brief 获取时钟频率
 */
int am_clk_rate_get (am_clk_id_t clk_id)
{
    uint32_t clk = 0;
    

    switch (clk_id) {
    
    case CLK_SLOW_IRC:
        clk = __AMDR_CLK_RATE_IRC_SLOW;
        break;

    case CLK_FAST_IRC:
        clk = __AMDR_CLK_RATE_IRC_FAST;
        break;

    case CLK_XTAL:
        clk = __gp_clk_dev->p_devinfo->xtal_rate;
        break;

    case CLK_CLKIN:
        clk = __gp_clk_dev->p_devinfo->xtal_rate;
        break;

    case CLK_CORE:
    case CLK_SYSTEM:
        clk = amdr_clk_core_clkrate_get();
        break;

    case CLK_BUS:
    case CLK_FLASH:
    case CLK_PERIPH:
        clk = amdr_clk_bus_clkrate_get();
        break;

    case CLK_SYSTIC:
        clk = amdr_clk_core_clkrate_get();
        break;

    case CLK_OSC32K:
        clk = amdr_clk_erclk32k_rate_get();
        break;

    case CLK_OSCERC:
        clk = amdr_clk_oscclk_rate_get();
        break;

    case CLK_RTCOUT:
        clk = amdr_clk_rtc_clkout_rate_get();
        break;

    case CLK_MCGFLL:
        clk = amdr_clk_mcgfllclk_rate_get();
        break;

    case CLK_MCGPLL:
        clk = amdr_clk_mcgpllclk_rate_get();
        break;

    case CLK_MCGEXT:
        clk = amdr_clk_oscclk_rate_get();
        break;

    case CLK_MCGIRC:
        clk = amdr_clk_mcgirclk_rate_get();
        break;

    case CLK_MCGOUT:
        clk = amdr_clk_mcgoutclk_rate_get();
        break;

    case CLK_LPO:
        clk = __AMDR_CLK_RATE_LPO;
        break;

    case CLK_WDT:
        clk = amdr_clk_wdt_rate_get();
        break;

#if 0
    /* Device clk */
    case CLK_TPM0:
    case CLK_TPM1:
    case CLK_TPM2:
        clk = amdr_clk_tpm_rate_get();
        break;

    case CLK_UART0:
        clk = amdr_clk_uart0_rate_get();
        break;

    case CLK_I2C1:
    case CLK_SPI1:
    case CLK_DMA:
        clk = amdr_clk_core_clkrate_get();
        break;

    case CLK_RTC:
        clk = amdr_clk_rtc_rate_get();
        break;

    case CLK_LPTMR:
        clk = amdr_clk_lptmr_rate_get();
        break;

    case CLK_PORTA:
    case CLK_PORTB:
    case CLK_PORTC:
    case CLK_PORTD:
    case CLK_PORTE:
    case CLK_UART1:
    case CLK_UART2:
    case CLK_SPI0:
    case CLK_I2C0:
    case CLK_CMP:
    case CLK_FTF:
    case CLK_DMAMUX:
    case CLK_PIT:
    case CLK_ADC0:
    case CLK_DAC0:
    case CLK_TSI:
        clk = amdr_clk_bus_clkrate_get();
        break;

    case CLK_I2S:
    /* I2S 时钟在I2S模块内部分频 */
        clk = 0;
        break;

#endif

    default : 
        clk = 0;
        break;  
    }
    
    return clk;
}

/**
 * \brief 使能指定的系统或外设时钟
 */
int am_clk_enable (am_clk_id_t clk_id)
{
    if ((clk_id < CLK_PERIPH_ID_MIN) ||
        (clk_id > CLK_PERIPH_ID_MAX)) {

        /* 时钟ID不在范围内*/
        return -AM_ENXIO;
    }
    
    amhw_sim_periph_clock_enable((amhw_sim_scgc_periph_t) (clk_id));
    
    return AM_OK;
}

/**
 * \brief 禁能指定的系统或外设时钟
 */
int am_clk_disable (am_clk_id_t clk_id)
{
    if ((clk_id < CLK_PERIPH_ID_MIN) ||
        (clk_id > CLK_PERIPH_ID_MAX)) {

        /* 时钟ID不在范围内*/
        return -AM_ENXIO;
    }
        
    amhw_sim_periph_clock_disable ((amhw_sim_scgc_periph_t) (clk_id));
    
    return AM_OK;
}

/** \brief get the fll_div
 *
 * \param clk 8-32MHz
 *
 * \return the fll_div
 * \note  31250< clk /(31250 << fll_div) < 390625
 * */
uint32_t __fll_div_get (uint32_t clk)
{
    uint8_t div = 0;

    for (div = 0; (clk >>(5 + div) > 390625); div++);

    return div;
}

/** \brief clk pee mode init */
uint32_t __clk_pee_init(void)
{
    /* 默认关闭ATM内部参考时钟修正功能 */
    amhw_mcg_atm_disable();

    /* CLKDIV */
    amhw_sim_clkdiv1_outdiv1_set(__gp_clk_dev->outdiv1);
    amhw_sim_clkdiv1_outdiv4_set(__gp_clk_dev->outdiv4);

    /* OSC32KSEL */
    amhw_sim_ercclk32k_src_set(AMHW_SIM_OSC32KSEL_LPO);

    /* 配置MCGPLLCLK/MCGFLLCLK时钟源  */
    amhw_sim_pllfll_sel_set(AMHW_SIM_PLLFLLSEL_PLL);

    /**< \brief  设置FCRDIV */
    amhw_mcg_irc_fast_div_set(0);

    /* OSCERCLK时钟配置 */
    amhw_mcg_osc_monitor_disable();
    amhw_mcg_osc_range_set(AMHW_MCG_OSC_RANGE_VERYHIGH);
    amhw_mcg_osc_src_set(AMHW_MCG_OSC_SRC_OSC);
    amhw_mcg_erc_enable();

    /* 设置FLL */
    amhw_mcg_fll_src_set(AMHW_MCG_FLL_SRC_ERC);
    amhw_mcg_fll_erc_div_set(__gp_clk_dev->fll_div);

    /* enable MCGIRCLK时钟 */
    amhw_mcg_irc_src_set(AMHW_MCG_IRC_SRC_SLOW);
    amhw_mcg_irc_enable();

    /* mcgooutclk use erc*/
    amhw_mcg_outclk_src_set(AMHW_MCG_OUTCLK_SRC_ERC);

    /* 等待外部晶振稳定 */
    while (FALSE == amhw_mcg_osc_stable_stat_get());

    /** \brief  检查FLL时钟源 */
    while (amhw_mcg_fll_stat_get() != 0x00U);

    /* fll mul */
    amhw_mcg_fll_dco_div_set((amhw_mcg_dco_div_t)__gp_clk_dev->fll_rate);

    /* PEE模式下 PLL */
    amhw_mcg_pll_disable();
    amhw_mcg_pll_erc_div_set(__gp_clk_dev->pll_div);
    amhw_mcg_pll_vco_div_set(__gp_clk_dev->pll_mul);

    /* PLL en*/
    amhw_mcg_pll_enable();

    /** \brief  FBE-->PBE*/
    amhw_mcg_plls_src_set(AMHW_MCG_PLLS_SRC_PLL);

    /** \brief  等待PLL锁定 */
    while (FALSE == amhw_mcg_pll_locked_stat_get());

    /** \brief  FBE-->PBE*/
    amhw_mcg_outclk_src_set(AMHW_MCG_OUTCLK_SRC_PLLS);

    /** \brief  PEE模式检查 */
    while (amhw_mcg_outclk_stat_get() != AMHW_MCG_OUTCLK_SATA_PLL);

    return AM_OK;
}

#if 0
/** \brief clk fei mode init， 内核时钟为48MHz,总线时钟为24MHz */
uint32_t __clk_fei_init(void)
{
    /* 默认关闭ATM内部参考时钟修正功能 */
    amhw_mcg_atm_disable();

    /* CLKDIV */
    amhw_sim_clkdiv1_outdiv1_set(__gp_clk_dev->outdiv1);
    amhw_sim_clkdiv1_outdiv4_set(__gp_clk_dev->outdiv4);

    /* 配置外设使用MCGFLLCLK作为时钟的来源  */
    amhw_sim_pllfll_sel_set(AMHW_SIM_PLLFLLSEL_FLL);
    
    /* OSC32KSEL                          */
    amhw_sim_ercclk32k_src_set(AMHW_SIM_OSC32KSEL_LPO);

    /* 设置内部快速时钟的分频为1分频，使MCGIRCCLK的频率为4MHz */
    amhw_mcg_irc_fast_div_set(0);
    
    /* C1使能FLL或 PLL的时钟输出，CLKS = 0 */
    amhw_mcg_outclk_src_set(AMHW_MCG_OUTCLK_SRC_PLLS);

    /* C1设置FLL的IREF位 */
    amhw_mcg_fll_src_set(AMHW_MCG_FLL_SRC_IRC_SLOW);
    
    /* C1设置FLL的FRDIV位,满足FLL CLK的输入频率在32.768K - 39.0625KHz之间 */
    amhw_mcg_fll_erc_div_set(__gp_clk_dev->fll_div);
    
    /* 与 AMHW_MCG->c2 寄存器直接赋值相似,选择MCGIRCLK的时钟为32.768KHz   */
     amhw_mcg_irc_src_set(AMHW_MCG_IRC_SRC_SLOW);   

    #if 0 
    AMHW_MCG->c2 = 0X00;
    #endif /* 0 */
    
    /* C1设置FLL的IRCLKEN=1, 使能内部参考时钟 */
    amhw_mcg_irc_enable( );
    
    AMHW_MCG->c4 = (AMHW_MCG->c4 & 0X1F) | 0x80 | 0x20;
    
    /* fll mul, C4, FLL CLK 倍频，与 AMHW_MCG->c4 寄存器直接赋值相似  */ 
    /* amhw_mcg_fll_dco_div_set((amhw_mcg_dco_div_t)__gp_clk_dev->fll_rate); */
     
    
    AMHW_OSC->cr = 0X00;
    
    AMHW_MCG->c5 =  0X00;
    AMHW_MCG->c5 =  0X00; 

    /** \brief  检查FLL时钟源 */
    while (amhw_mcg_fll_stat_get() == 0x00U);
    
   /** \brief  FLL已经选择 */
    while (amhw_mcg_outclk_stat_get() != 0x00U);

    return AM_OK;
}
#endif /* 0 */


#if 1
/** \brief clk fei mode init, 内核时钟为24MHZ，总线时钟为12MHz */
uint32_t __clk_fei_init(void)
{
    /* 默认关闭ATM内部参考时钟修正功能 */
    amhw_mcg_atm_disable();

    /* CLKDIV */
    amhw_sim_clkdiv1_outdiv1_set(__gp_clk_dev->outdiv1);
    amhw_sim_clkdiv1_outdiv4_set(__gp_clk_dev->outdiv4);

    /* 配置外设使用MCGFLLCLK作为时钟的来源  */
    amhw_sim_pllfll_sel_set(AMHW_SIM_PLLFLLSEL_FLL);
    
    /* OSC32KSEL                          */
    amhw_sim_ercclk32k_src_set(AMHW_SIM_OSC32KSEL_LPO);

    /* 设置内部快速时钟的分频为1分频，使MCGIRCCLK的频率为4MHz */
    amhw_mcg_irc_fast_div_set(0);
    
    /* C1使能FLL或 PLL的时钟输出作为MCGOUTCLK，CLKS = 0    */
    amhw_mcg_outclk_src_set(AMHW_MCG_OUTCLK_SRC_PLLS);

    /* C1设置FLL的IREFS位,选择FLL的时钟来源为32K的内部寄存器 */
    amhw_mcg_fll_src_set(AMHW_MCG_FLL_SRC_IRC_SLOW);
    
    /* C1设置FLL的FRDIV位,满足FLL CLK的输入频率在32.768K - 39.0625KHz之间 */
    amhw_mcg_fll_erc_div_set(__gp_clk_dev->fll_div);
    
    /* 与 AMHW_MCG->c2 寄存器直接赋值相似,选择MCGIRCLK的时钟为32.768KHz   */
    amhw_mcg_irc_src_set(AMHW_MCG_IRC_SRC_SLOW);   

    #if 0 
    AMHW_MCG->c2 = 0X00;
    #endif /* 0 */
    
    /* 
     * C1设置FLL的IRCLKEN=1, 使能FLL使用内部的32.768K参考时钟 
     * 在停止模式下，内部参考时钟停止使能
     */
    amhw_mcg_irc_enable( );
    
    //AMHW_MCG->c4 = (AMHW_MCG->c4 & 0X1F) | 0x80 | 0x00;
    
    /* fll mul, C4, FLL CLK 倍频，与 AMHW_MCG->c4 寄存器直接赋值相似  */ 
     amhw_mcg_fll_dco_div_set((amhw_mcg_dco_div_t)__gp_clk_dev->fll_rate);
     
    
    AMHW_OSC->cr = 0X00;
    
    AMHW_MCG->c5 =  0X00;
    AMHW_MCG->c5 =  0X00; 

    /** \brief  检查FLL时钟源 */
    while (amhw_mcg_fll_stat_get() == 0x00U);
    
   /** \brief  FLL已经选择 */
    while (amhw_mcg_outclk_stat_get() != 0x00U);

    return AM_OK;
}
#endif /* 1 */

/**
 * \brief CLK 初始化
 */
int amdr_clk_init (amdr_clk_dev_t           *p_dev,
                   const amdr_clk_devinfo_t *p_devinfo)
{
    if (p_dev == NULL || p_devinfo == NULL) {
        return -AM_EINVAL;
    }

    if (p_devinfo->clk_mode == AMDR_CLK_MODE_PEE) {
        /* 外部时钟频率不正确*/
        if ((p_devinfo->xtal_rate < 8000000) ||
            (p_devinfo->xtal_rate > 32000000)) {
            return -AM_EINVAL;
        }
    }

    p_dev->p_devinfo  = p_devinfo;

    /* PLLIN = 2MHz, PLLOUT = 96MHz*/
    p_dev->pll_div = (p_dev->p_devinfo->xtal_rate / 2000000UL) -1;
    p_dev->pll_mul = (96000000 / 2000000) - 24;

    /* FLLIN = 32.768KHz, FLLOUT = 40-50MHz */
    p_dev->fll_div = __fll_div_get(p_dev->p_devinfo->xtal_rate);
    p_dev->fll_rate = AMHW_MCG_DOC_DIV_732;    /* 精确时钟的延时 */

    /* outdiv1 = 1, outdiv4 = 1,
     * CORE_CLK = MCGOUTCLK / (1 + OUTDIV1) = 96MHz / (1+1)=48MHz
     * BUS_CLK = CORE_CLK   / (1 + OUTDIV4) = 48MHz / (1+1) = 24MHz
     */

    __gp_clk_dev = p_dev;

    /* PEE MODE */
    if (p_devinfo->clk_mode == AMDR_CLK_MODE_PEE) {
        
        p_dev->outdiv1 = 1;
        p_dev->outdiv4 = 1;
        
        return __clk_pee_init();
        
    } else {
        p_dev->outdiv1 = 0;
        p_dev->outdiv4 = 1;
        
        __clk_fei_init();
    }
    
    return AM_OK;
}


/**
 * \brief 获取时钟工作模式
 */
amdr_clk_mode_t amdr_clk_mode_get (void)
{
    amdr_clk_mode_t      mode   = AMDR_CLK_MODE_ERROR;
    amhw_mcg_outclk_stat_t clkst  = amhw_mcg_outclk_stat_get();
    amhw_mcg_fll_src_t     irefst = amhw_mcg_fll_src_get();
    bool_t                 lp     = amhw_mcg_lp_plls_get();
    bool_t                 pllst  = amhw_mcg_plls_stat_get();

    switch (clkst) {

        case AMHW_MCG_OUTCLK_SATA_FLL:
            if (!pllst) {
                if (AMHW_MCG_FLL_SRC_ERC == irefst) {
                    mode = AMDR_CLK_MODE_FEE;
                } else {
                    mode = AMDR_CLK_MODE_FEI;
                }
            }
        break;

        case AMHW_MCG_OUTCLK_SATA_IRC:
            if ((AMHW_MCG_FLL_SRC_IRC_SLOW == irefst) && (!pllst)) {
                if (!lp) {
                    mode = AMDR_CLK_MODE_FBI;
                } else {
                    mode = AMDR_CLK_MODE_BLPI;
                }
            }
        break;

        case AMHW_MCG_OUTCLK_SATA_ERC:
            if (AMHW_MCG_FLL_SRC_ERC == irefst) {
                if (!lp) {
                    if (pllst) {
                        mode = AMDR_CLK_MODE_PBE;
                    } else {
                        mode = AMDR_CLK_MODE_FBE;
                    }
                } else {
                    mode = AMDR_CLK_MODE_BLPE;
                }
            }
        break;

        case AMHW_MCG_OUTCLK_SATA_PLL:
            if ((AMHW_MCG_FLL_SRC_ERC == irefst) && pllst) {
                mode = AMDR_CLK_MODE_PEE;
            }
            break;

        default:
        break;
    }

    return mode;
}

/**
 * \brief 配置时钟工作模式PEE到BLPE模式（低功耗模式）
 * \return 无
 * \note BLPE模式，CORE_CLK = 4MHZ
 *                 BUS_CLK = 1MHZ
 */
int amdr_clk_mode_pee2blpe (void)
{
    volatile uint32_t i = 0;

    uint32_t xtal_rate = __gp_clk_dev->p_devinfo->xtal_rate;

    /** \brief 检查当前模式是否是PEE模式 */
    if (amdr_clk_mode_get() != AMDR_CLK_MODE_PEE) {
        return AM_ERROR;
    }

    /** \brief  选择ERC做为MCGOUTCLK时钟源 (PEE->PBE)*/
    amhw_mcg_outclk_src_set(AMHW_MCG_OUTCLK_SRC_ERC);

    /** \brief  等待变为PBE模式 */
    for (i = 0; i < 2000; i++) {
        if (AMHW_MCG_OUTCLK_SATA_ERC == amhw_mcg_outclk_stat_get()) {
            break;
        }
    }
    if (AMHW_MCG_OUTCLK_SATA_ERC != amhw_mcg_outclk_stat_get()) {
        return AM_ERROR;
    }

    /** \brief 关闭PLL/FLL模块 (PBE->BLPE) */
    amhw_mcg_lp_plls_disable();

    /** \brief 外部参考时钟频率分频到sysyclk=4MHz,再4分频到flashclk=1MHz */
    amhw_sim_clkdiv1_outdiv1_set(xtal_rate /(4*1000*1000));
    amhw_sim_clkdiv1_outdiv4_set(3);

    return AM_OK;
}

/**
 * \brief 配置时钟工作模式BLPE到PEE模式
 * \return 无
 * \note PEE模式,CORE_CLK = 48MHZ
 *               BUS_CLK = 24MHZ
 */
int amdr_clk_mode_blpe2pee (void)
{
    volatile uint32_t i = 0;

    /** \brief 检查当前模式是否是BLPE模式 */
    if (amdr_clk_mode_get() != AMDR_CLK_MODE_BLPE) {
        return AM_ERROR;
    }

    amhw_mcg_pll_erc_div_set(__gp_clk_dev->pll_div);
    amhw_mcg_pll_vco_div_set(__gp_clk_dev->pll_mul);

    /** \brief 打开PLL/FLL模块 (BLPE-PBE) */
    amhw_mcg_lp_plls_enable();

    /** \brief 等待PLLS时钟源为PLL输出 */
    for (i = 0; i < 2000; i++) {
        if (TRUE == amhw_mcg_plls_stat_get()) {
            break;
        }
    }
    if (TRUE != amhw_mcg_plls_stat_get()) {
        return AM_ERROR;
    }

    /** \brief 等待PLL锁定 */
    for (i = 0; i < 2000; i++) {
        if (TRUE == amhw_mcg_pll_locked_stat_get()) {
            break;
        }
    }
    if (TRUE != amhw_mcg_pll_locked_stat_get()) {
        return AM_ERROR;
    }

    /** \brief  选择PLLS做为MCGOUTCLK时钟源 (PBE->PEE)*/
    amhw_mcg_outclk_src_set(AMHW_MCG_OUTCLK_SRC_PLLS);

    /** \brief  等待变为PEE模式 */
    for (i = 0; i < 2000; i++) {
        if (AMHW_MCG_OUTCLK_SATA_PLL == amhw_mcg_outclk_stat_get()) {
            break;
        }
    }
    if (AMHW_MCG_OUTCLK_SATA_PLL != amhw_mcg_outclk_stat_get()) {
        return AM_ERROR;
    }

    /** \brief OUTCLK时钟频率96MHz,分频到sysyclk和分频到flashclk */
    amhw_sim_clkdiv1_outdiv1_set(__gp_clk_dev->outdiv1);
    amhw_sim_clkdiv1_outdiv4_set(__gp_clk_dev->outdiv4);

    return AM_OK;
}

/**
 * \brief 配置时钟工作模式FEI到BLPI模式（低功耗模式）
 * \return 无
 * \note BLPI模式,
 *       Core clock = 4MHz
 *       Bus clock  = 1MHz
 */
int amdr_clk_mode_fei2blpi (void)
{
    volatile uint32_t i = 0;

    /** \brief 检查当前模式是否是PEI模式 */
    if (amdr_clk_mode_get() != AMDR_CLK_MODE_FEI) {
        return AM_ERROR;
    }
    /**< \brief  设置IRCCLK时钟源为内部快速参考时钟 */
    amhw_mcg_irc_fast_div_set(0);
    amhw_mcg_irc_src_set(AMHW_MCG_IRC_SRC_FAST);

    /** \brief  选择IRC做为MCGOUTCLK时钟源 (FEI->FBI)*/
    amhw_mcg_outclk_src_set(AMHW_MCG_OUTCLK_SRC_IRC);

    /** \brief  等待变为FBI模式 */
    for (i = 0; i < 2000; i++) {
        if (AMHW_MCG_OUTCLK_SATA_IRC == amhw_mcg_outclk_stat_get()) {
            break;
        }
    }

    if (AMHW_MCG_OUTCLK_SATA_IRC != amhw_mcg_outclk_stat_get()) {
        return AM_ERROR;
    }

    /** \brief 关闭PLL/FLL模块 (FBI->BLPI) */
    amhw_mcg_lp_plls_disable();

    /** \brief 内部参考时钟频率是4MHz,1分频到sysyclk和4分频到bus/flashclk */
    amhw_sim_clkdiv1_outdiv1_set(0);
    amhw_sim_clkdiv1_outdiv4_set(3);

    return AM_OK;
}

/**
 * \brief 配置时钟工作模式BLPI到FEI模式
 * \return 无
 * \note FBI模式
 *       Core clock = 20.97152MHz
 *       Bus clock  = 20.97152MHz
 */
int amdr_clk_mode_blpi2fei (void)
{
    volatile uint32_t i = 0;

    /** \brief 检查当前模式是否是BLPI模式 */
    if (amdr_clk_mode_get() != AMDR_CLK_MODE_BLPI) {
        return AM_ERROR;
    }

    /** \brief  设置FLL */
    amhw_mcg_fll_erc_div_set(0);
    amhw_mcg_fll_dco_div_set(AMHW_MCG_DOC_DIV_640);

    /** \brief 打开PLL/FLL模块 (BLPI->FBI) */
    amhw_mcg_lp_plls_enable();

    /** \brief  选择FLL做为MCGOUTCLK时钟源 (FBI->FEI)*/
    amhw_mcg_outclk_src_set(AMHW_MCG_OUTCLK_SRC_PLLS);

    /** \brief  等待变为FBI模式 */
    for (i = 0; i < 2000; i++) {
        if (AMHW_MCG_OUTCLK_SATA_FLL == amhw_mcg_outclk_stat_get()) {
            break;
        }
    }
    if (AMHW_MCG_OUTCLK_SATA_FLL != amhw_mcg_outclk_stat_get()) {
        return AM_ERROR;
    }
    /** \brief MCGOUT时钟频率是20.97152MHz,1分频到sysyclk和1分频到busclk */
    amhw_sim_clkdiv1_outdiv1_set(0);
    amhw_sim_clkdiv1_outdiv4_set(0);

    return AM_OK;
}

/** \brief 得到外设的输入频率 */
uint32_t amdr_clk_periph_rate_get(void *p_periph)
{
    uint32_t base_addr = (uint32_t)(p_periph);

    if ( (base_addr == AMHW_UART1_BASE) ||
         (base_addr == AMHW_UART2_BASE) ||
         (base_addr == AMHW_SPI0_BASE)  ||
         (base_addr == AMHW_I2C0_BASE) ) {
        return amdr_clk_bus_clkrate_get();

    } else if ( (base_addr == AMHW_GPIO_BASE) ||
                (base_addr == AMHW_SPI1_BASE) ||
                (base_addr == AMHW_I2C1_BASE) ){
        return amdr_clk_core_clkrate_get();

    } else if (base_addr == AMHW_UART0_BASE) {
        return amdr_clk_uart0_rate_get();

    } else if ( (base_addr == AMHW_TPM0_BASE) ||
                (base_addr == AMHW_TPM1_BASE) ||
                (base_addr == AMHW_TPM2_BASE) ){
        return amdr_clk_tpm_rate_get();
                    
    } else if (base_addr == AMHW_LPTMR_BASE){  /* 添加低功耗定时器的正确时钟来源，它不是在BUS上面的 */
        return amdr_clk_mcgirclk_rate_get();

    } else {
        return amdr_clk_bus_clkrate_get();
    }

}


/* end of file */
