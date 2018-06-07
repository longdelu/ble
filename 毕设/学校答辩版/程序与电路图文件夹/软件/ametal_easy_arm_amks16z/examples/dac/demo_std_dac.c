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
 * \brief DAC数模转换例程，通过STD接口实现
 *
 * - 实验现象：
 *   1. 采集数模转换的输出口PE30的电压值
 *   2. 串口输出PE30的电压值。
 *
 *
 * \par 源代码
 * \snippet demo_std_dac.c src_std_dac 
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-20  xym, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_std_dac
 * \copydoc demo_std_dac.c
 */
 
 /** [src_std_dac] */
#include "ametal.h"
#include "am_board.h"

/**
 * \brief 主函数入口
 */

int main (void)
{

    am_dac_handle_t dac0_handle = NULL; /** \brief ADC标准服务操作句柄 */
    uint32_t dac_bits = 0;
    uint32_t dac_vref = 0;
    uint32_t mv_out   = 1523;           /* 需要输出的电压值  单位：mv   */

    /* 板级初始化  */
    am_board_init();

    /* 平台初始化 */
    amhw_plfm_dac0_init();

    dac0_handle = amdr_dac_init(&g_dac0_dev, &g_dac0_devinfo);

    /* 获取DAC转换精度 */
    dac_bits = am_dac_bits_get(dac0_handle);

    /* 获取DAC参考电压，单位：mV */
    dac_vref = am_dac_vref_get(dac0_handle);

    am_dac_mv_set(dac0_handle,AMHW_DAC0_CHAN_0,mv_out);

    am_dac_enable(dac0_handle,AMHW_DAC0_CHAN_0);       /* 使能DAC   */


    while (1) {


    }
}


 /** [src_std_dac] */
 
/* end of file */

