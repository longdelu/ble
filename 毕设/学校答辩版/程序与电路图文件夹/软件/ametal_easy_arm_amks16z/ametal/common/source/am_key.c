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
 * \brief matrix key scan implementation
 * 
 * \internal
 * \par Modification history
 * - 1.00 16-03-19  lgr, first implementation.
 * \endinternal
 */
 
#include "am_key.h"
  
uint8_t g_mouse_or_key_flag = 0xA1;   /**< \brief ѡ���������̱�־��Ĭ��Ϊ��� */

uint8_t g_mouse_button[2]   = {0,0};  /**< \brief ������Ҽ���ֵ������          */
uint8_t g_key_board[8]      = {0,0};  /**< \brief ���̰�����ֵ������            */

/** \brief ����״̬���ͳɶ�Ӧ�ļ�ֵ */
static const uint8_t __g_keyval_table[] = {
    AM_KEYBOARD_FLAG,
    AM_MOUSE_FLAG, 
    AM_MOUSE_LEFT_CLICK,
    AM_MOUSE_RIGHT_CLICK,
    AM_MODIFERKEYS_LEFT_ALT,
    AM_KEY_SPACEBAR,
    AM_KEY_ENTER,
    AM_KEY_ESCAPE,
    AM_KEY_RIGHTARROW,
    AM_KEY_LEFTARROW,
    AM_KEY_DOWNARROW,
    AM_KEY_UPARROW,
    AM_KEY_NONE
};

/** \brief ���������жϻص��ú���  */
void am_int_wake_up_cal (void  *p_arg) 
{
    uint8_t i = 0;
    
    am_matrix_key_dev_t *p_dev = (am_matrix_key_dev_t *)p_arg;
    
    if (p_dev == NULL ||  p_dev->p_devinfo == NULL) {
        return;
    }
    
    for (i = 0; i < 4; i++) {        
        amhw_port_pin_isf_clr (AMHW_PORT, p_dev->p_devinfo->row_pin + i);
    }
      
    amhw_port_pin_isf_clr (AMHW_PORT, PIOD_6);
    
    if (amhw_port_pin_isf_get(AMHW_PORT, PIOD_4)) {
        amhw_port_pin_isf_clr (AMHW_PORT, PIOD_4);
    }
        
}
 
/** \brief  �жϻ����������� */
void am_int_wake_up_cfg (am_matrix_key_handle_t handle, am_pfnvoid_t pfn_isr) 
{
    uint8_t i = 0; 
    
    am_matrix_key_dev_t *p_dev = (am_matrix_key_dev_t *)handle;
    
    if (p_dev == NULL || p_dev->p_devinfo == NULL) {
        return;
    }
    
    p_dev->int_status = 1;
    
    p_dev->pfn_isr = pfn_isr;
       
    for (i = 0; i < 4; i++) {               
        am_gpio_trigger_cfg(p_dev->p_devinfo->row_pin + i, AM_GPIO_TRIGGER_RISE);   
        am_gpio_pin_cfg(p_dev->p_devinfo->row_pin + i, AM_GPIO_PULLDOWN | AM_GPIO_INPUT);         
    }
    
    /* MPU6050�ж�����  */
    am_gpio_trigger_cfg(PIOD_6, AM_GPIO_TRIGGER_RISE);
    am_gpio_pin_cfg(PIOD_6, AM_GPIO_PULLDOWN | AM_GPIO_INPUT); 
    
    /* ��������������   */
    am_gpio_pin_cfg(PIOD_4, AM_GPIO_PULLUP   | AM_GPIO_OUTPUT_INIT_HIGH);  
 
    am_int_disable(p_dev->p_devinfo->inum);
    am_int_connect(p_dev->p_devinfo->inum, p_dev->pfn_isr, p_dev);
}

/** \brief ʹ���жϻ������� */
void am_int_wake_up_enable(am_matrix_key_handle_t handle)
{
    am_matrix_key_dev_t *p_dev = (am_matrix_key_dev_t *)handle;
    
    if (p_dev == NULL || p_dev->p_devinfo == NULL) {
        return;
    }

    am_int_enable(p_dev->p_devinfo->inum);    
}

/** \brief �����жϻ������� */
void am_int_wake_up_disable(am_matrix_key_handle_t handle)
{
    am_matrix_key_dev_t *p_dev = (am_matrix_key_dev_t *)handle;
    
    if (p_dev == NULL || p_dev->p_devinfo == NULL) {
        return;
    }

    am_int_disable(p_dev->p_devinfo->inum);   
}

/** \brief ���󰴼���ʼ�� */
am_matrix_key_handle_t am_matrix_key_init (am_matrix_key_dev_t           *p_dev, 
                                           const am_matrix_key_device_t  *p_devinfo)                            
{
    uint8_t i = 0; 
    
    if (p_dev == NULL ||  p_devinfo == NULL) {
        return NULL;
    }

    p_dev->int_status = 0;
    p_dev->pfn_isr    = NULL;
    
    p_dev->p_devinfo  = p_devinfo;
    
    /* PTC0 ~ PTC3��Ϊ�����ҳ�ʼ��Ϊ���룬ģʽΪ����        */
    for (i = 0; i < 4; i++) {   
        am_gpio_pin_cfg(p_devinfo->row_pin + i, AM_GPIO_PULLDOWN | AM_GPIO_INPUT);           
    }

    /* PTC4 ~ PTC7��Ϊ�����ҳ�ʼ��Ϊ���Ϊ�͵�ƽ��ģʽΪ���� */
    for (i = 0; i < 4; i++) {       
        am_gpio_pin_cfg(p_devinfo->col_pin + i, AM_GPIO_OUTPUT_INIT_LOW);
    }

    return p_dev;    
}

/** \brief ���������̳�ʼ�� */
void am_matrix_key_deinit (am_matrix_key_dev_t *p_dev)
{
    
    if (p_dev == NULL || p_dev->p_devinfo == NULL) {
        return;
    }

    if (p_dev->int_status == 1) {       
        /* �����������ж� */
        am_int_disable(p_dev->p_devinfo->inum);
        am_int_disconnect(p_dev->p_devinfo->inum, p_dev->pfn_isr, p_dev);

    }        
    
    p_dev->int_status = 0;
    p_dev->pfn_isr    = NULL; 
    
    p_dev->p_devinfo  = NULL;
    
    p_dev = NULL;       
}
 
/** \brief ɨ��õ�����״̬ */
uint16_t am_matrix_key_scan (am_matrix_key_handle_t  handle)
{
    
    uint16_t key_value   = 0x0;
    uint16_t key_val_tmp = 0;
    
    uint8_t n = 0, m = 0;
    
    am_matrix_key_dev_t *p_dev = (am_matrix_key_dev_t *)handle;
    
    if (p_dev == NULL || p_dev->p_devinfo == NULL) {
        return -AM_ERROR;
    }
    
    for (n = 0; n < 4; n++) {
        am_gpio_set(p_dev->p_devinfo->col_pin + n, 0);
    }
    
    for (n = 0 ; n < 4; n++) {       
        am_gpio_set(p_dev->p_devinfo->col_pin + n, 1);
        
        am_udelay(3);
        
        for (m = 0; m < 4; m++) {            
             /* ĳһ�з���ֵΪ1�����а������� */
             key_val_tmp = am_gpio_get(p_dev->p_devinfo->row_pin + m);
             
             /* 
              * ��������ʱ������״̬Ϊ1
              * ������������Ե�λ��1��������λ����İ����а��� 
              */
             key_value |= (((key_val_tmp & 0x01)) << (4 * n + m));
        }
               
        am_gpio_set(p_dev->p_devinfo->col_pin + n, 0);
    }
         
    for (n = 0; n < 4; n++) {
        am_gpio_set(p_dev->p_devinfo->col_pin + n, 1);
    }
           
    return  key_value; /* ���ذ���״̬ */
}

/** \brief  ���ɨ��õ���ֵ���������� */
uint16_t am_matrix_key_scan_repeat (am_matrix_key_handle_t  handle, uint8_t  count)
{
    uint8_t i = 0; 
    uint16_t key_val_now  = 0;
    uint16_t key_val_last = 0;

    am_matrix_key_dev_t *p_dev = (am_matrix_key_dev_t *)handle;
    
    if (p_dev == NULL || p_dev->p_devinfo == NULL) {
        return -AM_ERROR;
    }
    
    if (0 == count || 1 == count) {
        return am_matrix_key_scan(p_dev);
    }
    
    /* ��ɨ��һ�εõ���ֵ����������Ƚ� */
    key_val_now = key_val_last = am_matrix_key_scan(p_dev);  
  
    /* �����Ƕ��ɨ�������������      */
    for (i = 0; i < count - 1; i++) {
        key_val_now = am_matrix_key_scan(p_dev);
        
        if (key_val_now == key_val_last) {
            return key_val_now;
        } else {
            key_val_last = key_val_now;
        }
    }
  
   /* ����Ϊ1�����Ƕ�������û�а������� */
   return -AM_ERROR;       
}

/** \brief ���Ͱ���״̬Ϊ�������Ӧ��ֵ  */
void am_matrix_key_value_def (am_matrix_key_handle_t  handle, uint16_t  key_avl)
{
    uint8_t i = 0;
    
    am_matrix_key_dev_t *p_dev = (am_matrix_key_dev_t *)handle;
    
    if (p_dev == NULL || p_dev->p_devinfo == NULL) {
        return ;
    }
    
    for (i = 0; i < 16; i++) {             
        if (key_avl & 0x0001) {           
          /* ����������ѡ���ͬʱ���£�Ĭ��ѡ����깦�� */            
           if (i < 2) {
               g_mouse_or_key_flag = __g_keyval_table[i];
           } else if (i >= 2 && i < 4 ) {
               g_mouse_button[i - 2] = __g_keyval_table[i];
           }else if (i >= 4 && i < 12) { 
               g_key_board[i - 4 ] = __g_keyval_table[i]; 
           } else {
               ;       /* ���µİ����ļ�ֵ����δ����� */
           }     
        }else {
           if (i >= 2 && i < 4) {
               g_mouse_button[i - 2] = 0x00;
           }else if (i >= 4 && i < 12) { 
               g_key_board[i - 4] = 0x00; 
           } 
           
       }
        
       key_avl >>= 1;   /* �����ж�ÿ��λ����ȷ���Ƿ��λ���������Ƿ���  */
    }

}    
 
 
 /* end of file */
 



