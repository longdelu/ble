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
 * \brief GPIO平台辅助工具宏
 *
 * 该文件被包含于 \sa ks16xx_pin.h. 其定义了GPIO平台引脚的各个功能。
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
 * bit[11:6] 保留
 * bit[31:12] 用于平台定义
 *
 */

/**
 * \name PORT_PCRn 控制位域
 * @{
 */

#define AMDR_PORT_PULLUP               AM_BIT(12) /**< \brief 管脚选择内部上拉使能 */
#define AMDR_PORT_PULLEN               AM_BIT(13) /**< \brief 管脚内部上/下拉使能 */
#define AMDR_PORT_SLEWSLOW             AM_BIT(14) /**< \brief 管脚低转换率 */
#define AMDR_PORT_FILTEREN             AM_BIT(15) /**< \brief 无源输入滤波器使能 */
#define AMDR_PORT_DRIVERHIGH           AM_BIT(16) /**< \brief 管脚高驱动力 */
#define AMDR_PORT_FUNEN                AM_BIT(17) /**< \brief 管脚复用功能使能 */

/*******************************************************************************/

/** \brief 上/下拉位偏移 */
#define AMDR_GPIO_PULL_OFFSET          18

/** \brief 置上/下拉模式码 */
#define AMDR_GPIO_PULL_CODE(a)        (AM_SBF((a), AMDR_GPIO_PULL_OFFSET) | \
                                               AMDR_PORT_PULLUP)

/** \brief 获取上/下拉模式码 */
#define AMDR_GPIO_PULL_GET(data)       AM_BIT_GET((data), AMDR_GPIO_PULL_OFFSET)

/** \brief 上/下使能位偏移 */
#define AMDR_GPIO_PE_OFFSET            19

/** \brief 置上/下拉模式禁能码 */
#define AMDR_GPIO_PE_CODE(a)          (AM_SBF((a), AMDR_GPIO_PE_OFFSET) | \
                                               AMDR_PORT_PULLEN)

/** \brief 获取上/下拉模式禁能码 */
#define AMDR_GPIO_PE_GET(data)         AM_BIT_GET((data), AMDR_GPIO_PE_OFFSET)

/*******************************************************************************/

/** \brief 转换率位偏移 */
#define AMDR_GPIO_SRE_OFFSET           20

/** \brief 置转换率码 */
#define AMDR_GPIO_SRE_CODE(a)         (AM_SBF((a), AMDR_GPIO_SRE_OFFSET) | \
                                               AMDR_PORT_SLEWSLOW)

/** \brief 获取转换率码 */
#define AMDR_GPIO_SRE_GET(data)        AM_BIT_GET((data), AMDR_GPIO_SRE_OFFSET)

/*******************************************************************************/

/** \brief 无源滤波器位偏移 */
#define AMDR_GPIO_PFE_OFFSET           21

/** \brief 置无源滤波器码 */
#define AMDR_GPIO_PFE_CODE(a)         (AM_SBF((a), AMDR_GPIO_PFE_OFFSET) | \
                                               AMDR_PORT_FILTEREN)

/** \brief 获取无源滤波器码 */
#define AMDR_GPIO_PFE_GET(data)        AM_BIT_GET((data), AMDR_GPIO_PFE_OFFSET)

/*******************************************************************************/

/** \brief 驱动能力位偏移 */
#define AMDR_GPIO_DES_OFFSET           22

/** \brief 置驱动能力码 */
#define AMDR_GPIO_DES_CODE(a)         (AM_SBF((a), AMDR_GPIO_DES_OFFSET) | \
                                               AMDR_PORT_DRIVERHIGH)

/** \brief 获取驱动能力码 */
#define AMDR_GPIO_DES_GET(data)        AM_BIT_GET((data), AMDR_GPIO_DES_OFFSET)

/*******************************************************************************/

#define AMDR_GPIO_FUNC_BITS_START      23  /**< \brief 管脚复用功能起始位 */
#define AMDR_GPIO_FUNC_BITS_LEN        3   /**< \brief 管脚复用功能码长度 */

/** \brief 置管脚复用功能码 */
#define AMDR_GPIO_FUNC_CODE(a)         (AM_SBF((a), AMDR_GPIO_FUNC_BITS_START) | \
                                                AMDR_PORT_FUNEN)

/** \brief 获取管脚复用功能码 */
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
