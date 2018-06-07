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
 * \brief PIT驱动，服务Timer驱动层实现
 *
 * 1. PIT支持提供如下三种标准服务，本驱动提供的是服务Timer标准服务的驱动。
 *     - 定时
 *     - PWM输出
 *     - 捕获
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-20  cuishengen, first implementation.
 * \endinternal
 */

#ifndef __AMDR_PIT_TIMING_H
#define __AMDR_PIT_TIMING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_timer.h"
#include "amhw_pit.h"

/**
 * \addtogroup amdr_if_pit_timing
 * \copydoc amdr_pit_timing.h
 * @{
 */

/** \brief PIT运行在1个64位定时器模式, 仅能提供1个定时器通道    */
#define AMDR_PIT_TIMING_1_64BIT      1

/** \brief PIT运行在2个32位定时器模式, 能提供2个定时器通道      */
#define AMDR_PIT_TIMING_2_32BIT      2

/**
 * \brief PIT定时功能相关的设备信息
 */
typedef struct amdr_pit_timing_devinfo {
    amhw_pit_t   *p_hw_pit;    /**< \brief 指向PIT寄存器块的指针 */
    uint8_t       flag;        /**< \brief PIT配置标志(1-64bit 或者 2-32bit)  */
    uint8_t       inum;        /**< \brief PIT中断号  */
} amdr_pit_timing_devinfo_t;
    
/**
 * \brief PIT定时功能设备
 */
typedef struct amdr_pit_timing_dev {

    am_timer_serv_t   timer_serv;       /**< \brief 标准定时(Timer)服务 */

    void (*pfn_ch1_callback)(void *);       /**< \brief 回调函数 */
    void *p_arg_ch1;                        /**< \brief 回调函数的用户参数 */
    
    void (*pfn_ch2_callback)(void *);       /**< \brief 回调函数 */
    void *p_arg_ch2;                        /**< \brief 回调函数的用户参数 */
    
    /** \brief 指向PIT(定时功能)设备信息常量的指针 */
    const amdr_pit_timing_devinfo_t  *p_devinfo;

} amdr_pit_timing_dev_t;

/**
 * \brief 初始化PIT为定时功能
 *
 * \param[in] p_dev     : 指向PIT(定时功能)设备的指针
 * \param[in] p_devinfo : 指向PIT(定时功能)设备信息常量的指针
 *
 * \return Timer标准服务操作句柄，值为NULL时表明初始化失败
 */
am_timer_handle_t amdr_pit_timing_init(amdr_pit_timing_dev_t            *p_dev,
                                       const amdr_pit_timing_devinfo_t  *p_devinfo);

/**
 * \brief 不使用PIT定时功能时，解初始化PIT定时功能，释放相关资源
 * \param[in] p_dev : 指向PIT(定时功能)设备的指针
 * \return 无
 */
void amdr_pit_timing_deinit(amdr_pit_timing_dev_t *p_dev);

/** 
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMDR_PIT_TIMING_H */

/* end of file */
