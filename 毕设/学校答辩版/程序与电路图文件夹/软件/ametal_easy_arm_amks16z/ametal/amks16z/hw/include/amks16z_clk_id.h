/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co. Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief KS16Z 时钟ID定义
 * 
 * \internal
 * \par Modification History
 * - 1.00 15-12-01  lnk first implementation.
 * \endinternal
 */
 
#ifndef __AMKS16Z_CLK_ID_H
#define __AMKS16Z_CLK_ID_H

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * \addtogroup ks16z_if_clockid
 * \copydoc amks16z_clk_id.h
 * @{
 */
 
 
/**
 * \name 系统或外设的时钟ID
 * \anchor grp_clk_id
 * @{
 */
 
#define CLK_SLOW_IRC    (4 << 8 | 0)
#define CLK_FAST_IRC    (4 << 8 | 1)
#define CLK_XTAL        (4 << 8 | 2)
#define CLK_CLKIN       (4 << 8 | 3)

#define CLK_CORE        (4 << 8 | 4)
#define CLK_SYSTEM      (4 << 8 | 5)
#define CLK_BUS         (4 << 8 | 6)
#define CLK_FLASH       (4 << 8 | 7)
#define CLK_PERIPH      (4 << 8 | 8)
#define CLK_SYSTIC      (4 << 8 | 9)

#define CLK_OSC32K      (4 << 8 | 10)
#define CLK_OSCERC      (4 << 8 | 11)

/* rtc out clocks*/
#define CLK_RTCOUT      (4 << 8 | 12)

/* mcg clocks*/
#define CLK_MCGFLL      (4 << 8 | 13)
#define CLK_MCGPLL      (4 << 8 | 14)
#define CLK_MCGEXT      (4 << 8 | 15)
#define CLK_MCGIRC      (4 << 8 | 16)
#define CLK_MCGOUT      (4 << 8 | 17)

/* LPO clock */
#define CLK_LPO         (4 << 8 | 18)
#define CLK_WDT         (4 << 8 | 19)

/* periph clk */
#define CLK_I2C0        (0 << 8 | 6)
#define CLK_I2C1        (0 << 8 | 7)
#define CLK_UART0       (0 << 8 | 10)
#define CLK_UART1       (0 << 8 | 11)
#define CLK_UART2       (0 << 8 | 12)
#define CLK_CMP         (0 << 8 | 19)
#define CLK_SPI0        (0 << 8 | 22)
#define CLK_SPI1        (0 << 8 | 23)
/** \brief  SCGC5 */
#define CLK_LPTMR       (1 << 8 | 0)
#define CLK_TSI         (1 << 8 | 5)
#define CLK_PORTA       (1 << 8 | 9)
#define CLK_PORTB       (1 << 8 | 10)
#define CLK_PORTC       (1 << 8 | 11)
#define CLK_PORTD       (1 << 8 | 12)
#define CLK_PORTE       (1 << 8 | 13)
/** \brief  SCGC6 */
#define CLK_FTF         (2 << 8 | 0)
#define CLK_DMAMUX      (2 << 8 | 1)
#define CLK_I2S         (2 << 8 | 15)
#define CLK_PIT         (2 << 8 | 23)
#define CLK_TPM0        (2 << 8 | 24)
#define CLK_TPM1        (2 << 8 | 25)
#define CLK_TPM2        (2 << 8 | 26)
#define CLK_ADC0        (2 << 8 | 27)
#define CLK_RTC         (2 << 8 | 29)
#define CLK_DAC0        (2 << 8 | 31)
/** \brief  SCGC7 */
#define CLK_DMA         (3 << 8 | 8)

#define CLK_PERIPH_ID_MIN CLK_I2C0
#define CLK_PERIPH_ID_MAX CLK_DMA

/**
 * @} 
 */ 
 
 
/**
 * @} ks16z_if_clockid
 */

#ifdef __cplusplus
}
#endif

#endif /* __KS16Z_CLK_ID_H */

/* end of file */
