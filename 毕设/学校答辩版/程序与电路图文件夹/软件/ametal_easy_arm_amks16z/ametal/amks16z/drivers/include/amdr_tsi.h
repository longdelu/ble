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
 * \brief TSI������
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-21  wxj, first implementation.
 * \endinternal
 */

#ifndef __AMDR_TSI_H
#define __AMDR_TSI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "amhw_tsi.h"
#include "amhw_amks16z.h"
#include "am_int.h"

/**
 * \addtogroup amdr_if_tsi
 * \copydoc amdr_tsi.h
 * @{
 */

/**
 * \brief TSI �豸��Ϣ�ṹ��
 */
typedef struct amdr_tsi_devinfo {

    amhw_tsi_t   *p_hw_tsi;   /**< \brief ָ��TSI�Ĵ������ָ�� */	
    bool_t        lp_mode;    /**< \brief �͹��Ļ���MCU���� */
} amdr_tsi_devinfo_t;

/**
 * \brief TSI�豸ʵ��
 */
typedef struct amdr_tsi_dev {

    /** \brief ָ��TSI�豸��Ϣ��ָ�� */
    const amdr_tsi_devinfo_t *p_devinfo;

} amdr_tsi_dev_t;

/**
 * \brief TSI��ʼ��
 *
 * \param[in] p_dev     : ָ��TSI �豸��ָ��
 * \param[in] p_devinfo : ָ��TSI �豸��Ϣ��ָ��
 *
 * \return ��ʼ�������ɹ�
 */
int am_tsi_init (amdr_tsi_dev_t *p_dev, const amdr_tsi_devinfo_t *p_devinfo);

/**
 * \brief TSI���ʼ��
 *
 * \param[in] p_dev : ָ��TSI �豸��ָ��
 *
 * \return ���ʼ�������ɹ�
 */
int am_tsi_deinit (amdr_tsi_dev_t *p_dev);

/**
 * \brief TSI��������
 *
 * \param[in] chan      : TSI ͨ����
 * \param[in] work_mode : TSI ����ģʽ
 * \param[in] trig_mode : TSI ɨ�败��ģʽ
 *
 * \return �������ò����ɹ�
 */
int amdr_tsi_fun_cfg (amhw_tsi_channel_t    chan,
                      amhw_tsi_mode_t       work_mode,
                      amhw_tsi_trig_mode_t  trig_mode);

/**
 * \brief TSI�жϴ���ģʽ
 *
 * \param[in] flag  : �жϴ���ģʽ
 *
 * \return ��ȡ��ת��ֵ
 */
void amdr_tsi_trigger_cfg (amhw_tsi_irq_mode_t flag);

/**
 * \brief TSI�ж�ʹ��
 *
 * \return ��
 */
void amdr_tsi_int_enable (void);

/**
 * \brief TSI�жϽ���
 *
 * \return ��
 */
void amdr_tsi_int_disable (void);

/**
 * \brief TSI�жϷ���������
 *
 * \param[in] pfn_callback  : �жϷ������ӿ�
 *
 * \return ��
 */
int amdr_tsi_trigger_connect (am_pfnvoid_t pfn_callback);

 /**
 * \brief TSI ����������
 *
 * \param[in] val   : ������ֵ
 *
 * \return ��
 */
void amdr_tsi_sens_set (uint16_t val);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMDR_TSI_H */

/* end of file */
