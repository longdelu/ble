/*******************************************************************************
*                                 Apollo
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
 * \brief mma8451 implementation
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-11-19  lgr, first implementation.
 * \endinternal
 */ 
 
#include "am_mma8451.h"

/* MMA8451���豸ID  */ 
void am_mma8451_id_read (am_mma8451_handle_t handle, uint8_t *p_id)
{
    uint8_t buf = 0;
    
    uint8_t tmp = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }      
         
    
    tmp = am_i2c_read(&p_dev->i2c_device,
                      AM_MMA8451_WHO_AM_I, 
                      &buf, 
                      sizeof(buf));  
    tmp = am_i2c_read(&p_dev->i2c_device,
                     AM_MMA8451_WHO_AM_I, 
                     &buf, 
                     sizeof(buf));  
    
    if (tmp == 0) {
        AM_DBG_INFO("OK\r\n");
        
    }    
    *p_id = buf;    
}

/** \brief MMA8451�豸��λʹ�� */
void am_mma8451_reset_enable(am_mma8451_handle_t handle, uint32_t flag)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }      
    
    am_i2c_read(&p_dev->i2c_device,
               AM_MMA8451_CTRL_REG2, 
               &buf, 
               sizeof(buf));    

    if (!(buf & flag)) { 
        buf = buf  | flag;
        
        am_i2c_write(&p_dev->i2c_device,
                     AM_MMA8451_CTRL_REG2, 
                     &buf, 
                     sizeof(buf)); 
    }   
    
}

/** \brief MMA8451�豸��λ���� */
void am_mma8451_reset_disable(am_mma8451_handle_t handle, uint32_t flag)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }      
       
    am_i2c_read(&p_dev->i2c_device, 
               AM_MMA8451_CTRL_REG2, 
               &buf, 
               sizeof(buf));    

    if (buf & flag) { 
        buf = buf & (~ flag);
        
        am_i2c_write(&p_dev->i2c_device,
                     AM_MMA8451_CTRL_REG2, 
                     &buf, 
                     sizeof(buf)); 
    }   
}
 
/* MMA8451ѡ����ģʽ */ 
void am_mma8451_standby_mode_select (am_mma8451_handle_t handle, uint32_t flag)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }      
       
    am_i2c_read(&p_dev->i2c_device, 
               AM_MMA8451_CTRL_REG1, 
               &buf, 
               sizeof(buf));    

    if (buf & flag) { 
        buf = buf & (~ flag);
        
        am_i2c_write(&p_dev->i2c_device, 
                     AM_MMA8451_CTRL_REG1, 
                     &buf, 
                     sizeof(buf)); 
    }     

}

/* MMA8451ѡ�񼤻�ģʽ */ 
void am_mma8451_active_mode_select (am_mma8451_handle_t handle, uint32_t flag)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }      
    
    am_i2c_read(&p_dev->i2c_device, 
               AM_MMA8451_CTRL_REG1, 
               &buf, 
               sizeof(buf));    

    if (!(buf & flag)) { 
        buf = buf  | flag;
        
        am_i2c_write(&p_dev->i2c_device,
                     AM_MMA8451_CTRL_REG1, 
                     &buf, 
                     sizeof(buf)); 
    }     

}

/** \brief MMA8451���̷�Χ���� */
void am_mma8451_g_range_set (am_mma8451_handle_t handle, uint32_t value)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }  
    
    am_mma8451_standby_mode_select(p_dev, AM_MMA8451_STANDBY_MODE_CFG);
    
    am_i2c_read(&p_dev->i2c_device,
                AM_MMA8451_XYZ_DATA_CFG, 
                &buf, 
                sizeof(buf)); 
  
    buf = (buf & 0xfc) | value;
        
    am_i2c_write(&p_dev->i2c_device,
                 AM_MMA8451_XYZ_DATA_CFG, 
                 &buf, 
                 sizeof(buf)); 
    
    am_mma8451_active_mode_select(p_dev, AM_MMA8451_ACTIVE_MODE_CFG);
}

/** \brief MMA8451��������������� */
void am_mma8451_data_rate_out (am_mma8451_handle_t handle, uint32_t value)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }  
    
    am_mma8451_standby_mode_select(p_dev, AM_MMA8451_STANDBY_MODE_CFG);
    
    am_i2c_read(&p_dev->i2c_device,
                AM_MMA8451_CTRL_REG1, 
                &buf, 
                sizeof(buf)); 
  
    buf = (buf & 0xC7) | value;
        
    am_i2c_write(&p_dev->i2c_device,
                 AM_MMA8451_CTRL_REG1, 
                 &buf, 
                 sizeof(buf)); 
    
    am_mma8451_active_mode_select(p_dev, AM_MMA8451_ACTIVE_MODE_CFG);
}

/** \brief MMA8451��˯��ģʽ�²���Ƶ������ */
void am_mma8451_oversampling_mode_set (am_mma8451_handle_t handle, uint32_t value)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }  
    
    am_mma8451_standby_mode_select(p_dev, AM_MMA8451_STANDBY_MODE_CFG);
    
    am_i2c_read(&p_dev->i2c_device,
                AM_MMA8451_CTRL_REG2, 
                &buf, 
                sizeof(buf)); 
  
    buf = (buf & 0xfc) | value;
        
    am_i2c_write(&p_dev->i2c_device,
                 AM_MMA8451_CTRL_REG2, 
                 &buf, 
                 sizeof(buf)); 
    
    am_mma8451_active_mode_select(p_dev, AM_MMA8451_ACTIVE_MODE_CFG);
}

/** \brief MMA8451��ͨ�˲��Ľ�ֹƵ�� */
void am_mma8451_hpf_fra_cutoff (am_mma8451_handle_t handle, uint32_t value)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }  
    
    am_mma8451_standby_mode_select(p_dev, AM_MMA8451_STANDBY_MODE_CFG);
    
    am_i2c_read(&p_dev->i2c_device,
                AM_MMA8451_HPF_CUTOFF, 
                &buf, 
                sizeof(buf)); 
  
    buf = (buf & 0xfc) | value;
        
    am_i2c_write(&p_dev->i2c_device,
                 AM_MMA8451_HPF_CUTOFF, 
                 &buf, 
                 sizeof(buf)); 
    
    am_mma8451_active_mode_select(p_dev, AM_MMA8451_ACTIVE_MODE_CFG);
}

/* �����������ʱʹ�ø�ͨ�˲� */ 
void am_mma8451_data_hpf_enable (am_mma8451_handle_t handle, uint32_t flag)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }      
    
    am_mma8451_standby_mode_select(p_dev, AM_MMA8451_STANDBY_MODE_CFG);
    
    am_i2c_read(&p_dev->i2c_device,
               AM_MMA8451_XYZ_DATA_CFG, 
               &buf, 
               sizeof(buf));    

    if (!(buf & flag)) { 
        buf = buf  | flag;
        
        am_i2c_write(&p_dev->i2c_device, 
                     AM_MMA8451_XYZ_DATA_CFG, 
                     &buf, 
                     sizeof(buf)); 
    }  
    
    am_mma8451_active_mode_select(p_dev, AM_MMA8451_ACTIVE_MODE_CFG);
}

/* �����������ʱ��ʹ�ø�ͨ�˲� */ 
void am_mma8451_data_hpf_disable (am_mma8451_handle_t handle, uint32_t flag)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }      
    
    am_mma8451_standby_mode_select(p_dev, AM_MMA8451_STANDBY_MODE_CFG);
    
    am_i2c_read(&p_dev->i2c_device,
               AM_MMA8451_XYZ_DATA_CFG, 
               &buf, 
               sizeof(buf));    

    if (buf & flag) { 
        buf = buf & (~flag);
        
        am_i2c_write(&p_dev->i2c_device,
                     AM_MMA8451_XYZ_DATA_CFG, 
                     &buf, 
                     sizeof(buf)); 
    }     
    am_mma8451_active_mode_select(p_dev, AM_MMA8451_ACTIVE_MODE_CFG);
}

/* ���ÿ��ٶ���8λ���� */ 
void am_mma8451_data_fast_read (am_mma8451_handle_t handle, uint32_t flag)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }      
    
    am_mma8451_standby_mode_select(p_dev, AM_MMA8451_STANDBY_MODE_CFG);    
    
    am_i2c_read(&p_dev->i2c_device,
                AM_MMA8451_CTRL_REG1, 
                &buf, 
                sizeof(buf));    

    if (!(buf & flag)) { 
        buf = buf | flag;
        
        am_i2c_write(&p_dev->i2c_device,
                     AM_MMA8451_CTRL_REG1, 
                     &buf, 
                     sizeof(buf)); 
    }     
    am_mma8451_active_mode_select(p_dev, AM_MMA8451_ACTIVE_MODE_CFG);
}

/* ������������14λ���� */ 
void am_mma8451_data_normal_read (am_mma8451_handle_t handle, uint32_t flag)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }      
    
    am_mma8451_standby_mode_select(p_dev, AM_MMA8451_STANDBY_MODE_CFG);   
    
    am_i2c_read(&p_dev->i2c_device,
                AM_MMA8451_CTRL_REG1, 
                &buf, 
                sizeof(buf));    

    if (buf & flag) { 
        buf = buf & (~flag);
        
        am_i2c_write(&p_dev->i2c_device,
                     AM_MMA8451_CTRL_REG1, 
                     &buf, 
                     sizeof(buf)); 
    }     
    am_mma8451_active_mode_select(p_dev, AM_MMA8451_ACTIVE_MODE_CFG);
}

/*  8λ��14λ������ɷ�ʽѡ��  */ 
void am_mma8451_data_read_way_select(am_mma8451_handle_t handle, uint32_t flag)
{
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }  
    if (flag == 0) {
        am_mma8451_data_normal_read(p_dev, AM_MMA8451_DATA_NORMAL_READ_CFG);
    } else {
        am_mma8451_data_fast_read(p_dev, AM_MMA8451_DATA_FAST_READ_CFG);
    } 
}
    
/* ��x,y,z������ٶȵ�ֵ */ 
void am_mma8451_xyz_data_read (am_mma8451_handle_t handle, int16_t *buffer)
{
    /* ѭ���±�   */
    uint8_t i = 0; 
    
    /* ����ʽ��־ */
    uint8_t read_flag = 0;
    
    /* ���ݻ����� */
    uint8_t buf[6] = {0, 0, 0, 0, 0, 0};
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }      
   
    am_i2c_read(&p_dev->i2c_device,
                AM_MMA8451_CTRL_REG1, 
                &read_flag, 
                sizeof(read_flag));    

    if (!(read_flag & 0x02)) {
        
       am_i2c_read(&p_dev->i2c_device,
                   AM_MMA8451_OUT_X_MSB, 
                   buf, 
                   6);
       /* x,y,z 14λ���ݵļ��ٶ�ֵ */
       for (i = 0; i < 3; i++) {
           buffer[i] = ((int16_t)(buf[2*i]<<8 | buf[2*i+1])) >> 2;
       }
    } else {
        am_i2c_read(&p_dev->i2c_device,
                   AM_MMA8451_OUT_X_MSB, 
                   buf, 
                   3);
        /* x,y,z 8λ���ݼ��ٶ�ֵ */
        for (i = 0; i < 3; i++) {
            buffer[i] = ((int16_t)(buf[i]<<8 | 0x00)) >> 8;
        }           
   }  
    
}

/* FIFO��x,y,z������ٶȵ�ֵ */ 
void am_mma8451_fifo_data_read (am_mma8451_handle_t handle, int16_t *buffer)
{
    uint8_t i = 0, j = 0; 
    
    uint8_t read_flag = 0;
    
    uint32_t tmp = 0;
    
    uint8_t buf[192] = {0}; 
    uint8_t buf_tmp[6] = {0};
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }      
   
    am_i2c_read(&p_dev->i2c_device,
                AM_MMA8451_CTRL_REG1, 
                &read_flag, 
                sizeof(read_flag));    

    if (!(read_flag & 0x02)) {
        
       for (i = 0; i < 192; i++) {
        
           am_i2c_read(&p_dev->i2c_device,
                       AM_MMA8451_OUT_X_MSB, 
                       &buf[i], 
                       1);
       }
       /* x,y,z 14λ���ݵļ��ٶ����ֵ */
       for (j = 0; j < 6; j++) {    
           for (i = 0; i < 32; i++) {
               tmp += buf[6*i + j];
           }
           buf_tmp[j] = tmp / i;
           tmp = 0;
       }
       
      /* x,y,z 14λ���ݵļ��ٶ�ֵ */
       for (i = 0; i < 3; i++) {
           buffer[i] = ((int16_t)(buf_tmp[2*i]<<8 | buf_tmp[2*i+1])) >> 2;
       }
       
    } else {
        am_i2c_read(&p_dev->i2c_device,
                   AM_MMA8451_OUT_X_MSB, 
                   buf, 
                   3);
        
       /* x,y,z 14λ���ݵļ��ٶ����ֵ */
       for (j = 0; j < 3; j++) {    
           for (i = 0; i < 32; i++) {
               tmp += buf[3*i + j];
           }
           buf_tmp[j] = tmp / i;
           tmp = 0;
       }        
        /* x,y,z 8λ���ݼ��ٶ�ֵ */
        for (i = 0; i < 3; i++) {
            buffer[i] = ((int16_t)(buf_tmp[i]<<8 | 0x00)) >> 8;
        }           
   }  
    
}


/* ��ѯ��x,y,z������ٶȵ�ֵ */ 
void am_mma8451_xyz_data_poll_read (am_mma8451_handle_t handle, int16_t *buffer, uint32_t flag)
{
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    uint8_t buf = 0;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }     
    
    am_i2c_read(&p_dev->i2c_device,
                AM_MMA8451_F_STATUS, 
                &buf, 
                1); 
    
    if (buf & flag) {
        am_mma8451_xyz_data_read(p_dev, buffer);   
    }        
}

/* �жϼ�������� */ 
void am_mma8451_intterupt_pol_set (am_mma8451_handle_t handle, uint32_t flag)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }      
    
    am_mma8451_standby_mode_select(p_dev, AM_MMA8451_STANDBY_MODE_CFG);
    
    am_i2c_read(&p_dev->i2c_device,
               AM_MMA8451_CTRL_REG3, 
               &buf, 
               sizeof(buf));    

     buf = (buf & 0xfd) | flag;
        
     am_i2c_write(&p_dev->i2c_device,
                  AM_MMA8451_CTRL_REG3, 
                  &buf, 
                  sizeof(buf)); 
    
    am_mma8451_active_mode_select(p_dev, AM_MMA8451_ACTIVE_MODE_CFG);
}

/* �ж�����ģʽ���� */ 
void am_mma8451_int_pin_mod_set (am_mma8451_handle_t handle, uint32_t flag)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }      
    
    am_mma8451_standby_mode_select(p_dev, AM_MMA8451_STANDBY_MODE_CFG);
    
    am_i2c_read(&p_dev->i2c_device,
               AM_MMA8451_CTRL_REG3, 
               &buf, 
               sizeof(buf));    

     buf = (buf & 0xfe) | flag;
        
     am_i2c_write(&p_dev->i2c_device, 
                  AM_MMA8451_CTRL_REG3, 
                  &buf, 
                  sizeof(buf)); 
  
    am_mma8451_active_mode_select(p_dev, AM_MMA8451_ACTIVE_MODE_CFG);
}

/* �ж�״̬���أ�����ֵ�����ж��ж��Ƿ��� */ 
uint8_t am_mma8451_int_status_get (am_mma8451_handle_t handle)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return -AM_ERROR;
    }      
       
    am_i2c_read(&p_dev->i2c_device,
               AM_MMA8451_INT_SOURCE , 
               &buf, 
               sizeof(buf));    
    return buf;
}

/* �������ӵ��ж����� */ 
void am_mma8451_int_pin_rout (am_mma8451_handle_t handle, uint32_t flag, uint8_t pin)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }      
    
    am_mma8451_standby_mode_select(p_dev, AM_MMA8451_STANDBY_MODE_CFG);
    
    am_i2c_read(&p_dev->i2c_device,
               AM_MMA8451_CTRL_REG5, 
               &buf, 
               sizeof(buf)); 
    if (pin == 0) {    
        if (buf & flag) { 
            buf = buf  & (~flag);
        
            am_i2c_write(&p_dev->i2c_device,
                         AM_MMA8451_CTRL_REG5, 
                         &buf, 
                         sizeof(buf)); 
        } 
    }  else {
         if (!(buf & flag)) { 
             buf = buf | flag;
        
             am_i2c_write(&p_dev->i2c_device,
                          AM_MMA8451_CTRL_REG5, 
                          &buf, 
                          sizeof(buf)); 
             
         } 
    }
         
    am_mma8451_active_mode_select(p_dev, AM_MMA8451_ACTIVE_MODE_CFG);
}

/* ʹ�ܼ��ٶȼ���Ӧ�ж�  */ 
void am_mma8451_intterupt_enable (am_mma8451_handle_t handle, uint32_t flag)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }      
    
    am_mma8451_standby_mode_select(p_dev, AM_MMA8451_STANDBY_MODE_CFG);
    
    am_i2c_read(&p_dev->i2c_device,
               AM_MMA8451_CTRL_REG4, 
               &buf, 
               sizeof(buf));    

    if (!(buf & flag)) { 
        buf = buf  | flag;
        
        am_i2c_write(&p_dev->i2c_device,
                     AM_MMA8451_CTRL_REG4, 
                     &buf, 
                     sizeof(buf)); 
    }  
    
    am_mma8451_active_mode_select(p_dev, AM_MMA8451_ACTIVE_MODE_CFG);
}

/* ���ܼ��ٶȼ���Ӧ�ж�  */ 
void am_mma8451_intterupt_disable (am_mma8451_handle_t handle, uint32_t flag)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }      
    
    am_mma8451_standby_mode_select(p_dev, AM_MMA8451_STANDBY_MODE_CFG);
    
    am_i2c_read(&p_dev->i2c_device,
               AM_MMA8451_CTRL_REG4, 
               &buf, 
               sizeof(buf));    

    if (buf & flag) { 
        buf = buf & (~flag);
        
        am_i2c_write(&p_dev->i2c_device,
                     AM_MMA8451_CTRL_REG4, 
                     &buf, 
                     sizeof(buf)); 
    }  
    
    am_mma8451_active_mode_select(p_dev, AM_MMA8451_ACTIVE_MODE_CFG);
}

/* �����ж϶�x,y,z������ٶȵ�ֵ */ 
void am_mma8451_xyz_data_intterupt_cfg (am_mma8451_handle_t handle)
{
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }  
    
    /*
     * Configure the INT pins for open drain and Active Low
     */
    am_mma8451_int_pin_mod_set(p_dev, AM_MMA8451_INT_OD_CFG);
    am_mma8451_intterupt_pol_set(p_dev, AM_MMA8451_INT_POL_LOW_CFG);  
    
    /*
     * Enable the Data Ready Interrupt and route it to INT1.
     */
    am_mma8451_int_pin_rout (p_dev, AM_MMA8451_INT_DRDY_CFG, 1);
    am_mma8451_intterupt_enable (p_dev, AM_MMA8451_INT_DRDY_CFG);  
}    
 
/* �Զ�˯��ʹ�ܻ���� */ 
void am_mma8451_auto_sleep_cfg (am_mma8451_handle_t handle, uint32_t flag, uint8_t var)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }  
    am_i2c_read(&p_dev->i2c_device, 
               AM_MMA8451_CTRL_REG2, 
               &buf, 
               sizeof(buf));   
    
    if (var == 1) {
        if (!(buf & flag)) { 
            buf = buf  | flag;  
            
            am_i2c_write(&p_dev->i2c_device,
                         AM_MMA8451_CTRL_REG2, 
                         &buf, 
                         sizeof(buf)); 
        }
    } else {
        if (buf & flag) { 
            buf = buf  & (~flag);  
            
            am_i2c_write(&p_dev->i2c_device, 
                         AM_MMA8451_CTRL_REG2, 
                         &buf, 
                         sizeof(buf)); 
        } 
    }              
}

/* ˯��ģʽ������������� */ 
void am_mma8451_dat_rat_sleep_mod_set (am_mma8451_handle_t handle, uint32_t flag)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }      
    
    am_i2c_read(&p_dev->i2c_device, 
               AM_MMA8451_CTRL_REG1, 
               &buf, 
               sizeof(buf));    

     buf = (buf & 0x3f) | flag;
        
     am_i2c_write(&p_dev->i2c_device, 
                  AM_MMA8451_CTRL_REG1, 
                  &buf, 
                  sizeof(buf)); 
}

/** \brief MMA8451˯��ģʽ�²���Ƶ������ */
void am_mma8451_sleep_mod_over_mode_set (am_mma8451_handle_t handle, uint32_t value)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }  
    
    am_i2c_read(&p_dev->i2c_device,
                AM_MMA8451_CTRL_REG2, 
                &buf, 
                sizeof(buf)); 
  
    buf = (buf & 0xe7) | (value << 3);
        
    am_i2c_write(&p_dev->i2c_device,
                 AM_MMA8451_CTRL_REG2, 
                 &buf, 
                 sizeof(buf)); 
}

/* ����GATEΪ1���ڻ��������֮ǰ�����ݲ�������FIFO,��Ҫ�ֶ���������� */ 
void am_mma8451_gate_set (am_mma8451_handle_t handle, uint32_t flag)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }       
    
    am_i2c_read(&p_dev->i2c_device,
                AM_MMA8451_CTRL_REG3, 
                &buf, 
                sizeof(buf));    

    if (!(buf & flag)) { 
        buf = buf  | flag;
        
        am_i2c_write(&p_dev->i2c_device,
                     AM_MMA8451_CTRL_REG3, 
                     &buf, 
                     sizeof(buf)); 
    }     
}

/* ����GATEΪ0��˯��/����ģʽ�任��ʱ���Զ�����˻����� */ 
void am_mma8451_gate_clr (am_mma8451_handle_t handle, uint32_t flag)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }         
    
    am_i2c_read(&p_dev->i2c_device,
                AM_MMA8451_CTRL_REG3, 
                &buf, 
                sizeof(buf));    

    if (buf & flag) { 
        buf = buf & (~ flag);
        
        am_i2c_write(&p_dev->i2c_device,
                     AM_MMA8451_CTRL_REG3, 
                     &buf, 
                     sizeof(buf)); 
    }     
}

/* �豸�����¼�ʹ�� */ 
void am_mma8451_wake_from_sleep_enable (am_mma8451_handle_t handle, uint32_t flag)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL || p_dev->handle == NULL) {
        return;
    }       
    
    am_i2c_read(&p_dev->i2c_device,
                AM_MMA8451_CTRL_REG3, 
                &buf, 
                sizeof(buf));    

    if (!(buf & flag)) { 
        buf = buf  | flag;
        
        am_i2c_write(&p_dev->i2c_device,
                     AM_MMA8451_CTRL_REG3, 
                     &buf, 
                     sizeof(buf)); 
    }     
}

/* �豸�����¼����� */ 
void am_mma8451_wake_from_sleep_disable (am_mma8451_handle_t handle, uint32_t flag)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }         
    
    am_i2c_read(&p_dev->i2c_device,
                AM_MMA8451_CTRL_REG3, 
                &buf, 
                sizeof(buf));    

    if (buf & flag) { 
        buf = buf & (~flag);
        
        am_i2c_write(&p_dev->i2c_device,
                     AM_MMA8451_CTRL_REG3, 
                     &buf, 
                     sizeof(buf)); 
    }     
}

/** \brief MMA8451˯��ʱ������ */
void am_mma8451_sleep_time_set (am_mma8451_handle_t handle, uint32_t value)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }  
    
    buf = (buf & 0x00) | value ;
        
    am_i2c_write(&p_dev->i2c_device,
                 AM_MMA8451_ASLP_COUNT, 
                 &buf, 
                 sizeof(buf)); 
}

void am_mma8451_low_power_cfg (am_mma8451_handle_t handle)
{ 
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    } 
    
    am_mma8451_standby_mode_select(p_dev, AM_MMA8451_STANDBY_MODE_CFG);  
    
    am_mma8451_auto_sleep_cfg(p_dev, AM_MMA8451_AUTO_SLEEP, 1);
    
    /* ˯��ģʽ���������Ϊ12.5Hz */
    am_mma8451_dat_rat_sleep_mod_set(p_dev, AM_MMA8451_SLEEP_DATA_RATA_SET(1));
    
    /* ����Ƶ��ʹ�õ͹��� */
    am_mma8451_sleep_mod_over_mode_set(p_dev, AM_MMA8451_OVERSAMPLING_MODE_SET(3));
    
    /*
     * Configure the INT pins for open drain and Active Low
     */
    am_mma8451_int_pin_mod_set(p_dev, AM_MMA8451_INT_OD_CFG);
    am_mma8451_intterupt_pol_set(p_dev, AM_MMA8451_INT_POL_LOW_CFG);  
    
    /*
     * Enable the MOTION route it to INT1 but AUTO SLEEP/WAKE Interrupt route it to INT2.
     */
    am_mma8451_int_pin_rout(p_dev, AM_MMA8451_INT_FFMT_CFG, 1);
    am_mma8451_int_pin_rout(p_dev, AM_MMA8451_INT_ASLP_CFG, 0);
    
    am_mma8451_intterupt_enable(p_dev, AM_MMA8451_INT_FFMT_CFG);
    am_mma8451_intterupt_enable(p_dev, AM_MMA8451_INT_ASLP_CFG);
    
    am_mma8451_standby_mode_select(p_dev, AM_MMA8451_STANDBY_MODE_CFG);  
    
    /* 5sʱ������ */
    am_mma8451_sleep_time_set(p_dev, 5 / 0.32);
    
    am_mma8451_wake_from_sleep_enable(p_dev, AM_MMA8451_WAKE_FFMT_CFG);
     
    am_mma8451_active_mode_select(p_dev, AM_MMA8451_ACTIVE_MODE_CFG);
}

/* �˶�������� */
void am_mma8451_motion_detect_cfg (am_mma8451_handle_t handle)
{
    uint8_t buf[3] = {0xf8, 0x20, 0x0a};
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }  
    
    am_mma8451_standby_mode_select(p_dev, AM_MMA8451_STANDBY_MODE_CFG);    
    
    /* 
     *  Set Configuration Register for Motion Detection by setting 
     *  the ��OR�� condition OAE = 1, enablingX, Y, Z,and the latch  
     */ 
    am_i2c_write(&p_dev->i2c_device,
                 AM_MMA8451_FF_MT_CFG, 
                 &buf[0], 
                 1); 
    
    /* Threshold Setting Value for the Motion detection of > g����ֵ  1g / 0.0632 = 15 */
    am_i2c_write(&p_dev->i2c_device,
                 AM_MMA8451_FF_MT_THS, 
                 &buf[1], 
                 1);  
    /* �͹��� ����100ms */ 
    am_i2c_write(&p_dev->i2c_device, 
                 AM_MMA8451_FF_MT_COUNT, 
                 &buf[2], 
                 1);                  
    /*
     * Configure the INT pins for open drain and Active Low
     */
    am_mma8451_int_pin_mod_set(p_dev, AM_MMA8451_INT_OD_CFG);
    am_mma8451_intterupt_pol_set(p_dev, AM_MMA8451_INT_POL_LOW_CFG);                   

    /*
     * Enable the MOTION Interrupt and route it to INT1.
     */
    am_mma8451_int_pin_rout(p_dev, AM_MMA8451_INT_FFMT_CFG, 1);
    am_mma8451_intterupt_enable(p_dev, AM_MMA8451_INT_FFMT_CFG);
    
    am_mma8451_active_mode_select(p_dev, AM_MMA8451_ACTIVE_MODE_CFG);  
}

/* ʹ��FIFO���� */
void am_mma8451_fifo_cfg (am_mma8451_handle_t handle ,uint32_t flag)
{
    uint8_t buf = 0x80;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }  
    
    am_mma8451_standby_mode_select(p_dev, AM_MMA8451_STANDBY_MODE_CFG);  

    if (flag == AM_MMA8451_FIFO_DAT_COLLECT_CFG) {
        
       /* ѡ��8λ����     */
        am_mma8451_data_read_way_select(p_dev, 0);
       
        /* Put the FIFO in Circular Buffer Mode */ 
        am_i2c_write(&p_dev->i2c_device, 
                     AM_MMA8451_F_SETUP, 
                     &buf, 
                     1); 
        /*
         * Configure the INT pins for open drain and Active Low
         */
        am_mma8451_int_pin_mod_set(p_dev, AM_MMA8451_INT_OD_CFG);
        am_mma8451_intterupt_pol_set(p_dev, AM_MMA8451_INT_POL_LOW_CFG);  
        
        /*
         * Enable the FIFO Interrupt and route it to INT1.
         */
        am_mma8451_int_pin_rout(p_dev, AM_MMA8451_INT_FIFO_CFG, 1);
     
        am_mma8451_intterupt_enable(p_dev, AM_MMA8451_INT_FIFO_CFG);
    
    } else {
        /* Put the FIFO in Circular Buffer Mode */ 
        am_i2c_write(&p_dev->i2c_device,
                     AM_MMA8451_F_SETUP, 
                     &buf, 
                     1); 
    
        /* 
         * ��ģʽת��֮�䣬FIFO�������������ݽ�������
         * ֻ�л���������պ���ܽ��������ݣ����ᱣ��
         * ԭ����������������� 
         */
         am_mma8451_gate_set(p_dev, AM_MMA8451_GATA_CFG);
    
        /* �͹�������,5s���¼�����˯��, ��FIFO���谭����˯��״̬ motion wake */
         am_mma8451_low_power_cfg(p_dev);
    
        /* �˶�������ã�����100ms */ 
        am_mma8451_motion_detect_cfg(p_dev); 
    }        
    
    am_mma8451_active_mode_select(p_dev, AM_MMA8451_ACTIVE_MODE_CFG);  
}

/** \brief �˶�����¼�״̬Դ�Ĵ������� */

uint8_t am_mma8451_motion_event_src_get (am_mma8451_handle_t handle)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return -AM_ERROR;
    }      
       
    am_i2c_read(&p_dev->i2c_device, 
               AM_MMA8451_FF_MT_SRC, 
               &buf, 
               sizeof(buf));    
    return buf;
}


/* �˶�����жϸ������� */ 
void am_mma8451_int_motion_assist (am_mma8451_handle_t handle)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }         
    
    buf = am_mma8451_motion_event_src_get(p_dev);
        
    if (buf & AM_MMA8451_MOTION_EA_FLAG) {
        if (buf & AM_MMA8451_MOTION_ZHE_FLAG) {  
            if (buf & AM_MMA8451_MOTION_ZHP_FLAG) { 
#ifdef DEBUG
                AM_DBG_INFO("Z motion event happen and plo is negative\r\n");   
#endif /* DEBUG */               
            } else {
#ifdef DEBUG                
                AM_DBG_INFO("Z motion event happen and plo is positive\r\n");
#endif /* DEBUG */                
            }
        }
        if (buf & AM_MMA8451_MOTION_YHE_FLAG) {  
            if (buf & AM_MMA8451_MOTION_YHP_FLAG) { 
#ifdef DEBUG                
                AM_DBG_INFO("Y motion event happen and plo is negative\r\n");   
#endif /* DEBUG */                 
            } else {
#ifdef DEBUG                
                AM_DBG_INFO("Y motion event happen and plo is positive\r\n");
#endif /* DEBUG */                 
            }
        } 
        if (buf & AM_MMA8451_MOTION_XHE_FLAG) {  
            if (buf & AM_MMA8451_MOTION_XHP_FLAG) { 
#ifdef DEBUG                
                AM_DBG_INFO("X motion event happen and plo is negative\r\n");   
#endif /* DEBUG */                 
            } else {
#ifdef DEBUG                
                AM_DBG_INFO("X motion event happen and plo is positive\r\n");
#endif /* DEBUG */                 
            }
        } 
        
    }     
}

/** \brief ˯��/����ģʽ��״̬����*/
uint8_t am_mma8451_system_mode_get (am_mma8451_handle_t handle)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return -AM_ERROR;
    }      
       
    am_i2c_read(&p_dev->i2c_device, 
                AM_MMA8451_SYSMOD, 
                &buf, 
                sizeof(buf));    
    return buf;
}



/* ˯��/����ģʽ�жϸ������� */ 
void am_mma8451_system_mode_assist (am_mma8451_handle_t handle, int16_t *buffer)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    } 

    buf = am_mma8451_system_mode_get(p_dev);
        
    if ((buf & 0x03) == AM_MMA8451_SLEEP_MODE_FLAG) {

#ifdef DEBUG         
        AM_DBG_INFO("sleep mode\n\r");  
#endif /* DEBUG */         
        
       /* �����ٶȴ��������ߺ�ָʾ������Ҳ��������״̬ */
        
    } 
    
    if ((buf & 0x03) == AM_MMA8451_WAKE_MODE_FLAG) {
        
#ifdef DEBUG 
        AM_DBG_INFO("wake mode\n\r");
#endif  /* DEBUG */         

    }     
}

/* FIFO�жϸ������� */ 
void am_mma8451_fifo_mode_assist (am_mma8451_handle_t handle, int16_t *buffer)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    } 
    
    /* ��AM_MMA8451_F_STATUS�Ĵ������FIFO�ж�Դ��־ */
    am_i2c_read(&p_dev->i2c_device, 
                AM_MMA8451_F_STATUS, 
                &buf, 
                sizeof(buf));  
}




#if 0
/* ���ݴ���ת�����������ٶȵı�ʾ���� */
void am_mma8451_data_handle(am_mma8451_handle_t handle, float * buf, int16_t *buffer)
{  
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
   
    uint8_t j = 0;
        
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }
    
    for(j = 0; j < 3; j++) {
        if (buffer[j] & 0x2000) {
            buffer[j] = ~buffer[j] + 1;
            
           buf[j] = (float)buffer[j] / 0xfff;
            
        }
        
        buf[j] = (float)buffer[j] / 0xfff;        
    }
     
}
#endif /* 0 */

#if 0
/* ���ݴ���ת�����������ٶȵı�ʾ���� */
void am_mma8451_data_handle(am_mma8451_handle_t handle, float * buf, int16_t *buffer)
{  
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
   
    int16_t temp[] = {2, 5, 10, 20, 39, 78, 156, 313, 625, 1250, 2500, 5000};
    
    uint8_t i = 0, j = 0;
    
    int16_t tmp = 0;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }
     
    for(j = 0; j < 3; j++) {
        if (buffer[j] & 0x2000) {
            buffer[j] = ~buffer[j] + 1;          
        }
        
        
    }    
    
    for (j = 0; j < 3; j++) {
        
        for (i = 0; i < 14; i++) {
            
            buffer[j] >>= i;
            
            if ((buffer[j] & 0x0001) &&  (i < 12)) {
                
                tmp = tmp + temp[i];    
                
                continue;                
            } 
            
            if ((buffer[j] & 0x0001) &&  (i == 12))  {
                buf[j] = 9.8;
                
                continue;
            } 
            
            if (!(buffer[j] & 0x0001) &&  (i == 12)) {
                buf[j] = 0.0;
                
                continue;
            }
            
            if ((buffer[j] & 0x0001) &&  (i == 13))  { 
                buf[j] += (float)tmp / 1000.0f;
                buf[j] = - buf[j];
                
                continue;
            }  
            if (!(buffer[j] & 0x0001) &&  (i == 13)) {
                buf[j] += (float)tmp / 1000.0f;
            }  
        }
        
        tmp = 0;
    }       
}
#endif /* 0 */

#if 1
/* ���ݴ���ת�����������ٶȵı�ʾ���� */
void am_mma8451_data_handle(am_mma8451_handle_t handle, float * buf, int16_t *buffer)
{  
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    uint8_t i = 0;
    
    uint8_t buf_tmp = 0;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }      
       
    am_i2c_read(&p_dev->i2c_device, 
                AM_MMA8451_XYZ_DATA_CFG, 
                &buf_tmp, 
                sizeof(buf)); 
   
   buf_tmp = buf_tmp & 0x03; 
    
   if (buf_tmp == 0x00) {      
       for (i = 0; i < 3; i++) {
            buf[i] = (float)buffer[i] / 4096 * 9.8f;
       } 
   }  
   
   if (buf_tmp == 0x01) {      
       for (i = 0; i < 3; i++) {
            buf[i] = (float)buffer[i] / 2048 * 9.8f;
       } 
   }  
   
   if (buf_tmp == 0x02) {      
       for (i = 0; i < 3; i++) {
            buf[i] = (float)buffer[i] / 1024 * 9.8f;
       } 
   }   
}
#endif /* 1 */

/* ���ݴ����������ٶ�ת��ɽǶ� */
void am_mma8451_acc_to_angle(am_mma8451_handle_t handle, int16_t* buf, float *buffer)
{           
    buf[0] =  (buffer[0] / sqrt((buffer[2] * buffer[2] + buffer[1] * buffer[1]))) * 180 / 3.14f;
    buf[1] =  (buffer[1] / sqrt((buffer[0] * buffer[0] + buffer[2] * buffer[2]))) * 180 / 3.14f;    
    buf[2] =  (sqrt((buffer[0] * buffer[0] + buffer[1] * buffer[1])) / buffer[2]) * 180 / 3.14f;            
}
    
void am_mma8451_data_change_to_char (am_mma8451_handle_t handle, uint8_t *p_txbuf, const int16_t *buffer) 
{
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    uint8_t i = 0,j = 0;
    
    uint8_t buf[4] = {0};
    
    int16_t tmp = 0;
    int16_t buf_tmp = 0;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    } 
    
    p_txbuf[j++] = '*';  
    
    for (i = 0; i < 3; i++) {
        
        tmp = buffer[i];        
        
        if (tmp  <  0) {
            tmp = -tmp;
            *(p_txbuf + (6 * i + j)) = '-';  /* ����ȡ��Ҳ�������� */
            j++;
         }   
         
         buf[0]  = tmp / 1000;
         buf_tmp = tmp % 1000;
         buf[1]  = buf_tmp / 100; 
         buf_tmp = buf_tmp % 100;
         buf[2]  = buf_tmp / 10;         
         buf[3]  = buf_tmp % 10;    
          
        if (buf[0] != 0) {
            *(p_txbuf + (6 * i + j)) = buf[0] + 48;
            j++;            
            *(p_txbuf + (6 * i + j)) = buf[1] + 48;
            j++;            
            *(p_txbuf + (6 * i + j)) = buf[2] + 48;
            j++;            
            *(p_txbuf + (6 * i + j)) = buf[3] + 48;   
            j++;             
            
        } 
        if ((buf[0] == 0) && (buf[1] != 0)) {
            *(p_txbuf + (6 * i + j)) = buf[1] + 48;
            j++;            
            *(p_txbuf + (6 * i + j)) = buf[2] + 48;
            j++;            
            *(p_txbuf + (6 * i + j)) = buf[3] + 48;
            j++;                        
         } 
 
        if ((buf[0] == 0) && (buf[1] == 0) && (buf[2] != 0)) {          
           *(p_txbuf + (6 * i + j)) = buf[2] + 48;
            j++;            
            *(p_txbuf + (6 * i + j)) = buf[3] + 48;
            j++;                        
         }
        
        if ((buf[0] == 0) && (buf[1] == 0) && (buf[2] == 0) && (buf[3] != 0)) {          
        
            *(p_txbuf + (6 * i + j)) = buf[3] + 48;
            j++;
         }
        
        if ((buf[0] == 0) && (buf[1] == 0) && (buf[2] == 0) && (buf[3] == 0)) {          
        
            *(p_txbuf + (6 * i + j)) = buf[3] + 48;
            j++;
        } 
        
        for (j = j; j < 7; j++) { 
             
             *(p_txbuf + (6 * i + j)) = 0;    /* �ַ��������� */
        }            
             
         j = 1;          
         
     }
    
     p_txbuf[19] = '#';      
}    

void am_mma8451_data_change_to_int (am_mma8451_handle_t handle, const uint8_t *p_txbuf, int16_t *buffer) 
{
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    uint8_t i = 0;
    
    int16_t num = 0;
    
    bool_t minus  = FALSE; 
    
   /* �м����������ԭʼ�ַ�������ֹ���ƻ� */   
    const uint8_t *digit = NULL; 
    
    if (p_dev == NULL ||  p_dev->handle == NULL || p_txbuf == NULL) {
        return;
    }  
    
 #if 1      
    for (i = 0; i < 3; i++) {
         
         digit = (p_txbuf + 6 * i); 
    
         if (*digit == '-') {
             digit ++;
             minus =TRUE;
         }
         
         /* �ַ���������־ */
         while (*digit != '\0') {
            if (*digit >= '0' && *digit <= '9') {
                num = num * 10 + (*digit - '0');
                /* ������������ */
                if (num > 8192) {
                    num = 0;
                    break;
                }
                digit ++;
               
             }else {  /* if the char is not a digit, invalid input */
                num = 0;
                break;
             }
         }
         
         if (*digit == '\0') {            
             if (minus) {
                 num = 0 - num;   //ת��Ϊ�����ķ�ʽ
                 minus = FALSE;
                     
             }
             
             buffer[i] = num;      
         }
         
       num = 0; 
       digit = NULL;  
   }      
   
#endif

#if 0      
    /* �������ʹ�õ����Ƕ��ϵ��ڴ� */
    for (i = 0; i < 3; i++) {
         buffer[i] = atoi((const char *)p_txbuf[i]);
    } 
#endif  /* 0 */   
}


/** \brief �豸�ж����� */
void am_mma8451_int_cfg (am_mma8451_handle_t handle, am_pfnvoid_t pfn_isr1, am_pfnvoid_t pfn_isr2, void *p_arg)
{
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    } 
    
    /* ��������Ϊ��������ģʽ        */            
    am_gpio_pin_cfg(p_dev->p_devinfo->interrupt_pin1, AM_GPIO_INPUT | AM_GPIO_PULLUP);     
    
    /* �����жϷ�����              */
    am_gpio_trigger_connect(p_dev->p_devinfo->interrupt_pin1, pfn_isr1, p_arg);
        
    /* ���ô������ź��½��ش���ģʽ  */
    am_gpio_trigger_cfg(p_dev->p_devinfo->interrupt_pin1, AM_GPIO_TRIGGER_FALL);
        
    /* ʹ�����Ŵ����ж�              */
    am_gpio_trigger_on(p_dev->p_devinfo->interrupt_pin1);    
    
    /* ��������Ϊ��������ģʽ        */            
    am_gpio_pin_cfg(p_dev->p_devinfo->interrupt_pin2, AM_GPIO_INPUT | AM_GPIO_PULLUP);     
    
    /* �����жϷ�����              */
    am_gpio_trigger_connect(p_dev->p_devinfo->interrupt_pin2, pfn_isr2, p_arg);
        
    /* ���ô������ź��½��ش���ģʽ  */
    am_gpio_trigger_cfg(p_dev->p_devinfo->interrupt_pin2, AM_GPIO_TRIGGER_FALL);
        
    /* ʹ�����Ŵ����ж�              */
    am_gpio_trigger_on(p_dev->p_devinfo->interrupt_pin2);   
}



/* �豸�жϴ����� */
void am_mma8451_intterupt_handle (am_mma8451_handle_t handle, int16_t *buffer)
{
    uint8_t buf = 0;
    
    am_mma8451_dev_t *p_dev = (am_mma8451_dev_t *)handle;
    
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }  
        
    /*
     * Go read the Interrupt Source Register
     */
        
    buf = am_mma8451_int_status_get(p_dev);
        
    if (buf & AM_MMA8451_INT_FIFO_FLAG) {
        
        uint8_t fifo_flag = 0;
        
        am_mma8451_intterupt_disable(p_dev, AM_MMA8451_INT_FIFO_CFG);
        
#ifdef DEBUG             
        AM_DBG_INFO("FIFO intterupt happen\r\n");
#endif  /* DEBUG */       
        
        /* ��AM_MMA8451_F_STATUS�Ĵ������FIFO�ж�Դ��־ */
        am_i2c_read(&p_dev->i2c_device, 
                    AM_MMA8451_F_STATUS, 
                    &fifo_flag , 
                    sizeof(buf)); 
        /* һ��Ҫ��192���ֽ� */         
        am_mma8451_fifo_data_read(p_dev, buffer);
        
        am_mma8451_intterupt_enable(p_dev, AM_MMA8451_INT_FIFO_CFG);
    }       
        
    if (buf & AM_MMA8451_INT_ASLP_FLAG) {
        
#ifdef DEBUG             
        AM_DBG_INFO("AUTO WAKE/SLEEP intterupt happen\r\n");
#endif  /* DEBUG */
        
        am_mma8451_system_mode_assist(p_dev, buffer); 
    }
        
    if (buf & AM_MMA8451_INT_FFMT_FLAG) { 
        
        uint8_t auto_wake_flag = 0;
        
#ifdef DEBUG              
        AM_DBG_INFO("MOTION intterupt happen\r\n");
#endif  /* DEBUG */
        
        am_mma8451_int_motion_assist(p_dev);
        
        am_i2c_read(&p_dev->i2c_device, 
                     AM_MMA8451_SYSMOD, 
                     &auto_wake_flag, 
                     sizeof(buf));
        /* 5sʱ������ */
        am_mma8451_sleep_time_set(p_dev, 5 / 0.32);
        
        if ((auto_wake_flag & 0x01) == 0x01) {
            
#ifdef DEBUG              
            AM_DBG_INFO("motion read\r\n");
#endif /* DEBUG */ 
            
            am_mma8451_xyz_data_read(p_dev, buffer);   
        }
    }    
        
    if (buf & AM_MMA8451_INT_DRDY_FLAG) {
            
        am_mma8451_intterupt_disable(p_dev, AM_MMA8451_INT_DRDY_CFG);
        
#ifdef DEBUG            
        AM_DBG_INFO("DATA READY intterupt happen\r\n");
#endif  /* DEBUG */
        
        am_mma8451_xyz_data_read(p_dev, buffer); 
              
        am_mma8451_intterupt_enable(p_dev, AM_MMA8451_INT_DRDY_CFG);  
    }  
}


/** \brief ��ʼ��mma8451 */
am_mma8451_handle_t am_mma8451_init (am_mma8451_dev_t *p_dev,
                                     const am_mma8451_device_t *p_devinfo,
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
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
    /* ��ʼ��λ          */
    am_mma8451_reset_enable(p_dev, AM_MMA8451_RESET_ENALBE); 
    /* ������λ          */
    am_mma8451_reset_disable(p_dev, AM_MMA8451_RESET_DISALBE);
       
    /* ���뱸��ģʽ     */
    am_mma8451_standby_mode_select(p_dev, AM_MMA8451_STANDBY_MODE_CFG);
    
    /* ѡ��8λ����     */
    am_mma8451_data_read_way_select(p_dev, 0);
    
    /* 4G������         */
    am_mma8451_g_range_set(p_dev, AM_MMA8451_G_RANGES(1));
    
    /* 100Hz�������    */
    am_mma8451_data_rate_out(p_dev, AM_MMA8451_DATA_RATA_SET(3));
    
    /* ����Ƶ������ģʽ */
    am_mma8451_oversampling_mode_set(p_dev, AM_MMA8451_OVERSAMPLING_MODE_SET(0)); 
 
 #if 0 
    /* ��100Hz��������������4HzƵ�����ϵ��������  */
    am_mma8451_hpf_fra_cutoff(p_dev, AM_MMA8451_HPF_FRQ_CUFOFF(0));
    
    /* ʹ���������ʹ�ø�ͨ�˲� */                      
    am_mma8451_data_hpf_enable(p_dev, AM_MMA8451_DATA_HPF_ENALBE);
#endif /* 0 */
       
    /* ���뼤��ģʽ                             */
    am_mma8451_active_mode_select(p_dev, AM_MMA8451_ACTIVE_MODE_CFG);
    
    return p_dev;
}

/** \brief ���MMA8451��ʼ�� */
void am_mma8451_deinit (am_mma8451_dev_t *p_dev)
{
    amdr_i2c_dev_t *p_i2c_dev   = (amdr_i2c_dev_t *)p_dev->handle->p_drv;
      
    if (p_dev == NULL ||  p_dev->handle == NULL) {
        return;
    }         
    
    amdr_i2c_deinit(p_i2c_dev);
    
    p_dev->handle    = NULL;
    
    p_dev->p_devinfo = NULL;
}

/* end of file */
