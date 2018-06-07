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
 * \brief   TSI�����жϼ�⣬������ʵ��
 *
 *  ʵ������
 *  1.ʵ������PIOA_1����ΪTSIͨ��2���ܣ����ֲ��Ӵ�PIOA_1����ʱ���ᴥ���ж�
 *  2.������PIOA_1���ź󣬵��ݼ����ڷ�ֵ����1ֵʱ�������жϣ�LED1״̬��ת
 *
 *  ע�⣺
 *  ��TSI����Ϊ�������ģʽʱ����Ҫִ���������ɨ�躯������ִ��һ��ɨ�裬����
 *  ��ʹ��ʱ��������Ϊ�������ģʽʱ����Ҫһֱ���߶�ʱִ������ɨ�躯����
 *
 * \par Դ����
 * \snippet demo_std_tsi_int.c src_std_tsi_int
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-26  wangxiaojian, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_std_tsi_int
 * \copydoc demo_std_tsi_int.c
 */
 
/** [src_std_tsi_int] */
#include "ametal.h"
#include "am_board.h"

void tsi_isr(void * p_arg)
{
    /* TSI�жϽ��� */
    amdr_tsi_int_disable();

    /* �峬��ֵ��־ */
    amhw_tsi_outrgf_clr(AMHW_TSI);

    am_led_toggle(LED1);
    am_mdelay(100);

    /* TSI�ж�ʹ�� */
    amdr_tsi_int_enable();
}

/**
 * \brief ���������
 */
int main (void)
{  
    /* �弶��ʼ��  */
    am_board_init();
    
    /* tsiƽ̨��ʼ�� */
    amhw_plfm_tsi_init();

    /* TSI�豸��ʼ�� */
    am_tsi_init(&g_tsi_dev,&g_tsi_devinfo);

    /* TSI�������� */
    amdr_tsi_fun_cfg(AMHW_TSI_CHAN2,
                     AMHW_TSI_CAP_SENSING,
                     AMHW_TSI_TRIG_SOFTWARE);

     /* TSI�жϴ���ģʽΪ����ֵ����ģʽ */
    amdr_tsi_trigger_cfg(AMHW_TSI_OUT_RANGE);

    /* ����TSI�жϷ����� */
    amdr_tsi_trigger_connect(&tsi_isr);

    /* ����TSI������ */
    amdr_tsi_sens_set(500);

    /* TSI�ж�ʹ�� */
    amdr_tsi_int_enable();

    while (1) {

        /* �����������ɨ�� */
        amhw_tsi_soft_tirg_start(AMHW_TSI);

    }
}

/** [src_std_tsi_int] */
 
/* end of file */

