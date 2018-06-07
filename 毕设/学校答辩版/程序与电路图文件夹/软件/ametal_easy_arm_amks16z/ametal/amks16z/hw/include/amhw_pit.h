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
 * \brief KS16Z �������ж϶�ʱ����PIT�������ӿ�
 *
 * 1. ��ʱ������DMA�������������;
 * 2. ��ʱ�������жϵ�����;
 * 3. �������ж�;
 * 4. ÿ����ʱ�����ж����ĳ�ʱ����.
 *
 * \note ��ģ��û���ⲿ����
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-16  cuishengen, first implementation.
 * \endinternal
 */

#ifndef __AMHW_PIT_H
#define __AMHW_PIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"

/**
 * \addtogroup amhw_if_pit
 * \copydoc amhw_pit.h
 * @{
 */

/**
 * \brief TPMͨ����
 */
#define AMHW_PIT_CHANNELS_NUM      (2)

/**
 * \brief PITͨ��ID,���ڲ��������ĳ��ͨ������Ч��Χ��0 ~ (AMHW_PIT_CHANNELS_NUM - 1)
 */
#define AMHW_PIT_CH(n)             ((n) < AMHW_PIT_CHANNELS_NUM ? (n) : 0)


/**
 * \brief TPM �Ĵ�����ṹ��
 */
typedef struct amhw_pit {
    __IO uint32_t module_ctl;           /**< \brief ģ����� */
    __I  uint32_t reserved_0[55];       /**< \brief ��������Ӧʹ�� */
    __I  uint32_t lifetime64_h;         /**< \brief �������ڶ�ʱ����32λ */
    __I  uint32_t lifetime64_l;         /**< \brief �������ڶ�ʱ����32λ */
    __I  uint32_t reserved_1[6];        /**< \brief ��������Ӧʹ�� */

    struct {
        __IO uint32_t load_value;           /**< \brief װ��ֵ */
        __I  uint32_t current_value;        /**< \brief ��ǰֵ */
        __IO uint32_t control;              /**< \brief ��ʱ������ */
        __IO uint32_t flag;                 /**< \brief ��ʱ����־ */
    } channel[2];                           /**< \brief tpmͨ������   */
} amhw_pit_t;


/**
 * \name ģ����ƼĴ���λ����(R/W)
 * @{
 */

#define AMHW_PIT_MSC_MASK     (0x3u)            /**< \brief ״̬�Ϳ��ƼĴ������� */
#define AMHW_PIT_MSC_MDIS     (1UL << 1)        /**< \brief ģ��ʱ�ӽ��� */
#define AMHW_PIT_MSC_FRZ      (1UL << 0)        /**< \brief ����ģʽ�¶�ʱ����ͣ */

/** @} */

/**
 * \brief PITģ��ʹ��
 * \param[in] p_hw_pit : ָ��PIT�Ĵ������ָ��
 * \return ��
 */
am_static_inline
void amhw_pit_module_enable (amhw_pit_t *p_hw_pit)
{
    p_hw_pit->module_ctl &= ~AMHW_PIT_MSC_MDIS;
}

/**
 * \brief PITģ�����
 * \param[in] p_hw_pit : ָ��PIT�Ĵ������ָ��
 * \return ��
 */
am_static_inline
void amhw_pit_module_disable (amhw_pit_t *p_hw_pit)
{
    p_hw_pit->module_ctl |= AMHW_PIT_MSC_MDIS;
}

/**
 * \name �������ڼĴ���,��Ϊ�ߣ��������֣����һ��64λ��ʱ��(ֻ��)
 * @{
 */

#define AMHW_PIT_LTH_MASK     (0xFFFFFFFFu)   /**< \brief �������ڶ�ʱ����λ���� */
#define AMHW_PIT_LTL_MASK     (0xFFFFFFFFu)   /**< \brief �������ڶ�ʱ����λ���� */

/** @} */

/**
 * \brief ��ȡ�������ڶ�ʱ����32λ
 * \param[in] p_hw_pit : ָ��PIT�Ĵ������ָ��
 * \return �������ڶ�ʱ����32λ
 */
am_static_inline
uint32_t amhw_pit_ltmr64h_get (amhw_pit_t *p_hw_pit)
{
    return p_hw_pit->lifetime64_h;
}

/**
 * \brief ��ȡ�������ڶ�ʱ����λ
 * \param[in] p_hw_pit : ָ��PIT�Ĵ������ָ��
 * \return �������ڶ�ʱ����32λ
 */
am_static_inline
uint32_t amhw_pit_ltmr64l_get (amhw_pit_t *p_hw_pit)
{
    return p_hw_pit->lifetime64_l;
}

/**
 * \brief װ��ֵ�趨
 *
 * \param[in] p_hw_pit : ָ��PIT�Ĵ������ָ��
 * \param[in] chan     : ͨ��ID��ʹ�ú�#AMHW_PIT_CH(n)
 * \param[in] val      : ��Ҫд���ֵ
 *
 * \return ��
 */
am_static_inline
void amhw_pit_ldval_set (amhw_pit_t *p_hw_pit,
                         uint8_t     chan,
                         uint32_t    val)
{
    p_hw_pit->channel[chan].load_value = val;
}

/**
 * \brief װ��ֵ��ȡ
 *
 * \param[in] p_hw_pit : ָ��PIT�Ĵ������ָ��
 * \param[in] chan     : ͨ��ID��ʹ�ú�#AMHW_PIT_CH(n)
 *
 * \return װ��ֵ
 */
am_static_inline
uint32_t amhw_pit_ldval_get (amhw_pit_t *p_hw_pit, uint8_t chan)
{
    return p_hw_pit->channel[chan].load_value;
}

/**
 * \brief ��ǰֵ��ȡ
 *
 * \param[in] p_hw_pit : ָ��PIT�Ĵ������ָ��
 * \param[in] chan     : ͨ��ID��ʹ�ú�#AMHW_PIT_CH(n)
 *
 * \return ��ǰֵ
 */
am_static_inline
uint32_t amhw_pit_cval_get (amhw_pit_t *p_hw_pit, uint8_t chan)
{
    return p_hw_pit->channel[chan].current_value;
}

/**
 * \name PIT���ƼĴ�������(R/W)
 * @{
 */

#define AMHW_PIT_TCTL_MASK          (0x7u)           /**< \brief ģ������ */
#define AMHW_PIT_TCTL_CHAIN_MODE    (1UL << 2)       /**< \brief ��ģʽ */
#define AMHW_PIT_TCTL_TI_EN         (1UL << 1)       /**< \brief ��ʱ���ж�ʹ�� */
#define AMHW_PIT_TCTL_TMR_EN        (1UL << 0)       /**< \brief ��ʱ��ʹ�� */

/** @} */

/**
 * \brief ��ʱ�����ƼĴ�����λ
 *
 * \param[in] p_hw_pit : ָ��PIT�Ĵ������ָ��
 * \param[in] chan     : ͨ��ID��ʹ�ú�#AMHW_PIT_CH(n)
 * \param[in] flags    : AMHW_PIT_TCTL_* ��ֵ����
 *                       AMHW_PIT_TCTL_*��� ��OR��ֵ(#AMHW_PIT_TCTL_TI_EN)
 * \return ��
 */
am_static_inline
void amhw_pit_timer_ctrl_set (amhw_pit_t *p_hw_pit,
                              uint8_t     chan,
                              uint32_t    flags)
{
    p_hw_pit->channel[chan].control |= flags & AMHW_PIT_TCTL_MASK;
}

/**
 * \brief ��ʱ�����ƼĴ�������
 *
 * \param[in] p_hw_pit : ָ��PIT�Ĵ������ָ��
 * \param[in] chan     : ͨ��ID��ʹ�ú�#AMHW_PIT_CH(n)
 * \param[in] flags    : AMHW_PIT_TCTL_* ��ֵ����
 *                       AMHW_PIT_TCTL_*��� ��OR��ֵ(#AMHW_PIT_TCTL_TI_EN)
 * \return ��
 */
am_static_inline
void amhw_pit_timer_ctrl_clear (amhw_pit_t *p_hw_pit,
                                uint8_t     chan,
                                uint32_t    flags)
{
    p_hw_pit->channel[chan].control &= ~(flags & AMHW_PIT_TCTL_MASK);
}

/**
 * \name PIT��־�Ĵ�������(R/W)
 * @{
 */

#define AMHW_PIT_TMR_FLAG        (1UL << 0)             /**< \brief ��ʱ��ʹ�� */

/** @} */

/**
 * \brief ��ʱ���жϱ�־
 *
 * \param[in] p_hw_pit : ָ��PIT�Ĵ������ָ��
 * \param[in] chan     : ͨ��ID��ʹ�ú�#AMHW_PIT_CH(n)
 *
 * \retval  TRUE  : ��ʱ�ѷ���
 * \retval  FALSE : ��ʱδ����
 */
am_static_inline
bool_t amhw_pit_timerout_flag_check (amhw_pit_t *p_hw_pit,
                                     uint8_t     chan)
{
    return (bool_t)(p_hw_pit->channel[chan].flag & AMHW_PIT_TMR_FLAG);
}

/**
 * \brief ��ʱ���жϱ�־���
 *
 * \param[in] p_hw_pit : ָ��PIT�Ĵ������ָ��
 * \param[in] chan     : ͨ��ID��ʹ�ú�#AMHW_PIT_CH(n)
 *
 * \return ��
 */
am_static_inline
void amhw_pit_timerout_flag_clr (amhw_pit_t *p_hw_pit,
                                 uint8_t     chan)
{
    p_hw_pit->channel[chan].flag = AMHW_PIT_TMR_FLAG;
}

/**
 * @} amhw_if_pit
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_PIT_H */

/* end of file */

