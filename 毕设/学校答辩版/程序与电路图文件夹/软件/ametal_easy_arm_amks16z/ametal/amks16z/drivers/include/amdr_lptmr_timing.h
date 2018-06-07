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
 * \brief LPTMR����������Timer������ʵ��
 *
 * 1. LPTMRֻ֧���ṩ"��ʱ"��׼���񣬱������ṩ���Ƿ���"��ʱ"��׼�����������
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-20  cuishengen, first implementation.
 * \endinternal
 */

#ifndef __AMDR_LPTMR_TIMING_H
#define __AMDR_LPTMR_TIMING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_timer.h"
#include "amhw_lptmr.h"

/**
 * \addtogroup amdr_if_lptmr_timing
 * \copydoc amdr_lptmr_timing.h
 * @{
 */

/**
 * \brief LPTMR��ʱ������ص��豸��Ϣ
 */
typedef struct amdr_lptmr_timing_devinfo {
    amhw_lptmr_t   *p_hw_lptmr;    /**< \brief ָ��LPTMR�Ĵ������ָ�� */
    uint8_t         inum;          /**< \brief LPTMR�жϺ�  */
} amdr_lptmr_timing_devinfo_t;
    
/**
 * \brief LPTMR��ʱ�����豸
 */
typedef struct amdr_lptmr_timing_dev {

    am_timer_serv_t   timer_serv;       /**< \brief ��׼��ʱ(Timer)���� */

    void (*pfn_callback)(void *);       /**< \brief �ص����� */
    void *p_arg;                        /**< \brief �ص��������û����� */
    
    /** \brief ָ��LPTMR(��ʱ����)�豸��Ϣ������ָ�� */
    const amdr_lptmr_timing_devinfo_t  *p_devinfo;

} amdr_lptmr_timing_dev_t;

/**
 * \brief ��ʼ��LPTMRΪ��ʱ����
 *
 * \param[in] p_dev     : ָ��LPTMR(��ʱ����)�豸��ָ��
 * \param[in] p_devinfo : ָ��LPTMR(��ʱ����)�豸��Ϣ������ָ��
 *
 * \return Timer��׼������������ֵΪNULLʱ������ʼ��ʧ��
 */
am_timer_handle_t amdr_lptmr_timing_init(amdr_lptmr_timing_dev_t            *p_dev,
                                       const amdr_lptmr_timing_devinfo_t  *p_devinfo);

/**
 * \brief ��ʹ��LPTMR��ʱ����ʱ�����ʼ��LPTMR��ʱ���ܣ��ͷ������Դ
 * \param[in] p_dev : ָ��LPTMR(��ʱ����)�豸��ָ��
 * \return ��
 */
void amdr_lptmr_timing_deinit(amdr_lptmr_timing_dev_t *p_dev);

/** 
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMDR_LPTMR_TIMING_H */

/* end of file */
