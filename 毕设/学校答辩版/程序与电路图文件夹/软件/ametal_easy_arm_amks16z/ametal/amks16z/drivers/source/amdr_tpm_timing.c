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
 * \brief KS16z TPM ��ʱ����������ʵ��
 *
 * 1. TPM֧���ṩ�������ֱ�׼���񣬱������ṩ���Ƿ���"��ʱ"��׼���������
 *     - ��ʱ
 *     - PWM���
 *     - ����
 * 2. һ��TPMģ�飬ֻ��һ����ʱͨ��
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-20  cuishengen, first implementation.
 * \endinternal
 */

#include "amdr_tpm_timing.h"
#include "am_int.h"
#include "amdr_clk.h"

/*******************************************************************************
  ��ʱ��������������
*******************************************************************************/

static int __tpm_timing_info_get(void *p_drv, am_timer_info_t *p_info);

static int __tpm_timing_count_get(void     *p_drv,
                                  uint8_t   chan,
                                  uint32_t *p_count);

static int __tpm_timing_rollover_get(void     *p_drv,
                                     uint8_t   chan,
                                     uint32_t *p_rollover);

static int __tpm_timing_disable(void *p_drv, uint8_t chan);

static int __tpm_timing_enable(void     *p_drv,
                               uint8_t   chan,
                               uint32_t  max_tpm_count);

static int __tpm_timing_callback_set(void    *p_drv,
                                     uint8_t  chan,
                                     void   (*pfn_callback)(void *),
                                     void    *p_arg);

static int  __tpm_timing_connect(void *p_drv);

static void __tpm_irq_handler(void *p_arg);

/** \brief TPM��ʱ���������� */
static const struct am_timer_drv_funcs __g_tpm_timing_drv_funcs = {
    __tpm_timing_info_get,
    __tpm_timing_count_get,
    __tpm_timing_rollover_get,
    __tpm_timing_disable,
    __tpm_timing_enable,
    __tpm_timing_callback_set,
    NULL,
    NULL,
    NULL,
    __tpm_timing_connect,
};

/******************************************************************************/

/**
 * \brief TPM ��ʱ���ж�����
 */
int __tpm_timing_connect (void *p_drv)
{
    amdr_tpm_timing_dev_t *p_dev = (amdr_tpm_timing_dev_t *)p_drv;

    am_int_connect(p_dev->p_devinfo->inum, __tpm_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);

    return AM_OK;
}

void __tpm_irq_handler (void *p_arg)
{
    amdr_tpm_timing_dev_t *p_dev    = (amdr_tpm_timing_dev_t *)p_arg;
    amhw_tpm_t            *p_hw_tpm =  p_dev->p_devinfo->p_hw_tpm;

    /*
     * �ж��Ƿ��Ƕ�ʱ������ж�
     */
    if (amhw_tpm_stat_flag_get(p_hw_tpm) & AMHW_TPM_STAT_OV_FLAG ) {

        if (p_dev->pfn_callback != NULL) {
            p_dev->pfn_callback(p_dev->p_arg);
        }

        /* ��������־ */
        amhw_tpm_stat_flag_clear(p_hw_tpm, AMHW_TPM_STAT_OV_FLAG);
    }
}

/** \brief ��ȡTPM��Ϣ */
static int __tpm_timing_info_get (void *p_drv, am_timer_info_t *p_info)
{
    amdr_tpm_timing_dev_t *p_dev = (amdr_tpm_timing_dev_t *)p_drv;
    
    /* ��ȡ����Ƶ�� */
    p_info->clk_frequency = amdr_clk_periph_rate_get(p_dev->p_devinfo->p_hw_tpm);

    p_info->max_frequency = p_info->clk_frequency;
    /* ��ʱ����С�ж�Ƶ�ʣ�16λ��ʱ��������Ƶϵ��������Ϊ128 */
    p_info->min_frequency = p_info->clk_frequency / (0xFFFF * 128) + 1;

    p_info->features  = AM_TIMER_SIZE(16)           |   /* 16λ��ʱ�� */
                        AM_TIMER_CHAN_NUM(1)        |   /* ֧��1��ͨ�� */
                        AM_TIMER_CAN_INTERRUPT      |   /* ���Բ����ж� */
                        AM_TIMER_INTERMEDIATE_COUNT |   /* ����ʵʱ��������ֵ */
                        AM_TIMER_AUTO_RELOAD;           /* �Զ�װ�� */

    return AM_OK;
}

/**
 * \brief ��ȡTPM��ǰ����ֵ
 */
static int __tpm_timing_count_get (void     *p_drv,
                                   uint8_t   chan,
                                   uint32_t *p_count)
{
    amdr_tpm_timing_dev_t *p_dev    = (amdr_tpm_timing_dev_t *)p_drv;
    amhw_tpm_t            *p_hw_tpm =  p_dev->p_devinfo->p_hw_tpm;

    *p_count = amhw_tpm_count_get(p_hw_tpm);

    return AM_OK;
}

/**
 * \brief ��ȡTPM����ֵ
 */
static int __tpm_timing_rollover_get (void     *p_drv,
                                      uint8_t   chan,
                                      uint32_t *p_rollover)
{
    amdr_tpm_timing_dev_t *p_dev    = (amdr_tpm_timing_dev_t *)p_drv;
    amhw_tpm_t            *p_hw_tpm =  p_dev->p_devinfo->p_hw_tpm;

    *p_rollover = 0xFFFF / amdr_clk_periph_rate_get((void*)p_hw_tpm);


    return AM_OK;
}

/**
 * \brief TPM����
 */
static int __tpm_timing_disable (void *p_drv, uint8_t chan)
{
    amdr_tpm_timing_dev_t *p_dev    = (amdr_tpm_timing_dev_t *)p_drv;
    amhw_tpm_t            *p_hw_tpm =  p_dev->p_devinfo->p_hw_tpm;

    amhw_tpm_clock_mode(p_hw_tpm, AMHW_TPM_CLK_SRC_NONE);

    return AM_OK;
}

/**
 * \brief TPMʹ�ܲ����ü���ֵ
 */
static int __tpm_timing_enable (void     *p_drv,
                                uint8_t   chan,
                                uint32_t  max_tpm_count)
{
    amdr_tpm_timing_dev_t *p_dev    = (amdr_tpm_timing_dev_t *)p_drv;
    amhw_tpm_t            *p_hw_tpm =  p_dev->p_devinfo->p_hw_tpm;

    uint32_t match, temp;
    uint8_t  pre_real, pre_reg = 0;

    /* 16λ���������ֵ65535,��Ƶ�����ֵ128 */
    if (max_tpm_count > (0xFFFFu * 128)) {
        return -AM_EINVAL;
    }

    temp = max_tpm_count / 0xFFFFu + 1;

    /* ֻ֧�ַ�Ƶ��С1,2,4,8...128����ô��ڷ�Ƶ������С����2^n */
    for (pre_real = 1; pre_real < temp; ) {
        pre_reg++;                   /* ����д��Ĵ����ķ�Ƶֵ0,1,2,... */
        pre_real = pre_real << 1;           /* ��Ƶ��2^n */
    }
    match = max_tpm_count / pre_real ;

    amhw_tpm_prescale_set(p_hw_tpm, (amhw_tpm_prescale_t)(pre_reg));
    amhw_tpm_count_clear(p_hw_tpm);
    amhw_tpm_mod_set(p_hw_tpm, match - 1);
    amhw_tpm_clock_mode(p_hw_tpm, AMHW_TPM_CLK_SRC_MODULE);

    return AM_OK;
}

/**
 * \brief �����жϻص�����
 */
static int __tpm_timing_callback_set (void     *p_drv,
                                      uint8_t   chan,
                                      void    (*pfn_callback)(void *),
                                      void     *p_arg)
{
    amdr_tpm_timing_dev_t *p_dev    = (amdr_tpm_timing_dev_t *)p_drv;
    amhw_tpm_t            *p_hw_tpm =  p_dev->p_devinfo->p_hw_tpm;


    /* ����Ҫ�����ж� */
    if (pfn_callback == NULL) {
        amhw_tpm_ctrl_clear(p_hw_tpm, AMHW_TPM_SC_TOI_EN);

    } else {
        p_dev->pfn_callback = pfn_callback;
        p_dev->p_arg   = p_arg;

        amhw_tpm_ctrl_set(p_hw_tpm, AMHW_TPM_SC_TOI_EN);
    }

    return AM_OK;
}

/**
 * \brief TPM��ʼ��
 */
am_timer_handle_t amdr_tpm_timing_init (amdr_tpm_timing_dev_t           *p_dev,
                                        const amdr_tpm_timing_devinfo_t *p_devinfo)
{
    if (p_devinfo == NULL || p_devinfo->p_hw_tpm == NULL) {
        return NULL;
    }

    p_dev->p_devinfo          = p_devinfo;
    p_dev->timer_serv.p_funcs = (struct am_timer_drv_funcs *)&__g_tpm_timing_drv_funcs;
    p_dev->timer_serv.p_drv   = p_dev;

    p_dev->pfn_callback = NULL;
    p_dev->p_arg        = NULL;

    return &(p_dev->timer_serv);
}

/**
 * \brief TPM���ʼ��
 */
void amdr_tpm_timing_deinit (amdr_tpm_timing_dev_t *p_dev)
{
    amhw_tpm_t *p_hw_tpm = p_dev->p_devinfo->p_hw_tpm;

    /* �ر�ʱ�ӣ�ֹͣ���� */
    amhw_tpm_clock_mode(p_hw_tpm, AMHW_TPM_CLK_SRC_NONE);

    am_int_disable(p_dev->p_devinfo->inum);
}

/* end of file */
