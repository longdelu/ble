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
 * \brief Cortex M0+��Ƕ�������жϿ�����(NVIC) Ӳ�������ӿ�
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
 * \brief NVIC �Ĵ�����ṹ��
 */
typedef struct amhw_int {
    __IO uint32_t  iser[1];                    /**< \brief �ж�ʹ�ܼĴ���       */
    __I  uint32_t  reserved1[31];              /**< \brief ����                 */
    __IO uint32_t  icer[1];                    /**< \brief ����ж�ʹ�ܼĴ���   */
    __I  uint32_t  reserved2[31];              /**< \brief ����                 */
    __IO uint32_t  ispr[1];                    /**< \brief �жϵȴ��Ĵ���       */
    __I  uint32_t  reserved3[31];              /**< \brief ����                 */
    __IO uint32_t  icpr[1];                    /**< \brief ����жϵȴ��Ĵ���   */
    __I  uint32_t  reserved4[31];              /**< \brief ����                 */
    __I  uint32_t  iabr[1];                    /**< \brief �ж���Ч�Ĵ���       */
    __I  uint32_t  reserved5[63];              /**< \brief ����                 */
    __IO uint8_t   ipr[32];                    /**< \brief �ж����ȼ��Ĵ���     */
    __I  uint32_t  reserved6[569];             /**< \brief ����                 */
    __I  uint32_t  icsr;                       /**< \brief �жϿ��ƺ�״̬�Ĵ��� */
    __IO uint32_t  vtor;                       /**< \brief �ж�������ƫ�ƼĴ��� */
    __I  uint32_t  reserved;                   /**< \brief ����                 */
    __IO uint32_t  scr;                        /**< \brief ϵͳ���ƼĴ��� */
    __I  uint32_t  reserved7[123];             /**< \brief ����                 */
    __O  uint32_t  stir;                       /**< \brief ��������жϼĴ���   */
} amhw_int_t;


/**
 * \brief ��ȡ��ǰ�жϺ�
 * \param[in] p_hw_int : ָ��NVIC�Ĵ������ָ��
 * \return �жϺ�
 */
am_static_inline
int amhw_inum_get (amhw_int_t *p_hw_int)
{
    return (int)((p_hw_int->icsr - 16) & 0xFF);
}

/**
 * \brief ʹ���ж�
 *
 * \param[in] p_hw_int : ָ��NVIC�Ĵ������ָ��
 * \param[in] inum     : �жϺţ� �μ�
 *                       \sa amks16z_inum.h
 *
 * \return ��
 */
am_static_inline
void amhw_int_enable (amhw_int_t *p_hw_int, int inum)
{
    p_hw_int->iser[0] = (1UL << ((uint32_t)(inum) & 0x1F));
}

/**
 * \brief �����ж�
 *
 * \param[in] p_hw_int : ָ��NVIC�Ĵ������ָ��
 * \param[in] inum     : �жϺţ� �μ�
 *                       \sa amks16z_inum.h
 *
 * \return ��
 */
am_static_inline
void amhw_int_disable (amhw_int_t *p_hw_int, int inum)
{
    p_hw_int->icer[0] = (1UL << ((uint32_t)(inum) & 0x1F));
}

/**
 * \brief ��λ�жϵȴ�
 *
 * \param[in] p_hw_int : ָ��NVIC�Ĵ������ָ��
 * \param[in] inum     : �жϺţ� �μ�
 *                       \sa amks16z_inum.h
 *
 * \return    ��
 */
am_static_inline
void amhw_int_pending_set (amhw_int_t *p_hw_int, int inum)
{
    p_hw_int->ispr[0] = (1UL << ((uint32_t)(inum) & 0x1F));
}

/**
 * \brief ����ȴ��ж�
 *
 * \param[in] p_hw_int : ָ��NVIC�Ĵ������ָ��
 * \param[in] inum     : �жϺţ� �μ�
 *                       \sa amks16z_inum.h
 *
 * \return    ��
 */
am_static_inline
void amhw_int_pending_clr(amhw_int_t *p_hw_int, int inum)
{
    p_hw_int->icpr[0] = (1UL << ((uint32_t)(inum) & 0x1F));
}

/**
 * \brief ��ȡ�ж���Ч״̬
 *
 * \param[in] p_hw_int : ָ��NVIC�Ĵ������ָ��
 * \param[in] inum     : �жϺţ� �μ�
 *                       \sa amks16z_inum.h
 *
 * \retval TRUE  : ��Ч�ж�
 * \retval FALSE : ��Ч�ж�
 */
am_static_inline
bool_t amhw_int_active_state_get(amhw_int_t *p_hw_int, int inum)
{
    return ((p_hw_int->iabr[0] &
            (1UL << ((uint32_t)(inum) & 0x1F))) ? TRUE : FALSE);
}

/**
 * \brief �����ж����ȼ�
 *
 * \param[in] p_hw_int : ָ��NVIC�Ĵ������ָ��
 * \param[in] inum     : �жϺţ� �μ�
 *                       \sa amks16z_inum.h
 * \param[in] prio     : �ж����ȼ���ֵԽС���ȼ�Խ�ߣ�ȡֵ��ΧΪ0~3
 *
 * \return   ��
 */
am_static_inline
void amhw_int_priority_set (amhw_int_t *p_hw_int, int inum, uint32_t prio)
{
    p_hw_int->ipr[((uint32_t)(inum) & 0x1F)] = (prio << 6) & 0xFF;
}

/**
 * \brief ���ô������������˯��ģʽ��ֹͣģʽ��
 *
 * \param[in] p_hw_int : ָ��NVIC�Ĵ������ָ��
 * \return   ��
 */
am_static_inline
void amhw_int_deepsleep_set(amhw_int_t *p_hw_int)
{
    p_hw_int->scr |= 0x4;
}

/**
 * \brief ���ô�����������ͨ˯��ģʽ��ֹͣģʽ��
 *
 * \param[in] p_hw_int : ָ��NVIC�Ĵ������ָ��
 * \return   ��
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

