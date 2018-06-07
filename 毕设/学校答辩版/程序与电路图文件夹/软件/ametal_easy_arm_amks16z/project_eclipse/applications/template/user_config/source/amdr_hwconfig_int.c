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
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief LPC8XX �ж��û������ļ�
 * \sa amdr_hwconfig_int.c
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-01-29  hbt, first implementation.
 * \endinternal
 */
 
#include "amdr_hwconfig_int.h"

/**
 * \addtogroup amdr_if_hwconfig_src_int
 * \copydoc amdr_hwconfig_int.c
 * @{
 */


/** \brief ����ע���ISR����  */
#define __ISRINFO_COUNT    INUM_INT_COUNT

/** \brief ֧�ֵ��жϷ�����Ϣ */
struct amdr_int_isr_info __int_isr_infor[__ISRINFO_COUNT];

/** \brief ISR ��Ϣӳ�� */
uint8_t __int_isr_map[INUM_INT_COUNT];


/** \brief �ж��豸��Ϣ */
const amdr_int_devinfo_t g_int_devinfo =
{
    {
        INUM_INT_MIN,  /**< \brief ����ʼ�Ϻ� */
        INUM_INT_MAX   /**< \brief ��ĩβ�Ϻ� */
    },

    AMHW_INT,               /**< \brief ָ���жϿ������Ĵ������ָ��              */
    INUM_INT_COUNT,         /**< \brief ���ж�����                                */
    __int_isr_map,          /**< \brief ISR ��Ϣӳ��(��С�� input_cnt һ��)       */
    __ISRINFO_COUNT,        /**< \brief ISR ��Ϣ����                              */
    __int_isr_infor         /**< \brief ISR ��Ϣӳ���ڴ�(��С�� isrinfo_cnt һ��) */
};

/** \brief �ж��豸ʵ�� */
amdr_int_dev_t g_int_dev;


/**
 * @}
 */

/* end of file */
