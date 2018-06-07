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
 * \brief RTC 模块的驱动层接口
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-19  zsp, first implementation.
 * \endinternal
 */

#ifndef __AMDR_RTC_H
#define __AMDR_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"
#include "am_rtc.h"
#include "amhw_rtc.h"

/**
 * \addtogroup amdr_if_rtc
 * \copydoc amdr_rtc.h
 * @{
 */

/**
 * \brief RTC 设备信息结构体
 */
typedef struct amdr_rtc_devinfo {
    amhw_rtc_t  *p_hw_rtc;            /**< \brief 指向RTC寄存器块的结构体指针 */
} amdr_rtc_devinfo_t;

/**
 * \brief RTC 设备结构体
 */
typedef struct amdr_rtc_dev {
    am_rtc_serv_t             rtc_serv;     /**< \brief RTC标准服务           */
    const amdr_rtc_devinfo_t *p_devinfo;    /**< \brief 指向RTC设备信息的指针 */
} amdr_rtc_dev_t;

/**
 * \brief 初始化RTC
 *
 * \param[in] p_dev     : 指向RTC设备
 * \param[in] p_devinfo : 指向RTC设备信息
 *
 * \return RTC标准服务操作句柄。如果为 NULL，表明初始化失败。
 */
am_rtc_handle_t amdr_rtc_init (amdr_rtc_dev_t           *p_dev,
                               const amdr_rtc_devinfo_t *p_devinfo);

/**
 *  \brief 解除RTC初始化
 *
 *  \param p_dev 指向RTC设备的指针
 *
 *  \return 无
 */
void amdr_rtc_deinit (amdr_rtc_dev_t *p_dev);

/**
 *  \brief 根据日期计算一年中的第几天
 *
 *  \param day
 *  \param month
 *  \param year
 *
 *  \return 一年中的第几天 (1月1日记为第0天)
 *
 *  \details 如 amdr_rtc_date2yday(4, 11, 2015)计算的是11月4日是
 *           2015年的第307天
 */
int amdr_rtc_date2yday (uint8_t day, uint8_t month, uint32_t year);


/**
 *  \brief 设置RTC的校准配置
 *
 *  \param p_dev 指向RTC设备的指针
 *
 *  \param frq RTC时钟的实际频率，以Hz计
 *
 *  \return 实际校准后的频率
 */
int amdr_rtc_compensation_set (amdr_rtc_dev_t *p_dev, float frq);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /** __AMDR_RTC_H */

