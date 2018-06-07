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
 * \brief 系统控制接口
 *
 * 1. 外设系统时钟门控使能/禁能；
 * 2. 外设时钟源选择
 * 3. UART TX/RX 源；
 * 4. ADC 触发源；
 * 5. 设备 ID 寄存器；
 * 6. COP看门狗服务。
 *
 * \internal
 * \par History
 * - 1.00 14-10-19  link, first implementation.
 * \endinternal
 */

#ifndef __AMHW_SIM_H
#define __AMHW_SIM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "amhw_amks16z.h"

/**
 * \addtogroup amhw_if_sim
 * \copydoc amhw_sim.h
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/**
  * \brief 系统控制寄存器块结构体
  */
typedef struct amhw_sim {
    __IO uint32_t sopt1;            /**< \brief 系统配置寄存器1 */
    __I  uint32_t reserved0[1024];  /**< \brief 保留                              */
    __IO uint32_t sopt2;            /**< \brief 系统配置寄存器2 */
    __I  uint32_t reserved1[1];     /**< \brief 保留                              */
    __IO uint32_t sopt4;            /**< \brief 系统配置寄存器4 */
    __IO uint32_t sopt5;            /**< \brief 系统配置寄存器5 */
    __I  uint32_t reserved2[1];     /**< \brief 保留                              */
    __IO uint32_t sopt7;            /**< \brief 系统配置寄存器7 */
    __I  uint32_t reserved3[2];     /**< \brief 保留                              */
    __I  uint32_t sdid;             /**< \brief 系统设备ID寄存器 */
    __I  uint32_t reserved4[3];     /**< \brief 保留                              */
    __IO uint32_t scgc[4];          /**< \brief 系统时钟门控寄存器     */
    __IO uint32_t clkdiv1;          /**< \brief 系统时钟分频寄存器1 */
    __I  uint32_t reserved5[1];     /**< \brief 保留                              */
    __IO uint32_t fcfg1;            /**< \brief FLASH配置寄存器1 */
    __I  uint32_t fcfg2;            /**< \brief FLASH配置寄存器2 */
    __I  uint32_t reserved6[1];     /**< \brief 保留                              */
    __I  uint32_t uidmh;            /**< \brief 唯一ID寄存器       */
    __I  uint32_t uidml;            /**< \brief 唯一ID寄存器       */
    __I  uint32_t uidl;             /**< \brief 唯一ID寄存器       */
    __I  uint32_t reserved7[39];    /**< \brief 保留                              */
    __IO uint32_t copc;             /**< \brief 看门狗控制寄存器   */
    __O  uint32_t srvcop;           /**< \brief 喂狗寄存器         */
} amhw_sim_t;

/**
 * \brief 外设时钟门控定义
 */
typedef enum amhw_sim_scgc_periph {
    /** \brief  SCGC4 */
    AMHW_SIM_SCGC_I2C0  =  (0 << 8 | 6),
    AMHW_SIM_SCGC_I2C1  =  (0 << 8 | 7),
    AMHW_SIM_SCGC_UART0 =  (0 << 8 | 10),
    AMHW_SIM_SCGC_UART1 =  (0 << 8 | 11),
    AMHW_SIM_SCGC_UART2 =  (0 << 8 | 12),
    AMHW_SIM_SCGC_CMP   =  (0 << 8 | 19),
    AMHW_SIM_SCGC_SPI0  =  (0 << 8 | 22),
    AMHW_SIM_SCGC_SPI1  =  (0 << 8 | 23),
    /** \brief  SCGC5 */
    AMHW_SIM_SCGC_LPTMR =  (1 << 8 | 0),
    AMHW_SIM_SCGC_TSI   =  (1 << 8 | 5),
    AMHW_SIM_SCGC_PORTA =  (1 << 8 | 9),
    AMHW_SIM_SCGC_PORTB =  (1 << 8 | 10),
    AMHW_SIM_SCGC_PORTC =  (1 << 8 | 11),
    AMHW_SIM_SCGC_PORTD =  (1 << 8 | 12),
    AMHW_SIM_SCGC_PORTE =  (1 << 8 | 13),
    /** \brief  SCGC6 */
    AMHW_SIM_SCGC_FTF    = (2 << 8 | 0),
    AMHW_SIM_SCGC_DMAMUX = (2 << 8 | 1),
    AMHW_SIM_SCGC_I2S    = (2 << 8 | 15),
    AMHW_SIM_SCGC_PIT    = (2 << 8 | 23),
    AMHW_SIM_SCGC_TPM0   = (2 << 8 | 24),
    AMHW_SIM_SCGC_TPM1   = (2 << 8 | 25),
    AMHW_SIM_SCGC_TPM2   = (2 << 8 | 26),
    AMHW_SIM_SCGC_ADC0   = (2 << 8 | 27),
    AMHW_SIM_SCGC_RTC    = (2 << 8 | 29),
    AMHW_SIM_SCGC_DAC0   = (2 << 8 | 31),
    /** \brief  SCGC7 */
    AMHW_SIM_SCGC_DMA    = (3 << 8 | 8),
} amhw_sim_scgc_periph_t;

/**
 * \brief RTC和LPTMR的32K时钟源ERCLK32K
 */
typedef enum amhw_sim_ercclk32k_src {
    AMHW_SIM_OSC32KSEL_SYS   = 0,    /**< \brief OSC32KCLK  */
    AMHW_SIM_OSC32KSEL_CLKIN = 2 ,   /**< \brief RTC_CLKIN  */
    AMHW_SIM_OSC32KSEL_LPO   = 3,    /**< \brief LPO 1KHZ   */
} amhw_sim_ercclk32k_src_t;

/**
 * \brief RTC_CLKOUT引脚时钟输出源
 */
typedef enum amhw_sim_rtc_clkout {
    AMHW_SIM_RTC_CLKOUT_RTC      = 0,   /**< \brief RTC 1HZ   */
    AMHW_SIM_RTC_CLKOUT_OSCERCLK = 1,   /**< \brief OSCERCLK   */
} amhw_sim_rtc_clkout_t;

/**
 * \brief CLKOUT引脚时钟输出源
 */
typedef enum amhw_sim_clkout {
    AMHW_SIM_CLKOUT_BUSCLK   = 2,      /**< \brief BUS时钟   */
    AMHW_SIM_CLKOUT_LPOCLK   = 3,      /**< \brief LPO 1HZ   */
    AMHW_SIM_CLKOUT_MCGIRCLK = 4,      /**< \brief MCGIRCLK  */
    AMHW_SIM_CLKOUT_OSCERCLK = 6,      /**< \brief OSCERCLK  */
} amhw_sim_clkout_t;

/**
 * \brief 外设MCGPLLCLK/MCGFLLCLK时钟选择
 * \note  建议使用MCGFLLCLK时钟，MCGFLLCLK默认2分频后使用
 */
typedef enum amhw_sim_fllpll_sel {
    AMHW_SIM_PLLFLLSEL_FLL = 0,    /**< \brief FLL           */
    AMHW_SIM_PLLFLLSEL_PLL = 1,    /**< \brief PLL           */
} amhw_sim_fllpll_sel_t;

/**
 * \brief TPM时钟源选择
 * \note 选择PLL/FLL源时，要先使用amhw_sim_periph_pllfll_set函数
 */
typedef enum amhw_sim_tpm_src {
    AMHW_SIM_TPMSRC_DISABLED  = 0,    /**< \brief DISABLED   */
    AMHW_SIM_TPMSRC_PLLFLLCLK = 1,    /**< \brief PLL/FLL    */
    AMHW_SIM_TPMSRC_OSCERCLK  = 2,    /**< \brief OSCERCLK   */
    AMHW_SIM_TPMSRC_MCGIRCLK  = 3,    /**< \brief MCGIRCLK   */
} amhw_sim_tpm_src_t;

/**
 * \brief UART0时钟源选择
 * \note 选择PLL/FLL源时，要先使用 amhw_sim_periph_pllfll_set 函数
 */
typedef enum amhw_sim_uart0_src {
    AMHW_SIM_UART0SRC_DISABLED  = 0,    /**< \brief DISABLED   */
    AMHW_SIM_UART0SRC_PLLFLLCLK = 1,    /**< \brief PLL/FLL    */
    AMHW_SIM_UART0SRC_OSCERCLK  = 2,    /**< \brief OSCERCLK   */
    AMHW_SIM_UART0SRC_MCGIRCLK  = 3,    /**< \brief MCGIRCLK   */
} amhw_sim_uart0_src_t;

/**
 * \brief TPM1通道0捕获源选择
 */
typedef enum amhw_sim_tpm1ch0_src {
    AMHW_SIM_TPM1CH0SRC_TPM1CH0 = 0,    /**< \brief TPM1_CH0   */
    AMHW_SIM_TPM1CH0SRC_CMP0    = 1,    /**< \brief CMP output */
} amhw_sim_tpm1ch0_src_t;

/**
 * \brief TPM2通道0捕获源选择
 */
typedef enum amhw_sim_tpm2ch0_src {
    AMHW_SIM_TPM2CH0SRC_TPM2CH0 = 0,    /**< \brief TPM1_CH0   */
    AMHW_SIM_TPM2CH0SRC_CMP0    = 1,    /**< \brief CMP output */
} amhw_sim_tpm2ch0_src_t;

/**
 * \brief TPM0外部时钟输入引脚选择
 */
typedef enum amhw_sim_tpm0_clkin {
    AMHW_SIM_TPM0_CLKIN_0 = 0,    /**< \brief TPM0_CLKIN_0引脚  */
    AMHW_SIM_TPM0_CLKIN_1 = 1,    /**< \brief TPM0_CLKIN_1引脚  */
} amhw_sim_tpm0_clkin_t;

/**
 * \brief TPM1外部时钟输入引脚选择
 */
typedef enum amhw_sim_tpm1_clkin {
    AMHW_SIM_TPM1_CLKIN_0 = 0,    /**< \brief TPM1_CLKIN_0引脚  */
    AMHW_SIM_TPM1_CLKIN_1 = 1,    /**< \brief TPM1_CLKIN_1引脚  */
} amhw_sim_tpm1_clkin_t;

/**
 * \brief TPM2外部时钟输入引脚选择
 */
typedef enum amhw_sim_tpm2_clkin {
    AMHW_SIM_TPM2_CLKIN_0 = 0,    /**< \brief TPM2_CLKIN_0引脚  */
    AMHW_SIM_TPM2_CLKIN_1 = 1,    /**< \brief TPM2_CLKIN_1引脚  */
} amhw_sim_tpm2_clkin_t;

/**
 * \brief UART0发送数据TX源
 */
typedef enum amhw_sim_uart0_tx_src {
    AMHW_SIM_UART0_TXSRC_TX      = 0,    /**< \brief TX引脚  */
    AMHW_SIM_UART0_TXSRC_TX_TPM1 = 1,    /**< \brief TX引脚加上TPM1调制  */
    AMHW_SIM_UART0_TXSRC_TX_TPM2 = 2,    /**< \brief TX引脚加上TPM2调制  */
} amhw_sim_uart0_tx_src_t;

/**
 * \brief UART0接收数据TX源
 */
typedef enum amhw_sim_uart0_rx_src {
    AMHW_SIM_UART0_TXSRC_RX   = 0,      /**< \brief RX引脚       */
    AMHW_SIM_UART0_TXSRC_CMP0 = 1,      /**< \brief CMP0输出  */
} amhw_sim_uart0_rx_src_t;

/**
 * \brief UART1发送数据TX源
 */
typedef enum amhw_sim_uart1_tx_src {
    AMHW_SIM_UART1_TXSRC_TX      = 0,    /**< \brief TX引脚  */
    AMHW_SIM_UART1_TXSRC_TX_TPM1 = 1,    /**< \brief TX引脚加上TPM1调制  */
    AMHW_SIM_UART1_TXSRC_TX_TPM2 = 2,    /**< \brief TX引脚加上TPM2调制  */
} amhw_sim_uart1_tx_src_t;

/**
 * \brief UART1接收数据TX源
 */
typedef enum amhw_sim_uart1_rx_src {
    AMHW_SIM_UART1_TXSRC_RX   = 0,      /**< \brief RX引脚       */
    AMHW_SIM_UART1_TXSRC_CMP0 = 1,      /**< \brief CMP0输出  */
} amhw_sim_uart1_rx_src_t;

/**
 * \brief ADC0触发源TPM1通道选择
 */
typedef enum amhw_sim_adc0_pretrigger {
    AMHW_SIM_ADC0_PRE_TRIGGER_A = 0,    /**< \brief 定时器1通道0  */
    AMHW_SIM_ADC0_PRE_TRIGGER_B = 1,    /**< \brief 定时器1通道1  */
} amhw_sim_adc0_pretrigger_t;

/**
 * \brief ADC0触发源选择
 */
typedef enum amhw_sim_adc0_trigger {
    AMHW_SIM_ADC0_TRIGGER_EXTRG  = 0,    /**< \brief 外部触发引脚输入 */
    AMHW_SIM_ADC0_TRIGGER_CMP0   = 1,    /**< \brief CMP0 OUTPUT    */
    AMHW_SIM_ADC0_TRIGGER_PIT0   = 4,    /**< \brief PIT 触发0      */
    AMHW_SIM_ADC0_TRIGGER_PIT1   = 5,    /**< \brief PIT 触发1      */
    AMHW_SIM_ADC0_TRIGGER_TPM0   = 8,    /**< \brief 定时器0溢出           */
    AMHW_SIM_ADC0_TRIGGER_TPM1   = 9,    /**< \brief 定时器1溢出           */
    AMHW_SIM_ADC0_TRIGGER_TPM2   = 10,   /**< \brief 定时器2溢出           */
    AMHW_SIM_ADC0_TRIGGER_RTCALR = 12,   /**< \brief RTC报警                    */
    AMHW_SIM_ADC0_TRIGGER_RTCSEC = 13,   /**< \brief RTC秒                         */
    AMHW_SIM_ADC0_TRIGGER_LPTMR0 = 14,   /**< \brief LPTMR0触发            */
} amhw_sim_adc0_trigger_t;

/**
 * \brief COP看门狗超时时间
 */
typedef enum amhw_sim_cop_timeout {
    AMHW_SIM_COP_DISABLED      = 0,   /**< \brief COP 禁能  */
    AMHW_SIM_COP_TIMOUT_VALUE1 = 1,   /**< \brief 2的5次方个LPO周期或2的13次方BUS周期 */
    AMHW_SIM_COP_TIMOUT_VALUE2 = 2,   /**< \brief 2的8次方个LPO周期或2的16次方BUS周期 */
    AMHW_SIM_COP_TIMOUT_VALUE3 = 3,   /**< \brief 2的10次方个LPO周期或2的18次方BUS周期 */
} amhw_sim_cop_timeout_t;

/**
 * \brief COP看门狗时钟选择
 */
typedef enum amhw_sim_cop_clk {
    AMHW_SIM_COP_CLK_INTERNAL = 0,    /**< \brief 内部1KHZ时钟  */
    AMHW_SIM_COP_CLK_BUS      = 1,    /**< \brief 总线时钟 */
} amhw_sim_cop_clk_t;

/**
 * \brief COP看门狗模式
 */
typedef enum amhw_sim_cop_mode {
    AMHW_SIM_COP_MODE_NORMAL = 0,    /**< \brief 正常模式  */
    AMHW_SIM_COP_MODE_WINDOW = 1,    /**< \brief 窗口模式 */
} amhw_sim_cop_mode_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * \brief ERC32KCLK时钟源选择
 * \param[in] value : ERC32KCLK时钟源
 * \return 无
 */
am_static_inline
void amhw_sim_ercclk32k_src_set (amhw_sim_ercclk32k_src_t value)
{
    AMHW_SIM->sopt1 = (AMHW_SIM->sopt1 & (~(0x03 << 18))) |
                      ((value & 0x03) << 18);
}

/**
 * \brief 获取ERC32KCLK时钟源
 * \return ERC32KCLK时钟源
 */
am_static_inline
amhw_sim_ercclk32k_src_t amhw_sim_ercclk32k_src_get (void)
{
    return (amhw_sim_ercclk32k_src_t)((AMHW_SIM->sopt1 >> 18) & 0x03);
}

/**
 * \brief RTC_CLKOUT引脚时钟输出源选择
 * \param[in] value : 输出时钟源
 * \return 无
 */
am_static_inline
void amhw_sim_rtc_clkout_set (amhw_sim_rtc_clkout_t value)
{
    AMHW_SIM->sopt2 = (AMHW_SIM->sopt2 & (~(1UL << 4))) |
                      (value << 4);
}

/**
 * \brief 获取RTC_CLKOUT引脚时钟输出源
 * \return 无
 */
am_static_inline
amhw_sim_rtc_clkout_t amhw_sim_rtc_clkout_get (void)
{
    return (amhw_sim_rtc_clkout_t)((AMHW_SIM->sopt2 >> 4) & 0x01);
}

/**
 * \brief CLKOUT引脚时钟输出选择
 * \param[in] value : CLK输出时钟
 * \return 无
 */
am_static_inline
void amhw_sim_clkout_set (amhw_sim_clkout_t value)
{
    AMHW_SIM->sopt2 = (AMHW_SIM->sopt2 & (~(7UL << 5))) |
                      (value << 5);
}

/**
 * \brief 外设MCGPLLCLK/MCGFLLCLK时钟选择
 * \param[in] value : PLL/FLL时钟
 * \return 无
 */
am_static_inline
void amhw_sim_pllfll_sel_set (amhw_sim_fllpll_sel_t value)
{
    AMHW_SIM->sopt2 = (AMHW_SIM->sopt2 & (~(1UL << 16))) |
                      (value << 16);
}

/**
 * \brief 获取外设MCGPLLCLK/MCGFLLCLK时钟源
 * \return 外设MCGPLLCLK/MCGFLLCLK时钟源
 */
am_static_inline
amhw_sim_fllpll_sel_t amhw_sim_pllfll_sel_get (void)
{
    return (amhw_sim_fllpll_sel_t)((AMHW_SIM->sopt2 >> 16) & 0x01);
}

/**
 * \brief TPM时钟源选择
 * \param[in] value : TPM时钟源
 * \return 无
 */
am_static_inline
void amhw_sim_tpm_src_set (amhw_sim_tpm_src_t value)
{
    AMHW_SIM->sopt2 = (AMHW_SIM->sopt2 & (~(3UL << 24))) |
                      (value << 24);
}

/**
 * \brief TPM时钟源获取
 * \return TPM时钟源
 */
am_static_inline
amhw_sim_tpm_src_t amhw_sim_tpm_src_get (void)
{
    return (amhw_sim_tpm_src_t)((AMHW_SIM->sopt2 >> 24) & 0x03);
}

/**
 * \brief UART0时钟源选择
 * \param[in] value : UART0时钟源
 * \return 无
 */
am_static_inline
void amhw_sim_uart0_src_set (amhw_sim_uart0_src_t value)
{
    AMHW_SIM->sopt2 = (AMHW_SIM->sopt2 & (~(3UL << 26))) |
                      (value << 26);
}

/**
 * \brief UART0时钟源获取
 * \return UART0使用的时钟源
 */
am_static_inline
amhw_sim_uart0_src_t amhw_sim_uart0_src_get (void)
{
    return (amhw_sim_uart0_src_t)((AMHW_SIM->sopt2 >> 26) & 0x03);
}

/**
 * \brief TPM1通道0捕获源选择
 * \param[in] value : TPM1通道0捕获源
 * \return 无
 * \note 当TPM1不使用输入捕获模式时清空这些位
 */
am_static_inline
void amhw_sim_tpm1ch0_src_set (amhw_sim_tpm1ch0_src_t value)
{
    AMHW_SIM->sopt4 = (AMHW_SIM->sopt4 & (~(3UL << 18))) |
                      (value << 18);
}

/**
 * \brief TPM2通道0捕获源选择
 * \param[in] value : TPM1通道0捕获源
 * \return 无
 * \note 当TPM2不使用输入捕获模式时清空这些位
 */
am_static_inline
void amhw_sim_tpm2ch0_src_set (amhw_sim_tpm2ch0_src_t value)
{
    AMHW_SIM->sopt4 = (AMHW_SIM->sopt4 & (~(1UL << 20))) |
                      (value << 20);
}

/**
 * \brief TPM0外部时钟引脚选择
 * \param[in] value :外部时钟引脚
 * \return 无
 * \note 这些选择的引脚必须通过引脚配置寄存器配置成TPM时钟引脚功能
 */
am_static_inline
void amhw_sim_tpm0_clkin_set (amhw_sim_tpm0_clkin_t value)
{
    AMHW_SIM->sopt4 = (AMHW_SIM->sopt4 & (~(1UL << 24))) |
                      (value << 24);
}

/**
 * \brief TPM1外部时钟引脚选择
 * \param[in] value :外部时钟引脚
 * \return 无
 * \note 这些选择的引脚必须通过引脚配置寄存器配置成TPM时钟引脚功能
 */
am_static_inline
void amhw_sim_tpm1_clkin_set (amhw_sim_tpm1_clkin_t value)
{
    AMHW_SIM->sopt4 = (AMHW_SIM->sopt4 & (~(1UL << 25))) |
                      (value << 25);
}

/**
 * \brief TPM2外部时钟引脚选择
 * \param[in] value :外部时钟引脚
 * \return 无
 * \note 这些选择的引脚必须通过引脚配置寄存器配置成TPM时钟引脚功能
 */
am_static_inline
void amhw_sim_tpm2_clkin_set (amhw_sim_tpm2_clkin_t value)
{
    AMHW_SIM->sopt4 = (AMHW_SIM->sopt4 & (~(1UL << 26))) |
                      (value << 26);
}

/**
 * \brief  UART0 发送数据TX源设置
 * \param[in] value : TX发送源
 * \return 无
 * \note
 */
am_static_inline
void amhw_sim_uart0_tx_src_set (amhw_sim_uart0_tx_src_t value)
{
    AMHW_SIM->sopt5 = (AMHW_SIM->sopt5 & (~(3UL << 0))) |
                      (value << 0);
}

/**
 * \brief  UART0 接收数据RX源设置
 * \param[in] value : RX接收源
 * \return 无
 * \note
 */
am_static_inline
void amhw_sim_uart0_rx_src_set (amhw_sim_uart0_rx_src_t value)
{
    AMHW_SIM->sopt5 = (AMHW_SIM->sopt5 & (~(1UL << 2))) |
                      (value << 2);
}

/**
 * \brief  UART1 发送数据TX源设置
 * \param[in] value : TX发送源
 * \return 无
 * \note
 */
am_static_inline
void amhw_sim_uart1_tx_src_set (amhw_sim_uart1_tx_src_t value)
{
    AMHW_SIM->sopt5 = (AMHW_SIM->sopt5 & (~(3UL << 4))) |
                      (value << 4);
}

/**
 * \brief  UART1 接收数据RX源设置
 * \param[in] value : RX接收源
 * \return 无
 * \note
 */
am_static_inline
void amhw_sim_uart1_rx_src_set (amhw_sim_uart1_rx_src_t value)
{
    AMHW_SIM->sopt5 = (AMHW_SIM->sopt5 & (~(1UL << 6))) |
                      (value << 6);
}

/**
 * \brief UART0引脚开漏使能
 * \return 无
 * \note
 */
am_static_inline
void amhw_sim_uart0_open_drain_enable (void)
{
    AMHW_SIM->sopt5 |= (1UL << 16);
}

/**
 * \brief UART0引脚开漏禁能
 * \return 无
 * \note
 */
am_static_inline
void amhw_sim_uart0_odrain_disable (void)
{
    AMHW_SIM->sopt5 &= ~(1UL << 16);
}

/**
 * \brief UART1引脚开漏使能
 * \return 无
 * \note
 */
am_static_inline
void amhw_sim_uart1_open_drain_enable (void)
{
    AMHW_SIM->sopt5 |= (1UL << 17);
}

/**
 * \brief UART1引脚开漏禁能
 * \return 无
 * \note
 */
am_static_inline
void amhw_sim_uart1_open_drain_disable (void)
{
    AMHW_SIM->sopt5 &= ~(1UL << 17);
}

/**
 * \brief UART0引脚开漏使能
 * \return 无
 * \note
 */
am_static_inline
void amhw_sim_uart2_open_drain_enable (void)
{
    AMHW_SIM->sopt5 |= (1UL << 18);
}

/**
 * \brief UART2引脚开漏禁能
 * \return 无
 * \note
 */
am_static_inline
void amhw_sim_uart2_open_drain_disable (void)
{
    AMHW_SIM->sopt5 &= ~(1UL << 18);
}

/**
 * \brief ADC0多触发源使能（默认TPM1通道0或通道1触发）
 * \return 无
 * \note
 */
am_static_inline
void amhw_sim_adc0_alter_trigger_enable (void)
{
    AMHW_SIM->sopt7 |= (1UL << 7);
}

/**
 * \brief ADC0多触发源禁能（选择为TPM1通道0或通道1触发）
 * \return 无
 * \note
 */
am_static_inline
void amhw_sim_adc0_alter_trigger_disable (void)
{
    AMHW_SIM->sopt7 &= ~(1UL << 7);
}

/**
 * \brief ADC0触发源TPM1通道选择
 * \param[in] value : 通道0或通道1
 * \return 无
 * \note  当ADC0多触发源禁能时
 */
am_static_inline
void amhw_sim_adc0_pretrigger_set (amhw_sim_adc0_pretrigger_t value)
{
    AMHW_SIM->sopt7 = (AMHW_SIM->sopt7 & (~(1UL << 4))) |
                      (value << 4);
}

/**
 * \brief ADC0触发源选择
 * \param[in] flags : ADC触发源
 * \return 无
 * \note
 */
am_static_inline
void amhw_sim_adc0_trigger_set (amhw_sim_adc0_trigger_t flags)
{
    AMHW_SIM->sopt7 = (AMHW_SIM->sopt7 & (~(0x0f << 0))) |
                      (flags << 0);
}

/**
 * \brief 系统设备信息寄存器
 *        可以获得芯片的所在系列，SRAM大小，引脚数目等信息
 * \return SDID寄存器值
 */
am_static_inline
uint32_t amhw_sim_sdid_get (void)
{
    return (AMHW_SIM->sdid);
}

/**
 * \brief 外设系统时钟使能
 * \param[in] periph : 外设门控
 * \return 无
 */
am_static_inline
void amhw_sim_periph_clock_enable (amhw_sim_scgc_periph_t periph)
{
    AMHW_SIM->scgc[periph >> 8] |= 1UL << (periph & 0xff);
}

/**
 * \brief 外设系统时钟禁能
 * \param[in] periph : 外设门控
 * \return 无
 */
am_static_inline
void amhw_sim_periph_clock_disable (amhw_sim_scgc_periph_t periph)
{
    AMHW_SIM->scgc[periph >> 8] &= ~(1UL << (periph & 0xff));
}

/**
 * \brief CORE_CLK时钟分频系数OUTDIV1设置
 * \param[in] value :OUTDIV1分频系数 0~15
 * \return 无
 */
am_static_inline
void amhw_sim_clkdiv1_outdiv1_set (uint32_t value)
{
    AMHW_SIM->clkdiv1 = (AMHW_SIM->clkdiv1 & (~(0x0fUL << 28))) |
                        (value << 28);
}

/**
 * \brief 获取CORE_CLK时钟分频系数OUTDIV1
 * \return 无
 */
am_static_inline
uint32_t amhw_sim_clkdiv1_outdiv1_get (void)
{
    return ((AMHW_SIM->clkdiv1 >> 28) & 0x0f);
}

/**
 * \brief BUS_CLK时钟分频系数OUTDIV4设置
 * \param[in] value :OUTDIV4分频系数 0~7
 * \return 无
 */
am_static_inline
void amhw_sim_clkdiv1_outdiv4_set (uint32_t value)
{
    AMHW_SIM->clkdiv1 = (AMHW_SIM->clkdiv1 & (~(7UL << 16))) |
                        (value << 16);
}

/**
 * \brief 获取BUS_CLK时钟分频系数OUTDIV4
 * \return 无
 */
am_static_inline
uint32_t amhw_sim_clkdiv1_outdiv4_get (void)
{
    return ((AMHW_SIM->clkdiv1 >> 16) & 0x07);
}

/**
 * \brief 使能flash
 * \return 无
 */
am_static_inline
void amhw_sim_flash_enable (void)
{
    AMHW_SIM->fcfg1 &= ~(1UL << 0) ;
}

/**
 * \brief 禁止能flash
 * \return 无
 */
am_static_inline
void amhw_sim_flash_disable (void)
{
    AMHW_SIM->fcfg1 |= (1UL << 0) ;
}

/**
 * \brief doze模式下使能flash
 * \return 无
 */
am_static_inline
void amhw_sim_flash_doze_enable (void)
{
    AMHW_SIM->fcfg1 &= ~(1UL << 1) ;
}

/**
 * \brief doze模式下禁能flash
 * \return 无
 */
am_static_inline
void amhw_sim_flash_doze_disable (void)
{
    AMHW_SIM->fcfg1 |= (1UL << 1) ;
}

/**
 * \brief 查询flash 大小
 * \retval 0 : 8kb flash
 * \retval 1 : 16kb flash
 * \retval 3 : 32kb flash
 * \retval 5 : 64kb flash
 * \retval 7 : 128kb flash
 * \retval 9 : 256kb flash
 * \retval 15 : 128kb flash
 */
am_static_inline
uint32_t amhw_sim_flash_size_get (void)
{
    return (AMHW_SIM->fcfg1 >> 24) & 0x0f;
}

/**
 * \brief 获取FCFG MAXADDR0值
 * \return MAXADDR0地址值
 */
am_static_inline
uint32_t amhw_sim_flash_maxaddr0_get (void)
{
    return (AMHW_SIM->fcfg2 >> 24) & 0x7f;
}

/**
 * \brief 获取FCFG MAXADDR1值
 * \return MAXADDR1地址值
 */
am_static_inline
uint32_t amhw_sim_flash_maxaddr1_get (void)
{
    return (AMHW_SIM->fcfg2 >> 16) & 0x7f;
}

/**
 * \brief 获取通用唯一标识符UID高位
 * \return UID 高位
 */
am_static_inline
uint32_t amhw_sim_uid_high_get (void)
{
    return (AMHW_SIM->uidmh & 0xFFFF);
}

/**
 * \brief 获取通用唯一标识符UID中低位
 * \return UID 中低位
 */
am_static_inline
uint32_t amhw_sim_uid_mid_get (void)
{
    return (AMHW_SIM->uidml);
}

/**
 * \brief 获取通用唯一标识符UID低位
 * \return UID 低位
 */
am_static_inline
uint32_t amhw_sim_uid_low_get (void)
{
    return (AMHW_SIM->uidl);
}

/**
 * \brief COP配置
 *
 * \param[in] mode    : 模式
 * \param[in] clk     : 时钟源
 * \param[in] timeout : 超时时间
 *
 * \return 无
 *
 * \note 复位之后只能写一次COPC
 */
am_static_inline
void amhw_sim_cop_cfg (amhw_sim_cop_mode_t    mode,
                       amhw_sim_cop_clk_t     clk,
                       amhw_sim_cop_timeout_t timeout)
{
    AMHW_SIM->copc = ((timeout << 2) | (clk << 1) | (mode << 0));
}

am_static_inline
amhw_sim_cop_clk_t amhw_sim_cop_src_get (void)
{
    return (amhw_sim_cop_clk_t)((AMHW_SIM->copc & 0x02)? 1 : 0);
}

/**
 * \brief COP服务操作函数（看门狗喂狗操作函数）
 *
 * \param[in] value :0x55或0xAA
 *
 * \return 无
 */
am_static_inline
void amhw_sim_srvcop_set (uint8_t value)
{
    AMHW_SIM->srvcop = value & 0xff;
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_SIM_H */

/* end of file */
