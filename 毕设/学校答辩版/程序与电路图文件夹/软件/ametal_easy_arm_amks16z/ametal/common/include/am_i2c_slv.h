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
 * \brief I2C（从机）标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 15-09-26  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_I2C_SLV_H
#define __AM_I2C_SLV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"

/**
 * @addtogroup am_if_i2c_slv
 * @copydoc am_i2c_slv.h
 * @{
 */

/**
 * \name I2C 从机地址相关标志，用于 am_i2c_slv_device_t 的 addr_flags 成员
 * @{ 
 */

/** \brief 从机为7-bit地址（默认）     */
#define AM_I2C_S_ADDR_7BIT          0x0000u

/** \brief 从机为10-bits 地址          */
#define AM_I2C_S_ADDR_10BIT         0x0001u

/** \brief I2C从机地址控制标志掩码     */
#define AM_I2C_S_ADDR_MASK          0x000Fu

/** @} */
 
/**
 * \name I2C（从机）传输控制标识，用于am_i2c_slv_transfer 的 flags成员
 * @{ 
 */
 
/** \brief 忽略主机数据无应答（当向主机发送数据时）                  */
#define AM_I2C_S_TRANS_IGNORE_NAK         0x0010u

/** \brief 在读操作中（接收数据时），此传输的最后一个数据不发送应答  */
#define AM_I2C_S_TRANS_LAST_RD_NACK       0x0020u

/** \brief 在读操作中（接收数据时），所有数据均不发送应答            */
#define AM_I2C_S_TRANS_RD_NACK            0x0040u

/** @} */

/**
 * \name 从机的数据传输方向，这些宏值表示了地址匹配回调函数中 dir 参数值的含义
 * @{
 */

#define AM_I2C_S_DIR_READ   0  /**< \brief 从机读数据，传输方向：主机 -> 从机 */
#define AM_I2C_S_DIR_WRITE  1  /**< \brief 从机写数据，传输方向：从机 -> 主机 */

/** @} */

/** \brief 从机设备类型声明     */
struct am_i2c_slv_dev;

/** \brief 从机数据传输类型声明 */
struct am_i2c_slv_transfer;
   
/**
 * \name 回调函数类型定义
 * @{
 */

/**
 * \brief 获取一个传输并开始数据传输，传输完成后，调用传输完成回调函数
 *
 *     当需要接收或发送数据时，将调用该“传输获取函数”，获取一个传输。传输的方向
 * 由参数 dir 指明，用户不能自由设定传输方向。
 *
 *     参数 p_arg 为应用程序自定义参数。
 *
 *     参数 dir 表明了本次获取的传输的数据传输方向。
 *       - AM_I2C_S_DIR_READ  从机读数据，传输方向为：主机 -> 从机
 *       - AM_I2C_S_DIR_WRITE 从机写数据，传输方向为: 从机 -> 主机
 *
 *     参数 pp_trans 用于获取一个传输，由于该处需要获取一个传输的地址，因
 * 此类型为二维指针。获取方式很简单，如： *pp_trans = &trans;
 *
 * \note 务必确保“传输”的地址以及传输中的“数据缓冲区”在整个传输过程中均是有效的。
 *       建议使用静态或全局变量。
 */
typedef void (*am_i2c_slv_trans_get_cb_t) (void                        *p_arg,
                                           uint8_t                      dir,
                                           struct am_i2c_slv_transfer **pp_trans);
 
/** 
 * \brief 地址匹配回调函数类型定义
 *
 *     主机在START信号后，发送的从机地址与注册的从机地址一致时，视为地址匹配。
 * 如果该函数存在，则会调用该函数并指明主机请求数据传输的方向。
 *  
 *     参数 p_arg 为应用程序自定义参数。
 *
 *     参数 dir 表示主机启动信号后的地址信息中的方向位
 *       - AM_I2C_S_DIR_READ  从机读数据，传输方向为：主机 -> 从机
 *       - AM_I2C_S_DIR_WRITE 从机写数据，传输方向为: 从机 -> 主机
 *
 *     参数 p_isack 用于用户设定是否需要应答该地址匹配。
 *        \li TRUE   应答
 *        \li FALSE  不应答
 *
 * \note 大多数情况下，都是接收到地址匹配后正常应答，此时，如无特殊需求，可以
 *       不设定该函数，则驱动会自动对地址匹配事件产生应答信号。
 */
typedef void (*am_i2c_slv_addr_mat_cb_t) (void    *p_arg,
                                          uint8_t  dir,
                                          bool_t  *p_isack);


/** 
 * \brief 传输完成回调函数类型定义
 * 
 * 参数p_arg为应用程序自定义参数
 *
 * 参数 stat 表明了传输完成的状态
 *    \li  AM_OK     传输完成
 *    \li -AM_EIO    数据传输出现错误，如数据还未读/写完毕就遇到停止/启动信号等。
 *    \li -AM_ETIME  超时返回，超时时间的设置在 am_i2c_slv_mkdev() 中设置。
 *                    - 读/写数据还未完成指定的字节数，但超时时间内，未等到主机
 *                      继续进行相应操作。
 *                    - 写数据时，需要主机的应答信号，但超时时间内未等到主机的
 *                      应答信号。
 *                    - 简单的说，等待I2C的任何事件，只要等待时间超过超时时间设
 *                      置值，则视为超时。
 *
 * 参数 nbytes 表明了实际传输成功的字节数。即使状态不为AM_OK，已经成功完成的数
 * 据还是有效的，如传输设定为接收10个字节，但接收到5个字节后由于无法再收到数据
 * 而超时返回，此时，已经收到的5个字节数据还是有效的。
 */
typedef void (*am_i2c_slv_trans_complete_cb_t) (void     *p_arg,
                                                int       stat,
                                                uint32_t  nbytes);

/**
 * \brief I2C（从机）驱动函数结构体
 */
struct am_i2c_slv_drv_funcs {
    
    /** \brief 注册一个I2C从机            */
    int (*pfn_i2c_slv_register) (void *p_drv, struct am_i2c_slv_dev *p_dev);
    
    /** \brief 取消已注册的一个I2C从机    */
    int (*pfn_i2c_slv_unregister) (void *p_drv, struct am_i2c_slv_dev *p_dev);
};

/**
 * \brief I2C（从机）标准服务结构体 
 */
typedef struct am_i2c_slv_serv {
    struct am_i2c_slv_drv_funcs   *p_funcs;   /**< \brief I2C驱动函数         */
    void                          *p_drv;     /**< \brief I2C驱动函数入口函数 */
} am_i2c_slv_serv_t;

/** \brief I2C（从机）标准服务操作句柄定义 */
typedef am_i2c_slv_serv_t *am_i2c_slv_handle_t;

/**
 * \brief I2C从机设备相关信息结构体
 */
typedef struct am_i2c_slv_dev {
    am_i2c_slv_handle_t        handle;          /**< \brief I2C从机服务句柄   */
    uint16_t                   slv_addr;        /**< \brief 从机地址          */ 
    uint16_t                   addr_flags;      /**< \brief 从机地址标志      */
    uint32_t                   timeout_ms;      /**< \brief 超时时间          */
    am_i2c_slv_trans_get_cb_t  pfn_trans_get;   /**< \brief 获取传输回调函数  */
    void                      *p_arg_trans_get; /**< \brief 回调函数参数      */
    am_i2c_slv_addr_mat_cb_t   pfn_matched;     /**< \brief 地址匹配回调函数  */
    void                      *p_arg_matched;   /**< \brief 回调函数参数      */
} am_i2c_slv_dev_t;

/**
 * \brief I2C从机数据传输结构体
 */
typedef struct am_i2c_slv_transfer {
    uint16_t                  trans_flags;      /**< \brief 传输标志          */
    uint8_t                  *p_buf;            /**< \brief 传输数据缓存指针  */
    uint32_t                  length;           /**< \brief 数据传输长度      */
    
    /** \brief 传输完成回调函数  */
    am_i2c_slv_trans_complete_cb_t  pfn_complete_cb;  
    
    /**< \brief 回调函数入口参数  */
    void                           *p_arg;            
} am_i2c_slv_transfer_t;

/**
 * \brief I2C从机设备相关信息参数设置
 *
 *     在使用I2C从机前，应使用该函数配置好一个从机设备信息，包括从机地址等。设
 * 置好相关参数后，可以使用 am_i2c_slv_register()向I2C控制器中注册一个从机设备。
 *
 * \param[in] p_dev           : 指向从机设备的结构体的指针
 * \param[in] handle          : I2C从机标准服务操作句柄
 * \param[in] slv_addr        : 从机地址
 * \param[in] addr_flags      : 从机地址标志
 * \param[in] timeout_ms      : 超时时间（单位: ms），0表示不使用超时机制
 * \param[in] pfn_trans_get   : 获取“传输”回调函数（不可为NULL）
 * \param[in] p_arg_trans_get : 获取“传输”回调函数的参数  
 * \param[in] pfn_matched     : 地址匹配回调函数（可为NULL）
 * \param[in] p_arg_matched   : 地址匹配回调函数参数
 *
 * \return 无
 *
 * \note 关于超时时间，超时机制可以避免I2C从机因意外状况死锁。如果数据（发送或
 * 接收）间隔时间超过了超时时间值，即在超时时间内均无有效数据，则视为超时事件。
 * 未收到起始信号时，I2C处于空闲状态，不会导致I2C超时。超时后，I2C从机将复位内
 * 部逻辑，回到初始态，如需再次通信，则需要新的启动信号。实际的超时值可能与此处
 * 设置的超时值存在差异，注册后，设备的timeout_ms值即反映了实际的超时值。一般
 * 情况下，10ms左右即可。
 */
am_static_inline
void am_i2c_slv_mkdev (am_i2c_slv_dev_t           *p_dev,
                       am_i2c_slv_handle_t         handle,
                       uint16_t                    slv_addr,
                       uint16_t                    addr_flags,
                       uint32_t                    timeout_ms,
                       am_i2c_slv_trans_get_cb_t   pfn_trans_get,
                       void                       *p_arg_trans_get,
                       am_i2c_slv_addr_mat_cb_t    pfn_matched,
                       void                       *p_arg_matched)

{
    p_dev->handle          = handle;
    p_dev->slv_addr        = slv_addr;
    p_dev->addr_flags      = addr_flags;
    p_dev->timeout_ms      = timeout_ms;
    p_dev->pfn_trans_get   = pfn_trans_get;
    p_dev->p_arg_trans_get = p_arg_trans_get;
    p_dev->pfn_matched     = pfn_matched;
    p_dev->p_arg_matched   = p_arg_matched;
}

/**
 * \brief I2C从机传输结构体信息参数设置
 *
 * \param[in] p_trans      : 指向传输结构体的指针
 * \param[in] addr         : 操作的从机设备地址
 * \param[in] flags        : 传输过程中的控制标识位，如无特殊控制标志，值为0
 * \param[in] p_buf        : 指向发送或者接收数据缓存
 * \param[in] nbytes       : 当前传输的数据/地址长度
 * \param[in] pfn_callback : 传输完成回调函数
 * \param[in] p_arg        : 回调函数入口参数
 *
 * \retval  AM_OK     : 传输结构体参数设置完成
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
void am_i2c_slv_mktrans (am_i2c_slv_transfer_t           *p_trans, 
                         uint16_t                         flags, 
                         void                            *p_buf, 
                         uint32_t                         nbytes,
                         am_i2c_slv_trans_complete_cb_t   pfn_callback,
                         void                            *p_arg)
{ 
    p_trans->trans_flags     = flags;
    p_trans->p_buf           = p_buf; 
    p_trans->length          = nbytes;
    p_trans->pfn_complete_cb = pfn_callback; 
    p_trans->p_arg           = p_arg;
}

/**
 * \brief 注册一个I2C从机
 *
 *     一个I2C控制器往往能够提供多个I2C从机服务，此时可以使用该函数注册多个I2C
 * 从机，要确保每个I2C从机地址都是唯一的。在注册一个从机设备前，务必确保已经使用
 * am_i2c_slv_mkdev()初始化好设备相关的信息。
 *
 * \param[in] p_dev    : 指向从机设备的结构体的指针
 *
 * \retval  AM_OK      : 从机注册成功，可以正常使用
 * \retval -AM_EINVAL  : 参数错误
 * \retval -AM_ENOTSUP : 从机某些属性不支持，如有的设备不支持10-bit地址模式
 * \retval -AM_EPERM   : 操作不允许，表明该地址已经存在，不能重复注册
 * \retval -AM_EFULL   : 从机注册已满，如设备只支持4个从机，则注册第5个从机时
 *                       将会返回该值，可以使用 am_i2c_slv_unregister()取消一些
 *                       不用的从机设备后再重新注册。
 *
 * \note 成功注册后，将在合适的时机调用相应的回调函数。
 */
am_static_inline
int am_i2c_slv_register (am_i2c_slv_dev_t *p_dev)
{
    return p_dev->handle->p_funcs->pfn_i2c_slv_register(p_dev->handle->p_drv,
                                                        p_dev);
}

/**
 * \brief 取消一个已经注册的I2C从机
 *
 *     一个I2C控制器往往能够提供多个I2C从机服务，此时可以使用该函数注册多个I2C
 * 从机，要确保每个I2C从机地址都是唯一的。
 *
 * \param[in] p_dev   : 指向从机设备信息的结构体的指针
 *
 * \retval  AM_OK      : 取消成功
 * \retval -AM_EINVAL  : 参数错误
 * \retval -AM_EPERM   : 操作不允许，该从机还未注册
 */
am_static_inline
int am_i2c_slv_unregister (am_i2c_slv_dev_t *p_dev)
{
    return p_dev->handle->p_funcs->pfn_i2c_slv_unregister(p_dev->handle->p_drv,
                                                          p_dev);
}

/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_I2C_SLV_H */

/* end of file */
