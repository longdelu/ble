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
 * \brief TPM����������Timer������ʵ��
 *
 * 1. TPM֧���ṩ�������ֱ�׼���񣬱������ṩ���Ƿ���Timer��׼�����������
 *     - ��ʱ
 *     - PWM���
 *     - ����
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-20  cuishengen, first implementation.
 * \endinternal
 */

#ifndef __AMDR_TPM_TIMING_H
#define __AMDR_TPM_TIMING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_timer.h"
#include "amhw_tpm.h"

/**
 * \addtogroup amdr_if_tpm_timing
 * \copydoc amdr_tpm_timing.h
 * @{
 */

/**
 * \brief TPM��ʱ������ص��豸��Ϣ
 */
typedef struct amdr_tpm_timing_devinfo {
    amhw_tpm_t   *p_hw_tpm;    /**< \brief ָ��TPM�Ĵ������ָ�� */
    uint8_t       inum;        /**< \brief TPM�жϺ�  */
} amdr_tpm_timing_devinfo_t;
    
/**
 * \brief TPM��ʱ�����豸
 */
typedef struct amdr_tpm_timing_dev {

    am_timer_serv_t   timer_serv;       /**< \brief ��׼��ʱ(Timer)���� */

    void (*pfn_callback)(void *);       /**< \brief �ص����� */
    void *p_arg;                        /**< \brief �ص��������û����� */
    
    /** \brief ָ��TPM(��ʱ����)�豸��Ϣ������ָ�� */
    const amdr_tpm_timing_devinfo_t  *p_devinfo;

} amdr_tpm_timing_dev_t;

/**
 * \brief ��ʼ��TPMΪ��ʱ����
 *
 * \param[in] p_dev     : ָ��TPM(��ʱ����)�豸��ָ��
 * \param[in] p_devinfo : ָ��TPM(��ʱ����)�豸��Ϣ������ָ��
 *
 * \return Timer��׼������������ֵΪNULLʱ������ʼ��ʧ��
 */
am_timer_handle_t amdr_tpm_timing_init(amdr_tpm_timing_dev_t            *p_dev,
                                       const amdr_tpm_timing_devinfo_t  *p_devinfo);

/**
 * \brief ��ʹ��TPM��ʱ����ʱ�����ʼ��TPM��ʱ���ܣ��ͷ������Դ
 * \param[in] p_dev : ָ��TPM(��ʱ����)�豸��ָ��
 * \return ��
 */
void amdr_tpm_timing_deinit(amdr_tpm_timing_dev_t *p_dev);

/** 
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMDR_TPM_TIMING_H */

/* end of file */
