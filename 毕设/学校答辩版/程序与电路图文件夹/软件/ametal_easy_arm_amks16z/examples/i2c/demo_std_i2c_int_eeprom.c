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
 * \brief ��I2C�ж�ģʽ�²���EEPROM��ʾ���̣�ͨ����׼�ӿ�ʵ��
 *
 * - ��������:
 *   1. ��I2C�ӿں�EEPROM�ӿڶ�Ӧ�������ӣ�
 *   2. ���������ص�demo���ϣ��������ϵ��λ��
 *
 * - ʵ������:
 *   1.���ڴ�ӡ��transfer success����������ɹ�
 *
 * \note ����ʾ���̺� demo_std_i2c_master ��ʾ���̣�ʵ�ֵ����õĽӿں�����ͬ����������ͬ
 *
 * \par Դ����
 * \snippet demo_std_i2c_int_eeprom.c src_std_i2c_int_eeprom 
 * 
 * \internal
 * \par History
 * - 1.00 15-11-3  LXY , first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_std_i2c_int_eeprom
 * \copydoc demo_std_i2c_int_eeprom.c
 */

/** [src_std_i2c_int_eeprom] */
#include "ametal.h"
#include "am_board.h"
#include "am_prj_config.h"

#define EEPROM_ADDR             0x50            /**< \brief EEPROM�豸�ĵ�ַ */
#define TEST_LEN                0x06            /**< \brief ����EEPROM��С      */


/**
 * \brief ������
 */
int main (void)
{
    am_i2c_handle_t    i2c_handle;
    am_i2c_device_t    eeprom;                      /* I2C �����豸����                           */
    uint8_t            eeprom_buf[6] = {0};         /* ���ݻ��涨��                                     */
    uint8_t            test_addr     =  0x90 ;     /* I2C �����ӻ��豸�ӵ�ַ���� */
    uint8_t            i;
    int                ret;
    
    /* �弶��ʼ�� */
    am_board_init();

    /* I2C0 Ӳ��ƽ̨��ʼ�� */
    amhw_plfm_i2c0_init();

    /* I2C ��ʼ�� */
    i2c_handle = amdr_i2c_init(&g_i2c0_dev, &g_i2c0_devinfo);
    
    /* I2C �ж����� */
    am_i2c_connect(i2c_handle);

    am_i2c_mkdev(&eeprom,
                  i2c_handle,
                  EEPROM_ADDR,
                  AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    for (i = 0;i < TEST_LEN; i++) {
        eeprom_buf[i] = i;
    }
    
    /* д�����ݵ�ָ����EEPROM��ַ */
    ret = am_i2c_write(&eeprom,
                        test_addr,
                       (uint8_t *)eeprom_buf, 
                        TEST_LEN);
    
    if (ret != AM_OK) {
        am_kprintf("write failure\n");
        while(1);
    }

    am_mdelay(10);

    for (i = 0;i < TEST_LEN; i++) {
        eeprom_buf[i] = 0;
    }
    
    /* ��ȡ���ݵ�ָ����EEPROM��ַ */
    ret = am_i2c_read(&eeprom,
                       test_addr,
                      (uint8_t *)eeprom_buf, 
                       TEST_LEN);

    if (ret != AM_OK) {
        am_kprintf("read failure\n");
        while(1);
    }
    
    am_mdelay(10);

    /* У��д��Ͷ�ȡ�������Ƿ�һ�� */
    for (i = 0;i < TEST_LEN; i++) {

        /* У��ʧ�� */
        if (eeprom_buf[i] != (i)) {
            am_kprintf("transfer failure\n");
            while(1);
        }
    }
    am_kprintf("transfer success\n");

    am_mdelay(100);
    
    /* У��ɹ���LED��200msʱ������˸ */
    while (1) {
    }
}

/** [src_std_i2c_int_eeprom] */
/* end of file */
