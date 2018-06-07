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
 * \brief ��ģת����DAC�������ӿ�
 * 1. Ƭ�Ͽɱ�����������(��ѹ����� 1/4096Vin �� Vin Vin ������ 1/4096Vin)
 * 2. Vin ���Դ������ο���Դ��ѡ��
 * 3. ������ͣ��ģʽ�ľ�̬����
 * 4. ֧�� 16 λ�����ݻ�����
 * 5. ֧�� DMA
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-22  xiongyanming, first implementation.
 * \endinternal
 */

#ifndef __AMHW_DAC_H
#define __AMHW_DAC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"

/**
 * \addtogroup amhw_if_dac
 * \copydoc amhw_dac.h
 * @{
 */

/**
 * \brief dac �Ĵ�����ṹ��
 */
typedef struct amhw_dac {
    struct{
        __IO uint8_t datl;                /**< \brief DAC ���ݣ��ͣ��Ĵ���   */
        __IO uint8_t dath;                /**< \brief DAC ���ݣ��ߣ��Ĵ���   */
    } dat[2];                             /**< \brief DAC ���ݼĴ�������     */
    __IO uint8_t reserved[28];            /**< \brief ����                   */
     __IO uint8_t sr;                     /**< \brief DAC ״̬�Ĵ���         */
     __IO uint8_t c0;                     /**< \brief DAC���ƼĴ���0         */
     __IO uint8_t c1;                     /**< \brief DAC���ƼĴ��� 1        */
     __IO uint8_t c2;                     /**< \brief DAC���ƼĴ��� 2        */
} amhw_dac_t;


/**
 * \brief DAC ͨ�����
 */
#define AMHW_DAC0_CHAN_0          0      /**< \brief DAC ͨ�� 0              */

/**
 * \brief DAC ��������С
 */
#define AMHW_DAC_DATL_COUNT       2      /**< \brief DAC ��������С          */


/**
 * \name DAC���ݼĴ�������λ�궨��
 * @{
 */

/**
 * \brief DAC ����ͨ������
 */
typedef enum amhw_dac_dat_index {
    AMHW_DAC_DAT_0 = 0,                  /**< \brief  ����ͨ��0              */
    AMHW_DAC_DAT_1                       /**< \brief  ����ͨ��1              */
} amhw_dac_dat_index_t;

/** \brief ���ݼĴ�������                                                    */
#define AMHW_DAC_DAT_MASK     0xFFu
/** \brief ���ݼĴ�������                                                    */
#define AMHW_DAC_DAT(x)       (((uint8_t)(((uint8_t)(x))<<0))&AMHW_DAC_DAT_MASK)

/** @} */

/**
 * \name DAC ���ƼĴ���C0 λ�궨��
 * \anchor grp_amhw_dac_c0
 * @{
 */
 
/**
 * \brief DAC �ο���ѹѡ������
 */
typedef enum amhw_dac_vref_sel {
    AMHW_DAC_VREF_1,                    /**< \brief  DACREF_1 ��Ϊ�ο���ѹ */
    AMHW_DAC_VREF_2,                    /**< \brief  DACREF_2 ��Ϊ�ο���ѹ */
} amhw_dac_vref_sel_t;

 /**
 * \brief DAC����Դѡ������
 */
typedef enum amhw_dac_trg_sel {
    AMHW_DAC_TRG_HARD,                  /**< \brief  DAC Ӳ������         */
    AMHW_DAC_TRG_SOFT,                  /**< \brief  DAC �������         */
} amhw_dac_trg_sel_t;

 /**
 * \brief DAC����ģʽ����
 */
typedef enum amhw_dac_power_sel {
    AMHW_DAC_POWER_HIGH,                  /**< \brief  DAC Ӳ������       */
    AMHW_DAC_POWER_LOW                    /**< \brief  DAC �������       */
} amhw_dac_power_sel_t;

/** \brief DACʹ��                                                        */
#define AMHW_DAC_C0_EN                  ((uint8_t)(1<<7))
/** \brief DAC����                                                        */
#define AMHW_DAC_C0_DISEN               ((uint8_t)(0<<7))   
/** \brief DAC�ο���ѹѡ��                                                */
#define AMHW_DAC_C0_VREF_SEL(x)         ((uint8_t)(((uint8_t)(x))<<6))
/** \brief  DAC ����Դѡ��                                                */
#define AMHW_DAC_C0_TRG_SEL(x)          ((uint8_t)(((uint8_t)(x))<<5)) 
/** \brief DAC�������ʹ��                                                */
#define AMHW_DAC_C0_TRG_SOFT_EN         ((uint8_t)(1<<4))  
/** \brief DAC�����������                                                */
#define AMHW_DAC_C0_TRG_SOFT_DISEN      ((uint8_t)(0<<4))  
/** \brief ʹ��DAC�͹���ģʽ                                              */
#define AMHW_DAC_C0_POWER_SEL(x)        ((uint8_t)(((uint8_t)(x))<<3)) 

/** \brief DAC �����ȡ��ָ���ж�ʹ��                                     */
#define AMHW_DAC_C0_BTI_EN              ((uint8_t)(1<<1))
/** \brief DAC �����ȡ��ָ���жϽ���                                     */
#define AMHW_DAC_C0_BTI_DISEN           ((uint8_t)(0<<1))
/** \brief DAC �����ȡ��ָ���ж�ʹ��                                     */
#define AMHW_DAC_C0_BBI_EN              ((uint8_t)(1<<0))
/** \brief DAC �����ȡ��ָ���жϽ���                                     */
#define AMHW_DAC_C0_BBI_DISEN           ((uint8_t)(0<<0))

/** @} */

/**
 * \name DAC ���ƼĴ���C1 λ�궨��
 * @{
 */
 
 /**
 * \brief DAC�������ģʽѡ������
 */
typedef enum amhw_dac_buf_mode {
    AMHW_DAC_BUFMODE_NORMAL,              /**< \brief  DAC ��ͨģʽ         */
    AMHW_DAC_BUFMODE_SCAN,                /**< \brief  DAC ����ɨ��ģʽ     */
} amhw_dac_buf_mode_t;

/** \brief DAC��DMAʹ��                                                     */
#define AMHW_DAC_C1_DMA_EN              ((uint8_t)(1<<7))   
/** \brief DAC �������ģʽ                                                 */
#define AMHW_DAC_C1_BUFMODE_SEL(x)      ((uint8_t)(((uint8_t)(x))<<2)) 
/** \brief DAC ����ʹ��                                                     */
#define AMHW_DAC_C1_BUF_EN              ((uint8_t)(1<<0))

/** @} */

/**
 * \name DAC ״̬�Ĵ���SRλ�궨��
 * @{
 */
 
/** \brief DAC ���嶥ָ���������                                            */
#define AMHW_DAC_SR_BFRP_TOPF_MASK       ((uint8_t)(1<<1)) 
/** \brief DAC �����ָ���������                                            */
#define AMHW_DAC_SR_BFRP_BOTF_MASK       ((uint8_t)(1<<0))

/** @} */

/**
 * \brief DAC ���������ò����ṹ��
 */
typedef struct amhw_dac_buffer_config
{
    bool_t buffer_enable;          /**< \brief  DAC Ӳ������               */
    amhw_dac_trg_sel_t trig_mode;  /**< \brief  DAC Ӳ������               */
    bool_t start_int_enable;       /**< \brief  DAC �����ȡ��ָ���ж�ʹ�� */
    bool_t upper_int_enable;       /**< \brief  DAC �����ȡ��ָ���ж�ʹ�� */
    bool_t dma_enable;             /**< \brief  DAC DMAʹ��λ              */
    amhw_dac_buf_mode_t buf_mode;  /**< \brief  DAC BUF����ģʽѡ��λ      */
    uint8_t upperidx;              /**< \brief  DAC ����ֵ                 */
} amhw_dac_buffer_config_t;

/**
 * \brief dac �жϱ�־λö�ٶ���
 */
typedef enum amhw_dac_flag_t
{
    AMHW_DAC_BUF_FLAG_START = 1U, /**< \brief ����ָ�뿪ʼ�¼���־λ */
    AMHW_DAC_BUF_FLAG_UPPER = 2U  /**< \brief ����ָ�뵽�����ޱ�־λ */
} amhw_dac_flag_t;

/**
 * \brief DAC���ƼĴ���0���ú���
 *
 * \param[in] p_hw_dac : ָ��DAC�Ĵ������ָ��
 * \param[in] flags    : ���ò��� �μ�\ref grp_amhw_dac_c0
                          AMHW_DAC_C0_*��ֵ��AMHW_DAC_C0_*�����ֵ�Ļ�ֵ
 *
 * \return ��
 */
am_static_inline
void amhw_dac_c0_cfg (amhw_dac_t *p_hw_dac, uint8_t flags)
{
    p_hw_dac->c0 = flags;
}

/**
 * \brief д���ݵ�DAC �����ݼĴ���
 *
 * \param[in] p_hw_dac  : ָ��DAC�Ĵ������ָ��
 * \param[in] dat_index : AMHW_DAC_DAT_* ö������
 * \param[in] datl      : datlֵ
 *
 * \return ��
 */
am_static_inline
void amhw_dac_datl_set (amhw_dac_t *p_hw_dac, amhw_dac_dat_index_t dat_index, 
                        uint8_t datl)
{
    p_hw_dac->dat[dat_index].datl = AMHW_DAC_DAT(datl);
}

/**
 * \brief д���ݵ�DAC �����ݼĴ���
 *
 * \param[in] p_hw_dac  : ָ��DAC�Ĵ������ָ��
 * \param[in] dat_index : AMHW_DAC_DAT_* ö������
 * \param[in] dath      : dathֵ
 *
 * \return ��
 */
am_static_inline
void amhw_dac_dath_set (amhw_dac_t *p_hw_dac, amhw_dac_dat_index_t dat_index,
                        uint8_t dath)
{
    p_hw_dac->dat[dat_index].dath = AMHW_DAC_DAT(dath);
}

/**
 * \brief д���ݵ�DAC ���ݼĴ���
 *
 * \param[in] p_hw_dac  : ָ��DAC�Ĵ������ָ��
 * \param[in] dat_index : AMHW_DAC_DAT_* ö������
 * \param[in] dat       : datֵ
 *
 * \return ��
 */
am_static_inline
void amhw_dac_dat_set (amhw_dac_t *p_hw_dac, amhw_dac_dat_index_t dat_index,
                       uint16_t dat)
{
    p_hw_dac->dat[dat_index].datl = AMHW_DAC_DAT(dat);
    p_hw_dac->dat[dat_index].dath = AMHW_DAC_DAT(dat>>8);
}


/**
 * \brief ʹ��DAC���ܺ���
 *
 * \param[in] p_hw_dac : ָ��DAC�Ĵ������ָ��
 *
 * \return ��
 */
am_static_inline
void amhw_dac_enable (amhw_dac_t *p_hw_dac )
{
    p_hw_dac->c0 |= AMHW_DAC_C0_EN;
}

/**
 * \brief ����DAC���ܺ���
 *
 * \param[in] p_hw_dac : ָ��DAC�Ĵ������ָ��
 *
 * \return ��
 */
am_static_inline
void amhw_dac_disable (amhw_dac_t *p_hw_dac )
{
    p_hw_dac->c0 &= ~AMHW_DAC_C0_EN;
}

/**
 * \brief DAC�ο���ѹ���ú���
 *
 * \param[in] p_hw_dac : ָ��DAC�Ĵ������ָ��
 * \param[in] vref     : amhw_dac_vref_sel_tö������
 *                       AMHW_DAC_VREF_* ��ֵ
 *
 * \return ��
 */
am_static_inline
void amhw_dac_refv_sel (amhw_dac_t *p_hw_dac ,amhw_dac_vref_sel_t vref)
{
    p_hw_dac->c0 &= ~(1<<6);
    p_hw_dac->c0 |= AMHW_DAC_C0_VREF_SEL(vref);
}

/**
 * \brief DAC����Դ���ú���
 *
 * \param[in] p_hw_dac : ָ��DAC�Ĵ������ָ��
 * \param[in] trg      : amhw_dac_trg_sel_tö������
 *                       AMHW_DAC_TRG_* ��ֵ
 *
 * \return ��
 */
am_static_inline
void amhw_dac_trg_sel (amhw_dac_t *p_hw_dac ,amhw_dac_trg_sel_t trg)
{
    p_hw_dac->c0 &= ~(1<<5);
    p_hw_dac->c0 |= AMHW_DAC_C0_TRG_SEL(trg);
}

/**
 * \brief DAC�������ʹ�ܺ���
 *
 * \param[in] p_hw_dac : ָ��DAC�Ĵ������ָ��
 *
 * \return ��
 */
am_static_inline
void amhw_dac_soft_trg_enable (amhw_dac_t *p_hw_dac )
{
    p_hw_dac->c0 |= AMHW_DAC_C0_TRG_SOFT_EN;
}

/**
 * \brief DAC����������ܺ���
 *
 * \param[in] p_hw_dac : ָ��DAC�Ĵ������ָ��
 *
 * \return ��
 */
am_static_inline
void amhw_dac_soft_trg_disable (amhw_dac_t *p_hw_dac )
{
    p_hw_dac->c0 &= ~AMHW_DAC_C0_TRG_SOFT_EN;
}

/**
 * \brief DAC����ģʽ���ú���
 *
 * \param[in] p_hw_dac   : ָ��DAC�Ĵ������ָ��
 * \param[in] power_mode : amhw_dac_power_sel_tö������
 *                         AMHW_POWER_* ��ֵ
 * \return ��
 */
am_static_inline
void amhw_dac_power_set (amhw_dac_t *p_hw_dac ,amhw_dac_power_sel_t power_mode)
{
    p_hw_dac->c0 &= ~(1<<3);
    p_hw_dac->c0 |= AMHW_DAC_C0_POWER_SEL(power_mode);
}

/**
 * \brief �����ȡ��ָ���ж�ʹ�ܺ���
 *
 * \param[in] p_hw_dac : ָ��DAC�Ĵ������ָ��
 *
 * \return ��
 */
am_static_inline
void amhw_dac_buftop_int_enable (amhw_dac_t *p_hw_dac )
{
    p_hw_dac->c0 |= AMHW_DAC_C0_BTI_EN;
}

/**
 * \brief �����ȡ��ָ���жϽ��ܺ���
 *
 * \param[in] p_hw_dac : ָ��DAC�Ĵ������ָ��
 *
 * \return ��
 */
am_static_inline
void amhw_dac_buftop_int_disable (amhw_dac_t *p_hw_dac )
{
    p_hw_dac->c0 &= ~AMHW_DAC_C0_BTI_EN;
}

/**
 * \brief �����ȡ��ָ���ж�ʹ�ܺ���
 *
 * \param[in] p_hw_dac : ָ��DAC�Ĵ������ָ��
 *
 * \return ��
 */
am_static_inline
void amhw_dac_bufbot_int_enable (amhw_dac_t *p_hw_dac )
{
    p_hw_dac->c0 |= AMHW_DAC_C0_BBI_EN;
}

/**
 * \brief �����ȡ��ָ���жϽ��ܺ���
 *
 * \param[in] p_hw_dac : ָ��DAC�Ĵ������ָ��
 *
 * \return ��
 */
am_static_inline
void amhw_dac_bufbot_int_disable (amhw_dac_t *p_hw_dac )
{
    p_hw_dac->c0 &= ~AMHW_DAC_C0_BBI_EN;
}

/**
 * \brief DAC�������ģʽ���ú���
 *
 * \param[in] p_hw_dac : ָ��DAC�Ĵ������ָ��
 * \param[in] bf_mode  : amhw_dac_buf_mode_tö������
 *                       AMHW_DAC_BUFMODE_* ��ֵ
 *
 * \return ��
 */
am_static_inline
void amhw_dac_bufmode_set(amhw_dac_t *p_hw_dac ,amhw_dac_buf_mode_t bf_mode)
{
    p_hw_dac->c1 &= ~(1<<2);
    p_hw_dac->c1 |= AMHW_DAC_C1_BUFMODE_SEL(bf_mode);
}

/**
 * \brief ʹ��DAC��DMA����
 *
 * \param[in] p_hw_dac : ָ��DAC�Ĵ������ָ��
 *
 * \return ��
 */
am_static_inline
void amhw_dac_dma_enable (amhw_dac_t *p_hw_dac )
{
    p_hw_dac->c1 |= AMHW_DAC_C1_DMA_EN;
}

/**
 * \brief ����DAC��DMA����
 *
 * \param[in] p_hw_dac : ָ��DAC�Ĵ������ָ��
 *
 * \return ��
 */
am_static_inline
void amhw_dac_dma_disable (amhw_dac_t *p_hw_dac )
{
    p_hw_dac->c1 &= ~AMHW_DAC_C1_DMA_EN;
}

/**
 * \brief ʹ��DAC�Ļ���������
 *
 * \param[in] p_hw_dac : ָ��DAC�Ĵ������ָ��
 *
 * \return ��
 */
am_static_inline
void amhw_dac_buf_enable (amhw_dac_t *p_hw_dac )
{
    p_hw_dac->c1 |= AMHW_DAC_C1_BUF_EN;
}

/**
 * \brief ����DAC�Ļ���������
 *
 * \param[in] p_hw_dac : ָ��DAC�Ĵ������ָ��
 *
 * \return ��
 */
am_static_inline
void amhw_dac_buf_disable (amhw_dac_t *p_hw_dac )
{
    p_hw_dac->c1 &= ~AMHW_DAC_C1_BUF_EN;
}

/**
 * \brief ����DAC����������ֵ
 *
 * \param[in] p_hw_dac   : ָ��DAC�Ĵ������ָ��
 * \param[in] buf_up_val : ����������ֵ buf_up_val
 *
 * \return ��
 */
am_static_inline
void amhw_dac_c2_buf_up (amhw_dac_t *p_hw_dac , uint8_t buf_up_val)
{
    p_hw_dac->c2 |= ((uint8_t)(buf_up_val&0x01));
}

/**
 * \brief ��ȡDAC��������־λ
 *
 * \param[in] p_hw_dac : ָ��DAC�Ĵ������ָ��
 * \param[in] flag     : ��Ҫ�õ����жϱ�־λ amhw_dac_flag_t ö������
 *                                            AMHW_DAC_BUF_FLAG_* ��ֵ
 * \return    TRUE     : ��־λ��λ
 *            FALSE    : ��־λû����λ
 */
am_static_inline
bool_t amhw_dac_bufint_flag_get (amhw_dac_t *p_hw_dac ,amhw_dac_flag_t flag)
{


    bool_t b_ret = TRUE;
    switch (flag)
    {
    case AMHW_DAC_BUF_FLAG_START:
        b_ret = (bool_t)((p_hw_dac->sr & AMHW_DAC_SR_BFRP_BOTF_MASK));
        break;
    case AMHW_DAC_BUF_FLAG_UPPER:
        b_ret = (bool_t)(p_hw_dac->sr & AMHW_DAC_SR_BFRP_TOPF_MASK);
        break;
    default:
        b_ret = FALSE;
        break;
    }
    return b_ret;
}

/**
 * \brief ���DAC�������жϱ�־λ
 *
 * \param[in] p_hw_dac : ָ��DAC�Ĵ������ָ��
 * \param[in] flag     : ��Ҫ�õ����жϱ�־λ amhw_dac_flag_t ö������
 *                                            AMHW_DAC_BUF_FLAG_* ��ֵ
 * \return ��
 */
am_static_inline
void amhw_dac_bufint_flag_clr (amhw_dac_t *p_hw_dac ,amhw_dac_flag_t flag)
{
    switch (flag)
    {
    case AMHW_DAC_BUF_FLAG_START:
        p_hw_dac->sr &= ~AMHW_DAC_SR_BFRP_BOTF_MASK;
        break;
    case AMHW_DAC_BUF_FLAG_UPPER:
        p_hw_dac->sr &= ~AMHW_DAC_SR_BFRP_TOPF_MASK;
        break;
    default:
        p_hw_dac->sr &= ~AMHW_DAC_SR_BFRP_BOTF_MASK;
        p_hw_dac->sr &= ~AMHW_DAC_SR_BFRP_TOPF_MASK;
        break;
    }
}

/**
 * \brief ���û�����ֵ
 *
 * \param[in] p_hw_dac : ָ��DAC�Ĵ������ָ��
 * \param[in] start    : ��Ҫ����ֵ����ʼλ��
 * \param[in] offset   : ÿ��ƫ��ֵ
 * \param[in] arr[]    : ��Ž�Ҫд�뻺������ֵ
 *                                            
 * \return    AM_ERROR : ����bufֵ����
 *            AM_OK    : ����bufֵ�ɹ�
 */
am_static_inline
int amhw_dac_buf_val_set (amhw_dac_t *p_hw_dac ,uint8_t start, uint8_t offset,
                          uint16_t arr[])
{
    uint8_t i;

    if (  (!arr) || (start + offset > AMHW_DAC_DATL_COUNT)){
        return AM_ERROR;
    }

    for (i = 0; i < offset; i++) {
        amhw_dac_dat_set(p_hw_dac,(amhw_dac_dat_index_t)(start+i),arr[i]);
    }

    return AM_OK;
   
}
/**
 * \brief DAC���ݻ�������ʼ��
 *
 * \param[in] p_hw_dac : ָ��DAC�Ĵ������ָ��
 * \param[in] p_buf    : ָ��DAC���������ýṹ��
 *
 * \return ��
 */
void amhw_dac_dat_buf_init (amhw_dac_t *p_hw_dac,amhw_dac_buffer_config_t *p_buf);

/**
 * @} amhw_if_dac
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_DAC_H */

/* end of file */





