/*******************************************************************************
*                                 Apollo
*                       ---------------------------
*                       innovating embedded systems
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief 外设内存映射地址宏定义和外设定义
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-16  xjc, first implementation.
 * \endinternal
 */

#ifndef __AMHW_AMKS16Z_H
#define __AMHW_AMKS16Z_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 中断号定义，为 core_cm0plus.h文件保留
 */
typedef enum {
    /** \brief  1  复位向量 */
    Reset_IRQn               = -15,

    /** \brief  2  不可屏蔽中断, 不能被停止或抢占 */
    NonMaskableInt_IRQn      = -14,

    /** \brief  3  硬件错误中断 */
    HardFault_IRQn           = -13,

    /** \brief 11  系统服务调用通过SVC指令 */
    SVCall_IRQn              =  -5,

    /** \brief 14  系统的挂起请求 */
    PendSV_IRQn              =  -2,

    /** \brief 15  系统滴答定时器 */
    SysTick_IRQn             =  -1,

    /* ---------------  外设中断 ------------------------------------ */
    DMA_CH0_IRQn             = 0,    /**< \brief DMA通道0中断      */
    DMA_CH1_IRQn             = 1,    /**< \brief DMA通道1中断      */
    DMA_CH2_IRQn             = 2,    /**< \brief DMA通道2中断      */
    DMA_CH3_IRQn             = 3,    /**< \brief DMA通道3中断      */

    FTFA_IRQn                = 5,    /**< \brief FAFT中断            */
    PMC_IRQn                 = 6,    /**< \brief PMC中断              */
    LLWU_IRQn                = 7,    /**< \brief LLWU中断            */
    I2C0_IRQn                = 8,    /**< \brief I2C0中断             */
    I2C1_IRQn                = 9,    /**< \brief I2C1中断             */
    SPI0_IRQn                = 10,   /**< \brief SPI0中断             */
    SPI1_IRQn                = 11,   /**< \brief SPI1中断             */
    UART0_IRQn               = 12,   /**< \brief UART0中断           */
    UART1_IRQn               = 13,   /**< \brief UART1中断          */
    UART2_IRQn               = 14,   /**< \brief UART2中断           */
    ADC0_IRQn                = 15,   /**< \brief ADC0中断             */
    CMP0_IRQn                = 16,   /**< \brief CMP0中断             */
    TPM0_IRQn                = 17,   /**< \brief TPM0中断             */
    TPM1_IRTn                = 18,   /**< \brief TPM1中断              */
    TPM2_IRTn                = 19,   /**< \brief TPM2中断              */
    RTC_ALARM_IRQn           = 20,   /**< \brief RTC 闹钟中断    */
    RTC_SEC_IRQn             = 21,   /**< \brief RTC秒中断            */
    PIT_IRQn                 = 22,   /**< \brief PIT中断                 */
    I2S_IRQn                 = 23,   /**< \brief I2S中断                 */

    DAC0_IRQn                = 25,   /**< \brief DAC0中断                */
    TSI0_IRQn                = 26,   /**< \brief TSI0中断                */
    MCG_IRQn                 = 27,   /**< \brief MCG中断                  */
    LPTMR0_IRQn              = 28,   /**< \brief LPTMR0中断          */

    PORTA_IRQn               = 30,   /**< \brief PORTA 引脚中断      */
    PORTC_PORTD_IRQn         = 31,   /**< \brief PORTC或PORTD引脚中断 */
}IRQn_Type;

/**
 * \addtogroup amhw_if_amks16z
 * \copydoc amhw_amks16z.h
 * @{
 */

/**
 * \name Cortex-M0+ 核处理器和外设配置
 * @{
 */

#define AM_CORTEX_M0
#ifdef AM_CORTEX_M0

#define __MPU_PRESENT           0  /**< \brief M0+核无MPU                  */
#define __NVIC_PRIO_BITS        2  /**< \brief M0+核为2                    */
#define __Vendor_SysTickConfig  0  /**< \brief 设置为1 可以配置SysTick     */
#define __VTOR_PRESENT          1  /**< \brief 如果为1，可配置中断向量表   */
//#include "core_cm0.h"
#include "core_cm0plus.h"

#endif
/** @} */

/**
 * \name 外设内存映射地址定义
 * @{
 */
#define AMHW_NVIC_BASE           (0xE000E100UL)  /**< \brief NVIC基地址          */
#define AMHW_SIM_BASE            (0x40047000UL)  /**< \brief SIM基地址          */
#define AMHW_TPM0_BASE           (0x40038000UL)  /**< \brief TPM0基地址          */
#define AMHW_TPM1_BASE           (0x40039000UL)  /**< \brief TPM1基地址          */
#define AMHW_TPM2_BASE           (0x4003A000UL)  /**< \brief TPM2基地址          */
#define AMHW_MCG_BASE            (0x40064000UL)  /**< \brief MCG基地址           */
#define AMHW_OSC_BASE            (0x40065000UL)  /**< \brief OSC基地址           */
#define AMHW_PORT_BASE           (0x40049000UL)  /**< \brief PORT端口基地址 */
#define AMHW_GPIO_BASE           (0x400FF000UL)  /**< \brief GPIO通道基地址 */
#define AMHW_FGPIO_BASE          (0xF8000000UL)  /**< \brief FGPIO通道基地址 */
#define AMHW_UART0_BASE          (0x4006A000UL)  /**< \brief UART0基地址          */
#define AMHW_UART1_BASE          (0x4006B000UL)  /**< \brief UART0基地址          */
#define AMHW_UART2_BASE          (0x4006C000UL)  /**< \brief UART0基地址          */
#define AMHW_DAC0_BASE           (0x4003F000UL)  /**< \brief DAC0基地址          */
#define AMHW_ADC0_BASE           (0x4003B000UL)  /**< \brief ADC0基地址          */
#define AMHW_SPI0_BASE           (0x40076000UL)  /**< \brief SPI0基地址          */
#define AMHW_SPI1_BASE           (0x40077000UL)  /**< \brief SPI1基地址          */
#define AMHW_I2C0_BASE           (0x40066000UL)  /**< \brief I2C0基地址          */
#define AMHW_I2C1_BASE           (0x40067000UL)  /**< \brief I2C1基地址          */
#define AMHW_SYSTICK_BASE        (0xE000E010UL)  /**< \brief SysTick基地址       */
#define AMHW_CMP_BASE            (0x40073000UL)  /**< \brief SysTick基地址       */
#define AMHW_DMAMUX_BASE         (0x40021000UL)  /**< \brief DMAMUX基地址        */
#define AMHW_DMA_BASE            (0x40008100UL)  /**< \brief DMA基地址           */
#define AMHW_MCM_BASE            (0xf0003000UL)  /**< \brief MCM基地址           */
#define AMHW_RTC_BASE            (0x4003D000UL)  /**< \brief RTC基地址           */
#define AMHW_FTFA_BASE           (0x40020000UL)  /**< \brief FTFA基地址          */
#define AMHW_PIT0_BASE           (0x40037000UL)  /**< \brief PIT基地址          */
#define AMHW_TSI_BASE            (0x40045000UL)  /**< \brief TSI基地址          */
#define AMHW_LPTMR_BASE          (0x40040000UL)  /**< \brief LPTMR基地址          */
#define AMHW_LLWU_BASE           (0x4007C000UL)  /**< \brief LLWU基地址          */
#define AMHW_PMC_BASE            (0x4007D000UL)  /**< \brief PMC基地址          */
#define AMHW_SMC_BASE            (0x4007E000UL)  /**< \brief SMC基地址          */
#define AMHW_RCM_BASE            (0x4007F000UL)  /**< \brief RCM基地址          */
#define AMHW_I2S0_BASE           (0x4002F000UL)  /**< \brief I2S0基地址          */


/** @} */

/**
 * \name 外设定义
 * @{
 */
#define AMHW_INT      ((amhw_int_t        *)AMHW_NVIC_BASE)   /**< \brief  NVIC */
#define AMHW_SIM      ((amhw_sim_t        *)AMHW_SIM_BASE)    /**< \brief  NVIC */
#define AMHW_TPM0     ((amhw_tpm_t        *)AMHW_TPM0_BASE)   /**< \brief  TPM0 */
#define AMHW_TPM1     ((amhw_tpm_t        *)AMHW_TPM1_BASE)   /**< \brief  TPM0 */
#define AMHW_TPM2     ((amhw_tpm_t        *)AMHW_TPM2_BASE)   /**< \brief  TPM0 */
#define AMHW_MCG      ((amhw_mcg_t        *)AMHW_MCG_BASE)    /**< \brief  MCG */
#define AMHW_OSC      ((amhw_osc_t        *)AMHW_OSC_BASE)    /**< \brief  OSC */
#define AMHW_GPIO     ((amhw_gpio_t       *)AMHW_GPIO_BASE)   /**< \brief GPIO */
#define AMHW_FGPIO    ((amhw_fgpio_t      *)AMHW_FGPIO_BASE)  /**< \brief FGPIO */
#define AMHW_PORT     ((amhw_port_t       *)AMHW_PORT_BASE)   /**< \brief PORT */
#define AMHW_UART0    ((amhw_uart_t       *)AMHW_UART0_BASE)  /**< \brief UART  */
#define AMHW_UART1    ((amhw_uart_t       *)AMHW_UART1_BASE)  /**< \brief UART1 */
#define AMHW_UART2    ((amhw_uart_t       *)AMHW_UART2_BASE)  /**< \brief UART2 */
#define AMHW_DAC0     ((amhw_dac_t        *)AMHW_DAC0_BASE)   /**< \brief DAC0 */
#define AMHW_ADC0     ((amhw_adc_t        *)AMHW_ADC0_BASE)   /**< \brief ADC0 */
#define AMHW_SPI0     ((amhw_spi_t        *)AMHW_SPI0_BASE)   /**< \brief SPI0 */
#define AMHW_SPI1     ((amhw_spi_t        *)AMHW_SPI1_BASE)   /**< \brief SPI1 */
#define AMHW_I2C0     ((amhw_i2c_t        *)AMHW_I2C0_BASE)   /**< \brief I2C0 */
#define AMHW_I2C1     ((amhw_i2c_t        *)AMHW_I2C1_BASE)   /**< \brief I2C1 */
#define AMHW_SYSTICK  ((amhw_systick_t    *)AMHW_SYSTICK_BASE)/**< \brief SysTick */
#define AMHW_CMP0     ((amhw_cmp_t        *)AMHW_CMP_BASE)    /**< \brief  CMP0  */
#define AMHW_DMAMUX   ((amhw_dmamux_t     *)AMHW_DMAMUX_BASE) /**< \brief DMAMUX */
#define AMHW_DMA      ((amhw_dma_t        *)AMHW_DMA_BASE)    /**< \brief DMA  */
#define AMHW_MCM      ((amhw_mcm_t        *)AMHW_MCM_BASE)    /**< \brief MCM  */
#define AMHW_RTC      ((amhw_rtc_t        *)AMHW_RTC_BASE)    /**< \brief RTC  */
#define AMHW_FTFA     ((amhw_ftfa_t       *)AMHW_FTFA_BASE)   /**< \brief FTFA */
#define AMHW_PIT0     ((amhw_pit_t        *)AMHW_PIT0_BASE)   /**< \brief PIT */
#define AMHW_TSI      ((amhw_tsi_t        *)AMHW_TSI_BASE)    /**< \brief TSI */
#define AMHW_LPTMR    ((amhw_lptmr_t      *)AMHW_LPTMR_BASE)  /**< \brief LPTMR */
#define AMHW_SMC      ((amhw_smc_t        *)AMHW_SMC_BASE)    /**< \brief SMC */
#define AMHW_PMC      ((amhw_pmc_t        *)AMHW_PMC_BASE)    /**< \brief PMC */
#define AMHW_LLWU     ((amhw_llwu_t       *)AMHW_LLWU_BASE)   /**< \brief LLWU */
#define AMHW_RCM      ((amhw_rcm_t        *)AMHW_RCM_BASE)    /**< \brief RCM */
#define AMHW_I2S0     ((amhw_i2s_t        *)AMHW_I2S0_BASE)   /**< \brief I2S0 */
/** @} */

/**
 * @} amhw_if_amks16z
 */

#ifdef __cplusplus
}
#endif


#endif  /* __AMHW_AMKS16Z_H */

/* end of file */


