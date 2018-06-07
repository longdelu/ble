/*******************************************************************************
*                                 AMetal
*                       ---------------------------
*                       innovating embedded systems
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co.,  Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief FTFA����,ͨ��������Ľӿ�ʵ��
 *
 * - �������裺
 *      - ���ڽӵ��Դ���
 *
 * - ʵ������
 *      - �����������ڴ�ӡ ��erase error!��
 *      - д��������ڴ�ӡ "program error!"
 *      - д��ɹ������ڴ�ӡ 60������ (1024/4) ��32bitʮ����������
 *
 * \par Դ����
 * \snippet demo_dr_ftfa.c src_dr_ftfa
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-21  zsp, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_dr_ftfa
 * \copydoc demo_dr_ftfa.c
 */

/** [src_dr_ftfa] */
#include "ametal.h"
#include "am_board.h"
#include "am_debug.h"
#include "am_delay.h"
#include "am_prj_config.h"

int main()
{
    int i;
    uint32_t data[1024 / 4]; /** Ҫд��flash������ */
    uint32_t status;         /** flash����ִ��״̬ */

    /** �弶��ʼ�� */
    am_board_init();

    /** ���ݳ�ʼ�� */
    for (i = 0; i < 1024 / 4; i++) {
        data[i] = i;
    }

    /** FTFA��ʼ�� */
    amdr_ftfa_init();

    /** �������� 55 */
    status = amdr_ftfa_sector_erase(55 * 1024);

    /** ������������ ����ͣ�ڴ˴� */
    if (0 != status) {
        printf("erase error!\r\n");

        while (1) {
            ;
        }
    }

    /** ������ 55��д������ */
    status = amdr_ftfa_flash_program(55 * 1024, data, 1024 / 4);

    /** ����д���������ͣ�ڴ˴� */
    if ((1024/4) != status) {
        printf("program error!\r\n");

        while (1);
    }

    /** д��ɹ������ݴ�flash�ж��� */
    for (i = 0; i < 1024; i += 4) {
        printf("%8x ", *(uint32_t *)(1024 * 55 + i));
    }

    printf("flash test finished!\r\n");
    /** ��ѭ������ֹ���� */
    while (1) {
        ;
    }
}

/** [src_dr_ftfa] */

/* end of file */
