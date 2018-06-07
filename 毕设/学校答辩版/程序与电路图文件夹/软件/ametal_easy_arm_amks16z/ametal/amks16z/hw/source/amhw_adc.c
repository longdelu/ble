/*******************************************************************************
*                                 Apollo
*                       ----------------------------
*                       innovating embedded platform
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
 * \brief ADC Ӳ�������ӿ�ʵ�֡�
 *
 * 1. 16λ��αƽ�����ģת������
 * 2. �ߴ�4�Բ�����룬24������ģ�����룻
 * 3. ���λ�����ת����ת������Զ���Χ����״̬��
 * 4. �����ò���ʱ�䣬ת���ٶȻ򹦺ģ�
 * 5. ��ѡ���/����ֵ�ȽϺ�Ӳ��ƽ����
 * 6. ת����ɻ�Ӳ��ƽ������ж�����
 * 7. �ĸ���ѡʱ��Դ��
 * 8. �Դ��¶ȴ�������
 * 9. ��ѡ�Ĳο���ѹԴ��
 * 10. Ӳ��У��ģʽ;
 * 11. 2������������ת�����У�
 * 12. ֧�����ģʽ�У�
 *     ---16λ��13λ��11λ��9λ�������ģʽ
 *     ---16λ��12λ��18λ��8λ��������ģʽ
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-15  bob, first implementation.
 * \endinternal
 */

#include "amhw_adc.h"
#include "am_clk.h"
#include "amks16z_clk_id.h"

/******************************************************************************
* Public functions
*******************************************************************************/

/**
 * \brief ��ʱ����
 */
void __amhw_adc_delay (int dly)
{
    int x = 1000, y;
    while (x-- > 0) {
        y = dly;
        while (y-- > 0) {
            ;
        }
    }
}

/**
 * \brief ADC У��, Ĭ�Ͻ���ʱ��Ƶ��Ϊ2M
 */

uint8_t amhw_adc_calibrate (amhw_adc_t *p_hw_adc)
{
    uint16_t cal = 0;
    uint32_t cnt = 0;
    uint32_t bus_clk;
    uint8_t  adc_clkdiv;

    bus_clk = am_clk_rate_get(CLK_BUS);

    adc_clkdiv = bus_clk / 2000000UL ;

    if (adc_clkdiv <= 1) {
        adc_clkdiv = AMHW_ADC_CLK_DIV_1;
    } else if (adc_clkdiv <= 2) {
        adc_clkdiv = AMHW_ADC_CLK_DIV_2;
    } else if (adc_clkdiv <= 4) {
        adc_clkdiv = AMHW_ADC_CLK_DIV_4;
    } else {
        adc_clkdiv = AMHW_ADC_CLK_DIV_8;
    }
    amhw_adc_clkdiv_set (p_hw_adc, (amhw_adc_clk_div_t)adc_clkdiv);
    amhw_adc_clksrc_set(p_hw_adc, AMHW_ADC_CLK_SRC_BUS);

    /* ����Ӳ��ƽ�� */
    amhw_adc_avgs_set(p_hw_adc, AMHW_ADC_AVGS_32);
    amhw_adc_avg_enable(p_hw_adc);

    /* ����SC2, ������� */
    amhw_adc_hwtrg_disable(p_hw_adc);
    /* ʹ�ܽ��� �� ��ʼ���� */
    amhw_adc_cal_enable(p_hw_adc);

    /* ѭ���ڵȴ� ��ֱ��������� */
    while (amhw_adc_cal_get(p_hw_adc)) { /* ������������2ms���� */
        __amhw_adc_delay(1);
    }

    if (amhw_adc_cal_fail_is(p_hw_adc)) {
        /* ����ʧ�� */
        return FALSE;
    }

    cal = 0;
    for (cnt = 1; cnt < 7; cnt++) {
        cal += amhw_adc_clp_get(p_hw_adc, cnt);
    }
    cal >>= 1;
    cal |= 0x8000;
    amhw_adc_pg_set(p_hw_adc, cal);

    cal = 0;
    for (cnt = 1; cnt < 7; cnt++) {
       cal += amhw_adc_clm_get(p_hw_adc, cnt);
    }
    cal >>= 1;
    cal |= 0x8000;
    amhw_adc_mg_set(p_hw_adc, cal);
    return TRUE;
}

/* end of file */
