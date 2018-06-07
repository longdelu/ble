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
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief ADC���̣������жϷ�ʽ��ȡ���������ADCת�������ͨ��HW��ӿ�ʵ��
 *
 * - �������裺
 *   1. PIOA_1��������PC���ڵ�TXD��
 *   2. PIOA_2��������PC���ڵ�RXD��
 *   3. J12����ñ�̽ӣ���ʱ�ο���ѹΪ2.5v����
 *   4. PIOE_20(ADCͨ��0) ����ģ�����롣
 *
 * - ʵ������
 *   1. ADCģ����444.44kHz������(����ʱ��Ϊ24MHz)������16λ����;
 *   2. ���������ѹ����ֵ��
 *
 * \par Դ����
 * \snippet demo_hw_adc_int.c src_hw_adc_int
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-26  xjc, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_hw_adc_int
 * \copydoc demo_hw_adc_int.c
 */

/** [src_hw_adc_int] */
#include "ametal.h"
#include "am_board.h"

uint32_t g_adc_dat;               /**< \brief ����ֵ */
/**
 * \brief  ADC �жϷ�����
 */
void adc_isr (void *p_arg)
{
    g_adc_dat = amhw_adc_dat_get(AMHW_ADC0, AMHW_ADC_SEQ_A);
}
/**
 * \brief  ADC ���ú���
 * \param[in] ��
 * \return    ��
 */
void adc_config (void)
{
    /* ����ADCģ������ʻ�ʱ�� */
    amhw_adc_clkdiv_set (AMHW_ADC0, AMHW_ADC_CLK_DIV_2);
    amhw_adc_clksrc_set(AMHW_ADC0, AMHW_ADC_CLK_SRC_BUS);

    /* ���÷ֱ��� */
    amhw_adc_mode_set(AMHW_ADC0, AMHW_ADC_BITS_16);

    /* ʹ��������� , Ĭ��ʹ������A*/
    amhw_adc_hwtrg_disable(AMHW_ADC0);

    /* ʹ����������ģʽ */
    amhw_adc_continue_enable(AMHW_ADC0);

    /* ʹ�ܸ���ת��ģʽ */
    amhw_adc_hsc_enable(AMHW_ADC0);

    /* ��ʹ�ܵ͹��� */
    amhw_adc_lowpower_disable(AMHW_ADC0);

    /* �ر�Ӳ��ƽ���ͱȽϹ��� */
    amhw_adc_cmp_disable(AMHW_ADC0);
    amhw_adc_avg_disable(AMHW_ADC0);

    /* �����жϷ���������ʹ�� */
    am_int_disconnect(INUM_ADC0, adc_isr, NULL);
    am_int_connect(INUM_ADC0, adc_isr, NULL);
    am_int_enable(INUM_ADC0);

    /* ���óɵ���ģʽ�� ͨ��Ϊ0�����жϣ�ͬʱ�����������ȹر�ͨ�� */
    amhw_adc_sc1_cfg(AMHW_ADC0,
                     AMHW_ADC_SEQ_A,
                     AMHW_ADC_SC1_CHAN_SEL_CFG(AMHW_ADC_CHAN_CLOSE) |
                     AMHW_ADC_SC1_INT_EN_CFG);
}

/**
 * \brief  ADC �������ת������
 * \param[in] ��
 * \return    ��
 */
void adc_start (amhw_adc_chan_t chan)
{
     amhw_adc_chan_set(AMHW_ADC0, AMHW_ADC_SEQ_A, chan);
}

int main (void) 
{
    uint32_t adc_mv;                /**< \brief ������ѹ */

    /* �弶��ʼ�� */
    am_board_init();
    
    /* ����ADC ͨ��0ģ���������� */
    am_gpio_pin_cfg(PIOE_20, PIOE_20_ADC0_SE0);
    
    /* ʹ��ADCʱ��                  */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_ADC0);

    /* ADCУ׼ */
    amhw_adc_calibrate(AMHW_ADC0);
    
    /* ADC���ó�ʼ�� */
    adc_config();

    printf("The ADC Int Demo\r\n");

    adc_start(AMHW_ADC_CHAN_0);

    while(1){

      adc_mv = g_adc_dat * 2500 / ((1UL << 16) - 1);
      printf("Vol: %d mv\r\n", adc_mv);
      am_mdelay(200);

    }
}
/** [src_hw_adc_int] */

/* end of file */
