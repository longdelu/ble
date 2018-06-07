/*******************************************************************************
*                                 Apollo
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2012 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief SPI implementation
 * 
 * \internal
 * \par Modification history
 * - 1.00 14-11-01  jon, first implementation.
 * \endinternal
 */

#include "am_spi.h"
#include "am_wait.h"

/**
 * \brief SPI������ɻص�����
 */


static volatile uint8_t __g_trans_stat = AM_OK;
 
static void __transfer_complete(void *p_arg,int status)
{
    am_wait_t *p_wait = (am_wait_t *)p_arg;
    
    __g_trans_stat = status;
    
    /* �����ȴ����� */
    am_wait_done(p_wait);
}

/**
 * \brief ��д���
 */
int am_spi_write_then_read (am_spi_device_t *p_dev,
                            const uint8_t   *p_txbuf,
                            size_t           n_tx,
                            uint8_t         *p_rxbuf,
                            size_t           n_rx)
{
    am_spi_transfer_t trans;
    am_wait_t         trans_wait;
    
    if (p_dev == NULL) {
        return -AM_EINVAL;
    }
    
    if(p_txbuf == NULL && p_rxbuf == NULL) {
        return AM_OK;
    }
    
    am_wait_init(&trans_wait);
    
    if (p_txbuf != NULL) {
        if (p_rxbuf == NULL) {            /* ����Ҫ���գ����δ����ý�����־ */
            am_spi_mktrans(&trans,
                           p_txbuf,
                           NULL,
                           n_tx,
                           0,                /* Ƭѡ��Ӱ��          */
                           0,                /* ʹ��p_dev�е��ֿ�� */
                           0,                /* ����ʱ             */
                           0,                /* ʹ��Ĭ������       */
                           AM_SPI_TRANS_EOT, /* ���һ������       */
                           __transfer_complete,
                           (void *)&trans_wait);
        } else {
            am_spi_mktrans(&trans,
                           p_txbuf,
                           NULL,
                           n_tx,
                           0,               /* Ƭѡ��Ӱ��          */
                           0,               /* ʹ��p_dev�е��ֿ�� */
                           0,               /* ����ʱ             */
                           0,               /* ʹ��Ĭ������       */
                           0,               /* �������־         */
                           __transfer_complete,
                          (void *)&trans_wait);
        }
            
        am_spi_transfer(p_dev, &trans);
        am_wait_on(&trans_wait);

        if (__g_trans_stat != AM_OK) {
            return __g_trans_stat;
        }
    }

    if (p_rxbuf == NULL) {
        return AM_OK;
    }
    
    am_spi_mktrans(&trans,
                   NULL,
                   p_rxbuf,
                   n_rx,
                   0,                /* Ƭѡ��Ӱ��          */
                   0,                /* ʹ��p_dev�е��ֿ�� */
                   0,                /* ����ʱ             */
                   0,                /* ʹ��Ĭ������       */
                   AM_SPI_TRANS_EOT, /* ���һ������       */
                   __transfer_complete,
                   (void *)&trans_wait);
    
    am_spi_transfer(p_dev, &trans);
    am_wait_on(&trans_wait);
    
    if (__g_trans_stat != AM_OK) {
        return __g_trans_stat;
    }
    
    return AM_OK;
}

/**
 * \brief ִ������д����
 */
int am_spi_write_then_write (am_spi_device_t *p_dev,
                             const uint8_t   *p_txbuf0,
                             size_t           n_tx0,
                             const uint8_t   *p_txbuf1,
                             size_t           n_tx1)
                                 
{
    am_spi_transfer_t trans;
    am_wait_t         trans_wait;
    
    if (p_dev == NULL) {
        return -AM_EINVAL;
    }
    
    if (p_txbuf0 == NULL && p_txbuf1 == NULL) {
        return AM_OK;
    }
    am_wait_init(&trans_wait);
    
    if (p_txbuf0 != NULL) {
    
        if (p_txbuf1 == NULL) {          /* ����Ҫ���ͻ�����1����Ҫ�ý�����־ */
            am_spi_mktrans(&trans,
                           p_txbuf0,
                           NULL,
                           n_tx0,
                           0,                /* Ƭѡ��Ӱ��          */
                           0,                /* ʹ��p_dev�е��ֿ�� */
                           0,                /* ����ʱ             */
                           0,                /* ʹ��Ĭ������       */
                           AM_SPI_TRANS_EOT, /* ���һ������       */
                           __transfer_complete,
                           (void *)&trans_wait);
        } else {
            am_spi_mktrans(&trans,
                           p_txbuf0,
                           NULL,
                           n_tx0,
                           0,                /* Ƭѡ��Ӱ��          */
                           0,                /* ʹ��p_dev�е��ֿ�� */
                           0,                /* ����ʱ              */
                           0,                /* ʹ��Ĭ������        */
                           0,                /* �����⴫���־      */
                           __transfer_complete,
                          (void *)&trans_wait);
        }

        am_spi_transfer(p_dev, &trans);
        am_wait_on(&trans_wait);
        if (__g_trans_stat != AM_OK) {
            return __g_trans_stat;
        }        
    }
    
    if (p_txbuf1 == NULL) {
        return AM_OK;
    }
    
    am_spi_mktrans(&trans,
                   p_txbuf1,
                   NULL,
                   n_tx1,
                   0,                /* Ƭѡ��Ӱ��          */
                   0,                /* ʹ��p_dev�е��ֿ�� */
                   0,                /* ����ʱ             */
                   0,                /* ʹ��Ĭ������       */
                   AM_SPI_TRANS_EOT, /* ���һ������       */
                   __transfer_complete,
                   (void *)&trans_wait);
    
    am_spi_transfer(p_dev, &trans);
    am_wait_on(&trans_wait);

    if (__g_trans_stat != AM_OK) {
        return __g_trans_stat;
    }
    
    return AM_OK;
}

/* end of file */
