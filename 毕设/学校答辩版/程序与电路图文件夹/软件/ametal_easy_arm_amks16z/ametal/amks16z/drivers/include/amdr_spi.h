/*******************************************************************************
*                                 AMeatl
*                       ----------------------------
*                       innovating embedded platform
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
 * \brief SPI����������SPI��׼�ӿ�
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-20  liqing, first implementation.
 * \endinternal
 */

#ifndef __AMDR_SPI_H
#define __AMDR_SPI_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "am_types.h"    
#include "am_spi.h"
#include "amhw_spi.h"
#include "am_int.h"

/**
 * \addtogroup amdr_if_spi
 * \copydoc amdr_spi.h
 * @{
 */
 
/**
 * \name SPI ���ñ�ʶ
 * @{
 */

/** \brief ֹͣģʽSPI�������� */
#define AMDR_SPI_CFG_WIAT_EN        (AM_BIT(28) | AM_SBF(0,3))

/** \brief ֹͣģʽSPI������ */
#define AMDR_SPI_CFG_WIAT_DIS       (AM_BIT(28) | AM_SBF(1,3))

/** @} */

/**
 * \brief SPI �豸��Ϣ�ṹ��
 */
typedef struct amdr_spi_devinfo {
    amhw_spi_t       *p_hw_spi;    /**< \brief ָ��SPI�Ĵ������ָ��          */
    uint32_t          mode_flag;   /**< \brief ��ʼ��ģʽ                     */
    uint32_t          ini_speed;   /**< \brief ��ʼ������                     */
    uint32_t          flags;       /**< \brief SPI ���ñ�ʶ                   */
    uint16_t          inum;        /**< \brief SPI �жϱ��                   */
} amdr_spi_devinfo_t;

/**
 * \brief SPI �豸
 */
typedef struct amdr_spi_dev {
    am_spi_serv_t             spi_serve;     /**< \brief SPI��׼����������  */
    const amdr_spi_devinfo_t *p_devinfo;     /**< \brief ָ��SPI�豸��Ϣָ��  */

    /* ���ݴ��� */
    am_spi_transfer_t        *p_trans;       /**< \brief ָ��SPI����ṹ��ָ��*/
    uint32_t                  send_ptr;      /**< \brief ��������λ��         */
    uint32_t                  recv_ptr;      /**< \brief ��������λ��         */

    /* �ӻ��豸���� */
    am_spi_device_t          *p_f_dev;       /**< \brief ��ǰ�豸             */
    uint32_t                  cur_speed;     /**< \brief ��ǰSPI�ٶ�          */
    uint32_t                  speed_diff;    /**< \brief ��ǰ�ٶ�����         */
    uint8_t                   cur_bpw;       /**< \brief ��ǰ�豸����λ��     */

    /* ȫ�ֱ��� */
    bool_t                    cs_state;      /**< \brief ����֮ǰƬѡ״̬     */
    int                       state;         /**< \brief ������״̬��״̬     */
} amdr_spi_dev_t;

/**
 * \brief SPI ��ʼ��
 *
 * \param[in] p_dev     : ָ��SPI�豸�ṹ���ָ��
 * \param[in] p_devinfo : ָ��SPI�豸��Ϣ�ṹ���ָ��
 *
 * \return  SPI��׼���������� 
 */
am_spi_handle_t amdr_spi_init(amdr_spi_dev_t           *p_dev,
                               const amdr_spi_devinfo_t *p_devinfo);

/**
 * \brief ���SPI��ʼ��
 * \param[in] p_dev : ָ��SPI�豸�ṹ���ָ��
 * \return ��
 */
void amdr_spi_deinit(amdr_spi_dev_t *p_dev);

/** 
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMDR_SPI_H */

/*end of file */
