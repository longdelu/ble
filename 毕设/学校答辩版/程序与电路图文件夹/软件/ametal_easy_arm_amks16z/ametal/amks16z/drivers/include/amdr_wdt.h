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
 * \brief  WWDT驱动，服务WWDT标准接口
 *
 * 1. 看门狗的配置寄存器只能写一次，所以在使用KS16XX芯片时，当不需要使
 *    用看门狗外设时将工程配置文件里的看门狗开关宏置为0，系统将在板级初
 *    始化时关闭看门狗，当需要使用看门狗外设时，将看门狗开关宏置1，系统将
 *    在看门狗使能函数中配置看门狗，使之正常工作。
 * 2. 看门狗的超时时间只能设置为32ms,256ms,1024ms这三个时间中的一个。
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-23  wxj, first implementation.
 * \endinternal
 */

#ifndef __AMDR_WDT_H
#define __AMDR_WDT_H


#ifdef __cplusplus
extern "C" {
#endif

#include "am_wdt.h"
#include "amhw_sim.h"

/**
 * \addtogroup amdr_if_wdt
 * \copydoc amdr_wdt.h
 * @{
 */

/**
 * \brief WDT 设备信息结构体
 */
typedef struct amdr_wdt_devinfo {
    amhw_sim_t *p_hw_wdt;             /**< \brief 指向WDT寄存器块的结构体指针 */
} amdr_wdt_devinfo_t;

/**
 * \brief WDT 设备结构体
 */
typedef struct amdr_wdt_dev {

    am_wdt_serv_t              wdt_serv;   /**< \brief WDT标准服务         */
    const amdr_wdt_devinfo_t  *p_devinfo;  /**< \brief 指向WDT设备信息的指针 */
} amdr_wdt_dev_t;

/**
 * \brief 初始化WDT
 *
 * \param[in] p_dev     : 指向WDT设备
 * \param[in] p_devinfo : 指向WDT设备信息
 *
 * \return WDT标准服务操作句柄。如果为 NULL，表明初始化失败。
 */
am_wdt_handle_t amdr_wdt_init(amdr_wdt_dev_t           *p_dev,
                              const amdr_wdt_devinfo_t *p_devinfo);

/**
 * \brief 解除WDT初始化
 * \param[in] p_dev : 指向WDT设备
 * \return  无
 */
void amdr_wdt_deinit(amdr_wdt_dev_t *p_dev);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMDR_WDT_H */

/* end of file */
