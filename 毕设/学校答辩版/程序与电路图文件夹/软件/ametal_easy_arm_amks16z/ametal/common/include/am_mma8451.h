/*******************************************************************************
*                                  Apollo
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
 * \brief mma8451��׼�ӿ�
 *
 * \internal
 * \par Modification History
 * - 1.00 15-11-19  lgr, first implementation.
 * \endinternal
 */
 
#ifndef __AM_MMA8451_H
#define __AM_MMA8451_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"   
#include "am_gpio.h"
#include "am_i2c.h"
#include "amdr_i2c.h" 
#include <math.h>    
#include <string.h>
#include <stdlib.h>
    
/**
 * @addtogroup am_if_mma8451
 * @copydoc am_mma8451.h
 * @{
 */    

/**
 * \name MMA8451�ӻ��豸��ַ���������л�����һλ�����0x38��,��ʱSA0 = 0)
 * @{ 
 */      
#define AM_MMA8451_ADDRESS       0x1c    /**< \brief MMA8451�ӻ��豸��ַ */      
 
/** @} */       

/**
 * \name MMA8451�豸���Ĵ�����ַ��Ϊ�ӻ��豸���ӵ�ַ
 * @{ 
 */  

#define AM_MMA8451_F_STATUS      0x00
#define AM_MMA8451_OUT_X_MSB     0x01
#define AM_MMA8451_OUT_X_LSB     0x02
#define AM_MMA8451_OUT_Y_MSB     0x03
#define AM_MMA8451_OUT_Y_LSB     0x04
#define AM_MMA8451_OUT_Z_MSB     0x05
#define AM_MMA8451_OUT_Z_LSB     0x06
#define AM_MMA8451_F_SETUP       0x09
#define AM_MMA8451_TRIG_CFG      0x0A
#define AM_MMA8451_SYSMOD        0x0B
#define AM_MMA8451_INT_SOURCE    0x0C
#define AM_MMA8451_WHO_AM_I      0x0D
#define AM_MMA8451_XYZ_DATA_CFG  0x0E
#define AM_MMA8451_HPF_CUTOFF    0x0F
#define AM_MMA8451_PL_STATUS     0x10
#define AM_MMA8451_PL_CFG        0x11
#define AM_MMA8451_PL_COUNT      0x12
#define AM_MMA8451_PL_BF_ZCOMP   0x13
#define AM_MMA8451_PL_THS_REG    0x14
#define AM_MMA8451_FF_MT_CFG     0x15
#define AM_MMA8451_FF_MT_SRC     0x16
#define AM_MMA8451_FF_MT_THS     0x17
#define AM_MMA8451_FF_MT_COUNT   0x18
#define AM_MMA8451_TRANSIENT_CFG 0x1D
#define AM_MMA8451_TRANSIENT_SCR 0x1E
#define AM_MMA8451_TRANSIENT_THS 0x1F
#define AM_MMA8451_TRANSIENT_CNT 0x20
#define AM_MMA8451_PULSE_CFG     0x21
#define AM_MMA8451_PULSE_SRC     0x22
#define AM_MMA8451_PULSE_THSX    0x23
#define AM_MMA8451_PULSE_THSY    0x24
#define AM_MMA8451_PULSE_THSZ    0x25
#define AM_MMA8451_PULSE_TMLT    0x26
#define AM_MMA8451_PULSE_LTCY    0x27
#define AM_MMA8451_PULSE_WIND    0x28
#define AM_MMA8451_ASLP_COUNT    0x29
#define AM_MMA8451_CTRL_REG1     0x2A
#define AM_MMA8451_CTRL_REG2     0x2B
#define AM_MMA8451_CTRL_REG3     0x2C
#define AM_MMA8451_CTRL_REG4     0x2D
#define AM_MMA8451_CTRL_REG5     0x2E
#define AM_MMA8451_OFF_X         0x2F
#define AM_MMA8451_OFF_Y         0x30
#define AM_MMA8451_OFF_Z         0x31

/** @} */ 

/**
 * \name MMA8451��2g������С��λ������
 * @{ 
 */ 

#define AM_MMA8451_FRAC_2D1           5000
#define AM_MMA8451_FRAC_2D2           2500
#define AM_MMA8451_FRAC_2D3           1250
#define AM_MMA8451_FRAC_2D4           625
#define AM_MMA8451_FRAC_2D5           313
#define AM_MMA8451_FRAC_2D6           156
#define AM_MMA8451_FRAC_2D7           78
#define AM_MMA8451_FRAC_2D8           39
#define AM_MMA8451_FRAC_2D9           20
#define AM_MMA8451_FRAC_2D10          10
#define AM_MMA8451_FRAC_2D11           5
#define AM_MMA8451_FRAC_2D12           2

/** @} */ 

/**
 * \name MMA8451ѡ���������ʱ�Ƿ�ʹ�ø�ͨ�˲�
 * @{ 
 */
#define AM_MMA8451_RESET_ENALBE     (1UL << 6)  /**< \brief ����λ  */
#define AM_MMA8451_RESET_DISALBE    (1UL << 6)  /**< \brief ��ֹ��λ  */

/** @} */
/**
 * \name MMA8451ѡ����ģʽ�򼤻�ģʽ
 * @{ 
 */
#define AM_MMA8451_STANDBY_MODE_CFG     0x01  /**< \brief ����ģʽ  */
#define AM_MMA8451_ACTIVE_MODE_CFG      0x01  /**< \brief ����ģʽ  */

/** @} */

/**
 * \name MMA8451ѡ��ͬ�����̷�Χ
 * @{ 
 */
#define AM_MMA8451_G_RANGES(X)  ((X) & 0xff)          /**< \brief X��ȡֵ����Ϊ0~2 */  

/** @} */


/**
 * \name MA8451����(����)ģʽ�����������������
 * @{ 
 */
#define AM_MMA8451_DATA_RATA_SET(X)  (X << 3)         /**< \brief X��ȡֵ����Ϊ0~7 */  

/** @} */
 
/**
 * \name MMA8451˯��ģʽ�����������������
 * @{ 
 */

#define AM_MMA8451_SLEEP_DATA_RATA_SET(X)  (X << 6)  /**< \brief X��ȡֵ����Ϊ0~3 */ 

/** @} */

/**
 * \name MMA8451ѡ��ͬ�Ĳ���ģʽ
 * @{ 
 */

#define AM_MMA8451_OVERSAMPLING_MODE_SET(X)  (X)    /**< \brief X��ȡֵ����Ϊ0~3 */ 

/** @} */

/** \brief MMA8451��ͨ�˲���ֹƵ��ѡ�� */ 
#define AM_MMA8451_HPF_FRQ_CUFOFF(X)  ((X) & 0xff)  /**< \brief X��ȡֵ����Ϊ0~3 */  

/** @} */

/**
 * \name MMA8451ѡ���������ʱ�Ƿ�ʹ�ø�ͨ�˲�
 * @{ 
 */
#define AM_MMA8451_DATA_HPF_ENALBE     (1UL << 4)  /**< \brief ����ʹ�ø��˲�     */
#define AM_MMA8451_DATA_HPF_DISALBE    (1UL << 4)  /**< \brief ���ݲ�ʹ�ø�ͨ�˲� */

/** @} */

/**
 * \name MMA8451ѡ�����ݶ�д��λ����ʽ
 * @{ 
 */
#define  AM_MMA8451_DATA_FAST_READ_CFG   (1UL << 1)  /**< \BRIEF ����ʹ��8λ     */
#define  AM_MMA8451_DATA_NORMAL_READ_CFG (1UL << 1)  /**< \BRIEF ����ʹ��14λ    */

/** @} */

/**
 * \name MMA8451�ж����ã�����ʹ��������,�����뼤������
 * @{ 
 */
#define AM_MMA8451_INT_ASLP_CFG        (1UL << 7)  /**< \brief ˯���뻽���ж�  */
#define AM_MMA8451_INT_FIFO_CFG        (1UL << 6)  /**< \brief FIFO�ж�        */
#define AM_MMA8451_INT_TRANS_CFG       (1UL << 5)  /**< \brief ˲̬����ж�    */
#define AM_MMA8451_INT_LNDPRT_CFG      (1UL << 4)  /**< \brief �������ж�    */
#define AM_MMA8451_INT_PULSE_CFG       (1UL << 3)  /**< \brief �������ж�    */
#define AM_MMA8451_INT_FFMT_CFG        (1UL << 2)  /**< \brief �˶�����ж�    */
#define AM_MMA8451_INT_DRDY_CFG        (1UL << 0)  /**< \brief ����׼���ж�    */

#define AM_MMA8451_INT_POL_LOW_CFG     (0UL << 1)  /**< \brief �жϵ͵�ƽ����  */
#define AM_MMA8451_INT_POL_HIG_CFG     (1UL << 1)  /**< \brief �жϸߵ�ƽ����  */
#define AM_MMA8451_INT_PP_CFG          (0UL << 0)  /**< \brief �ж���������ģʽ*/
#define AM_MMA8451_INT_OD_CFG          (1UL << 0)  /**< \brief �ж����ſ�©ģʽ*/

/** @} */

/**
 * \name MMA8451�ж�״̬��־λ
 * @{ 
 */
#define AM_MMA8451_INT_ASLP_FLAG       (1UL << 7)  /**< \brief ˯�߻����жϱ�־ */
#define AM_MMA8451_INT_FIFO_FLAG       (1UL << 6)  /**< \brief FIFO�жϱ�־     */
#define AM_MMA8451_INT_TRANS_FLAG      (1UL << 5)  /**< \brief ˲̬����жϱ�־ */
#define AM_MMA8451_INT_LNDPRT_FLAG     (1UL << 4)  /**< \brief �������жϱ�־ */
#define AM_MMA8451_INT_PULSE_FLAG      (1UL << 3)  /**< \brief �������жϱ�־ */
#define AM_MMA8451_INT_FFMT_FLAG       (1UL << 2)  /**< \brief �˶�����жϱ�־ */
#define AM_MMA8451_INT_DRDY_FLAG       (1UL << 0)  /**< \brief ����׼���жϱ�־ */

/** @} */

/**
 * \name MMA8451������ٶ�ֵ�Ƿ��и��� 
 * @{ 
 */
#define AM_MMA8451_XYZ_DATA_CHECK      (1UL << 4)  /**< \brief ���ݸ��±�־     */

/** @} */

/**
 * \name MMA8451�͹������ã������Զ�˯��ʹ�ܵ�
 * @{ 
 */
#define AM_MMA8451_AUTO_SLEEP          (1UL << 2)  /**< \brief �Զ�˯��ʹ�ܱ�־λ */
  
#define AM_MMA8451_GATA_CFG            (1UL << 7) /**< \brief FIFO ��GATA��־  */
#define AM_MMA8451_WAKE_TRANS_CFG      (1UL << 6) /**< \brief ˲̬��⻽�ѱ�־ */
#define AM_MMA8451_WAKE_LNDPRT_CFG     (1UL << 5) /**< \brief �����⻽�ѱ�־ */
#define AM_MMA8451_WAKE_PULSE_CFG      (1UL << 4) /**< \brief �����⻽�ѱ�־ */
#define AM_MMA8451_WAKE_FFMT_CFG       (1UL << 3) /**< \brief �˶���⻽�ѱ�־ */

#define AM_MMA8451_SLEEP_MODE_FLAG     (0x02)     /**< \brief ˯��ģʽ         */
#define AM_MMA8451_WAKE_MODE_FLAG      (0x01)     /**< \brief ����ģʽ         */

/** @} */

/**
 * \name MMA8451�˶��������
 * @{ 
 */
#define AM_MMA8451_MOTION_CFG          (1UL << 6)  /**< \brief �˶�����־λ */

/**
 * \name �˶�����־ʹ�ܱ��浽�˶����״̬��־�Ĵ�������λ�궨��
 * @{ 
 */
#define AM_MMA8451_MOTION_ELE_CFG      (1UL << 7) 
#define AM_MMA8451_MOTION_ZEFE_CFG     (1UL << 5) /**< \brief Z������ʹ�ܱ�־ */
#define AM_MMA8451_MOTION_YEFE_CFG     (1UL << 4) /**< \brief Y������ʹ�ܱ�־ */
#define AM_MMA8451_MOTION_XEFE_CFG     (1UL << 3) /**< \brief X������ʹ�ܱ�־ */

/** @} */

/**
 * \name �˶�����¼�״̬��־
 * @{ 
 */   
#define AM_MMA8451_MOTION_EA_FLAG      (1UL << 7) /**< \brief ������һ���¼����� */
#define AM_MMA8451_MOTION_ZHE_FLAG     (1UL << 5) /**< \brief Z�������־ */
#define AM_MMA8451_MOTION_ZHP_FLAG     (1UL << 4) /**< \brief Z�����Ա�־ */
#define AM_MMA8451_MOTION_YHE_FLAG     (1UL << 3) /**< \brief Y�������־ */
#define AM_MMA8451_MOTION_YHP_FLAG     (1UL << 2) /**< \brief Y�����Ա�־ */
#define AM_MMA8451_MOTION_XHE_FLAG     (1UL << 1) /**< \brief X�������־ */
#define AM_MMA8451_MOTION_XHP_FLAG     (1UL << 0) /**< \brief X�����Ա�־ */

/** @} */

/**
 * \name MMA8451FIFO ����
 * @{ 
 */ 

#define AM_MMA8451_FIFO_LOW_MOTION_CFG      0      /**< \brief ʹ�õ͹��������µ�FIFO */
#define AM_MMA8451_FIFO_DAT_COLLECT_CFG     1      /**< \brief �ж�ģʽ�µ�FIFO      */

/** @} */


/**
 * \brief MMA8451�豸�����Ϣ�ṹ��
 * \note  ֻ�п�����Ӧ�ı���ʹ�ܿ����󣬲����趨��ֵ�Ż�������
 */
typedef struct am_mma8451_device {     
    uint16_t  dev_addr;            /**< \brief MMA8451�豸��ַ               */ 
    uint16_t  dev_flags;           /**< \brief MMA8451�豸����               */
    
    /** \brief ����MMA8451�豸���ж�����1 */  
    uint8_t   interrupt_pin1;     

    /** \brief ����MMA8451�豸���ж�����2 */  
    uint8_t   interrupt_pin2;      
                                             
} am_mma8451_device_t;

/**
 * \brief MMA8451�豸�ṹ��
 */
typedef struct am_mma8451_dev {    
    /** \brief MMA8451ʹ��I2C��׼����������  */
    am_i2c_handle_t   handle;
    
    /**< \brief I2C�豸��Ϣ�ṹ��              */
    am_i2c_device_t     i2c_device;               
    
    /** \brief ָ��MMA8451�豸��Ϣ��ָ��       */
    const am_mma8451_device_t *p_devinfo;
} am_mma8451_dev_t;

/** \brief MMA8451�豸����������Ͷ��� */
typedef struct am_mma8451_dev *am_mma8451_handle_t;

/**
 * \brief MMA8451�豸��λʹ�� 
 * \param[in] handle : ָ��MMA8451�豸���������ָ��
 * \param[in] flag   : ��λ��־��ֵΪ AM_MMA8451_RESET_ENALBE                 
 * \return    ��
 */
void am_mma8451_reset_enable(am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief MMA8451�豸��λ���� 
 * \param[in] handle : ָ��MMA8451�豸���������ָ��
 * \param[in] flag   : ��λ��־��ֵΪ AM_MMA8451_RESET_DISALBE                  
 * \return    ��
 */
void am_mma8451_reset_disable(am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief  MMA8451ѡ����ģʽ 
 * \param[in] handle : ָ��MMA8451�豸���������ָ��
 * \param[in] flag   : ֵΪ AM_MMA8451_STANDBY_MODE_CFG                  
 * \return    ��
 */
void am_mma8451_standby_mode_select (am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief  MMA8451ѡ�񼤻�ģʽ 
 * \param[in] handle : ָ��MMA8451�豸���������ָ��
 * \param[in] flag   : ֵΪ M_MMA8451_ACTIVE_MODE_CFG                
 * \return    ��
 */
void am_mma8451_active_mode_select (am_mma8451_handle_t handle, uint32_t flag);



/**
 * \brief \brief  MMA8451���豸ID
 *
 * \param[in] handle    : ָ��MPU6050�豸���������ָ��
 * \param[in] p_id      : �û�����ID�Ļ�����ָ��
 *
 * \return   ��
 */  
void am_mma8451_id_read (am_mma8451_handle_t handle, uint8_t *p_id);


/**
 * \brief MMA8451���̷�Χ����
 *
 * \param[in] handle    : ָ��MPU6050�豸���������ָ��
 * \param[in] value     : Ĭ��ֵΪ0x00
 *
 * \return   ��
 */
void am_mma8451_g_range_set (am_mma8451_handle_t handle, uint32_t value);


/**
 * \brief MMA8451���������������
 *
 * \param[in] handle    : ָ��MPU6050�豸���������ָ��
 * \param[in] value     : Ĭ��ֵΪ0x00
 *
 * \return   ��
 */
void am_mma8451_data_rate_out (am_mma8451_handle_t handle, uint32_t value);


/**
 * \brief MMA8451��˯��ģʽ�²���ģʽ����
 *
 * \param[in] handle    : ָ��MPU6050�豸���������ָ��
 * \param[in] value     : Ĭ��ֵΪ0x00
 *
 * \return   ��
 */
void am_mma8451_oversampling_mode_set (am_mma8451_handle_t handle, uint32_t value);


/**
 * \brief MMA8451��ͨ�˲��Ľ�ֹƵ��
 *
 * \param[in] handle    : ָ��MPU6050�豸���������ָ��
 * \param[in] value     : Ĭ��ֵΪ0x00
 *
 * \return   ��
 */
void am_mma8451_hpf_fra_cutoff (am_mma8451_handle_t handle, uint32_t value);


/**
 * \brief  �����������ʱʹ�ø�ͨ�˲� 
 *
 * \param[in] handle    : ָ��MPU6050�豸���������ָ��
 * \param[in] flag      : Ĭ��ֵΪ0x00
 *
 * \return   ��
 */
void am_mma8451_data_hpf_enable (am_mma8451_handle_t handle, uint32_t flag);


/**
 * \brief �����������ʱ��ʹ�ø�ͨ�˲� 
 *
 * \param[in] handle    : ָ��MPU6050�豸���������ָ��
 * \param[in] flag      : Ĭ��ֵΪ0x00
 *
 * \return   ��
 */
void am_mma8451_data_hpf_disable (am_mma8451_handle_t handle, uint32_t flag);


/**
 * \brief ���ÿ��ٶ���8λ���� 
 *
 * \param[in] handle    : ָ��MPU6050�豸���������ָ��
 * \param[in] flag      : Ĭ��ֵΪ0x00
 *
 * \return   ��
 */
void am_mma8451_data_fast_read (am_mma8451_handle_t handle, uint32_t flag);
    

/**
 * \brief   ������������14λ����
 *
 * \param[in] handle    : ָ��MPU6050�豸���������ָ��
 * \param[in] flag      : Ĭ��ֵΪ0x00
 *
 * \return   ��
 */
void am_mma8451_data_normal_read (am_mma8451_handle_t handle, uint32_t flag);


/**
 * \brief 8λ��14λ��ʽ���� 
 * \param[in] handle : ָ��MMA8451�豸���������ָ��
 * \param[in] flag   : ���ݸ�ʽ(8λ��14λ) ֵΪ 0 �� 1; 0������Ϊ14λ��1������Ϊ8λ                    
 * \return    ��
 */
void am_mma8451_data_read_way_select(am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief ��x,y,z������ٶȵ�ֵ,ע���ʱû��ʹ��FIFO
 * \param[in] handle : ָ��MMA8451�豸���������ָ��                
 * \param[in] buffer : ָ�򱣴���ٶȻ�������ָ��
 * \return    ��
 */
void am_mma8451_xyz_data_read (am_mma8451_handle_t handle, int16_t *buffer);

/**
 * \brief FIFO��x,y,z������ٶȵ�ֵ
 * \param[in] handle : ָ��MMA8451�豸���������ָ��                
 * \param[in] buffer : ָ�򱣴���ٶȻ�������ָ��
 * \return    ��
 */
void am_mma8451_fifo_data_read (am_mma8451_handle_t handle, int16_t *buffer);

/**
 * \brief ��ѯ��x,y,z������ٶȵ�ֵ
 * \param[in] handle : ָ��MMA8451�豸���������ָ��                 
 * \param[in] buffer : ָ�򱣴���ٶȻ�������ָ��
 * \param[in] flag   : ���ݸ��±�־��ֵΪ AM_MMA8451_XYZ_DATA_CHECK    
 * \return    ��
 */
void am_mma8451_xyz_data_poll_read (am_mma8451_handle_t handle, 
                                    int16_t *buffer, 
                                    uint32_t flag);
 
/**
 * \brief �жϼ�������� 
 * \param[in] handle : ָ��MMA8451�豸���������ָ��
 * \param[in] flag   : �жϼ���Եĺֵ꣬Ϊ AM_MMA8451_INT_POL_LOW_CFG 
 *                     �� AM_MMA8451_INT_POL_HIG_CFG                  
 * \return    ��
 */
void am_mma8451_intterupt_pol_set (am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief �ж�����ģʽ����
 * \param[in] handle : ָ��MMA8451�豸���������ָ��
 * \param[in] flag   : �ж�����ģʽ�ĺֵ꣬Ϊ AM_MMA8451_INT_OD_CFG �� AM_MMA8451_INT_PP_CFG                       
 * \return    ��
 */
void am_mma8451_int_pin_mod_set (am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief ʹ�ܼ��ٶȼ���Ӧ�ж� 
 * \param[in] handle : ָ��MMA8451�豸���������ָ��
 * \param[in] flag   : �жϱ�־��ֵΪ AM_MMA8451_INT_*_CFG ��AM_MMA8451_INT_DRDY_CFG                       
 * \return    ��
 */
void am_mma8451_intterupt_enable (am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief ���ܼ��ٶȼ���Ӧ�ж� 
 * \param[in] handle : ָ��MMA8451�豸���������ָ��
 * \param[in] flag   : �жϱ�־��ֵΪ AM_MMA8451_INT_*_CFG �� AM_MMA8451_INT_DRDY_CFG                    
 * \return    ��
 */
void am_mma8451_intterupt_disable (am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief �������ӵ��ж�����  
 * \param[in] handle : ָ��MMA8451�豸���������ָ��
 * \param[in] flag   : �жϱ�־��ֵΪ AM_MMA8451_INT_*_CFG �� AM_MMA8451_INT_DRDY_CFG    
 * \param[in] pin    : �ж����� ֵΪ 0 �� 1; 0���ж�����0��1���ж�����1                      
 * \param[in] buffer : ָ�򱣴���ٶȻ�������ָ��
 * \return    ��
 */
void am_mma8451_int_pin_rout (am_mma8451_handle_t handle, uint32_t flag, uint8_t pin);

/**
 * \brief �ж�״̬���أ�����ֵ�����ж��ж��Ƿ���
 * \param[in] handle : ָ��MMA8451�豸���������ָ��                    
 * \return    �����жϱ�־�Ĵ�����ֵ�����������ж�
 */
uint8_t am_mma8451_int_status_get (am_mma8451_handle_t handle);

/**
 * \brief �����ж϶�x,y,z������ٶȵ�ֵ
 * \param[in] handle : ָ��MMA8451�豸���������ָ��                 
 * \return    ��
 */
void am_mma8451_xyz_data_intterupt_cfg (am_mma8451_handle_t handle);

/**
 * \brief �Զ�˯��ʹ�ܻ����
 * \param[in] handle : ָ��MMA8451�豸���������ָ��
 * \param[in] flag   : �Զ�˯�߱�־λ��ֵΪ                      
 * \param[in] val    : ֵΪ0��1; 1: ʹ�ܣ�0������
 * \note               �豸Ҫ�ȵ��� am_mma8451_standby_mode_select�������뱸��ģʽ
 * \return    ��
 */
void am_mma8451_auto_sleep_cfg (am_mma8451_handle_t handle, uint32_t flag, uint8_t var);

/**
 * \brief ˯��ģʽ�������������
 * \param[in] handle : ָ��MMA8451�豸���������ָ��
 * \param[in] flag   : ֵΪ AM_MMA8451_SLEEP_DATA_RATA_SET(X) ����Ĳ���                
 * \note               �豸Ҫ�ȵ��� am_mma8451_standby_mode_select�������뱸��ģʽ
 * \return    ��
 */
void am_mma8451_dat_rat_sleep_mod_set (am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief MMA8451˯��ģʽ�²���ģʽ����
 * \param[in] handle : ָ��MMA8451�豸���������ָ��
 * \param[in] flag   : ֵΪ AM_MMA8451_OVERSAMPLING_MODE_SET ����Ĳ���                
 * \note               �豸Ҫ�ȵ��� am_mma8451_standby_mode_select�������뱸��ģʽ
 * \return    ��
 */
void am_mma8451_sleep_mod_over_mode_set (am_mma8451_handle_t handle, uint32_t value);

/**
 * \brief ����GATEΪ1���ڻ��������֮ǰ�����ݲ�������FIFO,��Ҫ�ֶ���������� 
 * \param[in] handle : ָ��MMA8451�豸���������ָ��
 * \param[in] flag   :  GATE��־�ֵ꣬ΪAM_MMA8451_GATA_CFG                
 * \note �豸Ҫ�ȵ��� am_mma8451_standby_mode_select�������뱸��ģʽ
 * \return    ��
 */
void am_mma8451_gate_set (am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief ����GATEΪ0��˯��/����ģʽ�任��ʱ���Զ�����˻����� 
 * \param[in] handle : ָ��MMA8451�豸���������ָ��
 * \param[in] flag   : GATE��־�ֵ꣬ΪAM_MMA8451_GATA_CFG                
 * \note �豸Ҫ�ȵ��� am_mma8451_standby_mode_select�������뱸��ģʽ
 * \return    ��
 */
void am_mma8451_gate_clr (am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief �豸�����¼�ʹ��
 * \param[in] handle : ָ��MMA8451�豸���������ָ��
 * \param[in] flag   : ֵΪAM_MMA8451_WAKE_*_CFG ,#AM_MMA8451_WAKE_TRANS_CFG                 
 * \note �豸Ҫ�ȵ��� am_mma8451_standby_mode_select�������뱸��ģʽ
 * \return    ��
 */
void am_mma8451_wake_from_sleep_enable (am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief �豸�����¼����� 
 * \param[in] handle : ָ��MMA8451�豸���������ָ��
 * \param[in] flag   : ֵΪAM_MMA8451_WAKE_*_CFG ,#AM_MMA8451_WAKE_TRANS_CFG     
 * 
 * \note �豸Ҫ�ȵ��� am_mma8451_standby_mode_select�������뱸��ģʽ
 *
 * \return    ��
 */
void am_mma8451_wake_from_sleep_disable (am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief MMA8451˯��ʱ������
 *
 * \param[in] handle : ָ��MMA8451�豸���������ָ��
 * \param[in] value  : ���㹫ʽΪ˯��ʱ�� / ASLP_COUNT Step 
 * 
 * \note �豸Ҫ�ȵ��� am_mma8451_standby_mode_select�������뱸��ģʽ
 *
 * \return    ��
 */
void am_mma8451_sleep_time_set (am_mma8451_handle_t handle, uint32_t value);

/**
 * \brief ˯��/���ѣ��͹���ģʽ����
 * \param[in] handle : ָ��MMA8451�豸���������ָ��
 * \return    ��
 */
void am_mma8451_low_power_cfg (am_mma8451_handle_t handle);

/**
 * \brief �˶��������
 * \param[in] handle : ָ��MMA8451�豸���������ָ��
 * \return    ��
 */
void am_mma8451_motion_detect_cfg (am_mma8451_handle_t handle);

/**
 * \brief �˶�����¼�״̬Դ�Ĵ�������
 * \param[in] handle : ָ��MMA8451�豸���������ָ��                    
 * \return    �����˶����Ĵ�����ֵ(���ǴӼĴ��������ֵ��
 */
uint8_t am_mma8451_motion_event_src_get (am_mma8451_handle_t handle);

/**
 * \brief �˶�����жϸ�������
 * \param[in] handle : ָ��MMA8451�豸���������ָ��                    
 * \return    ��
 */
void am_mma8451_int_motion_assist (am_mma8451_handle_t handle);

/**
 * \brief ˯��/����ģʽ��״̬����
 * \param[in] handle : ָ��MMA8451�豸���������ָ��                    
 * \return    ����ϵͳģʽ�Ĵ�����ֵ
 */
uint8_t am_mma8451_system_mode_get (am_mma8451_handle_t handle);

/**
 * \brief ˯��/����ģʽ�жϸ������� 
 * \param[in] handle : ָ��MMA8451�豸���������ָ��  
 * \param[in] buffer : ָ�򱣴���ٶȻ�������ָ�� 
 * \return    �����жϱ�־�Ĵ�����ֵ
 */
void am_mma8451_system_mode_assist (am_mma8451_handle_t handle, int16_t *buffer);
 
/**
 * \brief FIFO�жϸ�������
 * \param[in] handle : ָ��MMA8451�豸���������ָ��  
 * \param[in] buffer : ָ�򱣴���ٶȻ�������ָ�� 
 * \return    �����жϱ�־�Ĵ�����ֵ
 */
void am_mma8451_fifo_mode_assist (am_mma8451_handle_t handle, int16_t *buffer);

/**
 * \brief ʹ��FIFO���� 
 * \param[in] handle : ָ��MMA8451�豸���������ָ��   
 * \param[in] flag   : ֵΪAM_MMA8451_WAKE_*_CFG ,#AM_MMA8451_WAKE_TRANS_CFG  
 * \note  ʹ��FIFO����ʱ������Ϊ8λ��ʽ
 * \return    ��
 */
void am_mma8451_fifo_cfg (am_mma8451_handle_t handle, uint32_t flag);

/**
 * \brief MMA8451�豸�ж�����
 *
 * \param[in] handle    : ָ��MMA8451�豸���������ָ��   
 * \param[in] pfn_isr   : ע����жϻص�����
 * \param[in] p_arg     : ע����жϻص������Ĳ��� 
 *
 * \note  MMA8451���ж����ų�����Ϊ��©״̬����ҪоƬ�ڲ������������������
 *
 * \return    ����ϵͳģʽ�Ĵ�����ֵ
 */
void am_mma8451_int_cfg (am_mma8451_handle_t handle, 
                         am_pfnvoid_t pfn_isr1, 
                         am_pfnvoid_t pfn_isr2, 
                         void *p_arg);

/**
 * \brief ���ݴ�����ת��Ϊ�ַ���
 *
 * \param[in] handle    : ָ��MMA8451�豸���������ָ��   
 * \param[in] buffer    : ��������
 * \param[in] p_txbuf   : �ַ�������
 *
 * \note  MMA8451���ж����ų�����Ϊ��©״̬����ҪоƬ�ڲ������������������
 *
 * \return    ����ϵͳģʽ�Ĵ�����ֵ
 */
void am_mma8451_data_change_to_char (am_mma8451_handle_t handle, 
                                     uint8_t *p_txbuf, 
                                     const int16_t *buffer); 

/**
 * \brief ���ݴ��ַ���ת������
 *
 * \param[in] handle    : ָ��MMA8451�豸���������ָ��   
 * \param[in] buffer    : ��������
 * \param[in] p_txbuf   : �ַ�������
 *
 * \note  MMA8451���ж����ų�����Ϊ��©״̬����ҪоƬ�ڲ������������������
 *
 * \return    ����ϵͳģʽ�Ĵ�����ֵ
 */
void am_mma8451_data_change_to_int (am_mma8451_handle_t handle, 
                                    const uint8_t *p_txbuf, 
                                    int16_t *buffer); 

/**
 * \brief MMA8451�豸�жϴ����� 
 * \param[in] handle : ָ��MMA8451�豸���������ָ��  
 * \param[in] buffer : ָ��ɼ����ݻ�������ָ��   
 * \return    ��
 */
void am_mma8451_intterupt_handle (am_mma8451_handle_t handle, int16_t *buffer);

/**
 * \brief ���ݴ���ת�����������ٶȵı�ʾ����
 * \param[in] handle : ָ��MMA8451�豸���������ָ��  
 * \param[in] buf    : ָ���������ٶȱ�ʾ���ݵĻ�����ָ��
 * \param[in] buffer : ָ��ɼ����ݻ�������ָ��   
 * \return    ��
 */
void am_mma8451_data_handle(am_mma8451_handle_t handle, float *buf, int16_t *buffer);

/** 
 * \brief ���ݴ���ת�����������ٶȵı�ʾ����
 * \param[in] handle : ָ��MMA8451�豸���������ָ��  
 * \param[in] buf    : ָ��ǶȵĻ�����ָ��
 * \param[in] buffer : ָ���������ٶȵ�ָ��   
 * \return    ��
 */
void am_mma8451_acc_to_angle(am_mma8451_handle_t handle, int16_t * buf, float *buffer);

/**
 * \brief ��ʼ��mma8451
 *
 * \param[in] p_dev     : mma8451�豸�ṹ��ָ��
 * \param[in] handle    : I2C��׼����������
 * \param[in] p_devinfo : �豸��Ϣ�ṹ��
 *
 * \return   mma8451�Ĳ�����������ֵΪNULL��������ʼ��ʧ��
 */
am_mma8451_handle_t am_mma8451_init (am_mma8451_dev_t *p_dev,
                                     const am_mma8451_device_t *p_devinfo,
                                     am_i2c_handle_t   handle);
                                                                                                  
/**
 * \brief ���mma8451��ʼ��
 *
 * \param[in] p_dev  : mma8451�豸�ṹ��ָ��
 *
 * \return    ��
 */
void am_mma8451_deinit (am_mma8451_dev_t *p_dev);


/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_MMA8451_H */

/* end of file */



