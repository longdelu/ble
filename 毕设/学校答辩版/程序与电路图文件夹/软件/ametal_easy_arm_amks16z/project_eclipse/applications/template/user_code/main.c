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
 * \brief 
 *
 * - �������裺
 *   1. PIOA_1��������PC���ڵ�TXD��
 *   2. PIOA_2��������PC���ڵ�RXD��
 *   3. �ⲿ����8MHzʱ��������Ϊ115200
 *
 * - ʵ������
 *   1. LED��������
 *   2. ����������ӡ���"The program is running!"��
 *
 * \par Դ����
 * \snippet demo_main.c src_main
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-16  xjc, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_main
 * \copydoc demo_main.c
 */
 
/** [src_main] */
#include "ametal.h"
#include "am_board.h"
#include "am_clk.h"
#include "amks16z_clk_id.h"

/**
 * \brief ���������
 */
int main (void)
{  
    /* �弶��ʼ��  */
    am_board_init();

    while (1) {

        printf("The program is running!\r\n");
        printf("core_clk %d, bus_clk %d",
               am_clk_rate_get(CLK_SYSTEM),
               am_clk_rate_get(CLK_BUS));

        am_mdelay(500);
        am_led_toggle(LED0) ;/* LED0��ת */
    }
}

/** [src_main] */
 
/* end of file */

