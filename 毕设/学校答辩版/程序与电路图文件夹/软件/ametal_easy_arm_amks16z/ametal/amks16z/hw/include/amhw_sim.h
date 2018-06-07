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
 * \brief ϵͳ���ƽӿ�
 *
 * 1. ����ϵͳʱ���ſ�ʹ��/���ܣ�
 * 2. ����ʱ��Դѡ��
 * 3. UART TX/RX Դ��
 * 4. ADC ����Դ��
 * 5. �豸 ID �Ĵ�����
 * 6. COP���Ź�����
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
  * \brief ϵͳ���ƼĴ�����ṹ��
  */
typedef struct amhw_sim {
    __IO uint32_t sopt1;            /**< \brief ϵͳ���üĴ���1 */
    __I  uint32_t reserved0[1024];  /**< \brief ����                              */
    __IO uint32_t sopt2;            /**< \brief ϵͳ���üĴ���2 */
    __I  uint32_t reserved1[1];     /**< \brief ����                              */
    __IO uint32_t sopt4;            /**< \brief ϵͳ���üĴ���4 */
    __IO uint32_t sopt5;            /**< \brief ϵͳ���üĴ���5 */
    __I  uint32_t reserved2[1];     /**< \brief ����                              */
    __IO uint32_t sopt7;            /**< \brief ϵͳ���üĴ���7 */
    __I  uint32_t reserved3[2];     /**< \brief ����                              */
    __I  uint32_t sdid;             /**< \brief ϵͳ�豸ID�Ĵ��� */
    __I  uint32_t reserved4[3];     /**< \brief ����                              */
    __IO uint32_t scgc[4];          /**< \brief ϵͳʱ���ſؼĴ���     */
    __IO uint32_t clkdiv1;          /**< \brief ϵͳʱ�ӷ�Ƶ�Ĵ���1 */
    __I  uint32_t reserved5[1];     /**< \brief ����                              */
    __IO uint32_t fcfg1;            /**< \brief FLASH���üĴ���1 */
    __I  uint32_t fcfg2;            /**< \brief FLASH���üĴ���2 */
    __I  uint32_t reserved6[1];     /**< \brief ����                              */
    __I  uint32_t uidmh;            /**< \brief ΨһID�Ĵ���       */
    __I  uint32_t uidml;            /**< \brief ΨһID�Ĵ���       */
    __I  uint32_t uidl;             /**< \brief ΨһID�Ĵ���       */
    __I  uint32_t reserved7[39];    /**< \brief ����                              */
    __IO uint32_t copc;             /**< \brief ���Ź����ƼĴ���   */
    __O  uint32_t srvcop;           /**< \brief ι���Ĵ���         */
} amhw_sim_t;

/**
 * \brief ����ʱ���ſض���
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
 * \brief RTC��LPTMR��32Kʱ��ԴERCLK32K
 */
typedef enum amhw_sim_ercclk32k_src {
    AMHW_SIM_OSC32KSEL_SYS   = 0,    /**< \brief OSC32KCLK  */
    AMHW_SIM_OSC32KSEL_CLKIN = 2 ,   /**< \brief RTC_CLKIN  */
    AMHW_SIM_OSC32KSEL_LPO   = 3,    /**< \brief LPO 1KHZ   */
} amhw_sim_ercclk32k_src_t;

/**
 * \brief RTC_CLKOUT����ʱ�����Դ
 */
typedef enum amhw_sim_rtc_clkout {
    AMHW_SIM_RTC_CLKOUT_RTC      = 0,   /**< \brief RTC 1HZ   */
    AMHW_SIM_RTC_CLKOUT_OSCERCLK = 1,   /**< \brief OSCERCLK   */
} amhw_sim_rtc_clkout_t;

/**
 * \brief CLKOUT����ʱ�����Դ
 */
typedef enum amhw_sim_clkout {
    AMHW_SIM_CLKOUT_BUSCLK   = 2,      /**< \brief BUSʱ��   */
    AMHW_SIM_CLKOUT_LPOCLK   = 3,      /**< \brief LPO 1HZ   */
    AMHW_SIM_CLKOUT_MCGIRCLK = 4,      /**< \brief MCGIRCLK  */
    AMHW_SIM_CLKOUT_OSCERCLK = 6,      /**< \brief OSCERCLK  */
} amhw_sim_clkout_t;

/**
 * \brief ����MCGPLLCLK/MCGFLLCLKʱ��ѡ��
 * \note  ����ʹ��MCGFLLCLKʱ�ӣ�MCGFLLCLKĬ��2��Ƶ��ʹ��
 */
typedef enum amhw_sim_fllpll_sel {
    AMHW_SIM_PLLFLLSEL_FLL = 0,    /**< \brief FLL           */
    AMHW_SIM_PLLFLLSEL_PLL = 1,    /**< \brief PLL           */
} amhw_sim_fllpll_sel_t;

/**
 * \brief TPMʱ��Դѡ��
 * \note ѡ��PLL/FLLԴʱ��Ҫ��ʹ��amhw_sim_periph_pllfll_set����
 */
typedef enum amhw_sim_tpm_src {
    AMHW_SIM_TPMSRC_DISABLED  = 0,    /**< \brief DISABLED   */
    AMHW_SIM_TPMSRC_PLLFLLCLK = 1,    /**< \brief PLL/FLL    */
    AMHW_SIM_TPMSRC_OSCERCLK  = 2,    /**< \brief OSCERCLK   */
    AMHW_SIM_TPMSRC_MCGIRCLK  = 3,    /**< \brief MCGIRCLK   */
} amhw_sim_tpm_src_t;

/**
 * \brief UART0ʱ��Դѡ��
 * \note ѡ��PLL/FLLԴʱ��Ҫ��ʹ�� amhw_sim_periph_pllfll_set ����
 */
typedef enum amhw_sim_uart0_src {
    AMHW_SIM_UART0SRC_DISABLED  = 0,    /**< \brief DISABLED   */
    AMHW_SIM_UART0SRC_PLLFLLCLK = 1,    /**< \brief PLL/FLL    */
    AMHW_SIM_UART0SRC_OSCERCLK  = 2,    /**< \brief OSCERCLK   */
    AMHW_SIM_UART0SRC_MCGIRCLK  = 3,    /**< \brief MCGIRCLK   */
} amhw_sim_uart0_src_t;

/**
 * \brief TPM1ͨ��0����Դѡ��
 */
typedef enum amhw_sim_tpm1ch0_src {
    AMHW_SIM_TPM1CH0SRC_TPM1CH0 = 0,    /**< \brief TPM1_CH0   */
    AMHW_SIM_TPM1CH0SRC_CMP0    = 1,    /**< \brief CMP output */
} amhw_sim_tpm1ch0_src_t;

/**
 * \brief TPM2ͨ��0����Դѡ��
 */
typedef enum amhw_sim_tpm2ch0_src {
    AMHW_SIM_TPM2CH0SRC_TPM2CH0 = 0,    /**< \brief TPM1_CH0   */
    AMHW_SIM_TPM2CH0SRC_CMP0    = 1,    /**< \brief CMP output */
} amhw_sim_tpm2ch0_src_t;

/**
 * \brief TPM0�ⲿʱ����������ѡ��
 */
typedef enum amhw_sim_tpm0_clkin {
    AMHW_SIM_TPM0_CLKIN_0 = 0,    /**< \brief TPM0_CLKIN_0����  */
    AMHW_SIM_TPM0_CLKIN_1 = 1,    /**< \brief TPM0_CLKIN_1����  */
} amhw_sim_tpm0_clkin_t;

/**
 * \brief TPM1�ⲿʱ����������ѡ��
 */
typedef enum amhw_sim_tpm1_clkin {
    AMHW_SIM_TPM1_CLKIN_0 = 0,    /**< \brief TPM1_CLKIN_0����  */
    AMHW_SIM_TPM1_CLKIN_1 = 1,    /**< \brief TPM1_CLKIN_1����  */
} amhw_sim_tpm1_clkin_t;

/**
 * \brief TPM2�ⲿʱ����������ѡ��
 */
typedef enum amhw_sim_tpm2_clkin {
    AMHW_SIM_TPM2_CLKIN_0 = 0,    /**< \brief TPM2_CLKIN_0����  */
    AMHW_SIM_TPM2_CLKIN_1 = 1,    /**< \brief TPM2_CLKIN_1����  */
} amhw_sim_tpm2_clkin_t;

/**
 * \brief UART0��������TXԴ
 */
typedef enum amhw_sim_uart0_tx_src {
    AMHW_SIM_UART0_TXSRC_TX      = 0,    /**< \brief TX����  */
    AMHW_SIM_UART0_TXSRC_TX_TPM1 = 1,    /**< \brief TX���ż���TPM1����  */
    AMHW_SIM_UART0_TXSRC_TX_TPM2 = 2,    /**< \brief TX���ż���TPM2����  */
} amhw_sim_uart0_tx_src_t;

/**
 * \brief UART0��������TXԴ
 */
typedef enum amhw_sim_uart0_rx_src {
    AMHW_SIM_UART0_TXSRC_RX   = 0,      /**< \brief RX����       */
    AMHW_SIM_UART0_TXSRC_CMP0 = 1,      /**< \brief CMP0���  */
} amhw_sim_uart0_rx_src_t;

/**
 * \brief UART1��������TXԴ
 */
typedef enum amhw_sim_uart1_tx_src {
    AMHW_SIM_UART1_TXSRC_TX      = 0,    /**< \brief TX����  */
    AMHW_SIM_UART1_TXSRC_TX_TPM1 = 1,    /**< \brief TX���ż���TPM1����  */
    AMHW_SIM_UART1_TXSRC_TX_TPM2 = 2,    /**< \brief TX���ż���TPM2����  */
} amhw_sim_uart1_tx_src_t;

/**
 * \brief UART1��������TXԴ
 */
typedef enum amhw_sim_uart1_rx_src {
    AMHW_SIM_UART1_TXSRC_RX   = 0,      /**< \brief RX����       */
    AMHW_SIM_UART1_TXSRC_CMP0 = 1,      /**< \brief CMP0���  */
} amhw_sim_uart1_rx_src_t;

/**
 * \brief ADC0����ԴTPM1ͨ��ѡ��
 */
typedef enum amhw_sim_adc0_pretrigger {
    AMHW_SIM_ADC0_PRE_TRIGGER_A = 0,    /**< \brief ��ʱ��1ͨ��0  */
    AMHW_SIM_ADC0_PRE_TRIGGER_B = 1,    /**< \brief ��ʱ��1ͨ��1  */
} amhw_sim_adc0_pretrigger_t;

/**
 * \brief ADC0����Դѡ��
 */
typedef enum amhw_sim_adc0_trigger {
    AMHW_SIM_ADC0_TRIGGER_EXTRG  = 0,    /**< \brief �ⲿ������������ */
    AMHW_SIM_ADC0_TRIGGER_CMP0   = 1,    /**< \brief CMP0 OUTPUT    */
    AMHW_SIM_ADC0_TRIGGER_PIT0   = 4,    /**< \brief PIT ����0      */
    AMHW_SIM_ADC0_TRIGGER_PIT1   = 5,    /**< \brief PIT ����1      */
    AMHW_SIM_ADC0_TRIGGER_TPM0   = 8,    /**< \brief ��ʱ��0���           */
    AMHW_SIM_ADC0_TRIGGER_TPM1   = 9,    /**< \brief ��ʱ��1���           */
    AMHW_SIM_ADC0_TRIGGER_TPM2   = 10,   /**< \brief ��ʱ��2���           */
    AMHW_SIM_ADC0_TRIGGER_RTCALR = 12,   /**< \brief RTC����                    */
    AMHW_SIM_ADC0_TRIGGER_RTCSEC = 13,   /**< \brief RTC��                         */
    AMHW_SIM_ADC0_TRIGGER_LPTMR0 = 14,   /**< \brief LPTMR0����            */
} amhw_sim_adc0_trigger_t;

/**
 * \brief COP���Ź���ʱʱ��
 */
typedef enum amhw_sim_cop_timeout {
    AMHW_SIM_COP_DISABLED      = 0,   /**< \brief COP ����  */
    AMHW_SIM_COP_TIMOUT_VALUE1 = 1,   /**< \brief 2��5�η���LPO���ڻ�2��13�η�BUS���� */
    AMHW_SIM_COP_TIMOUT_VALUE2 = 2,   /**< \brief 2��8�η���LPO���ڻ�2��16�η�BUS���� */
    AMHW_SIM_COP_TIMOUT_VALUE3 = 3,   /**< \brief 2��10�η���LPO���ڻ�2��18�η�BUS���� */
} amhw_sim_cop_timeout_t;

/**
 * \brief COP���Ź�ʱ��ѡ��
 */
typedef enum amhw_sim_cop_clk {
    AMHW_SIM_COP_CLK_INTERNAL = 0,    /**< \brief �ڲ�1KHZʱ��  */
    AMHW_SIM_COP_CLK_BUS      = 1,    /**< \brief ����ʱ�� */
} amhw_sim_cop_clk_t;

/**
 * \brief COP���Ź�ģʽ
 */
typedef enum amhw_sim_cop_mode {
    AMHW_SIM_COP_MODE_NORMAL = 0,    /**< \brief ����ģʽ  */
    AMHW_SIM_COP_MODE_WINDOW = 1,    /**< \brief ����ģʽ */
} amhw_sim_cop_mode_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * \brief ERC32KCLKʱ��Դѡ��
 * \param[in] value : ERC32KCLKʱ��Դ
 * \return ��
 */
am_static_inline
void amhw_sim_ercclk32k_src_set (amhw_sim_ercclk32k_src_t value)
{
    AMHW_SIM->sopt1 = (AMHW_SIM->sopt1 & (~(0x03 << 18))) |
                      ((value & 0x03) << 18);
}

/**
 * \brief ��ȡERC32KCLKʱ��Դ
 * \return ERC32KCLKʱ��Դ
 */
am_static_inline
amhw_sim_ercclk32k_src_t amhw_sim_ercclk32k_src_get (void)
{
    return (amhw_sim_ercclk32k_src_t)((AMHW_SIM->sopt1 >> 18) & 0x03);
}

/**
 * \brief RTC_CLKOUT����ʱ�����Դѡ��
 * \param[in] value : ���ʱ��Դ
 * \return ��
 */
am_static_inline
void amhw_sim_rtc_clkout_set (amhw_sim_rtc_clkout_t value)
{
    AMHW_SIM->sopt2 = (AMHW_SIM->sopt2 & (~(1UL << 4))) |
                      (value << 4);
}

/**
 * \brief ��ȡRTC_CLKOUT����ʱ�����Դ
 * \return ��
 */
am_static_inline
amhw_sim_rtc_clkout_t amhw_sim_rtc_clkout_get (void)
{
    return (amhw_sim_rtc_clkout_t)((AMHW_SIM->sopt2 >> 4) & 0x01);
}

/**
 * \brief CLKOUT����ʱ�����ѡ��
 * \param[in] value : CLK���ʱ��
 * \return ��
 */
am_static_inline
void amhw_sim_clkout_set (amhw_sim_clkout_t value)
{
    AMHW_SIM->sopt2 = (AMHW_SIM->sopt2 & (~(7UL << 5))) |
                      (value << 5);
}

/**
 * \brief ����MCGPLLCLK/MCGFLLCLKʱ��ѡ��
 * \param[in] value : PLL/FLLʱ��
 * \return ��
 */
am_static_inline
void amhw_sim_pllfll_sel_set (amhw_sim_fllpll_sel_t value)
{
    AMHW_SIM->sopt2 = (AMHW_SIM->sopt2 & (~(1UL << 16))) |
                      (value << 16);
}

/**
 * \brief ��ȡ����MCGPLLCLK/MCGFLLCLKʱ��Դ
 * \return ����MCGPLLCLK/MCGFLLCLKʱ��Դ
 */
am_static_inline
amhw_sim_fllpll_sel_t amhw_sim_pllfll_sel_get (void)
{
    return (amhw_sim_fllpll_sel_t)((AMHW_SIM->sopt2 >> 16) & 0x01);
}

/**
 * \brief TPMʱ��Դѡ��
 * \param[in] value : TPMʱ��Դ
 * \return ��
 */
am_static_inline
void amhw_sim_tpm_src_set (amhw_sim_tpm_src_t value)
{
    AMHW_SIM->sopt2 = (AMHW_SIM->sopt2 & (~(3UL << 24))) |
                      (value << 24);
}

/**
 * \brief TPMʱ��Դ��ȡ
 * \return TPMʱ��Դ
 */
am_static_inline
amhw_sim_tpm_src_t amhw_sim_tpm_src_get (void)
{
    return (amhw_sim_tpm_src_t)((AMHW_SIM->sopt2 >> 24) & 0x03);
}

/**
 * \brief UART0ʱ��Դѡ��
 * \param[in] value : UART0ʱ��Դ
 * \return ��
 */
am_static_inline
void amhw_sim_uart0_src_set (amhw_sim_uart0_src_t value)
{
    AMHW_SIM->sopt2 = (AMHW_SIM->sopt2 & (~(3UL << 26))) |
                      (value << 26);
}

/**
 * \brief UART0ʱ��Դ��ȡ
 * \return UART0ʹ�õ�ʱ��Դ
 */
am_static_inline
amhw_sim_uart0_src_t amhw_sim_uart0_src_get (void)
{
    return (amhw_sim_uart0_src_t)((AMHW_SIM->sopt2 >> 26) & 0x03);
}

/**
 * \brief TPM1ͨ��0����Դѡ��
 * \param[in] value : TPM1ͨ��0����Դ
 * \return ��
 * \note ��TPM1��ʹ�����벶��ģʽʱ�����Щλ
 */
am_static_inline
void amhw_sim_tpm1ch0_src_set (amhw_sim_tpm1ch0_src_t value)
{
    AMHW_SIM->sopt4 = (AMHW_SIM->sopt4 & (~(3UL << 18))) |
                      (value << 18);
}

/**
 * \brief TPM2ͨ��0����Դѡ��
 * \param[in] value : TPM1ͨ��0����Դ
 * \return ��
 * \note ��TPM2��ʹ�����벶��ģʽʱ�����Щλ
 */
am_static_inline
void amhw_sim_tpm2ch0_src_set (amhw_sim_tpm2ch0_src_t value)
{
    AMHW_SIM->sopt4 = (AMHW_SIM->sopt4 & (~(1UL << 20))) |
                      (value << 20);
}

/**
 * \brief TPM0�ⲿʱ������ѡ��
 * \param[in] value :�ⲿʱ������
 * \return ��
 * \note ��Щѡ������ű���ͨ���������üĴ������ó�TPMʱ�����Ź���
 */
am_static_inline
void amhw_sim_tpm0_clkin_set (amhw_sim_tpm0_clkin_t value)
{
    AMHW_SIM->sopt4 = (AMHW_SIM->sopt4 & (~(1UL << 24))) |
                      (value << 24);
}

/**
 * \brief TPM1�ⲿʱ������ѡ��
 * \param[in] value :�ⲿʱ������
 * \return ��
 * \note ��Щѡ������ű���ͨ���������üĴ������ó�TPMʱ�����Ź���
 */
am_static_inline
void amhw_sim_tpm1_clkin_set (amhw_sim_tpm1_clkin_t value)
{
    AMHW_SIM->sopt4 = (AMHW_SIM->sopt4 & (~(1UL << 25))) |
                      (value << 25);
}

/**
 * \brief TPM2�ⲿʱ������ѡ��
 * \param[in] value :�ⲿʱ������
 * \return ��
 * \note ��Щѡ������ű���ͨ���������üĴ������ó�TPMʱ�����Ź���
 */
am_static_inline
void amhw_sim_tpm2_clkin_set (amhw_sim_tpm2_clkin_t value)
{
    AMHW_SIM->sopt4 = (AMHW_SIM->sopt4 & (~(1UL << 26))) |
                      (value << 26);
}

/**
 * \brief  UART0 ��������TXԴ����
 * \param[in] value : TX����Դ
 * \return ��
 * \note
 */
am_static_inline
void amhw_sim_uart0_tx_src_set (amhw_sim_uart0_tx_src_t value)
{
    AMHW_SIM->sopt5 = (AMHW_SIM->sopt5 & (~(3UL << 0))) |
                      (value << 0);
}

/**
 * \brief  UART0 ��������RXԴ����
 * \param[in] value : RX����Դ
 * \return ��
 * \note
 */
am_static_inline
void amhw_sim_uart0_rx_src_set (amhw_sim_uart0_rx_src_t value)
{
    AMHW_SIM->sopt5 = (AMHW_SIM->sopt5 & (~(1UL << 2))) |
                      (value << 2);
}

/**
 * \brief  UART1 ��������TXԴ����
 * \param[in] value : TX����Դ
 * \return ��
 * \note
 */
am_static_inline
void amhw_sim_uart1_tx_src_set (amhw_sim_uart1_tx_src_t value)
{
    AMHW_SIM->sopt5 = (AMHW_SIM->sopt5 & (~(3UL << 4))) |
                      (value << 4);
}

/**
 * \brief  UART1 ��������RXԴ����
 * \param[in] value : RX����Դ
 * \return ��
 * \note
 */
am_static_inline
void amhw_sim_uart1_rx_src_set (amhw_sim_uart1_rx_src_t value)
{
    AMHW_SIM->sopt5 = (AMHW_SIM->sopt5 & (~(1UL << 6))) |
                      (value << 6);
}

/**
 * \brief UART0���ſ�©ʹ��
 * \return ��
 * \note
 */
am_static_inline
void amhw_sim_uart0_open_drain_enable (void)
{
    AMHW_SIM->sopt5 |= (1UL << 16);
}

/**
 * \brief UART0���ſ�©����
 * \return ��
 * \note
 */
am_static_inline
void amhw_sim_uart0_odrain_disable (void)
{
    AMHW_SIM->sopt5 &= ~(1UL << 16);
}

/**
 * \brief UART1���ſ�©ʹ��
 * \return ��
 * \note
 */
am_static_inline
void amhw_sim_uart1_open_drain_enable (void)
{
    AMHW_SIM->sopt5 |= (1UL << 17);
}

/**
 * \brief UART1���ſ�©����
 * \return ��
 * \note
 */
am_static_inline
void amhw_sim_uart1_open_drain_disable (void)
{
    AMHW_SIM->sopt5 &= ~(1UL << 17);
}

/**
 * \brief UART0���ſ�©ʹ��
 * \return ��
 * \note
 */
am_static_inline
void amhw_sim_uart2_open_drain_enable (void)
{
    AMHW_SIM->sopt5 |= (1UL << 18);
}

/**
 * \brief UART2���ſ�©����
 * \return ��
 * \note
 */
am_static_inline
void amhw_sim_uart2_open_drain_disable (void)
{
    AMHW_SIM->sopt5 &= ~(1UL << 18);
}

/**
 * \brief ADC0�ഥ��Դʹ�ܣ�Ĭ��TPM1ͨ��0��ͨ��1������
 * \return ��
 * \note
 */
am_static_inline
void amhw_sim_adc0_alter_trigger_enable (void)
{
    AMHW_SIM->sopt7 |= (1UL << 7);
}

/**
 * \brief ADC0�ഥ��Դ���ܣ�ѡ��ΪTPM1ͨ��0��ͨ��1������
 * \return ��
 * \note
 */
am_static_inline
void amhw_sim_adc0_alter_trigger_disable (void)
{
    AMHW_SIM->sopt7 &= ~(1UL << 7);
}

/**
 * \brief ADC0����ԴTPM1ͨ��ѡ��
 * \param[in] value : ͨ��0��ͨ��1
 * \return ��
 * \note  ��ADC0�ഥ��Դ����ʱ
 */
am_static_inline
void amhw_sim_adc0_pretrigger_set (amhw_sim_adc0_pretrigger_t value)
{
    AMHW_SIM->sopt7 = (AMHW_SIM->sopt7 & (~(1UL << 4))) |
                      (value << 4);
}

/**
 * \brief ADC0����Դѡ��
 * \param[in] flags : ADC����Դ
 * \return ��
 * \note
 */
am_static_inline
void amhw_sim_adc0_trigger_set (amhw_sim_adc0_trigger_t flags)
{
    AMHW_SIM->sopt7 = (AMHW_SIM->sopt7 & (~(0x0f << 0))) |
                      (flags << 0);
}

/**
 * \brief ϵͳ�豸��Ϣ�Ĵ���
 *        ���Ի��оƬ������ϵ�У�SRAM��С��������Ŀ����Ϣ
 * \return SDID�Ĵ���ֵ
 */
am_static_inline
uint32_t amhw_sim_sdid_get (void)
{
    return (AMHW_SIM->sdid);
}

/**
 * \brief ����ϵͳʱ��ʹ��
 * \param[in] periph : �����ſ�
 * \return ��
 */
am_static_inline
void amhw_sim_periph_clock_enable (amhw_sim_scgc_periph_t periph)
{
    AMHW_SIM->scgc[periph >> 8] |= 1UL << (periph & 0xff);
}

/**
 * \brief ����ϵͳʱ�ӽ���
 * \param[in] periph : �����ſ�
 * \return ��
 */
am_static_inline
void amhw_sim_periph_clock_disable (amhw_sim_scgc_periph_t periph)
{
    AMHW_SIM->scgc[periph >> 8] &= ~(1UL << (periph & 0xff));
}

/**
 * \brief CORE_CLKʱ�ӷ�Ƶϵ��OUTDIV1����
 * \param[in] value :OUTDIV1��Ƶϵ�� 0~15
 * \return ��
 */
am_static_inline
void amhw_sim_clkdiv1_outdiv1_set (uint32_t value)
{
    AMHW_SIM->clkdiv1 = (AMHW_SIM->clkdiv1 & (~(0x0fUL << 28))) |
                        (value << 28);
}

/**
 * \brief ��ȡCORE_CLKʱ�ӷ�Ƶϵ��OUTDIV1
 * \return ��
 */
am_static_inline
uint32_t amhw_sim_clkdiv1_outdiv1_get (void)
{
    return ((AMHW_SIM->clkdiv1 >> 28) & 0x0f);
}

/**
 * \brief BUS_CLKʱ�ӷ�Ƶϵ��OUTDIV4����
 * \param[in] value :OUTDIV4��Ƶϵ�� 0~7
 * \return ��
 */
am_static_inline
void amhw_sim_clkdiv1_outdiv4_set (uint32_t value)
{
    AMHW_SIM->clkdiv1 = (AMHW_SIM->clkdiv1 & (~(7UL << 16))) |
                        (value << 16);
}

/**
 * \brief ��ȡBUS_CLKʱ�ӷ�Ƶϵ��OUTDIV4
 * \return ��
 */
am_static_inline
uint32_t amhw_sim_clkdiv1_outdiv4_get (void)
{
    return ((AMHW_SIM->clkdiv1 >> 16) & 0x07);
}

/**
 * \brief ʹ��flash
 * \return ��
 */
am_static_inline
void amhw_sim_flash_enable (void)
{
    AMHW_SIM->fcfg1 &= ~(1UL << 0) ;
}

/**
 * \brief ��ֹ��flash
 * \return ��
 */
am_static_inline
void amhw_sim_flash_disable (void)
{
    AMHW_SIM->fcfg1 |= (1UL << 0) ;
}

/**
 * \brief dozeģʽ��ʹ��flash
 * \return ��
 */
am_static_inline
void amhw_sim_flash_doze_enable (void)
{
    AMHW_SIM->fcfg1 &= ~(1UL << 1) ;
}

/**
 * \brief dozeģʽ�½���flash
 * \return ��
 */
am_static_inline
void amhw_sim_flash_doze_disable (void)
{
    AMHW_SIM->fcfg1 |= (1UL << 1) ;
}

/**
 * \brief ��ѯflash ��С
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
 * \brief ��ȡFCFG MAXADDR0ֵ
 * \return MAXADDR0��ֵַ
 */
am_static_inline
uint32_t amhw_sim_flash_maxaddr0_get (void)
{
    return (AMHW_SIM->fcfg2 >> 24) & 0x7f;
}

/**
 * \brief ��ȡFCFG MAXADDR1ֵ
 * \return MAXADDR1��ֵַ
 */
am_static_inline
uint32_t amhw_sim_flash_maxaddr1_get (void)
{
    return (AMHW_SIM->fcfg2 >> 16) & 0x7f;
}

/**
 * \brief ��ȡͨ��Ψһ��ʶ��UID��λ
 * \return UID ��λ
 */
am_static_inline
uint32_t amhw_sim_uid_high_get (void)
{
    return (AMHW_SIM->uidmh & 0xFFFF);
}

/**
 * \brief ��ȡͨ��Ψһ��ʶ��UID�е�λ
 * \return UID �е�λ
 */
am_static_inline
uint32_t amhw_sim_uid_mid_get (void)
{
    return (AMHW_SIM->uidml);
}

/**
 * \brief ��ȡͨ��Ψһ��ʶ��UID��λ
 * \return UID ��λ
 */
am_static_inline
uint32_t amhw_sim_uid_low_get (void)
{
    return (AMHW_SIM->uidl);
}

/**
 * \brief COP����
 *
 * \param[in] mode    : ģʽ
 * \param[in] clk     : ʱ��Դ
 * \param[in] timeout : ��ʱʱ��
 *
 * \return ��
 *
 * \note ��λ֮��ֻ��дһ��COPC
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
 * \brief COP����������������Ź�ι������������
 *
 * \param[in] value :0x55��0xAA
 *
 * \return ��
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
