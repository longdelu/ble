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
 * \brief UART����������UART��׼�ӿ�
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-19  xym, first implementation.
 * \endinternal
 */

#ifndef __AMDR_UART_H
#define __AMDR_UART_H


#ifdef __cplusplus
extern "C" {
#endif

#include "am_uart.h"
#include "amhw_uart.h"
/**
 * \addtogroup amdr_if_uart
 * \copydoc amdr_uart.h
 * @{
 */

/**
 * \name �������ţ�����ص�������code���������������ݸ��û���ָ����ǰ�Ĵ�������
 *
 *  ����ص������Ĳ���ԭ�ͣ�\n
 *  int uart_err(void *p_arg, int code, void *p_data, int size);
 *  p_arg���û�ע����û�������p_data��size��ͬ�Ĵ������Ͱ�������Ϣ������ͬ��
 *  ����ʹ��am_uart_callback_set() �������ô��ڴ���ص�������
 * @{
 */

/**
 * \brief �û�ָ���������жϷ���,p_data������Ϊuint32_t��ֵΪ���ڵ��ж�״̬����
 *        ���û��жϾ�����ж�Դ��sizeΪ1��
 */
#define AMDR_UART_ERRCODE_UART_OTHER_INT  2

/** @} */


/**
 * \brief �����豸��Ϣ�ṹ�壬���豸��Ϣ���ڴ��ڳ�ʼ��
 */
typedef struct amdr_uart_devinfo {

    amhw_uart_t *p_hw_uart;       /**< \brief ָ��UART�Ĵ������ָ��     */

    uint8_t      inum;            /**< \brief �����жϺ�                 */

    /**
     * \brief �������ñ�־��AMHW_UART_C1_M_*���AMHW_UART_C1_PARITY_*��
     *                     ��AMHW_UART_BDH_SBNS_STOP_*�Ļ�ֵ
     *        (�磺AMHW_UART_C1_M_8BIT|AMHW_UART_C1_PARITY_NO
     *                                |AMHW_UART_BDH_SBNS_STOP_1)
     */
    uint8_t     cfg_flags;


    uint32_t     baud_rate;           /**< \brief ��ʼ��������               */

    /**
     * \brief ָ��ʹ�ܵ������ж�,AMHW_UART_INT_*��ֵ����AMHW_UART_INT_*���
     *        ��ֵ������(#AMHW_UART_INT_C2_TCIE),(#AMHW_UART_INT_C2_IRIE),
     *        (#AMHW_UART_INT_C2_ILIE)��(#AMHW_UART_INT_C2_TIE)���ĸ��жϡ�
     */
    uint32_t     other_int_enable;


} amdr_uart_devinfo_t;

/**
 * \brief �����豸�ṹ�嶨��
 */
typedef struct amdr_uart_dev {

    am_uart_serv_t  uart_serv;             /**< \brief ��׼UART����          */

    /** \brief ָ���û�ע���txchar_get���� */
    int (*pfn_txchar_get)(void *, char *);  
    
    /** \brief ָ���û�ע���rxchar_put���� */
    int (*pfn_rxchar_put)(void *, char);   

    /** \brief ָ���û�ע��Ĵ���ص�����   */    
    int (*pfn_err)(void *, int, void *, int); 

    void     *txget_arg;                   /**< \brief txchar_get��������    */
    void     *rxput_arg;                   /**< \brief rxchar_put��������    */
    void     *err_arg;                     /**< \brief ����ص������û�����  */
    
    uint8_t   channel_mode;                /**< \brief ����ģʽ �ж�/��ѯ    */
    uint32_t  baud_rate;                   /**< \brief ���ڲ�����            */
    uint16_t  options;                     /**< \brief Ӳ������ѡ��          */

    uint32_t  other_int_enable;            /**< \brief ָ��ʹ�ܵ������ж�    */

    const amdr_uart_devinfo_t *p_devinfo;  /**< \brief ָ���豸��Ϣ������ָ��*/

} amdr_uart_dev_t;

/**
 * \brief ��ʼ��UART������UART��׼����������
 *
 * \param[in] p_dev     : ָ�򴮿��豸��ָ��
 * \param[in] p_devinfo : ָ�򴮿��豸��Ϣ������ָ��
 *
 * \return UART��׼������������ֵΪNULLʱ������ʼ��ʧ��
 */
am_uart_handle_t amdr_uart_init(amdr_uart_dev_t           *p_dev,
                                 const amdr_uart_devinfo_t *p_devinfo);

/**
 * \brief ��ʹ��UARTʱ�����ʼ��UART���ͷ������Դ
 * \param[in] p_dev : ָ�򴮿��豸��ָ��
 */
void amdr_uart_deinit(amdr_uart_dev_t *p_dev);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMDR_UART_H */

/* end of file */
