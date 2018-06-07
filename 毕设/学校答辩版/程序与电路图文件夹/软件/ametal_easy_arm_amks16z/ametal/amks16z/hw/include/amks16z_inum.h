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
 * \brief AMKS16Z �жϺŶ���
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
 * \name AMKS16Z CPU �жϺ�
 * @{
 */

#define INUM_DMA_CH0             0     /**< \brief DMAͨ��0�ж�               */
#define INUM_DMA_CH1             1     /**< \brief DMAͨ��1�ж�               */
#define INUM_DMA_CH2             2     /**< \brief DMAͨ��2�ж�               */
#define INUM_DMA_CH3             3     /**< \brief DMAͨ��3�ж�               */

#define INUM_FTFA                5     /**< \brief FAFT�ж�             */
#define INUM_PMC                 6     /**< \brief PMC�ж�               */
#define INUM_LLWU                7     /**< \brief LLWU�ж�             */
#define INUM_I2C0                8     /**< \brief I2C0�ж�             */
#define INUM_I2C1                9     /**< \brief I2C1�ж�             */
#define INUM_SPI0                10    /**< \brief SPI0�ж�             */
#define INUM_SPI1                11    /**< \brief SPI1�ж�             */
#define INUM_UART0               12    /**< \brief UART0�ж�           */
#define INUM_UART1               13    /**< \brief UART1�ж�           */
#define INUM_UART2               14    /**< \brief UART2�ж�           */
#define INUM_ADC0                15    /**< \brief ADC0�ж�             */
#define INUM_CMP0                16    /**< \brief CMP0�ж�             */
#define INUM_TPM0                17    /**< \brief TPM0�ж�             */
#define INUM_TPM1                18    /**< \brief TPM1�ж�             */
#define INUM_TPM2                19    /**< \brief TPM2�ж�             */
#define INUM_RTC_ALARM           20    /**< \brief RTC �����ж�      */
#define INUM_RTC_SEC             21    /**< \brief RTC���ж�           */
#define INUM_PIT                 22    /**< \brief PIT�ж�               */
#define INUM_I2S                 23    /**< \brief I2S�ж�               */

#define INUM_DAC0                25    /**< \brief DAC0�ж�             */
#define INUM_TSI0                26    /**< \brief TSI0�ж�             */
#define INUM_MCG                 27    /**< \brief MCG�ж�               */
#define INUM_LPTMR0              28    /**< \brief LPTMR0�ж�         */

#define INUM_PORTA               30    /**< \brief PORTA �����ж�               */
#define INUM_PORTC_PORTD         31    /**< \brief PORTC��PORTD�����ж�    */
/** @} */




/**
 * \brief ���ж���Ϊ��(INUM_PORTC_PORTD - INUM_DMA_CH0 + 1),
 *
 */
#define INUM_INT_COUNT     (INUM_PORTC_PORTD - INUM_DMA_CH0 + 1)

/**
 * \brief ����жϺ�Ϊ��INUM_PORTC_PORTD
 */
#define INUM_INT_MAX        INUM_PORTC_PORTD


/** \brief ��С�жϺ�Ϊ��INUM_DMA_CH0 */
#define INUM_INT_MIN        INUM_DMA_CH0

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMKS16Z_INUM_H */

/* end of file */
