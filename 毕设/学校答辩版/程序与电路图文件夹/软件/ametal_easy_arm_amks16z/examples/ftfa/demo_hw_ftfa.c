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
 * \brief FTFA例程,通过HW层的接口实现
 *
 * - 操作步骤：
 *      - 使用调试串口打印信息
 *      - 写入失败时：打印 “write error!”
 *      - 写入错误时：打印 “data not wrote correct!”
 *      - 成功写入时：打印写入的数据(32bit)和 “flash test finished”
 * - 实验现象：
 *
 *
 * \par 源代码
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

    /** 将flash命令执行函数拷贝到RAM */
    amhw_ftfa_func_copy();

    /** 擦除60扇区  */
    amhw_ftfa_sector_erase(AMHW_FTFA, 60 * 1024);

    /** 判断60扇区是否全为1，即可编程 */
    amhw_ftfa_1s_read(AMHW_FTFA, 60 * 1024, 1024 / 4, 0x00);

    for (i = 0; i < 1024; i += 4) {
        status = amhw_ftfa_word_program(AMHW_FTFA,
                                        60 * 1024 + i,
                                        i);

        /** 写入不成功时程序停在此处 */
        if (0 != status) {
            printf("write error!\r\n");

            while (1);
        }

        status = amhw_ftfa_program_check(AMHW_FTFA, 62 * 1024 + i, i, 0x1);

        /** 写入不正确时程序停在此处 */
        if (0 != status) {
            printf("data not wrote correct!\r\n");

            while (1);
        }
    }

    for (i = 0; i < 1024; i += 4) {
        printf("%8x ", *(uint32_t *)(60 * 1024 + i));
    }

    printf("flash test finished\r\n");

    /** 死循环，防止MCU重启 */
    while (1) {
        i++;
    }

    return 0;
}

/** [src_hw_ftfa] */

/* end of file */
