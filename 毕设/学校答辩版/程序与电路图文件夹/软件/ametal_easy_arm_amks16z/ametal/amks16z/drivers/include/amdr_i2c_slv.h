/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
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
 * \brief AMKS16 I2C（从机）驱动，服务I2C从机标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 15-12-18  mem, first implementation.
 * \endinternal
 */

#ifndef __AMDR_I2C_SLV_H
#define __AMDR_I2C_SLV_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "am_int.h"
#include "am_i2c_slv.h"
#include "amhw_i2c.h"
    
/**
 * \addtogroup amdr_if_i2c
 * \copydoc amdr_i2c.h
 * @{
 */

/**
 * \brief I2C 设备信息参数结构体
 */
typedef struct amdr_i2c_slv_devinfo {
    amhw_i2c_t       *p_hw_i2c;       /**< \brief 指向I2C寄存器块的指针  */
    uint32_t          max_bus_speed;  /**< \brief 支持的最大总线速率     */
    uint32_t         *p_slv_list;     /**< \brief 指向从机地址列表的指针 */
    uint8_t           dev_num;        /**< \brief 使用的设备数量         */
    uint16_t          inum;           /**< \brief I2C 控制器的中断号     */
} amdr_i2c_slv_devinfo_t;

/**
 * \brief I2C 设备结构体
 */
typedef struct amdr_i2c_slv_dev {
   
    /** \brief 标准I2C服务           */
    am_i2c_slv_serv_t             i2c_serv;

    /** \brief 指向I2C设备信息的指针 */
    const amdr_i2c_slv_devinfo_t *p_devinfo;
    
    /** \brief I2C输入时钟           */
    uint32_t                      clk_in;

    /** \brief I2C时钟频率           */
    uint32_t                      clk_rate;

    /** \brief 指向I2C传输结构体的指针,同一时间只能处理一个传输 */
    am_i2c_slv_transfer_t        *p_trans;

    /** \brief 用于数据接收/发送计数   */
    uint32_t                      data_ptr;

    /** \brief 指向当前正在进行数据传输的从机设备索引 */
    uint8_t                       cur_indx;

    /** \brief 状态                   */
    uint8_t                       state;
} amdr_i2c_slv_dev_t;


/**
 * \brief I2C（从机）初始化
 *
 * \param[in] p_dev     : 指向I2C从机设备结构体的指针
 * \param[in] p_devinfo : 指向I2C从机设备信息结构体的指针
 *
 * \return  I2C标准服务操作句柄 
 */
am_i2c_slv_handle_t amdr_i2c_slv_init(amdr_i2c_slv_dev_t           *p_dev,
                                      const amdr_i2c_slv_devinfo_t *p_devinfo);

/**
 * \brief I2C（从机）解初始化
 * \param[in] p_dev : 指向I2C从机设备结构体的指针
 * \return 无
 */
void amdr_i2c_slv_deinit(amdr_i2c_slv_dev_t *p_dev);
    
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __AMDR_I2C_SLV_H */

/* end of file */
