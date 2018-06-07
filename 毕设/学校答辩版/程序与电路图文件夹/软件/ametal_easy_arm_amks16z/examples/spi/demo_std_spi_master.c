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
 * \brief ��SPIΪ������ʾ��SPI��Transfer��ʹ�÷�����ͨ����׼�ӿ�ʵ��
 *
 * - ��������:
 *   1. ��SPI MOSI��MISO�̽�,����ӻ��豸���ӣ��ӻ��豸DEV0���ʹӻ��豸DEV1��
 *   2. ���°�������һ������
 *
 * - ʵ������:
 * \note
 *   1. ע�⴫���ʶ��AM_SPI_TRANS_EOT �����ã�ֻ�е����δ��������
 *      ��Ҫ��SSEL��Ϊ��Ч��ƽʱ������Ҫ���øñ�ʶ������������������øñ�ʶ��
 *
 * \par Դ����
 * \snippet demo_std_spi_master.c src_std_spi_master
 *
 * \internal
 * \par History
 * - 1.00 15-10-24  liqing, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_std_spi_master
 * \copydoc demo_std_spi_master.c
 */
 
/** [src_std_spi_master] */
#include "ametal.h"
#include "am_board.h"
#include "am_prj_config.h"

/* �ӻ��豸DEV0 */

am_spi_device_t   g_dev0;     /**< \brief �ӻ��豸0      */
am_spi_device_t   g_dev1;     /**< \brief �ӻ��豸1      */
am_spi_transfer_t g_trans;    /**< \brief ��������       */
                                                         
#define KEY_PIN     PIOC_0    /**< \brief ��������       */
#define BUZZER_PIN  PIOC_3    /**< \brief ����������     */

#define BUF_LEN   10          /**< \brief ���Ի�������С */
uint8_t g_tx_buf[BUF_LEN];    /**< \brief д���ݻ���     */
uint8_t g_rx_buf[BUF_LEN];    /**< \brief �����ݻ���     */

void dev1_cs_ctr (struct am_spi_device *p_dev, int state);
void buzzer_dirve (bool_t error_ocur);
void trans_once (am_spi_handle_t handle);
void trans_complete (void *p_arg,int status);

/**
 * \brief ������
 */
int main (void)
{
    am_spi_handle_t spi_handle;
    
    /* �弶��ʼ�� */
    am_board_init();
    
    /* SPI0 ��ʼ�� */
    amhw_plfm_spi0_init();
    spi_handle = amdr_spi_init(&g_spi0_dev, &g_spi0_devinfo);
    am_spi_connect(spi_handle);
    
    /* ����key���� */
    am_gpio_pin_cfg(KEY_PIN, AM_GPIO_INPUT | AM_GPIO_PULLUP);

    while (1) {
        
        /* ����key ����һ������ */
        if (am_gpio_get(KEY_PIN) == 0) {
            trans_once(spi_handle);
        }
    }
}

/* ���������� */
void buzzer_dirve (bool_t error_ocur)
{
    uint8_t count = 100;
    am_gpio_pin_cfg(BUZZER_PIN, AM_GPIO_OUTPUT | AM_GPIO_PULLUP);

    /* ���� */
    if (error_ocur) {
        
        /* һֱ���� */
        while (1) {
            am_gpio_toggle(BUZZER_PIN);
            am_mdelay(5);
        }

    } else {

        /* ������Լ 0.5s */
        for(count = 100; count > 0; count--) {
            am_gpio_toggle(BUZZER_PIN);
            am_mdelay(5);
        }
    }
}

/* ������ɻص����� */
void trans_complete (void *p_arg,int status)
{
    uint8_t i = 0;

    am_wait_t *p_wait = (am_wait_t *)p_arg;

    bool_t error_ocur = FALSE;

    if (status != AM_OK) {
        error_ocur = TRUE;
    } else {
        
        /* ������������� */
        for(i = 0; i < BUF_LEN; i++) {
            if (g_tx_buf[i] != g_rx_buf[i]) {
                error_ocur = TRUE;
                break;
            }
        }
    }

    /* �������������� */
    buzzer_dirve(error_ocur);

    am_wait_done(p_wait);
}

/* Dev1Ƭѡ���ſ��ƻص����� */
void dev1_cs_ctr (struct am_spi_device *p_dev, int state)
{
    am_gpio_pin_cfg(PIOC_8, AM_GPIO_OUTPUT);
    am_gpio_set(PIOC_8, !state);
}
 
/* ���亯�� */
void trans_once (am_spi_handle_t handle)
{
    uint8_t i;
    
    am_wait_t trans_wait;
    
    /* ��ʼ��Dev0 */
    am_spi_mkdev (&g_dev0,
                   handle,
                   8,
                   AM_SPI_MODE_1,
                   3000000,
                   PIOC_4,  /* ʹ�����ſ���Ƭѡ */
                   NULL);
                  
    /* ���ݴ���֮ǰӦʹ��am_spi_setup (am_spi_device_t *p_dev)�����豸�Ƿ�֧�� */
    if (am_spi_setup(&g_dev0) != AM_OK) {
        return;
    }
    
    for(i = 0;i < BUF_LEN; i++) {
        g_tx_buf[i] = i;
    }
    
    am_spi_mktrans(&g_trans,
                    g_tx_buf,
                    g_rx_buf,
                    BUF_LEN,
                    0,           /* Ƭѡ��Ӱ��          */
                    0,           /* ʹ��p_dev�е��ֿ�� */
                    0,           /* ����ʱ              */
                    0,           /* ʹ��Ĭ������        */
                    0,           /* �������־          */
                    trans_complete,
                   (void *)&trans_wait);
    am_spi_transfer(&g_dev0, &g_trans);
    
    am_wait_on(&trans_wait);
    
    am_mdelay(1000);
    
    for(i = 0;i < BUF_LEN; i++) {
        g_tx_buf[i] = i + 1;
    }
    
    am_spi_mktrans(&g_trans,
                    g_tx_buf,
                    g_rx_buf,
                    BUF_LEN,
                    0,                /* Ƭѡ��Ӱ��          */
                    0,                /* ʹ��p_dev�е��ֿ�� */
                    0,                /* ����ʱ              */
                    0,                /* ʹ��Ĭ������        */
                    AM_SPI_TRANS_EOT, /* ���һ������        */
                    trans_complete,
                   (void *)&trans_wait);
    am_spi_transfer(&g_dev0, &g_trans);
    
    am_wait_on(&trans_wait);
    
    am_mdelay(2000);

    /* ��ʼ��Dev1 */
    am_spi_mkdev (&g_dev1,
                   handle,
                   8,
                   AM_SPI_MODE_1,
                   3000000,
                   0,
                   dev1_cs_ctr);  /* ʹ�ûص���������Ƭѡ */
    
    if (am_spi_setup(&g_dev1) != AM_OK) {
        return;
    }
    
    for(i = 0;i < BUF_LEN; i++) {
        g_tx_buf[i] = i + 2;
    }
    
    am_spi_mktrans(&g_trans,
                    g_tx_buf,
                    g_rx_buf,
                    BUF_LEN,
                    0,               /* Ƭѡ��Ӱ��          */
                    0,               /* ʹ��p_dev�е��ֿ�� */
                    0,               /* ����ʱ              */
                    0,               /* ʹ��Ĭ������        */
                    0,               /* ���һ������        */
                    trans_complete,
                   (void *)&trans_wait);
    am_spi_transfer(&g_dev1, &g_trans);
    
    am_wait_on(&trans_wait);
    
    am_mdelay(1000);

    for(i = 0;i < BUF_LEN; i++) {
       g_tx_buf[i] = i + 3;
    }

    am_spi_mktrans(&g_trans,
                    g_tx_buf,
                    g_rx_buf,
                    BUF_LEN,
                    0,               /* Ƭѡ��Ӱ��          */
                    0,               /* ʹ��p_dev�е��ֿ�� */
                    0,               /* ����ʱ             */
                    0,               /* ʹ��Ĭ������       */
                    AM_SPI_TRANS_EOT,/* ���һ������       */
                    trans_complete,
                   (void *)&trans_wait);
    am_spi_transfer(&g_dev1, &g_trans);
    
    am_wait_on(&trans_wait);
    
    am_mdelay(2000);
}

/** [src_std_spi_master] */

/* end of file */
