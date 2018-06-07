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
 * \brief FTFA����,ͨ��HW��Ľӿ�ʵ��
 *
 * - �������裺
 *      - ʹ�õ��Դ��ڴ�ӡ��Ϣ
 *      - д��ʧ��ʱ����ӡ ��write error!��
 *      - д�����ʱ����ӡ ��data not wrote correct!��
 *      - �ɹ�д��ʱ����ӡд�������(32bit)�� ��flash test finished��
 * - ʵ������
 *
 *
 * \par Դ����
 * \snippet demo_hw_ftfa.c src_hw_ftfa
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-21  zsp, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_hw_ftfa
 * \copydoc demo_hw_ftfa.c
 */

/** [src_hw_ftfa] */
#include "ametal.h"
#include "am_board.h"
#include "am_debug.h"
#include "am_delay.h"
#include "am_prj_config.h"

int main()
{
    int i = 10;
    uint32_t status;

    am_board_init();

    /** ��flash����ִ�к���������RAM */
    amhw_ftfa_func_copy();

    /** ����60����  */
    amhw_ftfa_sector_erase(AMHW_FTFA, 60 * 1024);

    /** �ж�60�����Ƿ�ȫΪ1�����ɱ�� */
    amhw_ftfa_1s_read(AMHW_FTFA, 60 * 1024, 1024 / 4, 0x00);

    for (i = 0; i < 1024; i += 4) {
        status = amhw_ftfa_word_program(AMHW_FTFA,
                                        60 * 1024 + i,
                                        i);

        /** д�벻�ɹ�ʱ����ͣ�ڴ˴� */
        if (0 != status) {
            printf("write error!\r\n");

            while (1);
        }

        status = amhw_ftfa_program_check(AMHW_FTFA, 62 * 1024 + i, i, 0x1);

        /** д�벻��ȷʱ����ͣ�ڴ˴� */
        if (0 != status) {
            printf("data not wrote correct!\r\n");

            while (1);
        }
    }

    for (i = 0; i < 1024; i += 4) {
        printf("%8x ", *(uint32_t *)(60 * 1024 + i));
    }

    printf("flash test finished\r\n");

    /** ��ѭ������ֹMCU���� */
    while (1) {
        i++;
    }

    return 0;
}

/** [src_hw_ftfa] */

/* end of file */
