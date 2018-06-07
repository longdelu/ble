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
 * \brief GPIO implementation
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-21  wxj, first implementation.
 * \endinternal
 */

#include "am_gpio.h"
#include "amdr_gpio.h"
#include "amdr_gpio_util.h"
#include "am_gpio_util.h"
#include "am_int.h"

/*******************************************************************************
* ˽�ж���
*******************************************************************************/

/** \brief �ж�δ���ӱ�ʶ             */
#define AMDR_GPIO_INVALID_PIN_MAP    0xFF

/** \brief δ�����������ܱ�ʶ         */
#define AMDR_GPIO_NO_FUNC            0xFF

/** \brief ����ָ��GPIO�豸��Ϣ��ָ�� */
#define __GPIO_DEVINFO_DECL(p_gpio_devinfo, p_dev)  \
        const amdr_gpio_devinfo_t *p_gpio_devinfo = p_dev->p_devinfo

/******************************************************************************
  ȫ�ֱ���
******************************************************************************/

/** \brief ָ��GPIO�豸��ָ�� */
amdr_gpio_dev_t *__gp_gpio_dev;

/*******************************************************************************
  ��������
*******************************************************************************/

/**
 * \brief ��������
 */
int am_gpio_pin_cfg (int pin, uint32_t flags)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    amhw_port_t *p_hw_port = p_gpio_devinfo->p_hw_port;
    amhw_gpio_t *p_hw_gpio = p_gpio_devinfo->p_hw_gpio;

    uint32_t func, mode;

    /* ��ȡ��׼�㶨�����Ϣ */
    func = AM_GPIO_COM_FUNC_GET(flags);
    mode = AM_GPIO_COM_MODE_GET(flags);

    /* �ܽ�Ĭ��ΪGPIO���� */
    amhw_port_pin_func_cfg(p_hw_port, pin, AMHW_PORT_GPIO);
    /*
     * ���й������ã����ȼ��ߣ�
     */
    if (func != 0x00) {

        switch (func) {

        /* Ĭ�Ϲܽ�ΪGPIO���� */
        case AM_GPIO_INPUT:           
            amhw_gpio_pin_dir_input(p_hw_gpio, pin);
            break;

        case AM_GPIO_OUTPUT:
            amhw_gpio_pin_dir_output(p_hw_gpio, pin);
            break;

        case AM_GPIO_OUTPUT_INIT_HIGH:
            amhw_gpio_pin_dir_output(p_hw_gpio, pin);
            amhw_gpio_pin_init_out_high(p_hw_gpio, pin);
            break;

        case AM_GPIO_OUTPUT_INIT_LOW:
            amhw_gpio_pin_dir_output(p_hw_gpio, pin);
            amhw_gpio_pin_init_out_low(p_hw_gpio, pin);
            break;

        default:
            break;
        }
    }

    if (mode != 0x00) {

        if (mode == AM_GPIO_MODE_INVALID_VAL) {             /* ����/���� */
            amhw_port_pin_pull_disable (p_hw_port, pin);

        } else if (mode == AM_GPIO_PULL_UP_VAL) {           /* ��������ģʽ */
            amhw_port_pin_pull_enable (p_hw_port, pin);
            amhw_port_pin_pull_up (p_hw_port, pin);

        } else if (mode == AM_GPIO_PULL_DOWN_VAL) {         /* ��������ģʽ */
            amhw_port_pin_pull_enable (p_hw_port, pin);
            amhw_port_pin_pull_down (p_hw_port, pin);

        } else {
            return -AM_ENOTSUP;
        }
    }

    /* ƽ̨�������� */

    /* ��Ҫ�������Ź��� */
    if ((flags & AMDR_PORT_FUNEN) != 0) {

        amhw_port_pin_func_cfg (p_hw_port, pin, AMDR_GPIO_FUNC_GET(flags));
    }

    /* ��Ҫ�������ŵ�ת������     */
    if ((flags & AMDR_PORT_SLEWSLOW) != 0) {
        amhw_port_pin_slew_slow (p_hw_port, pin);
    }

    /* ��Ҫʹ���˲��� */
    if ((flags & AMDR_PORT_FILTEREN) != 0) {
        amhw_port_pin_filter_enable (p_hw_port, pin);
    }

    /* ��Ҫ�������Ÿ�������  */
    if ((flags & AMDR_PORT_DRIVERHIGH) != 0) {
        amhw_port_pin_drive_high (p_hw_port, pin);
    }

    return AM_OK;
}

/**
 * \brief ��ȡ����״̬
 */
int am_gpio_get (int pin)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    amhw_gpio_t *p_hw_gpio = p_gpio_devinfo->p_hw_gpio;

    return (int)amhw_gpio_pin_input_get(p_hw_gpio, pin);
}

/**
 * \brief �����������״̬
 */
int am_gpio_set (int pin, int value)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    amhw_gpio_t *p_hw_gpio = p_gpio_devinfo->p_hw_gpio;

    if (value == 0) {
        amhw_gpio_pin_out_low(p_hw_gpio, pin);
    } else {
        amhw_gpio_pin_out_high(p_hw_gpio, pin);
    }

    return AM_OK;
}

/**
 * \brief ���������ת
 */
int am_gpio_toggle (int pin)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    amhw_gpio_t *p_hw_gpio = p_gpio_devinfo->p_hw_gpio;

    amhw_gpio_pin_out_toggle (p_hw_gpio, pin);

    return AM_OK;
}

/**
 * \brief ���Ŵ�����ʽ����
 */
int am_gpio_trigger_cfg (int pin, uint32_t flag)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    amhw_port_t *p_hw_port = p_gpio_devinfo->p_hw_port;
    amhw_gpio_t *p_hw_gpio = p_gpio_devinfo->p_hw_gpio;

    /* ���ùܽ�Ϊ���뷽�� */
    amhw_gpio_pin_dir_input(p_hw_gpio, pin);

    /* �ܽ���/����ʹ�� */
    amhw_port_pin_pull_enable(p_hw_port, pin);

    /* ������������ */
    amhw_port_pin_pull_up(p_hw_port, pin);

    switch (flag) {

    case AM_GPIO_TRIGGER_OFF:           /* �رմ��� */
        amhw_port_pin_irq_disable(p_hw_port, pin);
        break;

    case AM_GPIO_TRIGGER_HIGH:          /* �ߵ�ƽ���� */
        amhw_port_pin_irq_cfg(p_hw_port, pin, AMHW_INT_HIGH);
        break;

    case AM_GPIO_TRIGGER_LOW:           /* �͵�ƽ���� */
        amhw_port_pin_irq_cfg(p_hw_port, pin, AMHW_INT_LOW);
        break;

    case AM_GPIO_TRIGGER_RISE:          /* �����ش��� */
        amhw_port_pin_irq_cfg(p_hw_port, pin, AMHW_INT_RISING);
        break;

    case AM_GPIO_TRIGGER_FALL:          /* �½��ش��� */
        amhw_port_pin_irq_cfg(p_hw_port, pin, AMHW_INT_FALLING);
        break;

    case AM_GPIO_TRIGGER_BOTH_EDGES:    /* ˫���ش��� */
        amhw_port_pin_irq_cfg(p_hw_port, pin, AMHW_PINT_EITHER);
        break;

    default:
        break;
    }

    /* �������жϱ�־λ */
    amhw_port_pin_isf_clr(p_hw_port, pin);

    return AM_OK;
}

/**
 * \brief GPIO �жϷ�����Ӧ������
 * \param[in] p_arg : ��ǰ�ж�ӳ���š�
 * \return ��
 */
static void __gpio_eint_isr (void *p_arg)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    int          slot      = (int)p_arg;
    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;

    int         pin_ium=0;

    /* ��ȡ�йػص����������� */
    pfn_isr = p_gpio_devinfo->p_triginfo[slot].pfn_callback;
    p_arg_tmp   = p_gpio_devinfo->p_triginfo[slot].p_arg;

    /* ����ж����ź� */
    pin_ium = p_gpio_devinfo->p_infomap[slot];

    /* ���жϱ�־ */
    amhw_port_pin_isf_clr (AMHW_PORT, pin_ium);

    if (pfn_isr != NULL) {
        pfn_isr(p_arg_tmp);
    }
}

/**
 * \brief ���������жϻص�����
 */
int am_gpio_trigger_connect (int           pin,
                             am_pfnvoid_t  pfn_callback,
                             void         *p_arg)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    uint32_t port;

    int          key;
    int          i;
    int          slot      = -1;
    int          inum      = 0;
    int          err       = AM_OK;
    bool_t       already   = FALSE;

    /** \brief ������Ŷ�Ӧ�����Ŷ˺� */
    port = pin >> 5;

    /* �ر�CPU�ж� */
    key = am_int_cpu_lock();

    for (i = 0; i < p_gpio_devinfo->pint_count; i++) {
        if ((p_gpio_devinfo->p_infomap)[i] == AMDR_GPIO_INVALID_PIN_MAP) {
            slot = i;
            break;
        }
        if (p_gpio_devinfo->p_infomap[i] == pin) {
            already = TRUE;
            break;
        }
    }

    if (already) {

        /* ��CPU�ж� */
        am_int_cpu_unlock(key);
        return -AM_EINVAL;
    }

    if (port < 2) {     /* �ж����ĸ��˿����õ��ж� */
        inum = 0;       /* A�˿����Ų������ж� */
    } else {
        inum = 1;       /* C\D�˿ڲ������ж� */
    }

    if (-1 != slot) {
        err = am_int_connect((p_gpio_devinfo->inum_pin)[inum], /* д���жϺ� */
                             (am_pfnvoid_t)__gpio_eint_isr,
                             (void *)slot);

        /*  ���津�����ż��ص����� */
        if (AM_OK == err) {
            (p_gpio_devinfo->p_infomap)[slot]               = pin;
            (p_gpio_devinfo->p_triginfo)[slot].pfn_callback = pfn_callback;
            (p_gpio_devinfo->p_triginfo)[slot].p_arg        = p_arg;
        }

        /* ���ж� */
        am_int_cpu_unlock(key);
    }
     return AM_OK;
}


/**
 * \brief ɾ�������жϻص���������
 */
int am_gpio_trigger_disconnect (int           pin,
                                am_pfnvoid_t  pfn_callback,
                                void         *p_arg)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    int key;
    int i;
    int slot = -1;

    /* �ر�CPU�ж� */
    key = am_int_cpu_lock();

    for (i = 0; i < p_gpio_devinfo->pint_count; i++) {
        if ((p_gpio_devinfo->p_infomap)[i] == pin) {
            slot = i;
            break;
        }
    }

    if (slot == -1) {

        /* ��CPU�ж� */
        am_int_cpu_unlock(key);
        return -AM_EINVAL;
    }

    am_int_disconnect((p_gpio_devinfo->inum_pin)[slot],
                      (am_pfnvoid_t)__gpio_eint_isr,
                      (void *)slot);

    (p_gpio_devinfo->p_infomap)[slot]               = AMDR_GPIO_INVALID_PIN_MAP;
    (p_gpio_devinfo->p_triginfo)[slot].pfn_callback = NULL;

    /* ��CPU�ж� */
    am_int_cpu_unlock(key);

    return AM_OK;
}

/**
 * \brief ʹ�������жϡ�
 */
int am_gpio_trigger_on (int pin)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    int key;
    int i;
    int slot = -1;

    uint32_t port, inum;

    /* �ر�CPU�ж� */
    key = am_int_cpu_lock();

    for (i = 0; i < p_gpio_devinfo->pint_count; i++) {
        if ((p_gpio_devinfo->p_infomap)[i] == pin) {
            slot = i;
            break;
        }
    }

    /* ��CPU�ж� */
    am_int_cpu_unlock(key);

    if (slot == -1) {
        return -AM_ENXIO;
    }

    /** \brief ������Ŷ�Ӧ�����Ŷ˺� */
    port = pin >> 5;

    if (port < 1) {     /* �ж����ĸ��˿����õ��ж� */
        inum = 0;       /* A�˿����Ų������ж� */
    } else {
        inum = 1;       /* C\D�˿ڲ������ж� */
    }

    return am_int_enable((p_gpio_devinfo->inum_pin)[inum]);
}

/**
 * \brief ���������жϡ�
 */
int am_gpio_trigger_off (int pin)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    int key;
    int i;
    int slot = -1;

    uint32_t port, inum;

    /* �ر�CPU�ж� */
    key = am_int_cpu_lock();

    for (i = 0; i < p_gpio_devinfo->pint_count; i++) {
        if ((p_gpio_devinfo->p_infomap)[i] == pin) {
            slot = i;
            break;
        }
    }

    /* ��CPU�ж� */
    am_int_cpu_unlock(key);

    if (slot == -1) {
        return -AM_ENXIO;
    }

    /** \brief ������Ŷ�Ӧ�����Ŷ˺� */
    port = pin >> 5;

    if (port < 2) {     /* �ж����ĸ��˿����õ��ж� */
        inum = 0;       /* A�˿����Ų������ж� */
    } else {
        inum = 1;       /* C\D�˿ڲ������ж� */
    }

    return am_int_disable((p_gpio_devinfo->inum_pin)[inum]);
}

/**
 * \brief GPIO��ʼ��
 */
int amdr_gpio_init (amdr_gpio_dev_t           *p_dev,
                    const amdr_gpio_devinfo_t *p_devinfo)
{
    int i;

    if (p_dev == NULL || p_devinfo == NULL) {
        return -AM_EINVAL;
    }

    p_dev->p_devinfo  = p_devinfo;
    __gp_gpio_dev     = p_dev;
    p_dev->valid_flg  = TRUE;

    if ((p_devinfo->p_triginfo == NULL) ||
        (p_devinfo->p_infomap == NULL)) {
        p_dev->valid_flg = FALSE;
    }

    /* �жϱ���� */
    if (p_dev->valid_flg) {
        for (i = 0; i < p_devinfo->pint_count; i++) {
            (p_devinfo->p_infomap)[i]               = AMDR_GPIO_INVALID_PIN_MAP;
            (p_devinfo->p_triginfo)[i].pfn_callback = NULL;
        }
    }

    return AM_OK;
}

/**
 * \brief GPIOȥ��ʼ��
 */
void amdr_gpio_deinit (amdr_gpio_dev_t *p_dev)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, p_dev);
    int i;

    if (p_dev->valid_flg) {
        for (i = 0; i < p_gpio_devinfo->pint_count; i++) {
            (p_gpio_devinfo->p_infomap)[i]               = 0;
            (p_gpio_devinfo->p_triginfo)[i].pfn_callback = NULL;
        }
    }

    for (i = 0; i < p_gpio_devinfo->pint_count; i++) {
        am_int_disable((p_gpio_devinfo->inum_pin)[i]);
    }
}

/* end of file */
