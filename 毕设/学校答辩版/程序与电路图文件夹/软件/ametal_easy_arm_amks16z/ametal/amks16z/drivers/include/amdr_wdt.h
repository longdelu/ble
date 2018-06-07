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
 * \brief  WWDT����������WWDT��׼�ӿ�
 *
 * 1. ���Ź������üĴ���ֻ��дһ�Σ�������ʹ��KS16XXоƬʱ��������Ҫʹ
 *    �ÿ��Ź�����ʱ�����������ļ���Ŀ��Ź����غ���Ϊ0��ϵͳ���ڰ弶��
 *    ʼ��ʱ�رտ��Ź�������Ҫʹ�ÿ��Ź�����ʱ�������Ź����غ���1��ϵͳ��
 *    �ڿ��Ź�ʹ�ܺ��������ÿ��Ź���ʹ֮����������
 * 2. ���Ź��ĳ�ʱʱ��ֻ������Ϊ32ms,256ms,1024ms������ʱ���е�һ����
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-23  wxj, first implementation.
 * \endinternal
 */

#ifndef __AMDR_WDT_H
#define __AMDR_WDT_H


#ifdef __cplusplus
extern "C" {
#endif

#include "am_wdt.h"
#include "amhw_sim.h"

/**
 * \addtogroup amdr_if_wdt
 * \copydoc amdr_wdt.h
 * @{
 */

/**
 * \brief WDT �豸��Ϣ�ṹ��
 */
typedef struct amdr_wdt_devinfo {
    amhw_sim_t *p_hw_wdt;             /**< \brief ָ��WDT�Ĵ�����Ľṹ��ָ�� */
} amdr_wdt_devinfo_t;

/**
 * \brief WDT �豸�ṹ��
 */
typedef struct amdr_wdt_dev {

    am_wdt_serv_t              wdt_serv;   /**< \brief WDT��׼����         */
    const amdr_wdt_devinfo_t  *p_devinfo;  /**< \brief ָ��WDT�豸��Ϣ��ָ�� */
} amdr_wdt_dev_t;

/**
 * \brief ��ʼ��WDT
 *
 * \param[in] p_dev     : ָ��WDT�豸
 * \param[in] p_devinfo : ָ��WDT�豸��Ϣ
 *
 * \return WDT��׼���������������Ϊ NULL��������ʼ��ʧ�ܡ�
 */
am_wdt_handle_t amdr_wdt_init(amdr_wdt_dev_t           *p_dev,
                              const amdr_wdt_devinfo_t *p_devinfo);

/**
 * \brief ���WDT��ʼ��
 * \param[in] p_dev : ָ��WDT�豸
 * \return  ��
 */
void amdr_wdt_deinit(amdr_wdt_dev_t *p_dev);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMDR_WDT_H */

/* end of file */
