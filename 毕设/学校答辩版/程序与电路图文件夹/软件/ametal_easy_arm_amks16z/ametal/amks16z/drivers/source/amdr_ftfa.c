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
 * \brief FTFAģ���������ʵ��
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-22  zsp, first implementation.
 * \endinternal
 */

#include "am_types.h"
#include "am_bitops.h"
#include "ametal.h"
#include "amdr_ftfa.h"
#include "amhw_ftfa.h"


#define SECTOR_SIZE (1024UL / 4)     /**< \brief ������С ����(32bit)Ϊ��λ     */

#define FLASH_SIZE  (1024*64UL)      /**< \brief flash�Ĵ�С ���ֽ�(8bit)Ϊ��λ */

/******************************************************************************
*   ��������
******************************************************************************/

/**
 *  \brief FTFAģ���ʼ��
 *
 *  \return ��
 */
void amdr_ftfa_init (void)
{
    amhw_ftfa_func_copy();
    return;
}

/**
 *  \brief ��������
 *
 *  \param start_addr ��������ʼ��ַ
 *
 *  \return >0: ִ�н��, -AM_EINVAL: �����ַ����
 */
int32_t amdr_ftfa_sector_erase (uint32_t start_addr)
{
    if (FLASH_SIZE < start_addr)
    {
        return -AM_EINVAL;
    }
    return amhw_ftfa_sector_erase(AMHW_FTFA, start_addr);
}

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
                                  uint32_t  size)
{
    uint32_t status;
    int      i;

    /** size���ܴ��������Ĵ�С */
    if (size > SECTOR_SIZE || dst_addr >= FLASH_SIZE) {
        return -AM_EINVAL;
    }

    /** ��֤flash�Ѳ��� */
    status = amhw_ftfa_1s_read(AMHW_FTFA,
                               dst_addr,
                               size,
                               0);

    /** ��flash��� */
    if (0 == status) {
        for (i = 0; i < size; i++) {

            /** ���1���� */
            status = amhw_ftfa_word_program(AMHW_FTFA,
                                            dst_addr + (i*4),
                                            p_src[i]);

            if (0 != status) {
                break;
            }

            /** ���д����Ƿ���ȷ */
            status = amhw_ftfa_program_check(AMHW_FTFA,
                                             dst_addr + i*4,
                                             p_src[i],
                                             0x01);

            if (0 != status) {
                break;
            }

        }

        if (i <= size && i > 0) {
            return i;          /** ����д��ɹ����� i           */
        }

        if (0 == i) {
            return -AM_EAGAIN; /** ��ȫû��д�� ���� -AM_EAGAIN */
        }
    } else {                   /** ����У����� ���� -AM_EAGAIN */
        return -AM_EAGAIN;
    }
    return -AM_ERROR;
}

/**
 *  \brief ������������
 *
 *  \return ִ�н��
 *
 */
uint32_t amdr_ftfa_all_sector_erase (void)
{
    return amhw_ftfa_all_blocks_erase(AMHW_FTFA);
}


