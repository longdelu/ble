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
 * \brief   TSI�����жϼ�⣬Ӳ����ʵ��
 *
 *  ʵ������
 *  1.���ֲ��Ӵ�PIOA_1����ʱ�������⵽�Ļ���ֵ
 *  2.������PIOA_1���ź󣬵��ݼ����ڷ�ֵ����1000ʱ�������жϣ�LED1״̬��ת
 *
 * \par Դ����
 * \snippet demo_hw_tsi_int.c src_hw_tsi_int
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-26  xjc, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_hw_tsi_int
 * \copydoc demo_hw_tsi_int.c
 */
 
/** [src_hw_tsi_int] */
#include "ametal.h"
#include "am_board.h"
    
void tsi_isr(void * p_arg)
{
    /* �峬��ֵ��־ */
    amhw_tsi_outrgf_clr(AMHW_TSI);
       
    /* ��ɨ�������־ */
    amhw_tsi_eosf_clr(AMHW_TSI);
    am_led_toggle(LED1);
}

/**
 * \brief ���������
 */
int main (void)
{  
    uint16_t value = 0;
    
    /* �弶��ʼ��  */
    am_board_init();
    
    /* ʹ��tsi�������ʱ�� */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_TSI);

    /* ����PIOA_1����ΪTSI0_CH2 */
    am_gpio_pin_cfg(PIOA_1, PIOA_1_TSI0_CH2);

    /* �����ⲿ�����㶨����Ϊ16uA */
    amhw_tsi_extchrg_val_set(AMHW_TSI, AMHW_TSI_EXTCHRG_16UA);

    /* ���õ�ѹ����Ϊ600mV (Vp - Vm)*/
    amhw_tsi_dvolt_set (AMHW_TSI, AMHW_TSI_DV073);

    /* ɨ��Ԥ��Ƶ����Ϊ2��Ƶ */
    amhw_tsi_oscfreq_div_set(AMHW_TSI, AMHW_TSI_OSCFRQ_DIV_2);

    /* ���õ缫ɨ�����Ϊ16 */
    amhw_tsi_scan_num_set(AMHW_TSI, 16);

    /* ���òο�������ŵ����Ϊ32uA */
    amhw_tsi_refchrg_val_set(AMHW_TSI, AMHW_TSI_REFCHRG_32UA);

    /* ���ô���ɨ��ģʽ */
    amhw_tsi_scan_trig_set(AMHW_TSI, AMHW_TSI_TRIG_SOFTWARE);

    /* ʹ��TSIͨ�� */
    amhw_tsi_chan_set(AMHW_TSI, AMHW_TSI_CHAN2);

    /* ʹ��TSIģ�� */
    amhw_tsi_mode_enable(AMHW_TSI);
    
    /* ����TSI�жϷ����� */
    am_int_connect(INUM_TSI0, tsi_isr, NULL);
    
    /* TSI�ж�ʹ�� */
    amhw_tsi_int_enable(AMHW_TSI);
    
    /* ϵͳ�ж�ʹ�� */
    am_int_enable(INUM_TSI0);
    
    /* ���÷�ֵ���� */
    amhw_tsi_wake_chan_thresholdh_write(AMHW_TSI, 1000);
    
    /* ���÷�ֵ���� */
    amhw_tsi_wake_chan_thresholdl_write(AMHW_TSI, 100);
       
    while (1) {

        /* �����������ɨ�� */
        amhw_tsi_soft_tirg_start(AMHW_TSI);  
        
        /* ��ȡ������ */
        value = amhw_tsi_convert_value_get(AMHW_TSI);

        printf("%d\r\n",value);
        am_mdelay(100);

    }
}

/** [src_hw_tsi_int] */
 
/* end of file */

