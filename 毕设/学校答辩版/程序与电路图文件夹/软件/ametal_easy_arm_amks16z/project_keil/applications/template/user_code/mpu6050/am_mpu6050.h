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
 * \brief mpu6050��׼�ӿ�
 *
 * \internal
 * \par Modification History
 * - 1.00 15-11-19  lgr, first implementation.
 * \endinternal
 */
 
#ifndef __AM_MPU6050_H
#define __AM_MPU6050_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "am_common.h"   
#include "am_gpio.h"
#include "am_i2c.h"
#include "amdr_i2c.h" 
#include "amdr_uart.h"
#include "am_imu.h"
#include <math.h>    
#include <string.h>
#include <stdlib.h>
    
/**
 * @addtogroup am_if_mpu6050
 * @copydoc am_mpu6050.h
 * @{
 */   

/**
 * \name MPU6050�ӻ��豸��ַ���������л�����һλ�����0xD0��,��ʱAD0 = 0)
 * @{ 
 */      
#define AM_MPU6050_ADDRESS             0x68    /**< \brief MPU6050�ӻ��豸��ַ              */      
 
/** @} */  

/**
 * \name  MPU6050�豸���Ĵ�����ַ��Ϊ�ӻ��豸���ӵ�ַ
 * @{ 
 */
#define AM_MPU6050_SMPLRT_DIV          0x19    /**< \brief �����ǲ�����                      */
#define AM_MPU6050_CONFIG_MPU          0x1A    /**< \brief ��ͨ�˲�Ƶ��                      */
#define AM_MPU6050_GYRO_CONFIG         0x1B    /**< \brief �������Լ켰������Χ��             */
#define AM_MPU6050_ACCEL_CONFIG        0x1C    /**< \brief ���ټ��Լ졢������Χ����ͨ�˲�Ƶ�� */
#define AM_MPU6050_MOTIONTHR_CONFIG    0x1F    /**< \brief �˶���ֵⷧ�Ĵ���                 */
#define AM_MPU6050_FIFO_EN             0x23    /**< \brief FIFOʹ�ܼĴ���                     */
#define AM_MPU6050_I2C_MASTER          0x24    /**< \brief MPU6050��ΪI2C�����Ŀ���           */ 
#define AM_MPU6050_PINFUC_ENABLE       0x37    /**< \brief MPU6050��ΪI2C�����Ŀ���           */ 
#define AM_MPU6050_INTSRC_ENABLE       0x38    /**< \brief MPU6050��ΪI2C�����Ŀ���           */ 
#define AM_MPU6050_INT_STATUS          0x3A    /**< \brief MPU6050�ж�״̬�Ĵ���              */ 
#define AM_MPU6050_ACCEL_XOUT_H        0x3B    /**< \brief MPU6050X����ٶ�����������ֽ�     */
#define AM_MPU6050_ACCEL_XOUT_L        0x3C    /**< \brief MPU6050X����ٶ�����������ֽ�     */
#define AM_MPU6050_ACCEL_YOUT_H        0x3D    /**< \brief MPU6050Y����ٶ�����������ֽ�     */
#define AM_MPU6050_ACCEL_YOUT_L        0x3E    /**< \brief MPU6050Y����ٶ�����������ֽ�     */
#define AM_MPU6050_ACCEL_ZOUT_H        0x3F    /**< \brief MPU6050Z����ٶ�����������ֽ�     */
#define AM_MPU6050_ACCEL_ZOUT_L        0x40    /**< \brief MPU6050Z����ٶ�����������ֽ�     */
#define AM_MPU6050_TEMP_OUT_H          0x41    /**< \brief MPU6050�¶�����������ֽ�          */
#define AM_MPU6050_TEMP_OUT_L          0x42    /**< \brief MPU6050�¶�����������ֽ�          */
#define AM_MPU6050_GYRO_XOUT_H         0x43    /**< \brief MPU6050X������������������ֽ�     */
#define AM_MPU6050_GYRO_XOUT_L         0x44    /**< \brief MPU6050X������������������ֽ�     */
#define AM_MPU6050_GYRO_YOUT_H         0x45    /**< \brief MPU6050Y������������������ֽ�     */
#define AM_MPU6050_GYRO_YOUT_L         0x46    /**< \brief MPU6050Y������������������ֽ�     */
#define AM_MPU6050_GYRO_ZOUT_H         0x47    /**< \brief MPU6050Z������������������ֽ�     */
#define AM_MPU6050_GYRO_ZOUT_L         0x48    /**< \brief MPU6050Z������������������ֽ�     */
#define AM_MPU6050_USER_CTRL           0x6A    /**< \brief �û����ƼĴ���                     */   
#define AM_MPU6050_PWR_MGMT_1          0x6B    /**< \brief ��Դ����Ĵ���1                    */
#define AM_MPU6050_PWR_MGMT_2          0x6C    /**< \brief ��Դ����Ĵ���2                    */
#define AM_MPU6050_WHO_AM_I            0x75    /**< \brief ��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)     */

/** @} */

/**
 * \name MPU6050��λ�뻽�����
 * @{ 
 */
#define AM_MPU6050_RESET_ENALBE       (1UL << 7)  /**< \brief ʹ�ܸ�λ                 */
#define AM_MPU6050_WAKE_ENALBE        (1UL << 6)  /**< \brief ����ʹ��                 */

/** @} */

/**
 * \name MPU6050�ж�ʹ��
 * @{ 
 */
#define AM_MPU6050_MOT_EN_ENALBE      (1UL << 6)  /**< \brief ʹ���˶�����ж�          */
#define AM_MPU6050_FIFO_OVER_ENALBE   (1UL << 4)  /**< \brief ʹ��FIFO����ж�          */
#define AM_MPU6050_I2C_MASINT_ENALBE  (1UL << 3)  /**< \brief ʹ��ģ��I2C���������ж�   */
#define AM_MPU6050_DATA_READY_ENALBE  (1UL << 0)  /**< \brief ʹ������׼�����ж�        */

/** @} */

/**
 * \name MPU6050���Ź������ж�״̬���� 
 * @{ 
 */
/** \brief �ж����Ŵ���ʱ�ĵ�ƽ,Ϊ1������ʱΪ�͵�ƽ��0Ϊ�͵�ƽ  */
#define AM_MPU6050_INT_LEVEL_SET      (1UL << 7)

/** \brief �ж����ŵ�������ģʽ��Ϊ1����©ģʽ��0ʱ������ģʽ   */
#define AM_MPU6050_INT_OPEN_SET       (1UL << 6) 

/** \brief �ж����Ŵ�����ĵ�ƽ״̬ Ϊ0���жϵ�ƽ����һ��ʱ�䣬1�жϵ�ƽ����ֱ������жϱ�־      */
#define AM_MPU6050_LATCH_INT_SET      (1UL << 5)  

#define AM_MPU6050_INT_RD_CLEAR       (1UL << 4)  /**< \brief �趨ֻ��ͨ������жϱ�־����ж� */
#define AM_MPU6050_FSYNC_INT_LEVEL    (1UL << 3)  /**< \brief ʹ��FSYNC���ŵ�ƽ��              */
#define AM_MPU6050_FSYNC_INT_EN       (1UL << 2)  /**< \brief ʹ��FSYNC                        */
#define AM_MPU6050_I2C_BYPASS_EN      (1UL << 1)  /**< \brief ʹ��ģ��I2C����                  */

/** @} */

/**
 * \name MPU6050�ж�״̬
 * @{ 
 */
#define AM_MPU6050_MOT_INT_CHECK      (1UL << 6)  /**< \brief �˶�����жϱ�־λ            */
#define AM_MPU6050_FIFO_OV_INT_CHECK  (1UL << 4)  /**< \brief FIFO����жϱ�־              */
#define AM_MPU6050_I2C_MST_INT_CHECK  (1UL << 3)  /**< \brief ģ��I2C�����жϱ�־           */
#define AM_MPU6050_DATA_RDY_INT_CHECK (1UL << 0)  /**< \brief ����׼�����жϱ�־            */

/** @} */

/**
 * \name MPU6050��ֵ���Ͷ�ȡѡ��
 * @{ 
 */
#define AM_MPU6050_DATA_ACC_SELECT      0          /**< \brief �˶�����жϱ�־λ            */
#define AM_MPU6050_DATA_GRO_SELECT      1          /**< \brief FIFO����жϱ�־              */
#define AM_MPU6050_DATA_TMP_SELECT      2          /**< \brief ģ��I2C�����жϱ�־           */
#define AM_MPU6050_DATA_ALL_SELECT      3          /**< \brief ����׼�����жϱ�־            */

/** @} */

/**
 * \name MPU6050���ٶ���Ƕȷ�Χ
 * @{ 
 */
#define AM_MPU6050_ACC_RANGE(X)    (X << 3)  /**< \brief ���ٶȷ�Χ     */
#define AM_MPU6050_GRO_RANGE(X)    (X << 3)  /**< \brief �Ƕȷ�Χ       */

/** @} */

/**
 * \name MPU6050��ʱ��Դѡ��
 * @{ 
 */
#define AM_MPU6050_CLK_SECTLECT(X)    (X)   /**< \brief ʱ��Դѡ��      */

/** @} */

/**
 * \name  MPU6050���ݼ���
 * @{ 
 */
#define AM_MPU6050MAX_AVE       5          /**< \brief ��ֵ�˲�         */
#define AM_MPU6050PI          (3.1415926f) /**< \brief Բ����           */

/** @} */

/**
 * \brief MPU6050�豸�����Ϣ�ṹ��
 * \note  
 */
typedef struct am_mpu6050_device {  
    uint16_t  dev_addr;                    /**< \brief MPU6050�豸��ַ  */ 
    uint16_t  dev_flags;                   /**< \brief MPU6050�豸����  */
    
    /** \brief ����MPU6050�豸���ж����� */  
    uint8_t   interrupt_pin;     
              
} am_mpu6050_device_t;

/**
 * \brief MPU6050�豸�ṹ��
 */
typedef struct am_mpu6050_dev {
    
    /** \brief MPU6050ʹ��I2C��׼����������  */
    am_i2c_handle_t   handle;
    
    /**< \brief I2C�豸��Ϣ�ṹ��              */
    am_i2c_device_t     i2c_device;  
    
    /** \brief ָ��MPU6050�豸��Ϣ��ָ��       */
    const am_mpu6050_device_t *p_devinfo;
} am_mpu6050_dev_t;

/** \brief MPU6050�豸����������Ͷ��� */
typedef struct am_mpu6050_dev *am_mpu6050_handle_t;

/**
 * \brief MPU6050�豸��λʹ�� 
 * \param[in] handle : ָ��MPU6050�豸���������ָ��
 * \param[in] flag   : ��λ��־��ֵΪAM_MPU6050_RESET_ENALBE                 
 * \return    ��
 */
void am_mpu6050_reset_enable (am_mpu6050_handle_t  handle, uint32_t  flag);

/**
 * \brief MPU6050�豸��λ���� 
 * \param[in] handle : ָ��MPU6050�豸���������ָ��
 * \param[in] flag   : ����ʹ�ܣ�ֵΪAM_MPU6050_WAKE_ENALBE                 
 * \return    ��
 */
void am_mpu6050_wake_enable (am_mpu6050_handle_t  handle, uint32_t  flag);

/**    
 * \brief ����MPU6050��ʱ��Դ 
 * \param[in] handle : ָ��MPU6050�豸���������ָ��
 * \param[in] flag   : ʱ��Դѡ��ĺֵ꣬ΪAM_MPU6050_CLK_SECTLECT��
 *                     ���к�ηֱ��ȡ0 ~ 7���ֱ����ѡ��ͬ��ʱ��Դ��
 *                     ��1�ʹ�����X��Ϊ�ο��������ǵ�PLL               
 * \return    ��
 */
void am_mpu6050_clk_select (am_mpu6050_handle_t  handle, uint32_t  flag);

/**
 * \brief ����MPU6050���ٶȷ�Χֵ 
 * \param[in] handle : ָ��MPU6050�豸���������ָ��
 * \param[in] flag   : ���ٶȷ�Χѡ��ֵΪAM_MPU6050_ACC_RANGE������εĺ꣬
 *                     ���к�ηֱ��ȡ0��1��2��3���ֱ�ΧΪ2g, 4g, 8g,16g                 
 * \return    ��
 */
void am_mpu6050_acc_range_set (am_mpu6050_handle_t  handle, uint32_t  flag);

/**
 * \brief ����MPU6050�����ǽǶȷ�Χֵ 
 * \param[in] handle : ָ��MPU6050�豸���������ָ��
 * \param[in] flag   : �Ƕȷ�Χѡ��ֵΪAM_MPU6050_GROP_RANGE������εĺ꣬ 
 *                     ���к�ηֱ��ȡ0��1��2��3���ֱ�ΧΪ 
 *                      250 ��/s,  500 ��/s,  1000 ��/s, 2000 ��/s                 
 * \return    ��
 */
void am_mpu6050_gro_range_set (am_mpu6050_handle_t  handle, uint32_t  flag);

/**
 * \brief ����MPU6050��������� flag = 0x04 Ϊ fs=1024 ,��ʱ��ͨ�˲�Ϊ20HZ 
 * \param[in] handle : ָ��MPU6050�豸���������ָ��
 * \param[in] flag   : flag����ֵһ��ȡֵΪ0x04 �������ʱfs=1024 ,��ʱ��ͨ�˲�Ϊ20HZ 
 *                                
 * \return    ��
 */
void am_mpu6050_fs_set (am_mpu6050_handle_t  handle, uint32_t  flag);

/**
 * \brief ����MPU6050������ 
 * \param[in] handle : ָ��MPU6050�豸���������ָ��
 * \param[in] flag   : flag����ֵһ��ȡֵΪ0x13 ����ʱ���������Ϊ50Hz
 *                                
 * \return    ��
 */
void am_mpu6050_sample_rate_set (am_mpu6050_handle_t  handle, uint32_t  flag);

/**
 * \brief �����˶����ķ�ֵ 
 * \param[in] handle : ָ��MPU6050�豸���������ָ��
 * \param[in] flag   ����ֵ��λΪ1MSB/MG,          
 * \return    ��
 */
void am_mpu6050_motion_thr_set (am_mpu6050_handle_t  handle, uint8_t  flag);

/**
 * \brief ʹ��MPU6050���ж�  
 * \param[in] handle : ָ��MPU6050�豸���������ָ��
 * \param[in] flag   : AM_MPU6050_MOT_EN_ENALBE һ��ĺ꣬�������                 
 * \return    ��
 */
void am_mpu6050_int_enable (am_mpu6050_handle_t  handle, uint32_t  flags);

/**
 * \brief ����MPU6050���ж�   
 * \param[in] handle : ָ��MPU6050�豸���������ָ��
 * \param[in] flag   : AM_MPU6050_MOT_EN_ENALBEһ��ĺ꣬�������                
 * \return    ��
 */
void am_mpu6050_int_disable (am_mpu6050_handle_t  handle, uint32_t  flags);

/**
 * \brief ��ѯMPU6050���ж�״̬ 
 * \param[in] handle : ָ��MPU6050�豸���������ָ��               
 * \return    ���ظ��ж�״̬��һ����AM_MPU6050_MOT_INT_CHECKһ��ĺ�����
 */
uint8_t am_mpu6050_int_staus_get (am_mpu6050_handle_t  handle);

/**
 * \brief ����MPU6050�����Ź��ܼ��ж�����״̬  
 * \param[in] handle : ָ��MPU6050�豸���������ָ��
 * \param[in] flags  : AM_MPU6050_INT_LEVEL_SETһ��ĺ꼰����һ��ĺ�ֵȡ����ϣ�
 *                     ��Ϻ����ֵΪ0x80                 
 * \return    ��
 */
void am_mpu6050_int_pin_set (am_mpu6050_handle_t  handle, uint32_t  flags);

/**
 * \brief ѡ���x,y,z������ٶ���Ƕȵ�ֵ   
 * \param[in] handle : ָ��MPU6050�豸���������ָ��
 * \param[in] buffer : ���ٶ��봫��������ֵ   
 * \param[in] flag   : ѡ�񷵻صļ��ٶȻ���ٶȵ���ֵ��ΪAM_MPU6050_DATA_ACC_SELECTһ��ĺ�ֵ 
 * \return    ��
 */
void am_mpu6050_xyz_data_read (am_mpu6050_handle_t  handle, int16_t  *buffer, uint8_t  flag);

/**   
 * \brief ������ÿ�����ֵ 
 * \param[in] handle   : ָ��MPU6050�豸���������ָ��
 * \param[in] sub_addr : AM_MPU6050_ACCEL_*_H �ĺ� ��# AM_MPU6050_ACCEL_XOUT_H   
 *
 * \return    ����ÿ�����ԭʼ��ֵ
 */
int16_t am_mpu6050_sigle_data_read (am_mpu6050_handle_t  handle, uint8_t  sub_addr);

/**   
 * \brief MPU6050���ݴ��� ���Ѽ��ٶȱ���������ٶ�G�ı�ʾ�������������Ǳ�ɽ��ٶȱ�ʾ
 * \param[in] handle   : ָ��MPU6050�豸���������ָ��
 * \param[in] buffer   : ָ��MPU6050ԭʼ���ݵĻ�����ָ��
 * \param[in] buf      : ָ��ת�����ݻ�����ָ��
 *
 * \return    ����ÿ�����ԭʼ��ֵ
 */
void am_mpu6050_data_handle (am_mpu6050_handle_t  handle, float  *buf, int16_t  *buffer);

/**
 * \brief ��ʼ��mpu6050
 *
 * \param[in] p_dev     : mpu6050�豸�ṹ��ָ��
 * \param[in] handle    : I2C��׼����������
 * \param[in] p_devinfo : mpu6050�豸��Ϣ�ṹ��
 *
 * \return   mpu6050�Ĳ�����������ֵΪNULL��������ʼ��ʧ��
 */
am_mpu6050_handle_t am_mpu6050_init (am_mpu6050_dev_t  *p_dev,
                                     const am_mpu6050_device_t  *p_devinfo,
                                     am_i2c_handle_t    handle);
                                                                                                  
/**
 * \brief ���mpu6050��ʼ��
 *
 * \param[in] p_dev  : mpu6050�豸�ṹ��ָ��
 *
 * \return    None
 */
void am_mpu6050_deinit (am_mpu6050_dev_t  *p_dev);
                                                                         
/**
 * \brief MPU6050���豸ID
 *
 * \param[in] handle    : ָ��MPU6050�豸���������ָ��
 * \param[in] p_id      : �û�����ID�Ļ�����ָ��
 *
 * \return   ��
 */                                                                    
void am_mpu6050_id_read (am_mpu6050_handle_t  handle, uint8_t  *p_id);

/**
 * \brief ��MPU6050��I2C��ģʽ
 *
 * \param[in] handle    : ָ��MPU6050�豸���������ָ��
 * \param[in] flag      : Ĭ��ֵΪ0
 *
 * \return   ��
 */ 
void am_mpu6050_i2c_master_open (am_mpu6050_handle_t  handle, uint8_t  flag);

/**
 * \brief �ر�MPU6050��I2C��ģʽ
 *
 * \param[in] handle    : ָ��MPU6050�豸���������ָ��
 * \param[in] flag      : Ĭ��ֵΪ0
 *
 * \return   ��
 */ 
void am_mpu6050_i2c_master_close (am_mpu6050_handle_t  handle, uint8_t  flag);

/**
 * \brief ����MPU6050��FIFO
 *
 * \param[in] handle    : ָ��MPU6050�豸���������ָ��
 * \param[in] flag      : Ĭ��ֵΪ0
 *
 * \return   ��
 */ 
void am_mpu6050_fifo_set (am_mpu6050_handle_t  handle, uint8_t  flags);                                     
                                                                        
                                     
#ifdef MPU6050_DMP                                       
/**
 * \brief DMP��ͨ��I2C����������
 *
 * \param[in] addr     : ������ַ 
 * \param[in] reg      : �Ĵ�����ַ
 * \param[in] len      : д�볤��
 * \param[in] p_buf    : ������
 *
 * \note �˺���������DMP���MPU6050����I2C��
 * \retval  AM_OK      : ���������;
 * \return  ����       : ���ش������
 */                   
int am_mpu_read_len (uint8_t  addr, uint8_t  reg, uint8_t  len, uint8_t  *p_buf);

/**
 * \brief DMP��ͨ��I2C��������д
 *
 * \param[in] addr     : ������ַ 
 * \param[in] reg      : �Ĵ�����ַ
 * \param[in] len      : д�볤��
 * \param[in] buf      : ������
 *
 * \note �˺���������DMP���MPU6050����I2Cд
 * \retval  AM_OK      : ��������� 
 * \return  ����       ���������
 */                   
int am_mpu_write_len (uint8_t  addr, uint8_t  reg, uint8_t  len, uint8_t  *p_buf);
  
extern am_i2c_handle_t      i2c_handle     ;  /**< \brief ����ȫ�ֱ���������DMP����̬���� */ 
extern am_mpu6050_handle_t  mpu6050_handle ;  /**< \brief ��am_mpu6050.c�ļ��ж���       */  
extern am_mpu6050_dev_t     g_mpu6050_dev  ;  /**< \brief ��am_mpu6050.c�ļ��ж���       */ 

#endif  /* MPU6050_DMP */                                   
 
/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_MPU6050_H */

/* end of file */     
