/*******************************************************************************
*                                 Apollo
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2016 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief mpu6050 implementation
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-11-19  lgr, first implementation.
 * \endinternal
 */ 
 
#include "am_mpu6050.h"

/** \brief ����MPU6050������� */
am_mpu6050_handle_t  mpu6050_handle = NULL; 

/** \brief ����MPU6050�豸�ṹ��  */
am_mpu6050_dev_t g_mpu6050_dev; 

/** \brief MPU6050���豸ID  */ 
void am_mpu6050_id_read (am_mpu6050_handle_t  handle, uint8_t  *p_id)
{
    uint8_t buf = 0;
    
    am_mpu6050_dev_t *p_dev = (am_mpu6050_dev_t *)handle;
    
    if (p_dev == NULL || p_dev->handle == NULL) {
        return;
    }      
       
    am_i2c_read(&p_dev->i2c_device, 
                AM_MPU6050_WHO_AM_I, 
                &buf, 
                sizeof(buf));  
    *p_id = buf;     
}

/** \brief MPU6050�豸��λʹ��  */
void am_mpu6050_reset_enable (am_mpu6050_handle_t  handle, uint32_t  flag)
{
    uint8_t buf = 0;
    
    am_mpu6050_dev_t *p_dev = (am_mpu6050_dev_t *)handle;
    
    if (p_dev == NULL || p_dev->handle == NULL) {
        return;
    }      
    
    am_i2c_read(&p_dev->i2c_device, 
               AM_MPU6050_PWR_MGMT_1, 
               &buf, 
               sizeof(buf));    

    if (!(buf & flag)) { 
        buf = buf | flag;
        
        am_i2c_write(&p_dev->i2c_device,
                     AM_MPU6050_PWR_MGMT_1, 
                     &buf, 
                     sizeof(buf)); 
    }      
}

/** \brief  MPU6050�豸����ʹ��  */
void am_mpu6050_wake_enable (am_mpu6050_handle_t  handle, uint32_t  flag)
{
    uint8_t buf = 0;
    
    am_mpu6050_dev_t *p_dev = (am_mpu6050_dev_t *)handle;
    
    if (p_dev == NULL || p_dev->handle == NULL) {
        return;
    }      
    
    am_i2c_read(&p_dev->i2c_device,
               AM_MPU6050_PWR_MGMT_1, 
               &buf, 
               sizeof(buf));    

    if (buf & flag) { 
        buf = buf & ~flag;
        
        am_i2c_write(&p_dev->i2c_device,
                     AM_MPU6050_PWR_MGMT_1, 
                     &buf, 
                     sizeof(buf)); 
    }     
}    

/** \brief ����MPU6050��ʱ��Դ     */
void am_mpu6050_clk_select (am_mpu6050_handle_t  handle, uint32_t  flag)
{
    uint8_t buf = 0;
    
    am_mpu6050_dev_t *p_dev = (am_mpu6050_dev_t *)handle;
    
    if (p_dev == NULL || p_dev->handle == NULL) {
        return;
    }      
    
    am_i2c_read(&p_dev->i2c_device, 
               AM_MPU6050_PWR_MGMT_1, 
               &buf, 
               sizeof(buf));   
    
    buf = (buf & 0xf8) | flag;    
       
    am_i2c_write(&p_dev->i2c_device,
                 AM_MPU6050_PWR_MGMT_1, 
                 &buf, 
                 sizeof(buf));     
}

/** \brief ����MPU6050���ٶȷ�Χֵ */
void am_mpu6050_acc_range_set (am_mpu6050_handle_t  handle, uint32_t  flag)
{
    uint8_t buf = 0;
    
    am_mpu6050_dev_t *p_dev = (am_mpu6050_dev_t *)handle;
    
    if (p_dev == NULL || p_dev->handle == NULL) {
        return;
    }  
    
    am_i2c_read(&p_dev->i2c_device,
               AM_MPU6050_ACCEL_CONFIG, 
               &buf, 
               sizeof(buf));    

    buf = (buf & 0xe7) | flag;
      
    am_i2c_write(&p_dev->i2c_device,
                 AM_MPU6050_ACCEL_CONFIG, 
                 &buf, 
                 sizeof(buf)); 
    
}

/** \brief ����MPU6050�����ǽǶȷ�Χֵ */
void am_mpu6050_gro_range_set (am_mpu6050_handle_t  handle, uint32_t  flag)
{
    uint8_t buf = 0;
    
    am_mpu6050_dev_t *p_dev = (am_mpu6050_dev_t *)handle;
    
    if (p_dev == NULL || p_dev->handle == NULL) {
        return;
    }  
    
    am_i2c_read(&p_dev->i2c_device,
               AM_MPU6050_GYRO_CONFIG, 
               &buf, 
               sizeof(buf));    

    buf = (buf & 0xe7) | flag;
                 
    am_i2c_write(&p_dev->i2c_device, 
                 AM_MPU6050_GYRO_CONFIG, 
                 &buf, 
                 sizeof(buf));     
}

/** \brief ����MPU6050���������   */
void am_mpu6050_fs_set (am_mpu6050_handle_t  handle, uint32_t  flag)
{
    uint8_t buf = 0;
    
    am_mpu6050_dev_t *p_dev = (am_mpu6050_dev_t *)handle;
    
    if (p_dev == NULL || p_dev->handle == NULL) {
        return;
    }  
    
    am_i2c_read(&p_dev->i2c_device,
               AM_MPU6050_CONFIG_MPU,
               &buf, 
               sizeof(buf));    

    buf = (buf & 0xf8) | flag;    
        
    am_i2c_write(&p_dev->i2c_device, 
                 AM_MPU6050_CONFIG_MPU, 
                 &buf, 
                 sizeof(buf));      
}

/** \brief ����MPU6050�����ʣ���flagΪ0x13ʱ��������Ϊ50HZ   */
void am_mpu6050_sample_rate_set (am_mpu6050_handle_t  handle, uint32_t  flag)
{
    uint8_t buf = 0;
    
    am_mpu6050_dev_t *p_dev = (am_mpu6050_dev_t *)handle;
    
    if (p_dev == NULL || p_dev->handle == NULL) {
        return;
    }  
    
    buf = flag;     
        
    am_i2c_write(&p_dev->i2c_device,
                 AM_MPU6050_SMPLRT_DIV, 
                 &buf, 
                 sizeof(buf));          
}

/** \brief �ر�MPU6050��I2C��ģʽ����flagΪ0x00��fifo    */
void am_mpu6050_i2c_master_close (am_mpu6050_handle_t  handle, uint8_t  flags)
{    
    am_mpu6050_dev_t *p_dev = (am_mpu6050_dev_t *)handle;
    
    if (p_dev == NULL || p_dev->handle == NULL) {
        return;
    }  
    
    am_i2c_write(&p_dev->i2c_device,
                 AM_MPU6050_USER_CTRL, 
                 &flags, 
                 sizeof(flags));      
}

/** \brief ����MPU6050��FIFO��������������ʹ�����           */
void am_mpu6050_fifo_set (am_mpu6050_handle_t  handle, uint8_t  flag)
{
    am_mpu6050_dev_t *p_dev = (am_mpu6050_dev_t *)handle;
    
    if (p_dev == NULL || p_dev->handle == NULL) {
        return;
    }  
    
    am_i2c_write(&p_dev->i2c_device,
                 AM_MPU6050_FIFO_EN, 
                 &flag, 
                 sizeof(flag));          
}

/** \brief �����˶����ķ�ֵ     */
void am_mpu6050_motion_thr_set (am_mpu6050_handle_t  handle, uint8_t  flag)
{
    am_mpu6050_dev_t *p_dev = (am_mpu6050_dev_t *)handle;
    
    if (p_dev == NULL || p_dev->handle == NULL) {
        return;
    }
    
    am_i2c_write(&p_dev->i2c_device,
                 AM_MPU6050_MOTIONTHR_CONFIG, 
                 &flag, 
                 sizeof(flag));     
    
}

/** \brief ʹ��MPU6050���ж�           */
void am_mpu6050_int_enable (am_mpu6050_handle_t  handle, uint32_t  flags)
{
    uint8_t buf = 0;
    
    am_mpu6050_dev_t *p_dev = (am_mpu6050_dev_t *)handle;
    
    if (p_dev == NULL || p_dev->handle == NULL) {
        return;
    }  
    
    am_i2c_read(&p_dev->i2c_device,
               AM_MPU6050_INTSRC_ENABLE ,
               &buf, 
               sizeof(buf));    

    buf = buf | flags;    
        
    am_i2c_write(&p_dev->i2c_device, 
                 AM_MPU6050_INTSRC_ENABLE , 
                 &buf, 
                 sizeof(buf)); 
}

/** \brief ����MPU6050���ж�           */
void am_mpu6050_int_disable (am_mpu6050_handle_t  handle, uint32_t  flags)
{
    uint8_t buf = 0;
    
    am_mpu6050_dev_t *p_dev = (am_mpu6050_dev_t *)handle;
    
    if (p_dev == NULL || p_dev->handle == NULL) {
        return;
    }  
    
    am_i2c_read(&p_dev->i2c_device,
               AM_MPU6050_INTSRC_ENABLE ,
               &buf, 
               sizeof(buf));    

    buf = buf & (~flags);    
        
    am_i2c_write(&p_dev->i2c_device, 
                 AM_MPU6050_INTSRC_ENABLE , 
                 &buf, 
                 sizeof(buf));     
}

/** \brief ��ѯMPU6050���ж�״̬       */
uint8_t am_mpu6050_int_staus_get (am_mpu6050_handle_t  handle)
{
    uint8_t buf = 0;
    
    am_mpu6050_dev_t *p_dev = (am_mpu6050_dev_t *)handle;   
    
    if (p_dev == NULL || p_dev->handle == NULL) {
        return -AM_ERROR;
    }  
    
    am_i2c_read(&p_dev->i2c_device,
               AM_MPU6050_INT_STATUS ,
               &buf, 
               sizeof(buf));
    
    return buf;
}

/** \brief ����MPU6050���ж�����״̬    */
void am_mpu6050_int_pin_set (am_mpu6050_handle_t  handle, uint32_t  flags)
{
    uint8_t buf = 0;
    
    am_mpu6050_dev_t *p_dev = (am_mpu6050_dev_t *)handle;
    
    if (p_dev == NULL || p_dev->handle == NULL) {
        return;
    }  
    
    am_i2c_read(&p_dev->i2c_device,
                AM_MPU6050_PINFUC_ENABLE ,
                &buf, 
                sizeof(buf));    

    if (flags & AM_MPU6050_INT_LEVEL_SET) {
        buf |= AM_MPU6050_INT_LEVEL_SET;
    } else {
        buf &= ~AM_MPU6050_INT_LEVEL_SET;
    }

    if (flags & AM_MPU6050_INT_OPEN_SET) {
        buf |= AM_MPU6050_INT_OPEN_SET;
    } else {
        buf &= ~AM_MPU6050_INT_OPEN_SET;
    }
    
    if (flags & AM_MPU6050_LATCH_INT_SET ) {
        buf |= AM_MPU6050_LATCH_INT_SET ;
    } else {
        buf &= ~AM_MPU6050_LATCH_INT_SET ;
    } 

    if (flags & AM_MPU6050_INT_RD_CLEAR) {
        buf |= AM_MPU6050_INT_RD_CLEAR;
    } else {
        buf &= ~AM_MPU6050_INT_RD_CLEAR;
    }
    
    if (flags & AM_MPU6050_FSYNC_INT_LEVEL) {
        buf |= AM_MPU6050_FSYNC_INT_LEVEL;
    } else {
        buf &= ~AM_MPU6050_FSYNC_INT_LEVEL;
    }
    
    if (flags & AM_MPU6050_FSYNC_INT_EN) {
        buf |= AM_MPU6050_FSYNC_INT_EN ;
    } else {
        buf &= ~AM_MPU6050_FSYNC_INT_EN ;
    }    

    if (flags & AM_MPU6050_I2C_BYPASS_EN) {
        buf |= AM_MPU6050_I2C_BYPASS_EN ;
    } else {
        buf &= ~AM_MPU6050_I2C_BYPASS_EN;
    }    
           
    am_i2c_write(&p_dev->i2c_device, 
                 AM_MPU6050_PINFUC_ENABLE , 
                 &buf, 
                 sizeof(buf));     
}

/** \brief ������ÿ�����ֵ */ 
int16_t  am_mpu6050_sigle_data_read (am_mpu6050_handle_t  handle, uint8_t  sub_addr)
{
    uint8_t buf[2] = {0, 0};
    
    am_mpu6050_dev_t *p_dev = (am_mpu6050_dev_t *)handle;
    
    if (p_dev == NULL || p_dev->handle == NULL) {
        return -AM_ERROR;
    }  

    am_i2c_read(&p_dev->i2c_device,
                sub_addr, 
                buf, 
                sizeof(buf));   

    return (buf[0]<<8 | buf[1]);    

}    

/** \brief ����ѡ���ԵĶ�x,y,z������ٶ�����ٶȵ�ֵ */ 
void am_mpu6050_xyz_data_read (am_mpu6050_handle_t  handle, int16_t  *buffer, uint8_t  flag)
{
    uint8_t i = 0;  
    
    uint8_t buf[14] = {0, 0, 0, 0, 0, 0};
    
    am_mpu6050_dev_t *p_dev = (am_mpu6050_dev_t *)handle;
    
    if (p_dev == NULL || p_dev->handle == NULL) {
        return;
    }      
    
    if (flag == AM_MPU6050_DATA_ALL_SELECT) {    
        am_i2c_read(&p_dev->i2c_device,
                    AM_MPU6050_ACCEL_XOUT_H, 
                    buf, 
                    sizeof(buf));
        
       /* x,y,z 14λ���ݵļ��ٶ�ֵ */
       for (i = 0; i < 7; i++) {
            buffer[i] = (int16_t)(buf[2*i]<<8 | buf[2*i+1]);
       }
    }
    
    if (flag == AM_MPU6050_DATA_ACC_SELECT) {    
        am_i2c_read(&p_dev->i2c_device,
                    AM_MPU6050_ACCEL_XOUT_H, 
                    buf, 
                    6);
        
       /* x,y,z 14λ���ݵļ��ٶ�ֵ */
       for (i = 0; i < 3; i++) {
            buffer[i] = (int16_t)(buf[2*i]<<8 | buf[2*i+1]);
       }
    }   
    
    if (flag == AM_MPU6050_DATA_GRO_SELECT) {    
        am_i2c_read(&p_dev->i2c_device,
                    AM_MPU6050_GYRO_XOUT_H , 
                    buf, 
                    6);
        
       /* x,y,z 14λ���ݵļ��ٶ�ֵ */
       for (i = 0; i < 3; i++) {
            buffer[i] = (int16_t)(buf[2*i]<<8 | buf[2*i+1]);
       }
    } 
  
    if (flag == AM_MPU6050_DATA_TMP_SELECT) {    
        am_i2c_read(&p_dev->i2c_device,
                    AM_MPU6050_TEMP_OUT_H, 
                    buf, 
                    2);
        
       /* x,y,z 14λ���ݵļ��ٶ�ֵ */
       for (i = 0; i < 1; i++) {
            buffer[i] = (int16_t)(buf[2*i]<<8 | buf[2*i+1]);
       }
    } 
    
    /* ��ͨ�Ĳ���У�� */
    
#if 0    
    if (buffer[2] > 0) {
        buffer[2] = buffer[2] / 0.86;
    } else {
        buffer[2] = buffer[2] * 0.86;
    } 
#endif /* 0 */
                             
}

/** \brief MPU6050���ݴ��� */
void am_mpu6050_data_handle (am_mpu6050_handle_t  handle, float *buf, int16_t  *buffer)
{
    uint8_t i = 0;
    
    am_mpu6050_dev_t *p_dev = (am_mpu6050_dev_t *)handle;
    
    if (p_dev == NULL || p_dev->handle == NULL) {
        return;
    }  
    
    for (i = 0; i < 3; i++){                         /* ���ٶ�      */   
        buf[i] = (float)buffer[i] / 16384.0f * 9.8f; /* 2g������    */
    }
   
    buf[i] = (float)buffer[i] / 340.0f + 36.53f;     /* �¶�       */
    i++;
    
    for (; i < 7; i++) {                             /* ���ٶ�     */      
        buf[i]  =(float)buffer[i] / 16.4f;           /* 2000������ */
    }    
}

/** \brief ��ʼ��mpu6050 */
am_mpu6050_handle_t am_mpu6050_init (am_mpu6050_dev_t  *p_dev,
                                     const am_mpu6050_device_t  *p_devinfo,
                                     am_i2c_handle_t   handle)
{
    if (handle == NULL || p_devinfo == NULL) {
        return NULL;
    }
    
    p_dev->handle    = handle;
    
    p_dev->p_devinfo = p_devinfo;
    
    am_i2c_mkdev(&p_dev->i2c_device, 
                 p_dev->handle,        
                 p_dev->p_devinfo->dev_addr,
                 p_dev->p_devinfo->dev_flags);    
    
    /* ��ʼ��λ                        */
    am_mpu6050_reset_enable(p_dev, AM_MPU6050_RESET_ENALBE);
        
    /* ���ٶȷ�ΧΪ2g                  */
    am_mpu6050_acc_range_set(p_dev, AM_MPU6050_ACC_RANGE(0));
    
    /* �Ƕȷ�ΧΪ2000's                */
    am_mpu6050_gro_range_set(p_dev, AM_MPU6050_GRO_RANGE(3));
    
    /* �����������Ϊ1kHz, ������20Hz   */
    am_mpu6050_fs_set(p_dev, 0x04);
    
    /* ������Ϊ50Hz                    */
    am_mpu6050_sample_rate_set(p_dev, 0x13);
    
    /* �˶����ķ�ֵΪ0x00            */
    am_mpu6050_motion_thr_set(p_dev, 0x00);
    
    /* �����жϴ����ж����ŵĵ�ƽΪ��   */
    am_mpu6050_int_pin_set(p_dev, 0x80);
    
    /* �������е��ж�                  */
    am_mpu6050_int_disable(p_dev, 0x59);
    
    /* ʱ��Դѡ����������x���PLLΪ�ο� */
    am_mpu6050_clk_select (p_dev,AM_MPU6050_CLK_SECTLECT(1));
        
    /* ����ʹ��                       */
    am_mpu6050_wake_enable(p_dev, AM_MPU6050_WAKE_ENALBE);
    
    return p_dev;
}

/** \brief ���MPU6050��ʼ�� */
void am_mpu6050_deinit (am_mpu6050_dev_t  *p_dev)
{
    amdr_i2c_dev_t *p_i2c_dev   = (amdr_i2c_dev_t *)p_dev->handle->p_drv;
      
    if (p_dev == NULL || p_dev->handle == NULL) {
        return;
    }         
    
    amdr_i2c_deinit(p_i2c_dev);
    
    p_dev->handle    = NULL;
    
    p_dev->p_devinfo = NULL;
}


#ifdef MPU6050_DMP  

/** \brief  DMPͨ��I2C����������MPU6050  */                   
int am_mpu_read_len (uint8_t  addr,uint8_t  reg,uint8_t  len,uint8_t  *p_buf)
{
    int status = 0;
    
    am_mpu6050_dev_t *p_dev = (am_mpu6050_dev_t *) mpu6050_handle;
    
    if (p_dev == NULL || p_dev->handle == NULL) {
        return -AM_ERROR;
    }    
    
    (void) addr;
    
    if (len > 1) {        
       status = am_i2c_read(&p_dev->i2c_device, reg, p_buf, len); 
    } else {
        status = am_i2c_read(&p_dev->i2c_device, reg, p_buf, 2); 
    }
    
    return status;
}    

/** \brief  DMPͨ��I2C��������дMPU6050  */                   
int am_mpu_write_len (uint8_t  addr, uint8_t  reg, uint8_t  len, uint8_t  *p_buf)
{
    int status = 0;
    
    am_mpu6050_dev_t *p_dev = (am_mpu6050_dev_t *)mpu6050_handle;
    
    if (p_dev == NULL || p_dev->handle == NULL) {
        return -AM_ERROR;
    }  
     
    (void) addr;
    
    status = am_i2c_write(&p_dev->i2c_device, reg, p_buf, len); 
    
    return status;
}

#endif /* MPU6050_DMP */


/* end of file */
