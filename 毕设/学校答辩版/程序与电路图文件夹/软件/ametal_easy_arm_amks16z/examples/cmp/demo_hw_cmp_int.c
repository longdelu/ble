/*******************************************************************************
*                                 Apollo
*                       ---------------------------
*                       innovating embedded systems
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co.,  Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief 模拟比较器比较电压例程，通过HW层的接口中断方式实现

 * - 操作步骤：
 *   1. PIOA_1引脚连接PC串口的TXD；
 *   2. PIOA_2引脚连接PC串口的RXD；
 *   3. PIOC_6作为同相输入端，反相输入端接DAC输出
 * - 实验现象：
 *   当输入电压经过1.65v附近时，LED反转（类似与过零检测），串口打印信息：“CMP INT！”
 *
 *
 * \par 源代码
 * \snippet demo_hw_cmp_int.c src_hw_cmp_int
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-26  xjc, first implementation.
 * \endinternal
 */ 

/**
 * \addtogroup demo_if_hw_cmp_int
 * \copydoc demo_hw_cmp_int.c
 */

/** [src_hw_cmp_int] */
#include "ametal.h"  
#include "am_board.h"

/** @} */
/**
 * \brief CMP中断回调函数
 */
void cmp_complete(void *p_arg)
{
    /* 清除中断标志位         */
    amhw_cmp_cff_clr(AMHW_CMP0);

    printf("CMP INIT!\r\n");
    am_led_toggle(0);
}

/**
 * \brief  CMP 配置函数
 * \param[in] 无
 * \return    无
 */
void cmp_init (void)
{

    /* 使能CMP时钟 */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_CMP);

    /* 配置6位DAC */
    /* 选择VDD（Vin2）作为DAC参考输入电压Vin */
    amhw_cmp_dac_vin2_enable(AMHW_CMP0);

    /* 配置电压输出为1.65V = (3.3 / 64) *(31 + 1))*/
    amhw_cmp_dac_vol_set(AMHW_CMP0, 31);

    /* 使能6位DAC */
    amhw_cmp_dac_enable(AMHW_CMP0);

    /* 6位DAC输出连接到反向输入端 */
    amhw_cmp_msel_set(AMHW_CMP0, 7);

    /* PIOC_6为CMP0_IN0   */
    am_gpio_pin_cfg (PIOC_6, PIOC_6_CMP0_IN0);

    /* CMP0_IN0输出连接到同相输入端 */
    amhw_cmp_psel_set(AMHW_CMP0, 0);

    /* 使能比较下降沿中断 */
    amhw_cmp_ief_enable(AMHW_CMP0);

    /* 清除中断标志 */
    amhw_cmp_cff_clr(AMHW_CMP0);

    /* 连接中断 */
    am_int_connect(INUM_CMP0, cmp_complete, NULL);
    am_int_enable(INUM_CMP0);

    /* 使能CMP模块*/
    amhw_cmp_enable(AMHW_CMP0);
}

/**
 * \brief 主函数入口
 */
int main (void)                        
{
    /* 板级初始化  */
    am_board_init();

    printf("CMP Demo:\r\n");
    /* 初始化CMP模块 */
    cmp_init();

    while (1) {
        am_mdelay(1000);
        printf("CMP...\r\n");
    }

}

/** [src_hw_cmp_int] */

/* end of file */
