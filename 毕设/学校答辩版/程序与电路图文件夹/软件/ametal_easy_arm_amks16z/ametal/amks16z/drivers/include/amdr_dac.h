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
 * \brief DAC����������DAC��׼�ӿ�
 *
 * \internal
 * \par Modification History
 * - 1.00 15-07-14  xiongyanming, first implementation.
 * \endinternal
 */
 
#ifndef  __AMDR_DAC_H
#define  __AMDR_DAC_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "am_types.h"
#include "am_dac.h"
#include "amhw_dac.h"

/**
 * \addtogroup amdr_if_dac
 * \copydoc amdr_dac.h
 * @{
 */
/**
 * \brief DAC �豸��Ϣ 
 */
typedef struct amdr_dac_devinfo {
    
    /** \brief ָ��DAC�Ĵ������ָ�� */
    amhw_dac_t *p_hw_dac;

    /** \brief DAC�жϺ� */
    uint8_t    inum;

    /** \brief DACת������ */
    uint8_t     bits;

    /** 
     * \brief DAC�ο���ѹ����λ��mV 
     * 
     * \note �òο���ѹ�ɾ���ĵ�·���� 
     *
     */
    uint32_t    vref;
    /**
     * \brief DAC�ο���ѹԴѡ��
     *
     * \note �òο���ѹ�ɾ���ĵ�·����
     *
     */
    amhw_dac_vref_sel_t    vref_source;


} amdr_dac_devinfo_t;

/**
 * \brief DAC�豸ʵ�� 
 */
typedef struct amdr_dac_dev {
    
    /** \brief DAC��׼���� */
    am_dac_serv_t             dac_serve;

    /** \brief ָ��DAC�豸��Ϣ��ָ�� */
    const amdr_dac_devinfo_t *p_devinfo;


    /** \brief ��ǰת����ͨ�� */
    uint32_t                 chan;
    
} amdr_dac_dev_t;

/**
 * \brief DAC��ʼ�� 
 *
 * Ĭ�ϳ�ʼ������A 
 *
 * \param[in] p_dev     : ָ��DAC�豸��ָ�� 
 * \param[in] p_devinfo : ָ��DAC�豸��Ϣ��ָ�� 
 *
 * \return DAC��׼���������� ���Ϊ NULL��������ʼ��ʧ�� 
 */
am_dac_handle_t amdr_dac_init(amdr_dac_dev_t           *p_dev, 
                              const amdr_dac_devinfo_t *p_devinfo);

/**
 * \brief DACȥ��ʼ�� 
 *
 * Ĭ��ȥ��ʼ������A 
 *
 * \param[in] p_dev : ָ��DAC�豸��ָ�� 
 *
 * \return �� 
 */
void amdr_dac_deinit(amdr_dac_dev_t *p_dev);                              
                              
/**
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /*__AMDR_DAC_H  */

/* end of file */
