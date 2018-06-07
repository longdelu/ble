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
 * \brief FTFA驱动层接口
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-28  zsp, first implementation.
 * \endinternal
 */

#ifndef __AMDR_FTFA_H
#define __AMDR_FTFA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"

/**
 * \addtogroup amdr_if_ftfa
 * \copydoc amdr_ftfa.h
 * @{
 */

/**
 *  \brief FTFA模块初始化
 *
 *  \return 无
 */
void amdr_ftfa_init (void);

/**
 *  \brief 擦除扇区
 *
 *  \param start_addr 扇区的起始地址
 *
 *  \return >0: 执行结果, -AM_EINVAL: 输入地址过大
 */
int32_t amdr_ftfa_sector_erase (uint32_t start_addr);

/**
 *  \brief 对扇区编程或部分扇区编程
 *
 *  \param dst_addr 写入到flash的起始地址
 *  \param p_src 要写入到flash中的数据的起始地址
 *  \param size 写入字(32bit)的个数
 *
 *  \retval >=0 实际成功写入的字数
 *  \retval -AM_EINVAL 输入的写入字数过大
 *  \retval -AM_EINVAL 指定的区域存在未擦除区域
 *  \retval -AM_EAGAIN 需编程的flash还未擦除，请擦除后重试
 *  \retval -AM_ERROR  未知错误
 */
int32_t amdr_ftfa_flash_program (uint32_t  dst_addr,
                                  uint32_t *p_src,
                                  uint32_t  size);

/**
 *  \brief 擦除所有扇区
 *
 *  \return 执行结果
 *
 */
uint32_t amdr_ftfa_all_sector_erase (void);



/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /** __AMDR_FTFA_H */
