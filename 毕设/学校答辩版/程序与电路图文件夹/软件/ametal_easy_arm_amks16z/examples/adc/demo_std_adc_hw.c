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
 * \brief ADC���̣�Ӳ������ת����DMA����ת�������ͨ����׼�ӿ�ʵ��
 *
 * - �������裺
 *   1. PIOA_1��������PC���ڵ�TXD��
 *   2. PIOA_2��������PC���ڵ�RXD��
 *   3. J12����ñ�̽ӣ���ʱ�ο���ѹΪ2.5v����
 *   4. PIOE_20(ADCͨ��0) ����ģ�����롣
 *
 * - ʵ������
 *   1. ���������ѹ����ֵ��
 *
 * \par Դ����
 * \snippet demo_std_adc_hw.c src_std_adc_hw
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-26  xjc, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_adc_hw
 * \copydoc demo_std_adc_hw.c
 */

/** [src_std_adc_hw] */
#include "ametal.h"
#include "am_board.h"

/** \brief ADC �豸��Ϣ ��Ӳ���������ޱȽ���Ӳ��ƽ����*/
const amdr_adc_devinfo_t g_adc0_devinfo_hw =
{
    AMHW_ADC0,                     /**< \brief ָ��ADC0�Ĵ������ָ�� */
    2500,                          /**< \brief ADC�ο���ѹ����λ��mV  */
    12,                            /**< \brief ADC��������(λ��)   */
    AMDR_ADC_DIFF_DIS,             /**< \brief ADC0����ģʽ        */
    INUM_ADC0,                     /**< \brief ADC�ж�������  */
    AMDR_ADC_MODE_TRG_HW,          /**< \brief ADCӲ������ģʽ      */
    AMHW_ADC_SEQ_A,                /**< \brief ADC��������            */
    DMA_CHAN_0,                    /**< \brief DMAͨ��0���� ��ֵΪ-1ʱ��ʹ��DMA */
    NULL,                          /**< \brief ��ʹ��Ӳ��ƽ������       */
    NULL,                          /**< \brief ��ʹ�ñȽϹ���       */
};

/**
 * \brief  ��ʱ��0�ص�����
 * \param[in] p_arg : �û��Զ����������am_timer_callback_set() �������ݡ�
 * \return  ��
 */
void tpm0_timing_callback (void *p_arg)
{
    am_led_toggle(LED0);
}
/**
 * \brief ���������
 */
int main (void)
{
    uint32_t adc_mv[5];             /**< \brief ������ѹ */
    int chan = 0;                   /**< \brief ͨ��0 */
    int i;

    am_adc_handle_t   adc0_handle;          /**< \brief ADC��׼���������� */
    am_timer_handle_t tpm0_timing_handle;   /**< \brief TPM��׼���������� */
    am_timer_info_t   tpm0_timing_info;     /**< \brief ��ʱ��Ϣ                     */


    /* �弶��ʼ�� */
    am_board_init();

    /* DMAƽ̨��ʼ�� */
    amhw_plfm_dma_init();

    /* DMA��ʼ�� */
    amdr_dma_init(&g_dma_dev, &g_dma_devinfo);

    /* ADCƽ̨��ʼ�� */
    amdr_plfm_adc0_init(); 
   
    /* TPMƽ̨��ʼ�� */
    amhw_plfm_tpm0_timing_init();

    /* ��ʼ����ʱ��0Ϊ��ʱ���� */
    tpm0_timing_handle = amdr_tpm_timing_init(&g_tpm0_timing_dev,
                                              &g_tpm0_timing_devinfo);
    am_timer_connect(tpm0_timing_handle);

    am_timer_info_get(tpm0_timing_handle, &tpm0_timing_info);
    am_timer_callback_set(tpm0_timing_handle, 1, tpm0_timing_callback, NULL);

    /* �趨�ж�Ƶ��20Hz 1���Ӵ���20��ADCת�� */
    am_timer_enable(tpm0_timing_handle, 1, tpm0_timing_info.clk_frequency / 20);

    /* ʹ��ADCӲ����ѡ����Դ��ѡ��TPM0������� */
    amhw_sim_adc0_alter_trigger_enable();
    amhw_sim_adc0_trigger_set(AMHW_SIM_ADC0_TRIGGER_TPM0);

    /* ����ADC ͨ��0ģ���������� */
    am_gpio_pin_cfg(PIOE_20, PIOE_20_ADC0_SE0);
    
    /* ��ʼ��ADC������ȡADC��׼���������� */
    adc0_handle = amdr_adc_init(&g_adc0_dev, &g_adc0_devinfo_hw);

    printf("The ADC HW Demo\r\n");

    while (1) {
        
        /* ��ȡADC�ɼ���ѹ��������ɲŷ��� */
        am_adc_read_mv(adc0_handle, chan, adc_mv, 5);
        for (i = 1; i < 5; i++) {
            adc_mv[0] += adc_mv[i];
        }
        adc_mv[0] /= 5;
        printf("Vol: %d mv\r\n", adc_mv[0]);
    }
}

/** [src_std_adc_hw] */
 
/* end of file */
