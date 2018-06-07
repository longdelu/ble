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
 * \brief SPI�ӻ���ʾ���̣�ͨ��HW��Ľӿ�ʵ��
 *
 * - ��������:
 *   1. ��SPI�ӻ��ӿں������ӿڶ�Ӧ����������
 *   2. ���������ص�demo�壬�����ϵ���߸�λ��
 *
 * - ʵ������:
 *   1. �ӻ������������ݣ���ͨ�����ڽ����յ������ݴ�ӡ������
 *   2. �ӻ��������ݵ���������ͨ�����ڽ����͵����ݴ�ӡ������
 *   3. �����������֮��LED0��200msʱ������˸��
 *
 * \par Դ����
 * \snippet demo_hw_spi_slave.c src_hw_spi_slave
 *
 * \internal
 * \par History
 * - 1.00 15-10-30  liqing, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_hw_spi_slave
 * \copydoc demo_hw_spi_slave.c
 */
 
/** [src_hw_spi_slave] */
#include "ametal.h"
#include "am_board.h"
#include "am_prj_config.h"

/**
 * \brief SPI�����ų�ʼ��
 * \return  ��
 */
void spi_pin_init (void)
{
    /* ������ҪƬѡ */
    am_gpio_pin_cfg(PIOE_16, PIOE_16_SPI0_PCS0); 
    am_gpio_pin_cfg(PIOE_17, PIOE_17_SPI0_SCK);
    am_gpio_pin_cfg(PIOE_18, PIOE_18_SPI0_MOSI);
    am_gpio_pin_cfg(PIOE_19, PIOE_19_SPI0_MISO);
}


/**
 * \brief SPI�ӻ���ʼ����8bit���ݿ�� SPI_MODE1��
 */
void spi_slv_init (amhw_spi_t *p_hw_spi)
{
    /* ����SPIΪ�ӻ� */
    amhw_spi_workmode_cfg(p_hw_spi, AMHW_SPI_WMODE_SLAVE);

    /* ����8bit���ݿ�� */
    amhw_spi_feature_cfg(p_hw_spi, AMHW_SPI_CFG_8BIT_WIDTH);

    /* ����ʱ����λ�ͼ��� */
    amhw_spi_mode_cfg(p_hw_spi, AMHW_SPI_MODE_1);

    /* ʹ��SPIģ�� */
    amhw_spi_enable(p_hw_spi);
}

/**
 * \brief SPI�ӻ���������
 */
void spi_slv_send (amhw_spi_t      *p_hw_spi,
                   uint8_t         *p_buf,
                   uint32_t         count)
{
    uint32_t i = 0;
    
    for (i = 0; i < count ; i++) {

        while ((amhw_spi_stat_get(p_hw_spi) & AMHW_SPI_STAT_T_EMPTY) == 0);
        amhw_spi_data8_wirte(p_hw_spi, p_buf[i]);
    }
    
}

/**
 * \brief SPI�ӻ���������
 */
void spi_slv_recv (amhw_spi_t      *p_hw_spi,
                   uint8_t         *p_buf,
                   uint32_t         count)
{
    uint32_t i = 0;

    for (i = 0; i < count; i++) {
        while ( (amhw_spi_stat_get(p_hw_spi) & AMHW_SPI_STAT_R_FULL) == 0 );

        p_buf[i] = amhw_spi_data8_read(p_hw_spi);
    }
}


/**
 * \brief ������
 */
int main (void)
{
    uint8_t spi_slv_txbuf[32] = {0};
    uint8_t spi_slv_rxbuf[32] = {0};
    uint8_t length =32;
    uint8_t i;
    
    /* �弶��ʼ�� */
    am_board_init();
    
    spi_pin_init();
    
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_SPI0);
    
    /* SPI �ӻ���ʼ�� */
    spi_slv_init(AMHW_SPI0);

    /* ���췢�͵����� */
    for (i = 0; i < length; i++) {
        spi_slv_txbuf[i] = i;
    }

    spi_slv_recv(AMHW_SPI0, spi_slv_rxbuf, length);
    printf("\r\n--Slave receive Data:\r\n");
    for (i = 0; i<length; i++) {
        printf("The %2dst data is %2d \r\n", i, spi_slv_rxbuf[i]);
    }
    
    /* �ӻ��������ݵ����� */
    spi_slv_send(AMHW_SPI0, spi_slv_txbuf, length);
    printf("\r\n--Slave transmit Data:\r\n");
    for (i = 0; i < length; i++) {
        printf("The %2dst data is %2d \r\n", i, spi_slv_txbuf[i]);
    }
    
    while (1) {
        am_led_on(LED0);
        am_mdelay(200);
        am_led_off(LED0);
        am_mdelay(200);
    }
}

/** [src_hw_spi_slave] */

/* end of file */

