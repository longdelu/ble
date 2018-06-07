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
 * \brief SPI从机演示例程，通过HW层的接口实现
 *
 * - 操作步骤:
 *   1. 将SPI从机接口和主机接口对应连接起来；
 *   2. 将程序下载到demo板，重新上电或者复位。
 *
 * - 实验现象:
 *   1. 从机接收主机数据，并通过串口将接收到的数据打印出来；
 *   2. 从机发送数据到主机，并通过串口将发送的数据打印出来；
 *   3. 上述操作完成之后，LED0以200ms时间间隔闪烁。
 *
 * \par 源代码
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
 * \brief SPI的引脚初始化
 * \return  无
 */
void spi_pin_init (void)
{
    /* 必须需要片选 */
    am_gpio_pin_cfg(PIOE_16, PIOE_16_SPI0_PCS0); 
    am_gpio_pin_cfg(PIOE_17, PIOE_17_SPI0_SCK);
    am_gpio_pin_cfg(PIOE_18, PIOE_18_SPI0_MOSI);
    am_gpio_pin_cfg(PIOE_19, PIOE_19_SPI0_MISO);
}


/**
 * \brief SPI从机初始化（8bit数据宽度 SPI_MODE1）
 */
void spi_slv_init (amhw_spi_t *p_hw_spi)
{
    /* 配置SPI为从机 */
    amhw_spi_workmode_cfg(p_hw_spi, AMHW_SPI_WMODE_SLAVE);

    /* 配置8bit数据宽度 */
    amhw_spi_feature_cfg(p_hw_spi, AMHW_SPI_CFG_8BIT_WIDTH);

    /* 配置时钟相位和极性 */
    amhw_spi_mode_cfg(p_hw_spi, AMHW_SPI_MODE_1);

    /* 使能SPI模块 */
    amhw_spi_enable(p_hw_spi);
}

/**
 * \brief SPI从机发送数据
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
 * \brief SPI从机接收数据
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
 * \brief 主函数
 */
int main (void)
{
    uint8_t spi_slv_txbuf[32] = {0};
    uint8_t spi_slv_rxbuf[32] = {0};
    uint8_t length =32;
    uint8_t i;
    
    /* 板级初始化 */
    am_board_init();
    
    spi_pin_init();
    
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_SPI0);
    
    /* SPI 从机初始化 */
    spi_slv_init(AMHW_SPI0);

    /* 构造发送的数据 */
    for (i = 0; i < length; i++) {
        spi_slv_txbuf[i] = i;
    }

    spi_slv_recv(AMHW_SPI0, spi_slv_rxbuf, length);
    printf("\r\n--Slave receive Data:\r\n");
    for (i = 0; i<length; i++) {
        printf("The %2dst data is %2d \r\n", i, spi_slv_rxbuf[i]);
    }
    
    /* 从机发送数据到主机 */
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

