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
 * \brief ģ��Ƚ����Ƚϵ�ѹ���̣�ͨ��HW��Ľӿ��жϷ�ʽʵ��

 * - �������裺
 *   1. PIOA_1��������PC���ڵ�TXD��
 *   2. PIOA_2��������PC���ڵ�RXD��
 *   3. PIOC_6��Ϊͬ������ˣ���������˽�DAC���
 * - ʵ������
 *   �������ѹ����1.65v����ʱ��LED��ת������������⣩�����ڴ�ӡ��Ϣ����CMP INT����
 *
 *
 * \par Դ����
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
 * \brief CMP�жϻص�����
 */
void cmp_complete(void *p_arg)
{
    /* ����жϱ�־λ         */
    amhw_cmp_cff_clr(AMHW_CMP0);

    printf("CMP INIT!\r\n");
    am_led_toggle(0);
}

/**
 * \brief  CMP ���ú���
 * \param[in] ��
 * \return    ��
 */
void cmp_init (void)
{

    /* ʹ��CMPʱ�� */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_CMP);

    /* ����6λDAC */
    /* ѡ��VDD��Vin2����ΪDAC�ο������ѹVin */
    amhw_cmp_dac_vin2_enable(AMHW_CMP0);

    /* ���õ�ѹ���Ϊ1.65V = (3.3 / 64) *(31 + 1))*/
    amhw_cmp_dac_vol_set(AMHW_CMP0, 31);

    /* ʹ��6λDAC */
    amhw_cmp_dac_enable(AMHW_CMP0);

    /* 6λDAC������ӵ���������� */
    amhw_cmp_msel_set(AMHW_CMP0, 7);

    /* PIOC_6ΪCMP0_IN0   */
    am_gpio_pin_cfg (PIOC_6, PIOC_6_CMP0_IN0);

    /* CMP0_IN0������ӵ�ͬ������� */
    amhw_cmp_psel_set(AMHW_CMP0, 0);

    /* ʹ�ܱȽ��½����ж� */
    amhw_cmp_ief_enable(AMHW_CMP0);

    /* ����жϱ�־ */
    amhw_cmp_cff_clr(AMHW_CMP0);

    /* �����ж� */
    am_int_connect(INUM_CMP0, cmp_complete, NULL);
    am_int_enable(INUM_CMP0);

    /* ʹ��CMPģ��*/
    amhw_cmp_enable(AMHW_CMP0);
}

/**
 * \brief ���������
 */
int main (void)                        
{
    /* �弶��ʼ��  */
    am_board_init();

    printf("CMP Demo:\r\n");
    /* ��ʼ��CMPģ�� */
    cmp_init();

    while (1) {
        am_mdelay(1000);
        printf("CMP...\r\n");
    }

}

/** [src_hw_cmp_int] */

/* end of file */
