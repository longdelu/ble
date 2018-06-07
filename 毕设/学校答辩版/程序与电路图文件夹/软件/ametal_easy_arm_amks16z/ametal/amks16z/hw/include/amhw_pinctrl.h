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
 * \brief PORT Ӳ�������ӿ�
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-16  wxj, first implementation.
 * \endinternal
 */

#ifndef AMHW_PINCTRL_H_
#define AMHW_PINCTRL_H_

#include "am_types.h"
#include "ametal.h"

/** 
 * \addtogroup amhw_if_pinctrl
 * \copydoc amhw_pinctrl.h
 * @{
 */
 
/**
 * \brief PORT �˿ڼĴ����ṹ��
 */
typedef struct amhw_port_reg {
    __IO uint32_t port_pcr[32];         /**< \brief �˿ڹܽ����üĴ��� */
    __O  uint32_t port_gpclr;           /**< \brief �˿ڿ��Ƶ��ֽڼĴ��� */
    __O  uint32_t port_gpchr;           /**< \brief �˿ڿ��Ƹ��ֽڼĴ��� */
    __IO uint32_t reserve[6];           /**< \brief �˿ڱ���λ*/
    __IO uint32_t port_isfr;            /**< \brief �˿���״̬��־�Ĵ��� */
    __IO uint32_t reserve1[983];        /**< \brief �˿ڱ���λ*/
}amhw_port_reg_t;

/**
 * \brief PORT �Ĵ�����ṹ��
 */
typedef struct amhw_port {

    amhw_port_reg_t port[5];            /**< \brief 5���˿� */
    
} amhw_port_t;

/**
 * \name  PORT_PCR�Ĵ���λ����
 * @{
 */
 
/** \brief  �����ж�״̬��־λ */
#define AMHW_PORT_PCR_ISF               (0x1UL << 24)

/** \brief  DMA�������ж����� */
#define AMHW_INT_DMA_RISING             (0x1UL)

/** \brief  DMA�½����ж����� */
#define AMHW_INT_DMA_FALLING            (0x2UL)

/** \brief  DMA�����ػ����½����ж����� */
#define AMHW_INT_DMA_EITHER             (0x3UL)

/** \brief  ���ŵ͵�ƽ�����ж� */
#define AMHW_INT_LOW                    (0x8UL)

/** \brief  ���������ش����ж� */
#define AMHW_INT_RISING                 (0x9UL)

/** \brief  �����½��ش����ж� */
#define AMHW_INT_FALLING                (0xAUL)

/** \brief  ���������ػ����½��ش����ж� */
#define AMHW_PINT_EITHER                (0xBUL)

/** \brief  ���Ÿߵ�ƽ�����ж� */
#define AMHW_INT_HIGH                   (0xCUL)

/** \brief  ����ǿ�������� */
#define AMHW_PORT_DRIVE_HIGH            (0x1UL << 6)

/** \brief  ���������˲� */
#define AMHW_PORT_FILTER                (0x1UL << 4)

/** \brief  ����ת������ */
#define AMHW_PORT_SLEW_SLOW             (0x1UL << 2)

/** \brief  ������/����ʹ�� */
#define AMHW_PORT_PULL_EN               (0x1UL << 1)

/** \brief  ��������ѡ�� */
#define AMHW_PORT_PULL_SEL              (0x1UL << 0)

/** \brief  Ĭ��GPIO���� */
#define AMHW_PORT_GPIO                  (0x1UL)

/** @} */

/**
 * \brief �������ж�״̬��־λ
 *
 * \param[in] p_hw_port : ָ��PORT�Ĵ������ָ��
 * \param[in] pin       : ���ű��,ֵΪ PIO* (#PIOA_0)
 *
 * \retval 0 : û�д����ж�
 * \retval 1 : �������ж�
 */
am_static_inline
int amhw_port_pin_isf_get (amhw_port_t *p_hw_port, int pin)
{
    return ((((p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)])
            & AMHW_PORT_PCR_ISF) !=0) ? 1 : 0);
}

/**
 * \brief ���������ж�״̬��־λ
 *
 * \param[in] p_hw_port : ָ��PORT�Ĵ������ָ��
 * \param[in] pin       : ���ű��,ֵΪ PIO* (#PIOA_0)
 *
 * \return ��
 */
am_static_inline
void amhw_port_pin_isf_clr (amhw_port_t *p_hw_port, int pin)
{
    p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)] |= AMHW_PORT_PCR_ISF;
}

/**
 * \brief ���������жϴ�����ʽ
 *
 * \param[in] p_hw_port : ָ��PORT�Ĵ������ָ��
 * \param[in] pin       : ���ű��,ֵΪ PIO* (#PIOA_0)
 * \param[in] irq       : ��Ҫ���õ����Ŵ�����ʽ
 *
 * \return ��
 */
am_static_inline
void amhw_port_pin_irq_cfg (amhw_port_t *p_hw_port, int pin, uint32_t irq)
{
    AM_BITS_SET(p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)], 16, 4, irq);
}

/**
 * \brief ���������ж�
 *
 * \param[in] p_hw_port : ָ��PORT�Ĵ������ָ��
 * \param[in] pin       : ���ű��,ֵΪ PIO* (#PIOA_0)
 *
 * \return ��
 */
am_static_inline
void amhw_port_pin_irq_disable (amhw_port_t *p_hw_port, int pin)
{
    AM_BITS_SET(p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)], 16, 4, 0UL);
}

/**
 * \brief �������Ÿ��ù���
 *
 * \param[in] p_hw_port : ָ��PORT�Ĵ������ָ��
 * \param[in] pin       : ���ű��,ֵΪ PIO* (#PIOA_0)
 * \param[in] func      : Ҫ���õ����Ź���
 *
 * \return ��
 */
am_static_inline
void amhw_port_pin_func_cfg (amhw_port_t *p_hw_port, int pin, uint32_t func)
{
    AM_BITS_SET(p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)], 8, 3, func);
}

/**
 * \brief ����������������ǿ
 *
 * \param[in] p_hw_port : ָ��PORT�Ĵ������ָ��
 * \param[in] pin       : ���ű�ţ�ֵΪ PIO* (#PIOA_0)
 *
 * \return ��
 */
am_static_inline
void amhw_port_pin_drive_high (amhw_port_t *p_hw_port, int pin)
{
    p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)] |= AMHW_PORT_DRIVE_HIGH;
}

/**
 * \brief ������������������
 *
 * \param[in] p_hw_port : ָ��PORT�Ĵ������ָ��
 * \param[in] pin       : ���ű��,ֵΪ PIO* (#PIOA_0)
 *
 * \return ��
 */
am_static_inline
void amhw_port_pin_drive_low (amhw_port_t *p_hw_port, int pin)
{
    p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)] &= (~AMHW_PORT_DRIVE_HIGH);
}

/**
 * \brief �������������˲�ʹ��
 *
 * \param[in] p_hw_port : ָ��PORT�Ĵ������ָ��
 * \param[in] pin       : ���ű��,ֵΪ PIO* (#PIOA_0)
 *
 * \return ��
 */
am_static_inline
void amhw_port_pin_filter_enable (amhw_port_t *p_hw_port, int pin)
{
    p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)] |= AMHW_PORT_FILTER;
}

/**
 * \brief �������������˲�����
 *
 * \param[in] p_hw_port : ָ��PORT�Ĵ������ָ��
 * \param[in] pin       : ���ű��,ֵΪ PIO* (#PIOA_0)
 *
 * \return ��
 */
am_static_inline
void amhw_port_pin_filter_disable (amhw_port_t *p_hw_port, int pin)
{
    p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)] &= (~AMHW_PORT_FILTER);
}

/**
 * \brief ��������ת�����ʵ�
 *
 * \param[in] p_hw_port : ָ��PORT�Ĵ������ָ��
 * \param[in] pin       : ���ű��,ֵΪ PIO* (#PIOA_0)
 *
 * \return ��
 */
am_static_inline
void amhw_port_pin_slew_slow (amhw_port_t *p_hw_port, int pin)
{
    p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)] |= AMHW_PORT_SLEW_SLOW;
}

/**
 * \brief ��������ת�����ʿ�
 *
 * \param[in] p_hw_port : ָ��PORT�Ĵ������ָ��
 * \param[in] pin       : ���ű��,ֵΪ PIO* (#PIOA_0)
 *
 * \return ��
 */
am_static_inline
void amhw_port_pin_slew_fast (amhw_port_t *p_hw_port, int pin)
{
    p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)] &= (~AMHW_PORT_SLEW_SLOW);
}

/**
 * \brief ����������/����ʹ��
 *
 * \param[in] p_hw_port : ָ��PORT�Ĵ������ָ��
 * \param[in] pin       : ���ű��,ֵΪ PIO* (#PIOA_0)
 *
 * \return ��
 */
am_static_inline
void amhw_port_pin_pull_enable (amhw_port_t *p_hw_port, int pin)
{
    p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)] |= AMHW_PORT_PULL_EN;
}


/**
 * \brief ����������/��������
 *
 * \param[in] p_hw_port : ָ��PORT�Ĵ������ָ��
 * \param[in] pin       : ���ű��,ֵΪ PIO* (#PIOA_0)
 *
 * \return ��
 */
am_static_inline
void amhw_port_pin_pull_disable (amhw_port_t *p_hw_port, int pin)
{
    p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)] &= (~AMHW_PORT_PULL_EN);
}

/**
 * \brief ������������
 *
 * \param[in] p_hw_port : ָ��PORT�Ĵ������ָ��
 * \param[in] pin       : ���ű��,ֵΪ PIO* (#PIOA_0)
 *
 * \return ��
 */
am_static_inline
void amhw_port_pin_pull_up (amhw_port_t *p_hw_port, int pin)
{
    p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)] |= AMHW_PORT_PULL_SEL;
}

/**
 * \brief ������������
 *
 * \param[in] p_hw_port : ָ��PORT�Ĵ������ָ��
 * \param[in] pin       : ���ű��,ֵΪ PIO* (#PIOA_0)
 *
 * \return ��
 */
am_static_inline
void amhw_port_pin_pull_down (amhw_port_t *p_hw_port, int pin)
{
    p_hw_port->port[pin >> 5].port_pcr[(pin & 0x1F)] &= (~AMHW_PORT_PULL_SEL);
}

/**
 * \brief ���ֽ�д�˿ڹܽ�pin[15:0]
 *
 * \param[in] p_hw_port : ָ��PORT�Ĵ������ָ��
 * \param[in] pin       : ���ű��,ֵΪ PIO* (#PIOA_0)
 * \param[in] gpwe      : PORT�˿ڹܽ�ʹ��
 * \param[in] gpwd      : Ҫ���õ�PORT�˿ڹܽ�ֵ
 *
 * \return ��
 */
am_static_inline
void amhw_port_low_word_write (amhw_port_t *p_hw_port,
                               int          pin,
                               uint32_t     gpwe,
                               uint32_t     gpwd)
{
    uint32_t data=0;
    data = (gpwe << 16) | gpwd;
    p_hw_port->port[pin >> 5].port_gpclr = data;
}

/**
 * \brief ���ֽ�д�˿ڹܽ�pin[31:16]
 *
 * \param[in] p_hw_port : ָ��PORT�Ĵ������ָ��
 * \param[in] pin       : ���ű��,ֵΪ PIO* (#PIOA_0)
 * \param[in] gpwe      : PORT�˿ڹܽ�ʹ��
 * \param[in] gpwd      : Ҫ���õ�PORT�˿ڹܽ�ֵ
 *
 * \return ��
 */
am_static_inline
void amhw_port_high_word_write (amhw_port_t *p_hw_port,
                                int          pin,
                                uint32_t     gpwe,
                                uint32_t     gpwd)
{
    uint32_t data=0;
    data = (gpwe << 16) | gpwd;
    p_hw_port->port[pin >> 5].port_gpchr = data;
}

/**
 * \brief ���˿��ж�״̬��־
 *
 * \param[in] p_hw_port : ָ��PORT�Ĵ������ָ��
 * \param[in] pin       : ���ű��,ֵΪ PIO* (#PIOA_0)
 *
 * \return �˿��ж�״̬��־ֵ
 */
am_static_inline
int amhw_port_irq_flag_read (amhw_port_t *p_hw_port, int pin)
{
    return (p_hw_port->port[pin >> 5].port_isfr);
}

/**
 * \brief ����˿��ж�״̬��־
 *
 * \param[in] p_hw_port : ָ��PORT�Ĵ������ָ��
 * \param[in] pin       : ���ű��,ֵΪ PIO* (#PIOA_0)
 *
 * \return ��
 */
am_static_inline
void amhw_port_irq_flag_clr (amhw_port_t *p_hw_port, int pin)
{
    p_hw_port->port[pin >> 5].port_isfr = 0xFFFFFFFF;
}

/**
 * @} amhw_if_pinctrl
 */

#ifdef __cplusplus
}
#endif

#endif /* AMHW_PINCTRL_H_ */
