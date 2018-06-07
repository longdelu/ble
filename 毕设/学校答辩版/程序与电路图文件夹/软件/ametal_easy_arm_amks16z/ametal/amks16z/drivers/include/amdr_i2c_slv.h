/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
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
 * \brief AMKS16 I2C���ӻ�������������I2C�ӻ���׼�ӿ�
 *
 * \internal
 * \par Modification History
 * - 1.00 15-12-18  mem, first implementation.
 * \endinternal
 */

#ifndef __AMDR_I2C_SLV_H
#define __AMDR_I2C_SLV_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "am_int.h"
#include "am_i2c_slv.h"
#include "amhw_i2c.h"
    
/**
 * \addtogroup amdr_if_i2c
 * \copydoc amdr_i2c.h
 * @{
 */

/**
 * \brief I2C �豸��Ϣ�����ṹ��
 */
typedef struct amdr_i2c_slv_devinfo {
    amhw_i2c_t       *p_hw_i2c;       /**< \brief ָ��I2C�Ĵ������ָ��  */
    uint32_t          max_bus_speed;  /**< \brief ֧�ֵ������������     */
    uint32_t         *p_slv_list;     /**< \brief ָ��ӻ���ַ�б��ָ�� */
    uint8_t           dev_num;        /**< \brief ʹ�õ��豸����         */
    uint16_t          inum;           /**< \brief I2C ���������жϺ�     */
} amdr_i2c_slv_devinfo_t;

/**
 * \brief I2C �豸�ṹ��
 */
typedef struct amdr_i2c_slv_dev {
   
    /** \brief ��׼I2C����           */
    am_i2c_slv_serv_t             i2c_serv;

    /** \brief ָ��I2C�豸��Ϣ��ָ�� */
    const amdr_i2c_slv_devinfo_t *p_devinfo;
    
    /** \brief I2C����ʱ��           */
    uint32_t                      clk_in;

    /** \brief I2Cʱ��Ƶ��           */
    uint32_t                      clk_rate;

    /** \brief ָ��I2C����ṹ���ָ��,ͬһʱ��ֻ�ܴ���һ������ */
    am_i2c_slv_transfer_t        *p_trans;

    /** \brief �������ݽ���/���ͼ���   */
    uint32_t                      data_ptr;

    /** \brief ָ��ǰ���ڽ������ݴ���Ĵӻ��豸���� */
    uint8_t                       cur_indx;

    /** \brief ״̬                   */
    uint8_t                       state;
} amdr_i2c_slv_dev_t;


/**
 * \brief I2C���ӻ�����ʼ��
 *
 * \param[in] p_dev     : ָ��I2C�ӻ��豸�ṹ���ָ��
 * \param[in] p_devinfo : ָ��I2C�ӻ��豸��Ϣ�ṹ���ָ��
 *
 * \return  I2C��׼���������� 
 */
am_i2c_slv_handle_t amdr_i2c_slv_init(amdr_i2c_slv_dev_t           *p_dev,
                                      const amdr_i2c_slv_devinfo_t *p_devinfo);

/**
 * \brief I2C���ӻ������ʼ��
 * \param[in] p_dev : ָ��I2C�ӻ��豸�ṹ���ָ��
 * \return ��
 */
void amdr_i2c_slv_deinit(amdr_i2c_slv_dev_t *p_dev);
    
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __AMDR_I2C_SLV_H */

/* end of file */
