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
 * \brief FTFA例程,通过驱动层的接口实现
 *
 * - 操作步骤：
 *      - 串口接调试串口
 *
 * - 实验现象：
 *      - 擦除出错：串口打印 “erase error!”
 *      - 写入出错：串口打印 "program error!"
 *      - 写入成功：串口打印 60扇区的 (1024/4) 个32bit十六进制数据
 *
 * \par 源代码
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
    uint32_t data[1024 / 4]; /** 要写入flash的数据 */
    uint32_t status;         /** flash命令执行状态 */

    /** 板级初始化 */
    am_board_init();

    /** 数据初始化 */
    for (i = 0; i < 1024 / 4; i++) {
        data[i] = i;
    }

    /** FTFA初始化 */
    amdr_ftfa_init();

    /** 擦除扇区 55 */
    status = amdr_ftfa_sector_erase(55 * 1024);

    /** 扇区擦除出错， 程序停在此处 */
    if (0 != status) {
        printf("erase error!\r\n");

        while (1) {
            ;
        }
    }

    /** 向扇区 55中写入数据 */
    status = amdr_ftfa_flash_program(55 * 1024, data, 1024 / 4);

    /** 扇区写入出错，程序停在此处 */
    if ((1024/4) != status) {
        printf("program error!\r\n");

        while (1);
    }

    /** 写入成功后将数据从flash中读出 */
    for (i = 0; i < 1024; i += 4) {
        printf("%8x ", *(uint32_t *)(1024 * 55 + i));
    }

    printf("flash test finished!\r\n");
    /** 死循环，防止重启 */
    while (1) {
        ;
    }
}

/** [src_dr_ftfa] */

/* end of file */
