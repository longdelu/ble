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
 * \brief TPM����������CAP��׼�ӿ�
 *
 * 1. TPM֧���ṩ�������ֱ�׼���񣬱������ṩ���Ƿ��񡰲��񡱱�׼�����������
 *     - ��ʱ
 *     - PWM���
 *     - ����
 * 2. TPM0������6������ͨ����TPM1��TPM2������2������ͨ����
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  cuishengen, first implementation.
 * \endinternal
 */

#ifndef __AMDR_TPM_CAP_H
#define __AMDR_TPM_CAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_cap.h"
#include "amhw_tpm.h"

/**
 * \addtogroup amdr_if_tpm_cap
 * \copydoc amdr_tpm_cap.h
 * @{
 */

/**
 * \brief TPM��������ص�GPIO��Ϣ
 */
typedef struct amdr_tpm_cap_ioinfo {
    uint32_t gpio;           /**< \brief ��Ӧ��GPIO�ܽ�                       */
    uint32_t func;           /**< \brief Ϊ������ʱ��GPIO��������           */
    uint32_t dfunc;          /**< \brief ���ܹܽŲ�����ʱ��Ĭ��GPIO�������� */
} amdr_tpm_cap_ioinfo_t;

/**
 * \brief TPM��������ص��豸��Ϣ
 */
typedef struct amdr_tpm_cap_devinfo {
    amhw_tpm_t             *p_hw_tpm;      /**< \brief ָ��TPM�Ĵ������ָ��  */
    uint8_t                 inum;          /**< \brief TPM��Ӧ���жϺ� */
    uint8_t                 channels_num;  /**< \brief ʹ�õ�ͨ���������Ϊ6 */
    amdr_tpm_cap_ioinfo_t  *p_ioinfo;      /**< \brief ָ�򲶻�ܽ���Ϣ�ṹ�� */
} amdr_tpm_cap_devinfo_t;

/**
 * \brief TPM�������豸
 */
typedef struct amdr_tpm_cap_dev {
    am_cap_serv_t          cap_serv;      /**< \brief ��׼CAP����            */

    struct {
        am_cap_callback_t  callback_func; /**< \brief �ص�����                    */
        void              *p_arg;         /**< \brief �ص��������û�����          */
    } callback_info[6];                   /**< \brief �洢���6ͨ���Ļص�������Ϣ */

    /** \brief ָ��TPM(������)�豸��Ϣ������ָ�� */
    const amdr_tpm_cap_devinfo_t *p_devinfo;

} amdr_tpm_cap_dev_t;

/**
 * \brief ��ʼ��TPMΪ������
 *
 * \param[in] p_dev     : ָ��TPM(������)�豸��ָ��
 * \param[in] p_devinfo : ָ��TPM(������)�豸��Ϣ������ָ��
 *
 * \return CAP��׼������������ֵΪNULLʱ������ʼ��ʧ��
 */
am_cap_handle_t amdr_tpm_cap_init(amdr_tpm_cap_dev_t           *p_dev,
                                  const amdr_tpm_cap_devinfo_t *p_devinfo);

/**
 * \brief ��ʹ��TPM������ʱ�����ʼ��TPM�����ܣ��ͷ������Դ
 * \param[in] p_dev : ָ��TPM(������)�豸��ָ��
 * \return ��
 */
void amdr_tpm_cap_deinit(amdr_tpm_cap_dev_t *p_dev);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMDR_TPM_CAP_H */

/* end of file */
