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
 * \brief TPM���������벶��������ʵ��
 *
 * 1. TPM֧���ṩ�������ֱ�׼���񣬱������ṩ���Ƿ���"����"��׼���������;
 *     - ��ʱ
 *     - PWM���
 *     - ����
 * 2. TPM0��6��ͨ������Ϊ�����ܣ�TPM1��TPM2����2��ͨ������Ϊ������.
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  cuishengen, first implementation.
 * \endinternal
 */

#include "amdr_tpm_cap.h"
#include "amdr_clk.h"
#include "am_gpio.h"
#include "am_int.h"

/*******************************************************************************
* ��������
*******************************************************************************/

/** \brief �����������  */
static int __tpm_cap_config(void              *p_cookie,
                            int                chan,
                            unsigned int       flags,
                            am_cap_callback_t  pfn_callback,
                            void              *p_arg);

/** \brief ʹ�ܲ���ͨ�� */
static int __tpm_cap_enable(void *p_drv, int chan);

/** \brief ���ܲ���ͨ�� */
static int __tpm_cap_disable(void *p_drv, int chan);
                                 
/** \brief ��λ����ͨ������ֵ*/
static int __tpm_cap_reset(void *p_drv, int chan);

static int __tpm_cap_count_to_time (void         *p_drv,
                                    int           chan,
                                    unsigned int  count1,
                                    unsigned int  count2,
                                    unsigned int *p_time_ns);
                                    
static void __tpm_cap_irq_handler(void *p_arg);

/** \brief ��������������� */
static const struct am_cap_drv_funcs __g_tpm_cap_drv_funcs = {
    __tpm_cap_config,
    __tpm_cap_enable,
    __tpm_cap_disable,
    __tpm_cap_reset,
    __tpm_cap_count_to_time,
};

/******************************************************************************/

/** \brief ����һ�����벶��ͨ��  */
static int __tpm_cap_config (void              *p_drv,
                             int                chan,
                             unsigned int       options,
                             am_cap_callback_t  pfn_callback,
                             void              *p_arg)
{
    amdr_tpm_cap_dev_t *p_dev    = (amdr_tpm_cap_dev_t *)p_drv;
    amhw_tpm_t         *p_hw_tpm = p_dev->p_devinfo->p_hw_tpm;
    
    /* ��Чͨ���ŷ�Χ 0 ~ (channels_num - 1) */
    if (chan >= p_dev->p_devinfo->channels_num) {
        return -AM_EINVAL;
    }
    
    /*  ѡ������/�½��ش��� */
    if ((options & AM_CAP_TRIGGER_BOTH_EDGES) == AM_CAP_TRIGGER_BOTH_EDGES) {
        amhw_tpm_ch_mode(p_hw_tpm, chan, AMHW_TPM_COUNTING_UP,
                         AMHW_TPM_CHSC_MS(0) | AMHW_TPM_CHSC_EL_SEL(3));
    }

    /*  ѡ�������ش��� */
    if ((options & AM_CAP_TRIGGER_RISE) == AM_CAP_TRIGGER_RISE) {
        amhw_tpm_ch_mode(p_hw_tpm, chan, AMHW_TPM_COUNTING_UP,
                         AMHW_TPM_CHSC_MS(0) | AMHW_TPM_CHSC_EL_SEL(1));
    }
    
    /*  ѡ���½��ش��� */
    if ((options & AM_CAP_TRIGGER_FALL) == AM_CAP_TRIGGER_FALL) {
        amhw_tpm_ch_mode(p_hw_tpm, chan, AMHW_TPM_COUNTING_UP,
                         AMHW_TPM_CHSC_MS(0) | AMHW_TPM_CHSC_EL_SEL(2));
    }
    
    p_dev->callback_info[chan].callback_func = pfn_callback;
    p_dev->callback_info[chan].p_arg         = p_arg;
    
    return AM_OK;
}

/** \brief ����ʹ�� */
static int __tpm_cap_enable (void *p_drv, int chan)
{
    amdr_tpm_cap_dev_t    *p_dev    = (amdr_tpm_cap_dev_t *)p_drv;
    amhw_tpm_t            *p_hw_tpm = p_dev->p_devinfo->p_hw_tpm;
    amdr_tpm_cap_ioinfo_t *p_ioinfo = p_dev->p_devinfo->p_ioinfo;
    
    /* ��Чͨ���ŷ�Χ 0 ~ (channels_num - 1) */
    if (chan >= p_dev->p_devinfo->channels_num) {
        return -AM_EINVAL;
    }
    
    /* �������� */
    am_gpio_pin_cfg(p_ioinfo[chan].gpio, p_ioinfo[chan].func);

    /* ʹ��ͨ���ж� */
    amhw_tpm_ch_sc_set(p_hw_tpm, chan, AMHW_TPM_CHSC_IE);
    
    /* �ж����Ӳ�ʹ�� */
    am_int_connect(p_dev->p_devinfo->inum, __tpm_cap_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);

    /* ��ʱ�ӣ��������� */
    amhw_tpm_clock_mode(p_hw_tpm, AMHW_TPM_CLK_SRC_MODULE);
    
    return AM_OK;
}

/** \brief ������� */
static int __tpm_cap_disable (void *p_drv, int chan)
{
    amdr_tpm_cap_dev_t    *p_dev    = (amdr_tpm_cap_dev_t *)p_drv;
    amhw_tpm_t            *p_hw_tpm = p_dev->p_devinfo->p_hw_tpm;
    amdr_tpm_cap_ioinfo_t *p_ioinfo = p_dev->p_devinfo->p_ioinfo;
 
    /* ��Чͨ���ŷ�Χ 0 ~ (channels_num - 1) */
    if (chan >= p_dev->p_devinfo->channels_num) {
        return -AM_EINVAL;
    }
    
    /* ��ԭGPIO���� */
    am_gpio_pin_cfg(p_ioinfo[chan].gpio, p_ioinfo[chan].dfunc);
    
    amhw_tpm_ch_sc_clear(p_hw_tpm, chan, AMHW_TPM_CHSC_IE);

    /* �ر�ʱ�ӣ�ֹͣ���� */
    amhw_tpm_clock_mode(p_hw_tpm, AMHW_TPM_CLK_SRC_NONE);
    
    return AM_OK;
}

/**
  * \brief ��λ����ͨ������ֵ
  */
static int __tpm_cap_reset (void *p_drv, int chan)
{
    amdr_tpm_cap_dev_t *p_dev    = (amdr_tpm_cap_dev_t *)p_drv;
    amhw_tpm_t         *p_hw_tpm = p_dev->p_devinfo->p_hw_tpm;
    
    /* ֹͣTPM���� */
    amhw_tpm_clock_mode(p_hw_tpm, AMHW_TPM_CLK_SRC_NONE);
    
    return AM_OK;
}

/**
  * \brief ת�����β���ֵΪʱ��ֵ
  */
static int __tpm_cap_count_to_time (void         *p_drv,
                                    int           chan,
                                    unsigned int  count1,
                                    unsigned int  count2,
                                    unsigned int *p_time_ns)
{
    amdr_tpm_cap_dev_t *p_dev    = (amdr_tpm_cap_dev_t *)p_drv;
    amhw_tpm_t         *p_hw_tpm = p_dev->p_devinfo->p_hw_tpm;

    uint32_t clkfreq  = amdr_clk_periph_rate_get(p_dev->p_devinfo->p_hw_tpm);
    uint32_t count_err;
    uint64_t time_ns;
    uint8_t  reg_pre, pre;

    reg_pre = (uint8_t)amhw_tpm_prescale_get(p_hw_tpm);
    if (reg_pre == 0) {
        pre = 1;
    } else {
        pre = 2 << reg_pre;
    }
    
    count_err = count2 - count1;

    /* �����ζ�ȡֵ�Ĳ�ת����ʱ��  */
    time_ns = (uint64_t)1000000000 * (uint64_t)count_err * pre / (uint64_t)clkfreq;
    
    *p_time_ns = time_ns;
    
    return AM_OK;
}

/**
  * \brief �жϷ�����
  */
static void __tpm_cap_irq_handler (void *p_arg)
{
    amdr_tpm_cap_dev_t *p_dev     = (amdr_tpm_cap_dev_t *)p_arg;
    amhw_tpm_t         *p_hw_tpm  = p_dev->p_devinfo->p_hw_tpm;
    uint8_t             chans_num = p_dev->p_devinfo->channels_num;
    
    uint8_t             i;
    am_cap_callback_t   callback_func;
    uint32_t            value;
    
    for (i = 0; i < chans_num; i++) {
        if (amhw_tpm_stat_flag_get(p_hw_tpm) & (AMHW_TPM_STAT_CH0_FLAG << i) ) {
            
            callback_func = p_dev->callback_info[i].callback_func;
            value = amhw_tpm_ch_value_get(p_hw_tpm, i);
            
            if (callback_func != NULL) {
                callback_func(p_dev->callback_info[i].p_arg, value);
            }
            
            /* ���ͨ��i��־ */
            amhw_tpm_stat_flag_clear(p_hw_tpm, (AMHW_TPM_STAT_CH0_FLAG << i));
        }
    }
}

/** 
  * \brief �����ʼ��
  */
void __tpm_cap_init (amhw_tpm_t *p_hw_tpm)
{
    /* ��������� */
    amhw_tpm_count_clear(p_hw_tpm);
    amhw_tpm_prescale_set(p_hw_tpm, AMHW_TPM_DIVIDED_BY_1);
}

am_cap_handle_t amdr_tpm_cap_init (amdr_tpm_cap_dev_t           *p_dev,
                                   const amdr_tpm_cap_devinfo_t *p_devinfo)
{
    amhw_tpm_t *p_hw_tpm;
    uint8_t    i;
    
    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }
    
    p_hw_tpm                = p_devinfo->p_hw_tpm;
    p_dev->p_devinfo        = p_devinfo;
    p_dev->cap_serv.p_funcs = (struct am_cap_drv_funcs *)&__g_tpm_cap_drv_funcs;
    p_dev->cap_serv.p_drv   = p_dev;
    
    for (i = 0; i < p_devinfo->channels_num; i++) {
        p_dev->callback_info[i].callback_func = NULL;
    }
    
    /* �����ʼ�� */
    __tpm_cap_init(p_hw_tpm);
    
    return &(p_dev->cap_serv);
}

void amdr_tpm_cap_deinit (amdr_tpm_cap_dev_t *p_dev)
{
    amhw_tpm_t *p_hw_tpm  = p_dev->p_devinfo->p_hw_tpm;
 
    /* �ر�TPMģ�� */
    amhw_tpm_clock_mode(p_hw_tpm, AMHW_TPM_CLK_SRC_NONE);
    
    am_int_disable(p_dev->p_devinfo->inum);
}

/* end of file */
