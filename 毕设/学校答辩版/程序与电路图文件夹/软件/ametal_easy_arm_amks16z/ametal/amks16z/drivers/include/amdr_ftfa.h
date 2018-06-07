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
 * \brief FTFA������ӿ�
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
 *  \brief FTFAģ���ʼ��
 *
 *  \return ��
 */
void amdr_ftfa_init (void);

/**
 *  \brief ��������
 *
 *  \param start_addr ��������ʼ��ַ
 *
 *  \return >0: ִ�н��, -AM_EINVAL: �����ַ����
 */
int32_t amdr_ftfa_sector_erase (uint32_t start_addr);

/**
 *  \brief ��������̻򲿷��������
 *
 *  \param dst_addr д�뵽flash����ʼ��ַ
 *  \param p_src Ҫд�뵽flash�е����ݵ���ʼ��ַ
 *  \param size д����(32bit)�ĸ���
 *
 *  \retval >=0 ʵ�ʳɹ�д�������
 *  \retval -AM_EINVAL �����д����������
 *  \retval -AM_EINVAL ָ�����������δ��������
 *  \retval -AM_EAGAIN ���̵�flash��δ�����������������
 *  \retval -AM_ERROR  δ֪����
 */
int32_t amdr_ftfa_flash_program (uint32_t  dst_addr,
                                  uint32_t *p_src,
                                  uint32_t  size);

/**
 *  \brief ������������
 *
 *  \return ִ�н��
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
