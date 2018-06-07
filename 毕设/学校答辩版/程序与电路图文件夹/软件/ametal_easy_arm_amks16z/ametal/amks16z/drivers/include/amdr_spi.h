/*******************************************************************************
*                                 AMeatl
*                       ----------------------------
*                       innovating embedded platform
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
 * \brief SPI驱动，服务SPI标准接口
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-20  liqing, first implementation.
 * \endinternal
 */

#ifndef __AMDR_SPI_H
#define __AMDR_SPI_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "am_types.h"    
#include "am_spi.h"
#include "amhw_spi.h"
#include "am_int.h"

/**
 * \addtogroup amdr_if_spi
 * \copydoc amdr_spi.h
 * @{
 */
 
/**
 * \name SPI 配置标识
 * @{
 */

/** \brief 停止模式SPI继续工作 */
#define AMDR_SPI_CFG_WIAT_EN        (AM_BIT(28) | AM_SBF(0,3))

/** \brief 停止模式SPI不工作 */
#define AMDR_SPI_CFG_WIAT_DIS       (AM_BIT(28) | AM_SBF(1,3))

/** @} */

/**
 * \brief SPI 设备信息结构体
 */
typedef struct amdr_spi_devinfo {
    amhw_spi_t       *p_hw_spi;    /**< \brief 指向SPI寄存器块的指针          */
    uint32_t          mode_flag;   /**< \brief 初始化模式                     */
    uint32_t          ini_speed;   /**< \brief 初始化速率                     */
    uint32_t          flags;       /**< \brief SPI 配置标识                   */
    uint16_t          inum;        /**< \brief SPI 中断编号                   */
} amdr_spi_devinfo_t;

/**
 * \brief SPI 设备
 */
typedef struct amdr_spi_dev {
    am_spi_serv_t             spi_serve;     /**< \brief SPI标准服务操作句柄  */
    const amdr_spi_devinfo_t *p_devinfo;     /**< \brief 指向SPI设备信息指针  */

    /* 数据传输 */
    am_spi_transfer_t        *p_trans;       /**< \brief 指向SPI传输结构体指针*/
    uint32_t                  send_ptr;      /**< \brief 发送数据位置         */
    uint32_t                  recv_ptr;      /**< \brief 接收数据位置         */

    /* 从机设备配置 */
    am_spi_device_t          *p_f_dev;       /**< \brief 当前设备             */
    uint32_t                  cur_speed;     /**< \brief 当前SPI速度          */
    uint32_t                  speed_diff;    /**< \brief 当前速度死区         */
    uint8_t                   cur_bpw;       /**< \brief 当前设备数据位宽     */

    /* 全局变量 */
    bool_t                    cs_state;      /**< \brief 保存之前片选状态     */
    int                       state;         /**< \brief 控制器状态机状态     */
} amdr_spi_dev_t;

/**
 * \brief SPI 初始化
 *
 * \param[in] p_dev     : 指向SPI设备结构体的指针
 * \param[in] p_devinfo : 指向SPI设备信息结构体的指针
 *
 * \return  SPI标准服务操作句柄 
 */
am_spi_handle_t amdr_spi_init(amdr_spi_dev_t           *p_dev,
                               const amdr_spi_devinfo_t *p_devinfo);

/**
 * \brief 解除SPI初始化
 * \param[in] p_dev : 指向SPI设备结构体的指针
 * \return 无
 */
void amdr_spi_deinit(amdr_spi_dev_t *p_dev);

/** 
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMDR_SPI_H */

/*end of file */
