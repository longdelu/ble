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
 * \brief DAC��ģת�����̣�ͨ��STD�ӿ�ʵ��
 *
 * - ʵ������
 *   1. �ɼ���ģת���������PE30�ĵ�ѹֵ
 *   2. �������PE30�ĵ�ѹֵ��
 *
 *
 * \par Դ����
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
 * \brief ���������
 */

int main (void)
{

    am_dac_handle_t dac0_handle = NULL; /** \brief ADC��׼���������� */
    uint32_t dac_bits = 0;
    uint32_t dac_vref = 0;
    uint32_t mv_out   = 1523;           /* ��Ҫ����ĵ�ѹֵ  ��λ��mv   */

    /* �弶��ʼ��  */
    am_board_init();

    /* ƽ̨��ʼ�� */
    amhw_plfm_dac0_init();

    dac0_handle = amdr_dac_init(&g_dac0_dev, &g_dac0_devinfo);

    /* ��ȡDACת������ */
    dac_bits = am_dac_bits_get(dac0_handle);

    /* ��ȡDAC�ο���ѹ����λ��mV */
    dac_vref = am_dac_vref_get(dac0_handle);

    am_dac_mv_set(dac0_handle,AMHW_DAC0_CHAN_0,mv_out);

    am_dac_enable(dac0_handle,AMHW_DAC0_CHAN_0);       /* ʹ��DAC   */


    while (1) {


    }
}


 /** [src_std_dac] */
 
/* end of file */

