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
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief KS16 SPI�û�����ͷ�ļ�
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-10-23  liqing, first implementation.
 * \endinternal
 */
 
#ifndef __AMDR_HWCONFIG_SPI_H
#define __AMDR_HWCONFIG_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"


extern const amdr_spi_devinfo_t g_spi0_devinfo; /**< \brief SPI0 �豸��Ϣ */
extern amdr_spi_dev_t           g_spi0_dev;     /**< \brief SPI0 �豸     */

/**
 * \brief SPI0 ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_spi0_init(void);

/**
 * \brief ���SPI0 ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_spi0_deinit(void);


extern const amdr_spi_devinfo_t  g_spi1_devinfo; /**< \brief SPI1 �豸��Ϣ */
extern amdr_spi_dev_t            g_spi1_dev;     /**< \brief SPI1 �豸     */

/**
 * \brief SPI1 ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_spi1_init(void);

/**
 * \brief SPI1 ƽ̨��ʼ��
 * \return ��
 */
void amhw_plfm_spi1_deinit(void);

#ifdef __cplusplus
}
#endif

#endif  /* __AMDR_HWCONFIG_SPI_H */

/* end of file */
