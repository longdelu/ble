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
 * \brief ADC���̣��������ת����DMA����ת�������ͨ����׼�ӿ�ʵ��
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
 * \snippet demo_std_adc_sw.c src_std_adc_sw
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-26  xjc, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_adc_sw
 * \copydoc demo_std_adc_sw.c
 */

/** [src_std_adc_sw] */
#include "ametal.h"
#include "am_board.h"

/**
 * \brief ���������
 */
int main (void)
{
    uint32_t adc_bits = 0;          /**< \brief ADCλ�� */
    uint32_t adc_vref = 0;          /**< \brief ADC�ο���ѹ*/
    uint32_t adv_val0[10];          /**< \brief ADC����ԭʼֵ */
    uint32_t adc_mv;                /**< \brief ������ѹ */
    uint32_t cnt = 0;               /**< \brief ��ȡ���� */
    int chan = 0;                   /**< \brief ͨ��0 */
    int i;

    am_adc_handle_t adc0_handle; /**< \brief ADC��׼���������� */

    /* �弶��ʼ�� */
    am_board_init();

    /* ƽ̨��ʼ�� */
    amdr_plfm_adc0_init(); 

    /* ����ADC ͨ��0ģ���������� */
    am_gpio_pin_cfg(PIOE_20, PIOE_20_ADC0_SE0);
    
    /* ƽ̨��ʼ�� */
      amhw_plfm_dma_init();

      /* DMA��ʼ�� */
      amdr_dma_init(&g_dma_dev, &g_dma_devinfo);

    /* ��ʼ��ADC������ȡADC��׼���������� */
    adc0_handle = amdr_adc_init(&g_adc0_dev, &g_adc0_devinfo);

    /* ��ȡADCת������ */
    adc_bits = am_adc_bits_get(adc0_handle, chan);
    
    /* ��ȡADC�ο���ѹ����λ��mV */
    adc_vref = am_adc_vref_get(adc0_handle, chan);
    printf("The ADC SW Demo\r\n");

    while (1) {
        
        /* ��ȡADCת�������������ɲŷ��� */
        am_adc_read(adc0_handle, chan, adv_val0, 10);
        cnt++;
        if (cnt >= 200UL) { /* ��ʱ�����ƴ��ڴ�ӡ����  */
            cnt = 0;
            for (i = 1; i < 10; i++) {
                adv_val0[0] += adv_val0[i];
            }
            adv_val0[0] /= 10;
            adc_mv = adv_val0[0] * adc_vref / ((1UL << adc_bits) - 1);
            /* �������������ѹֵ */
            printf("Vol: %d mv\r\n", adc_mv);
        }
    }
}

/** [src_std_adc_sw] */
 
/* end of file */
