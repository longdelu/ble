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
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief Cortex M0+可嵌套向量中断控制器(NVIC) 硬件操作接口
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-16  xjc, first implementation.
 *
 * \endinternal
 */


#ifndef __AMHW_INT_H
#define __AMHW_INT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "amhw_amks16z.h"

/**
 * \addtogroup amhw_if_int
 * \copydoc amhw_int.h
 * @{
 */

/**
 * \brief NVIC 寄存器块结构体
 */
typedef struct amhw_int {
    __IO uint32_t  iser[1];                    /**< \brief 中断使能寄存器       */
    __I  uint32_t  reserved1[31];              /**< \brief 保留                 */
    __IO uint32_t  icer[1];                    /**< \brief 清除中断使能寄存器   */
    __I  uint32_t  reserved2[31];              /**< \brief 保留                 */
    __IO uint32_t  ispr[1];                    /**< \brief 中断等待寄存器       */
    __I  uint32_t  reserved3[31];              /**< \brief 保留                 */
    __IO uint32_t  icpr[1];                    /**< \brief 清除中断等待寄存器   */
    __I  uint32_t  reserved4[31];              /**< \brief 保留                 */
    __I  uint32_t  iabr[1];                    /**< \brief 中断有效寄存器       */
    __I  uint32_t  reserved5[63];              /**< \brief 保留                 */
    __IO uint8_t   ipr[32];                    /**< \brief 中断优先级寄存器     */
    __I  uint32_t  reserved6[569];             /**< \brief 保留                 */
    __I  uint32_t  icsr;                       /**< \brief 中断控制和状态寄存器 */
    __IO uint32_t  vtor;                       /**< \brief 中断向量表偏移寄存器 */
    __I  uint32_t  reserved;                   /**< \brief 保留                 */
    __IO uint32_t  scr;                        /**< \brief 系统控制寄存器 */
    __I  uint32_t  reserved7[123];             /**< \brief 保留                 */
    __O  uint32_t  stir;                       /**< \brief 软件触发中断寄存器   */
} amhw_int_t;


/**
 * \brief 获取当前中断号
 * \param[in] p_hw_int : 指向NVIC寄存器块的指针
 * \return 中断号
 */
am_static_inline
int amhw_inum_get (amhw_int_t *p_hw_int)
{
    return (int)((p_hw_int->icsr - 16) & 0xFF);
}

/**
 * \brief 使能中断
 *
 * \param[in] p_hw_int : 指向NVIC寄存器块的指针
 * \param[in] inum     : 中断号， 参见
 *                       \sa amks16z_inum.h
 *
 * \return 无
 */
am_static_inline
void amhw_int_enable (amhw_int_t *p_hw_int, int inum)
{
    p_hw_int->iser[0] = (1UL << ((uint32_t)(inum) & 0x1F));
}

/**
 * \brief 禁能中断
 *
 * \param[in] p_hw_int : 指向NVIC寄存器块的指针
 * \param[in] inum     : 中断号， 参见
 *                       \sa amks16z_inum.h
 *
 * \return 无
 */
am_static_inline
void amhw_int_disable (amhw_int_t *p_hw_int, int inum)
{
    p_hw_int->icer[0] = (1UL << ((uint32_t)(inum) & 0x1F));
}

/**
 * \brief 置位中断等待
 *
 * \param[in] p_hw_int : 指向NVIC寄存器块的指针
 * \param[in] inum     : 中断号， 参见
 *                       \sa amks16z_inum.h
 *
 * \return    无
 */
am_static_inline
void amhw_int_pending_set (amhw_int_t *p_hw_int, int inum)
{
    p_hw_int->ispr[0] = (1UL << ((uint32_t)(inum) & 0x1F));
}

/**
 * \brief 清除等待中断
 *
 * \param[in] p_hw_int : 指向NVIC寄存器块的指针
 * \param[in] inum     : 中断号， 参见
 *                       \sa amks16z_inum.h
 *
 * \return    无
 */
am_static_inline
void amhw_int_pending_clr(amhw_int_t *p_hw_int, int inum)
{
    p_hw_int->icpr[0] = (1UL << ((uint32_t)(inum) & 0x1F));
}

/**
 * \brief 获取中断有效状态
 *
 * \param[in] p_hw_int : 指向NVIC寄存器块的指针
 * \param[in] inum     : 中断号， 参见
 *                       \sa amks16z_inum.h
 *
 * \retval TRUE  : 有效中断
 * \retval FALSE : 无效中断
 */
am_static_inline
bool_t amhw_int_active_state_get(amhw_int_t *p_hw_int, int inum)
{
    return ((p_hw_int->iabr[0] &
            (1UL << ((uint32_t)(inum) & 0x1F))) ? TRUE : FALSE);
}

/**
 * \brief 设置中断优先级
 *
 * \param[in] p_hw_int : 指向NVIC寄存器块的指针
 * \param[in] inum     : 中断号， 参见
 *                       \sa amks16z_inum.h
 * \param[in] prio     : 中断优先级，值越小优先级越高，取值范围为0~3
 *
 * \return   无
 */
am_static_inline
void amhw_int_priority_set (amhw_int_t *p_hw_int, int inum, uint32_t prio)
{
    p_hw_int->ipr[((uint32_t)(inum) & 0x1F)] = (prio << 6) & 0xFF;
}

/**
 * \brief 设置处理器进入深度睡眠模式（停止模式）
 *
 * \param[in] p_hw_int : 指向NVIC寄存器块的指针
 * \return   无
 */
am_static_inline
void amhw_int_deepsleep_set(amhw_int_t *p_hw_int)
{
    p_hw_int->scr |= 0x4;
}

/**
 * \brief 设置处理器进入普通睡眠模式（停止模式）
 *
 * \param[in] p_hw_int : 指向NVIC寄存器块的指针
 * \return   无
 */
am_static_inline
void amhw_int_deepsleep_clr(amhw_int_t *p_hw_int)
{
    p_hw_int->scr &= ~0x4;
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_INT_H */

/* end of file */

