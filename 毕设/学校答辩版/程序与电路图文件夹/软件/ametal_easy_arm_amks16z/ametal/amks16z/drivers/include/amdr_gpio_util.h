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
 * \brief GPIOƽ̨�������ߺ�
 *
 * ���ļ��������� \sa ks16xx_pin.h. �䶨����GPIOƽ̨���ŵĸ������ܡ�
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-18  wangxiaojian, first implementation.
 * \endinternal
 */

#ifndef __AMDR_GPIO_UTIL_H
#define __AMDR_GPIO_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"

/**
 * \addtogroup amdr_if_gpio_util
 * \copydoc amdr_gpio_util.h
 * @{
 */

/*
 * bit[11:6] ����
 * bit[31:12] ����ƽ̨����
 *
 */

/**
 * \name PORT_PCRn ����λ��
 * @{
 */

#define AMDR_PORT_PULLUP               AM_BIT(12) /**< \brief �ܽ�ѡ���ڲ�����ʹ�� */
#define AMDR_PORT_PULLEN               AM_BIT(13) /**< \brief �ܽ��ڲ���/����ʹ�� */
#define AMDR_PORT_SLEWSLOW             AM_BIT(14) /**< \brief �ܽŵ�ת���� */
#define AMDR_PORT_FILTEREN             AM_BIT(15) /**< \brief ��Դ�����˲���ʹ�� */
#define AMDR_PORT_DRIVERHIGH           AM_BIT(16) /**< \brief �ܽŸ������� */
#define AMDR_PORT_FUNEN                AM_BIT(17) /**< \brief �ܽŸ��ù���ʹ�� */

/*******************************************************************************/

/** \brief ��/����λƫ�� */
#define AMDR_GPIO_PULL_OFFSET          18

/** \brief ����/����ģʽ�� */
#define AMDR_GPIO_PULL_CODE(a)        (AM_SBF((a), AMDR_GPIO_PULL_OFFSET) | \
                                               AMDR_PORT_PULLUP)

/** \brief ��ȡ��/����ģʽ�� */
#define AMDR_GPIO_PULL_GET(data)       AM_BIT_GET((data), AMDR_GPIO_PULL_OFFSET)

/** \brief ��/��ʹ��λƫ�� */
#define AMDR_GPIO_PE_OFFSET            19

/** \brief ����/����ģʽ������ */
#define AMDR_GPIO_PE_CODE(a)          (AM_SBF((a), AMDR_GPIO_PE_OFFSET) | \
                                               AMDR_PORT_PULLEN)

/** \brief ��ȡ��/����ģʽ������ */
#define AMDR_GPIO_PE_GET(data)         AM_BIT_GET((data), AMDR_GPIO_PE_OFFSET)

/*******************************************************************************/

/** \brief ת����λƫ�� */
#define AMDR_GPIO_SRE_OFFSET           20

/** \brief ��ת������ */
#define AMDR_GPIO_SRE_CODE(a)         (AM_SBF((a), AMDR_GPIO_SRE_OFFSET) | \
                                               AMDR_PORT_SLEWSLOW)

/** \brief ��ȡת������ */
#define AMDR_GPIO_SRE_GET(data)        AM_BIT_GET((data), AMDR_GPIO_SRE_OFFSET)

/*******************************************************************************/

/** \brief ��Դ�˲���λƫ�� */
#define AMDR_GPIO_PFE_OFFSET           21

/** \brief ����Դ�˲����� */
#define AMDR_GPIO_PFE_CODE(a)         (AM_SBF((a), AMDR_GPIO_PFE_OFFSET) | \
                                               AMDR_PORT_FILTEREN)

/** \brief ��ȡ��Դ�˲����� */
#define AMDR_GPIO_PFE_GET(data)        AM_BIT_GET((data), AMDR_GPIO_PFE_OFFSET)

/*******************************************************************************/

/** \brief ��������λƫ�� */
#define AMDR_GPIO_DES_OFFSET           22

/** \brief ������������ */
#define AMDR_GPIO_DES_CODE(a)         (AM_SBF((a), AMDR_GPIO_DES_OFFSET) | \
                                               AMDR_PORT_DRIVERHIGH)

/** \brief ��ȡ���������� */
#define AMDR_GPIO_DES_GET(data)        AM_BIT_GET((data), AMDR_GPIO_DES_OFFSET)

/*******************************************************************************/

#define AMDR_GPIO_FUNC_BITS_START      23  /**< \brief �ܽŸ��ù�����ʼλ */
#define AMDR_GPIO_FUNC_BITS_LEN        3   /**< \brief �ܽŸ��ù����볤�� */

/** \brief �ùܽŸ��ù����� */
#define AMDR_GPIO_FUNC_CODE(a)         (AM_SBF((a), AMDR_GPIO_FUNC_BITS_START) | \
                                                AMDR_PORT_FUNEN)

/** \brief ��ȡ�ܽŸ��ù����� */
#define AMDR_GPIO_FUNC_GET(data)       (AM_BITS_GET((data), AMDR_GPIO_FUNC_BITS_START, \
                                                AMDR_GPIO_FUNC_BITS_LEN))

/** @} */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMDR_GPIO_UTIL_H */

/* end of file */
