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
 * \brief DAC��ģת�����̣�ͨ��HW�ӿ�ʵ��
 *
 * - ʵ������
 *   1. �ɼ���ģת���������PE30�ĵ�ѹֵ
 *   2. �������PE30�ĵ�ѹֵ��
 *
 *
 * \par Դ����
 * \snippet demo_hw_dac.c src_hw_dac 
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-22  xym, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_hw_dac
 * \copydoc demo_hw_dac.c
 */
 
 /** [src_hw_dac] */
#include "ametal.h"
#include "am_board.h"

/**
 * \brief ���������
 */

void __dac_init (void)
{

    /* ����PIOE_30ΪDAC0_OUT����                 */
    am_gpio_pin_cfg (PIOE_30,PIOE_30_DAC0_OUT);

    /* ����DACʱ��                               */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_DAC0);

    /*
     * ����DAC��ع���
     */
    amhw_dac_c0_cfg(AMHW_DAC0,AMHW_DAC_C0_EN                        |
                              AMHW_DAC_C0_VREF_SEL(AMHW_DAC_VREF_2) |
                              AMHW_DAC_C0_TRG_SEL(AMHW_DAC_TRG_SOFT)|
                              AMHW_DAC_C0_POWER_SEL(AMHW_DAC_POWER_LOW));
}

void  __dac_trigger (uint16_t data)
{
     /*  DAC�Ĵ�����ֵ*/
    amhw_dac_dat_set(AMHW_DAC0,AMHW_DAC_DAT_0,data);
     /* ʹ��DAC�������            */
    amhw_dac_soft_trg_enable(AMHW_DAC0);
}

int main (void)
{
    /* ����ת��ֵ              */
    uint16_t v_out = 1.823 * 4096 / 3.3;
    /* �弶��ʼ��  */
    am_board_init();
    __dac_init();
    while (1) {
        __dac_trigger(v_out);

    }
}


 /** [src_hw_dac] */
 
/* end of file */


