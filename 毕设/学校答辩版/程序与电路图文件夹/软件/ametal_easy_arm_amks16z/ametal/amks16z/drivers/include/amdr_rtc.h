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
 * \brief RTC ģ���������ӿ�
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-19  zsp, first implementation.
 * \endinternal
 */

#ifndef __AMDR_RTC_H
#define __AMDR_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"
#include "am_rtc.h"
#include "amhw_rtc.h"

/**
 * \addtogroup amdr_if_rtc
 * \copydoc amdr_rtc.h
 * @{
 */

/**
 * \brief RTC �豸��Ϣ�ṹ��
 */
typedef struct amdr_rtc_devinfo {
    amhw_rtc_t  *p_hw_rtc;            /**< \brief ָ��RTC�Ĵ�����Ľṹ��ָ�� */
} amdr_rtc_devinfo_t;

/**
 * \brief RTC �豸�ṹ��
 */
typedef struct amdr_rtc_dev {
    am_rtc_serv_t             rtc_serv;     /**< \brief RTC��׼����           */
    const amdr_rtc_devinfo_t *p_devinfo;    /**< \brief ָ��RTC�豸��Ϣ��ָ�� */
} amdr_rtc_dev_t;

/**
 * \brief ��ʼ��RTC
 *
 * \param[in] p_dev     : ָ��RTC�豸
 * \param[in] p_devinfo : ָ��RTC�豸��Ϣ
 *
 * \return RTC��׼���������������Ϊ NULL��������ʼ��ʧ�ܡ�
 */
am_rtc_handle_t amdr_rtc_init (amdr_rtc_dev_t           *p_dev,
                               const amdr_rtc_devinfo_t *p_devinfo);

/**
 *  \brief ���RTC��ʼ��
 *
 *  \param p_dev ָ��RTC�豸��ָ��
 *
 *  \return ��
 */
void amdr_rtc_deinit (amdr_rtc_dev_t *p_dev);

/**
 *  \brief �������ڼ���һ���еĵڼ���
 *
 *  \param day
 *  \param month
 *  \param year
 *
 *  \return һ���еĵڼ��� (1��1�ռ�Ϊ��0��)
 *
 *  \details �� amdr_rtc_date2yday(4, 11, 2015)�������11��4����
 *           2015��ĵ�307��
 */
int amdr_rtc_date2yday (uint8_t day, uint8_t month, uint32_t year);


/**
 *  \brief ����RTC��У׼����
 *
 *  \param p_dev ָ��RTC�豸��ָ��
 *
 *  \param frq RTCʱ�ӵ�ʵ��Ƶ�ʣ���Hz��
 *
 *  \return ʵ��У׼���Ƶ��
 */
int amdr_rtc_compensation_set (amdr_rtc_dev_t *p_dev, float frq);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /** __AMDR_RTC_H */

