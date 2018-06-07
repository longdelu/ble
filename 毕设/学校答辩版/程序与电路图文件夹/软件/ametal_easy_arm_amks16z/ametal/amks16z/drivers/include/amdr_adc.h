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
 * \brief ADC����������ADC��׼�ӿ�
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-23  xjc, first implementation.
 * \endinternal
 */

#ifndef  __AMDR_ADC_H
#define  __AMDR_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_adc.h"
#include "amhw_adc.h"

/**
 * \addtogroup amdr_if_adc
 * \copydoc amdr_adc.h
 * @{
 */
#define AMDR_ADC_AVG_DIS     0    /**< \brief ��ʹ��Ӳ��ƽ������ */
#define AMDR_ADC_AVG_EN      1    /**< \brief ʹ��Ӳ��ƽ������ */
#define AMDR_ADC_CMP_DIS     0    /**< \brief ��ʹ�ñȽϹ��� */
#define AMDR_ADC_CMP_EN      1    /**< \brief ʹ�ñȽϹ��� */

/**
 * \name ADC �������ñ�־
 * \anchor grp_amdr_adc_diff
 * @{
 */
#define AMDR_ADC_DIFF_EN      TRUE    /**< \brief ʹ�ò������ģʽ */
#define AMDR_ADC_DIFF_DIS     FALSE   /**< \brief ʹ�õ�������ģʽ */
/** @} */

/**
 * \name ADC ����ģʽ���ñ�־
 * \anchor grp_amdr_adc_mode_trg
 * @{
 */
#define AMDR_ADC_MODE_TRG_SF      0    /**< \brief �������ģʽ��־ */
#define AMDR_ADC_MODE_TRG_HW      1    /**< \brief Ӳ������ģʽ���� */
/** @} */

/**
 * \name ADC ״̬���ƼĴ���3λ����
 * \anchor grp_amdr_cmp_mode
 * @{
 */
/** \brief �Ƚ�����Ϊ "res < cmp_val1" */
#define AMDR_ADC_CMP_MODE_1   0

/** \brief �Ƚ�����Ϊ "res >= cmp_val1" */
#define AMDR_ADC_CMP_MODE_2   1

/** \brief �Ƚ�����Ϊ "res < a" ����  "res > b" ��a=min(cmp_val1, cmp_val2),b=max(cmp_val1, cmp_val2)��*/
#define AMDR_ADC_CMP_MODE_3   2

/** \brief �Ƚ�����Ϊ " a < res < b" ��a=min(cmp_val1, cmp_val2),b=max(cmp_val1, cmp_val2)��*/
#define AMDR_ADC_CMP_MODE_4   3

/** \brief �Ƚ�����Ϊ " a <= res <= b" ��a=min(cmp_val1, cmp_val2),b=max(cmp_val1, cmp_val2)��*/
#define AMDR_ADC_CMP_MODE_5   4

/** \brief �Ƚ�����Ϊ "res <= a" ����  "res >= b" ��a=min(cmp_val1, cmp_val2),b=max(cmp_val1, cmp_val2)��*/
#define AMDR_ADC_CMP_MODE_6   5
/** @} */

/**
 * \brief ADC Ӳ��ƽ��������Ϣ�ṹ��
 */
typedef struct amdr_adc_avg_info {
    /** \brief Ӳ��ƽ��ʹ�ܱ�־ */
    uint8_t         enable;
    /** \brief ƽ���Ĳ���ֵ���� */
    amhw_adc_avgs_t avgs_num;
} amdr_adc_avg_info_t;

/**
 * \brief ADC �ȽϹ�����Ϣ�ṹ��
 */
typedef struct amdr_adc_cmp_info {
    /** \brief �ȽϹ���ʹ�ܱ�־ */
    uint8_t  enable;

    /** \brief �Ƚ�ģʽ */
    uint16_t cmp_mode;

    /** \brief �ȽϹ�����ֵ1 */
    uint16_t cmp_val1;

    /** \brief �ȽϹ�����ֵ2 */
    uint16_t cmp_val2;

} amdr_adc_cmp_info_t;



/**
 * \brief ADC �豸��Ϣ
 */
typedef struct amdr_adc_devinfo {

    /** \brief ָ��ADC�Ĵ������ָ�� */
    amhw_adc_t *p_hw_adc;

    /**
     * \brief ADC�ο���ѹ����λ��mV
     *
     * �ο���ѹ Vref = (Vrefh - Vrefl)  Ϊ��������������ʱ
     * �ȶ�����ADC��ȷ����Vrefh = 1.13~VDDA��Vrefl����ֵΪ�� Vss�� Vssa
     *
     * \note �òο���ѹ�ɾ���ĵ�·����
     *
     */
    uint32_t   vref;

    /** \brief �������ȣ���is_diff=TRUE��ȡֵΪ[9,11,13,16], ����ȡֵΪ[8,10,12,16]�� */
    uint8_t    bits;

    /** \brief �Ƿ�ʹ�ò��ģʽ��/ref grp_amdr_adc_diff, ��chan > 3,��ֵ����ԣ�*/
    uint8_t    is_diff;

    /** \brief ADC�ж������� */
    uint8_t    inum;

    /** \brief ����ģʽ �ο�/ref grp_amdr_adc_mode_trg */
    uint8_t    mode_trg;

    /** \brief ʹ�õ����� */
    amhw_adc_seq_index_t  seq;

    /** \brief DMA����ͨ����,��DMA_CHAN_0����Ϊ-1ָ��ʹ��DMA���� */
    int                       dma_chan;

    /** \brief ָ��Ӳ��ƽ����Ϣ�ṹ��ָ�� */
   const amdr_adc_avg_info_t *p_adc_avg_info;

    /** \brief ָ��ȽϹ�����Ϣ�ṹ��ָ�� */
   const amdr_adc_cmp_info_t *p_adc_cmp_info;
} amdr_adc_devinfo_t;

/**
 * \brief ADC�豸ʵ��
 */
typedef struct amdr_adc_dev {

    /** \brief ADC��׼���� */
    am_adc_serv_t             adc_serv;

    /** \brief ָ��ADC�豸��Ϣ��ָ�� */
    const amdr_adc_devinfo_t *p_devinfo;

    /** \brief ����û�����ת����Ļص����� */
    pfn_adc_seq_complete_t    pfn_callback;

    /** \brief �û������ص������Ļص��������� */
    void                    *p_arg;

    /** \brief ��ǰת�������������������׵�ַ */
    am_adc_buf_desc_t       *p_desc;

    /** \brief ��ǰת������������������ */
    uint32_t                desc_num;

    /** \brief ת���Ĵ��� */
    uint32_t                count;

    /** \brief ת����־   */
    uint32_t                flags;

    /** \brief ��ǰת����ͨ�� */
    uint32_t                chan;

    /** \brief ָ��������   */
    uint32_t                desc_pos;

    /** \brief ָ��ǰ�������ݵ�λ�� */
    uint32_t                index;

    /** \brief ��ǰת������ */
    uint32_t                count_pos;
} amdr_adc_dev_t;

/**
 * \brief ADC��ʼ��
 *
 * Ĭ�ϳ�ʼ������A
 *
 * \param[in] p_dev     : ָ��ADC�豸��ָ��
 * \param[in] p_devinfo : ָ��ADC�豸��Ϣ��ָ��
 *
 * \return ADC��׼���������� ���Ϊ NULL��������ʼ��ʧ��
 * \note   Ĭ������100KHz�����ʣ���bus����ʱ���㹻��
 */
am_adc_handle_t amdr_adc_init(amdr_adc_dev_t           *p_dev,
                              const amdr_adc_devinfo_t *p_devinfo);

/**
 * \brief ADCȥ��ʼ��
 *
 * Ĭ��ȥ��ʼ������A
 *
 * \param[in] p_dev : ָ��ADC�豸��ָ��
 *
 * \return ��
 */
void amdr_adc_deinit(amdr_adc_dev_t *p_dev);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /*__AMDR_ADC_H  */

/* end of file */
