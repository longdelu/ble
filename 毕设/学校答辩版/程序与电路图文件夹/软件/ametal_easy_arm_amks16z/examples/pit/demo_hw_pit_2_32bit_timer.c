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
 * \brief PIT 32λ��ʱ��ʵ�ֶ�ʱ���ܵ����̣�ͨ��HW��Ľӿ�ʵ��
 *
 * - ʵ������
 *   1. LED��10Hz��Ƶ����˸��
 *
 * \par Դ����
 * \snippet demo_hw_pit_2_32bit_timer.c src_hw_pit_2_32bit_timer
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-28  cuishengen, first implementation.
 * \endinternal
 */
 
 
/**
 * \addtogroup demo_if_hw_pit_2_32bit_timer
 * \copydoc demo_hw_pit_2_32bit_timer.c
 */
 
/** [src_hw_pit_2_32bit_timer] */
#include "ametal.h"
#include "am_board.h"


/** 
 * \brief  PIT0�жϷ�����
 * \param[in]  p_arg : �û��Զ���������� am_int_connect() �������ݡ�
 * \return  ��
 */
void pit0_irq_handler(void *p_arg)
{
    /* ͨ��0�����ж� */
    if (amhw_pit_timerout_flag_check(AMHW_PIT0, AMHW_PIT_CH(0)) == TRUE ) {
        
        am_led_toggle(LED0);
        
        /* �����־ */
        amhw_pit_timerout_flag_clr(AMHW_PIT0, AMHW_PIT_CH(0));
    }
}

/** 
 * \brief ��ʼ��PITΪ��ʱ�����ܡ�
 *
 * \param[in] p_pit : ָ��PIT�Ĵ������ָ��
 * \param[in] freq  : �ж�Ƶ�ʡ�
 *
 * \return ��
 */
void pit_timer_init (amhw_pit_t *p_hw_pit, uint32_t freq)
{
    uint32_t pit_clk, load_val;

    /* ����װ��ֵ */
    pit_clk = amhw_clock_periph_freq_get(p_hw_pit);
    load_val = pit_clk / freq;

    /* ʹ��PITģ�� */
    amhw_pit_module_enable(p_hw_pit);
    
    /* ����װ��ֵ */
    amhw_pit_ldval_set(p_hw_pit, AMHW_PIT_CH(0), load_val);
    
    /* ���жϲ�ʹ�ܼ��� */
    amhw_pit_timer_ctrl_set(p_hw_pit,
                            AMHW_PIT_CH(0),
                            AMHW_PIT_TCTL_TI_EN | AMHW_PIT_TCTL_TMR_EN);
   
    am_int_connect(INUM_PIT, pit0_irq_handler, (void *)0);
    am_int_enable(INUM_PIT);
}


/** 
 * \brief ʹ��PITΪ��ʱ��(��ʼ����)
 * \param[in] p_pit : ָ��PIT�Ĵ������ָ��
 * \return ��
 */
void pit_timer_enable (amhw_pit_t *p_hw_pit)
{
    /* ��ʼ���� */
    amhw_pit_timer_ctrl_set(p_hw_pit,
                            AMHW_PIT_CH(0),
                            AMHW_PIT_TCTL_TMR_EN);
}

/** 
 * \brief  ��ֹPITΪ��ʱ��(ֹͣ����)
 * \param[in] p_pit : ָ��PIT�Ĵ������ָ��
 * \return  ��
 */
void pit_timer_disable (amhw_pit_t *p_hw_pit)
{
    /* ��ֹͣλ��PITֹͣ���� */
    amhw_pit_timer_ctrl_clear(p_hw_pit,
                              AMHW_PIT_CH(0),
                              AMHW_PIT_TCTL_TMR_EN);
}

/**
 * \brief ���������
 */
int main (void)
{  
    /* �弶��ʼ�� */
    am_board_init();

    /* ƽ̨��ʼ�� */
    amhw_plfm_pit0_timing_init();

    AM_DBG_INFO("The demo for PIT timing:\r\n");
    AM_DBG_INFO("The led toggle in 10Hz \r\n");

    /* ʹ��PITʵ�ֶ�ʱ���ܣ��ж�Ƶ��Ϊ10Hz */
    pit_timer_init(AMHW_PIT0, 10);
    
    /* ʹ��PIT���� */
    pit_timer_enable(AMHW_PIT0);
    
    while (1) {

    }
}

/** [src_hw_pit_2_32bit_timer] */

/* end of file */
