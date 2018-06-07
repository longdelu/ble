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
 * \brief LPTMRʵ�ֶ�ʱ�����̣�ͨ��HW��ӿ�ʵ��
 *
 * - ʵ������
 *   1. LED0��10Hz��Ƶ����˸��
 *
 * \note LPTMRʱ��Դ��4��ѡ����amdr_hwconfig_lptmr_timing.c�н������ã�ѡ��ͬ��С
 *       ��ʱ��Դ����ʱ�ķ�Χ�;��ȴ��ڲ��
 *
 * \par Դ����
 * \snippet demo_hw_lptmr_timer.c src_hw_lptmr_timer
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  cuishengen, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_hw_lptmr_timer
 * \copydoc demo_hw_lptmr_timer.c
 */
 
/** [src_hw_lptmr_timer] */
#include "ametal.h"
#include "am_board.h"

/** 
 * \brief  ��ʱ��0�ص�����
 * \param[in] p_arg : �û��Զ����������am_timer_callback_set() �������ݡ�
 * \return  ��
 */
void lptmr_timing_callback (void *p_arg)
{
    if (amhw_lptmr_flag_check(AMHW_LPTMR)) {
        am_led_toggle(LED0);
        /* ��������־��д1���� */
        amhw_lptmr_ctl_set(AMHW_LPTMR, AMHW_LPTMR_CSR_TCF);
    }
}

/**
 * \brief ��ʼ��LPTMR
 *
 * \param[in] p_hw_lptmr : ָ��LPTMR�Ĵ������ָ��
 * \param[in] freq       : �ж�Ƶ�ʡ�
 *
 * \retval  AM_OK      : ��ʼ���ɹ�
 * \retval -AM_EINVAL  : ��������
 */
int lptmr_timer_init(amhw_lptmr_t *p_hw_lptmr, uint32_t count)
{
    uint32_t pre_reg = 0, pre_real;
    uint32_t temp;

    if (count > (0xFFFFu * 65536)) {
        return -AM_EINVAL;
    }

    /* ����Ҫ��Ƶ�����ܷ�Ƶ�� */
    if (count < 0xFFFF) {
        pre_real = 1;
        amhw_lptmr_prescaler_filter_set(p_hw_lptmr, TRUE);
    } else {
        temp = count / 0xFFFFu + 1;
        /* �����Ƶֵ 2^n */
        for (pre_real = 2; pre_real < temp;) {
            pre_reg++;
            pre_real = pre_real << 1;
        }
        /* ��Ƶ����Ч */
        amhw_lptmr_prescaler_filter_set(p_hw_lptmr, FALSE);
        /* ����Ԥ��Ƶֵ */
        amhw_lptmr_prescaler_set(p_hw_lptmr, (amhw_lptmr_ps_t)pre_reg);
    }

    count = count / pre_real;

    /* ����װ��ֵ */
    amhw_lptmr_compare_set(p_hw_lptmr, count - 1);

    amhw_lptmr_ctl_set(p_hw_lptmr, AMHW_LPTMR_CSR_TIE);        /* ���ж� */
    am_int_connect(INUM_LPTMR0, lptmr_timing_callback, (void *)0);
    am_int_enable(INUM_LPTMR0);

    return AM_OK;
}

/**
 * \brief ʹ��LPTMR��ʱ��(��ʼ����)
 * \param[in] p_hw_lptmr : ָ��LPTMR�Ĵ������ָ��
 * \return ��
 */
void lptmr_timer_enable (amhw_lptmr_t *p_hw_lptmr)
{
    /* �ر�ʱ�ӣ�LPTMR��ʼ���� */
    amhw_lptmr_ctl_set(p_hw_lptmr, AMHW_LPTMR_CSR_TEN);
}

/**
 * \brief  ��ֹLPTMR��ʱ��(ֹͣ����)
 * \param[in] p_hw_lptmr : ָ��LPTMR�Ĵ������ָ��
 * \return  ��
 */
void sct_timer_disable (amhw_lptmr_t *p_hw_lptmr)
{
    /* �ر�ʱ�ӣ�TPMֹͣ���� */
    amhw_lptmr_ctl_clear(p_hw_lptmr, AMHW_LPTMR_CSR_TEN);
}

/**
 * \brief ���������
 */
int main (void)
{
    uint32_t lptmr_clock;

    /* �弶��ʼ�� */
    am_board_init();
    
    /* ƽ̨��ʼ�� */
    amhw_plfm_lptmr_timing_init();
    
    AM_DBG_INFO("The LPTMR demo for standard timing service\r\n");
    AM_DBG_INFO("The led toggle in 10Hz \r\n");
    
    /* ��ȡ����Ƶ�� */
    lptmr_clock = amhw_lptmr_clock_get(AMHW_LPTMR);

    /* �ж�Ƶ��20Hz */
    lptmr_timer_init(AMHW_LPTMR, lptmr_clock / 20);
    
    /* ʹ��LPTMR,��ʼ���� */
    lptmr_timer_enable(AMHW_LPTMR);
    
    while (1) {

    }
}

/** [src_hw_lptmr_timer] */

/* end of file */
