/*******************************************************************************
*                                 Apollo
*                       ----------------------------
*                       innovating embedded platform
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
 * \brief AMKS16Z 中断号定义
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-16  xjc, first implementation.
 * \endinternal
 */


#ifndef __AMKS16Z_INUM_H
#define __AMKS16Z_INUM_H

#ifdef __cplusplus
extern "C" {

#endif

/**
 * \addtogroup amhw_if_inum
 * \copydoc amks16z_inum.h
 * @{
 */

/**
 * \name AMKS16Z CPU 中断号
 * @{
 */

#define INUM_DMA_CH0             0     /**< \brief DMA通道0中断               */
#define INUM_DMA_CH1             1     /**< \brief DMA通道1中断               */
#define INUM_DMA_CH2             2     /**< \brief DMA通道2中断               */
#define INUM_DMA_CH3             3     /**< \brief DMA通道3中断               */

#define INUM_FTFA                5     /**< \brief FAFT中断             */
#define INUM_PMC                 6     /**< \brief PMC中断               */
#define INUM_LLWU                7     /**< \brief LLWU中断             */
#define INUM_I2C0                8     /**< \brief I2C0中断             */
#define INUM_I2C1                9     /**< \brief I2C1中断             */
#define INUM_SPI0                10    /**< \brief SPI0中断             */
#define INUM_SPI1                11    /**< \brief SPI1中断             */
#define INUM_UART0               12    /**< \brief UART0中断           */
#define INUM_UART1               13    /**< \brief UART1中断           */
#define INUM_UART2               14    /**< \brief UART2中断           */
#define INUM_ADC0                15    /**< \brief ADC0中断             */
#define INUM_CMP0                16    /**< \brief CMP0中断             */
#define INUM_TPM0                17    /**< \brief TPM0中断             */
#define INUM_TPM1                18    /**< \brief TPM1中断             */
#define INUM_TPM2                19    /**< \brief TPM2中断             */
#define INUM_RTC_ALARM           20    /**< \brief RTC 闹钟中断      */
#define INUM_RTC_SEC             21    /**< \brief RTC秒中断           */
#define INUM_PIT                 22    /**< \brief PIT中断               */
#define INUM_I2S                 23    /**< \brief I2S中断               */

#define INUM_DAC0                25    /**< \brief DAC0中断             */
#define INUM_TSI0                26    /**< \brief TSI0中断             */
#define INUM_MCG                 27    /**< \brief MCG中断               */
#define INUM_LPTMR0              28    /**< \brief LPTMR0中断         */

#define INUM_PORTA               30    /**< \brief PORTA 引脚中断               */
#define INUM_PORTC_PORTD         31    /**< \brief PORTC或PORTD引脚中断    */
/** @} */




/**
 * \brief 总中断数为：(INUM_PORTC_PORTD - INUM_DMA_CH0 + 1),
 *
 */
#define INUM_INT_COUNT     (INUM_PORTC_PORTD - INUM_DMA_CH0 + 1)

/**
 * \brief 最大中断号为：INUM_PORTC_PORTD
 */
#define INUM_INT_MAX        INUM_PORTC_PORTD


/** \brief 最小中断号为：INUM_DMA_CH0 */
#define INUM_INT_MIN        INUM_DMA_CH0

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMKS16Z_INUM_H */

/* end of file */
