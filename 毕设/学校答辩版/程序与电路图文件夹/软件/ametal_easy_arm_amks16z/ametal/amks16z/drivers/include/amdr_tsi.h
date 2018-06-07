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
 * \brief TSI驱动层
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-21  wxj, first implementation.
 * \endinternal
 */

#ifndef __AMDR_TSI_H
#define __AMDR_TSI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "amhw_tsi.h"
#include "amhw_amks16z.h"
#include "am_int.h"

/**
 * \addtogroup amdr_if_tsi
 * \copydoc amdr_tsi.h
 * @{
 */

/**
 * \brief TSI 设备信息结构体
 */
typedef struct amdr_tsi_devinfo {

    amhw_tsi_t   *p_hw_tsi;   /**< \brief 指向TSI寄存器块的指针 */	
    bool_t        lp_mode;    /**< \brief 低功耗唤醒MCU功能 */
} amdr_tsi_devinfo_t;

/**
 * \brief TSI设备实例
 */
typedef struct amdr_tsi_dev {

    /** \brief 指向TSI设备信息的指针 */
    const amdr_tsi_devinfo_t *p_devinfo;

} amdr_tsi_dev_t;

/**
 * \brief TSI初始化
 *
 * \param[in] p_dev     : 指向TSI 设备的指针
 * \param[in] p_devinfo : 指向TSI 设备信息的指针
 *
 * \return 初始化操作成功
 */
int am_tsi_init (amdr_tsi_dev_t *p_dev, const amdr_tsi_devinfo_t *p_devinfo);

/**
 * \brief TSI解初始化
 *
 * \param[in] p_dev : 指向TSI 设备的指针
 *
 * \return 解初始化操作成功
 */
int am_tsi_deinit (amdr_tsi_dev_t *p_dev);

/**
 * \brief TSI功能配置
 *
 * \param[in] chan      : TSI 通道号
 * \param[in] work_mode : TSI 工作模式
 * \param[in] trig_mode : TSI 扫描触发模式
 *
 * \return 功能配置操作成功
 */
int amdr_tsi_fun_cfg (amhw_tsi_channel_t    chan,
                      amhw_tsi_mode_t       work_mode,
                      amhw_tsi_trig_mode_t  trig_mode);

/**
 * \brief TSI中断触发模式
 *
 * \param[in] flag  : 中断触发模式
 *
 * \return 读取的转换值
 */
void amdr_tsi_trigger_cfg (amhw_tsi_irq_mode_t flag);

/**
 * \brief TSI中断使能
 *
 * \return 无
 */
void amdr_tsi_int_enable (void);

/**
 * \brief TSI中断禁能
 *
 * \return 无
 */
void amdr_tsi_int_disable (void);

/**
 * \brief TSI中断服务函数连接
 *
 * \param[in] pfn_callback  : 中断服务函数接口
 *
 * \return 无
 */
int amdr_tsi_trigger_connect (am_pfnvoid_t pfn_callback);

 /**
 * \brief TSI 灵敏度设置
 *
 * \param[in] val   : 灵敏度值
 *
 * \return 无
 */
void amdr_tsi_sens_set (uint16_t val);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMDR_TSI_H */

/* end of file */
