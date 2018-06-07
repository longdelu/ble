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
 *   1. ��SPI0�ӻ��ӿں�SPI1�����ӿڶ�Ӧ����������
 *   2. ���������ص�demo�壬�����ϵ���߸�λ��
 *
 * - ʵ������:
 *   1. ����SPI1�ʹӻ�SPI0��������(SPI0ʹ���жϷ�ʽ�շ�����)��
 *   2. �Ƚ�SPI1��SPI0���ݣ����һ�µ���LED0������LED0��200msʱ������˸��
 *
 * \par Դ����
 * \snippet demo_hw_spi_slave_irq.c src_hw_spi_slave_irq
 *
 * \internal
 * \par History
 * - 1.00 15-10-30  liqing, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_hw_spi_slave_irq
 * \copydoc demo_hw_spi_slave_irq.c
 */
 
/** [src_hw_spi_slave_irq] */
#include "ametal.h"
#include "am_board.h"
#include "am_prj_config.h"


#define NUM_COUNT   32                            /**< \brief ��������С     */
uint8_t g_spi0_slve_txbuf[NUM_COUNT]   = {0};     /**< \brief �ӻ����ͻ����� */
uint8_t g_spi0_slve_rxbuf[NUM_COUNT]   = {0};     /**< \brief �ӻ����ջ����� */
uint8_t g_spi1_master_txbuf[NUM_COUNT] = {0};     /**< \brief �������ͻ����� */
uint8_t g_spi1_master_rxbuf[NUM_COUNT] = {0};     /**< \brief �������ջ����� */

void spi0_pin_init (void);
void spi1_pin_init (void);
void spi_slve_init (amhw_spi_t *p_hw_spi);
void spi_master_init (amhw_spi_t *p_hw_spi);

/**
 * \brief SPI0�ӻ��жϴ�����
 */
void spi0_irq(void *p_arg)
{
    amhw_spi_t *p_hw_spi = (amhw_spi_t*)p_arg;
    
    static uint8_t send_pos = 0;
    static uint8_t recv_pos = 0;
    
    /* �������� */
    if (amhw_spi_stat_get(p_hw_spi) & AMHW_SPI_STAT_R_FULL) {
        g_spi0_slve_rxbuf[recv_pos++] = amhw_spi_data8_read(p_hw_spi);
    }
    
    /* �������� */
    if (amhw_spi_stat_get(p_hw_spi) & AMHW_SPI_STAT_T_EMPTY) {
        amhw_spi_data8_wirte(p_hw_spi,g_spi0_slve_txbuf[send_pos++]);
    }
}


/**
 * \brief ������
 */
int main (void)
{
    uint8_t i;
    
    bool_t error_ocur = FALSE;
    
    /* �弶��ʼ�� */
    am_board_init();
    
    /* �������� */
    spi0_pin_init();
    spi1_pin_init();
    
    /* ʱ��ʹ�� */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_SPI0);
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_SPI1);
    
    /* SPI��ʼ����SPI0Ϊ�ӻ���SPI1Ϊ���� */
    spi_slve_init(AMHW_SPI0);
    spi_master_init(AMHW_SPI1);


    /* ���췢�͵����� */
    for (i = 0; i < NUM_COUNT; i++) {
        g_spi0_slve_txbuf[i]   = i * 3;
        g_spi1_master_txbuf[i] = i * 2;
    }
    
    /* SPI0�ӻ�ʹ���жϽ��պͷ������� */
    amhw_spi_int_enable(AMHW_SPI0, AMHW_SPI_IRQ_R_MODDEF);
    amhw_spi_int_enable(AMHW_SPI0, AMHW_SPI_IRQ_T);

    am_int_connect(INUM_SPI0, spi0_irq, (void*)AMHW_SPI0);
    am_int_enable(INUM_SPI0);
    
    /* SPI�����շ����� */
    for (i = 0; i < NUM_COUNT; i++) {
        while((amhw_spi_stat_get(AMHW_SPI1) & AMHW_SPI_STAT_T_EMPTY) == 0);
        amhw_spi_data8_wirte(AMHW_SPI1,g_spi1_master_txbuf[i]);
        
        while((amhw_spi_stat_get(AMHW_SPI1) & AMHW_SPI_STAT_R_FULL) == 0);
        g_spi1_master_rxbuf[i] = amhw_spi_data8_read(AMHW_SPI1);
    }
   
    /* �ȴ��������ݷ������ */
    am_mdelay(1000);
    
    /* ���ݼ��� */
   for (i = 0; i < NUM_COUNT; i++) {
        if (g_spi0_slve_txbuf[i]   != g_spi1_master_rxbuf[i] ||
            g_spi1_master_txbuf[i] != g_spi0_slve_rxbuf[i]) {
                
            error_ocur  =TRUE;
            break;   
        }
    } 

    while (1) {
        
        /* ����У�����LED��˸���� */
        if(error_ocur) {
            am_led_on(LED0);
            am_mdelay(200);
            am_led_off(LED0);
            am_mdelay(200);
        
        /* LED�Ƴ��� */
        } else {
            am_led_on(LED0);
        }

    }
}

void spi_speed_cfg (amhw_spi_t *p_hw_spi, uint32_t target_speed)
{

    uint32_t real_speed;              /* ��������ٶ�    */
    uint32_t pdiv, best_pdiv;         /* Ԥ��Ƶֵ        */
    uint32_t div, best_div;           /* ����ֵ          */
    uint32_t diff, min_diff;          /* �ٶȲ�ֵ        */
    uint32_t spi_freq;                /* SPI����ʱ��Ƶ�� */

    spi_freq = amhw_clock_periph_freq_get(p_hw_spi);

   min_diff = 0xFFFFFFFFU;

   /* ����Ϊ���ķ�Ƶֵ,�ٶ�Ϊ��С */
   best_pdiv  = 7;
   best_div   = 8;

   /* ��������ʵ��ٶ� */
   for (pdiv = 0; (pdiv <= 7) && min_diff; pdiv++)
   {
       for (div = 0; (div <= 8) && min_diff; div++)
       {
           /* all_div = (pdiv+1) * 2^(div+1) */
           real_speed = (spi_freq / ((pdiv + 1) << (div+1)));

           /* ʵ���ٶ�ӦС��Ŀ���ٶ� */
           if (target_speed >= real_speed)
           {
               diff = target_speed-real_speed;

               if (min_diff > diff)
               {
                   /* ����ʵ��ٶ� */
                   min_diff   = diff;
                   best_pdiv  = pdiv;
                   best_div   = div;
               }
           }
       }
   }

   amhw_spi_div_cfg(p_hw_spi, best_pdiv, best_div);
}

/**
 * \brief SPI0�ӻ������ų�ʼ��
 */
void spi0_pin_init (void)
{
    /* ������ҪƬѡ */
    am_gpio_pin_cfg(PIOE_16, PIOE_16_SPI0_PCS0); 
    am_gpio_pin_cfg(PIOE_17, PIOE_17_SPI0_SCK );
    am_gpio_pin_cfg(PIOE_18, PIOE_18_SPI0_MOSI);
    am_gpio_pin_cfg(PIOE_19, PIOE_19_SPI0_MISO);
}

/**
 * \brief SPI1���������ų�ʼ��
 */
void spi1_pin_init (void)
{
    am_gpio_pin_cfg(PIOD_4, PIOD_4_SPI1_PCS0); 
    am_gpio_pin_cfg(PIOD_5, PIOD_5_SPI1_SCK  | AM_GPIO_PULLUP );
    am_gpio_pin_cfg(PIOD_6, PIOD_6_SPI1_MOSI | AM_GPIO_PULLUP);
    am_gpio_pin_cfg(PIOD_7, PIOD_7_SPI1_MISO | AM_GPIO_PULLUP);
}

/**
 * \brief SPI0�ӻ���ʼ����8bit���ݿ�� SPI_MODE1��
 */
void spi_slve_init (amhw_spi_t *p_hw_spi)
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
 * \brief SPI1�ӻ���ʼ����8bit���ݿ�� SPI_MODE1��
 */
void spi_master_init (amhw_spi_t *p_hw_spi)
{
    /* ����SPIΪ���� */
    amhw_spi_workmode_cfg(p_hw_spi, AMHW_SPI_WMODE_MASTER);

    /* ����8bit���ݿ�� */
    amhw_spi_feature_cfg(p_hw_spi, AMHW_SPI_CFG_8BIT_WIDTH);

    /* ����ʱ����λ�ͼ��� */
    amhw_spi_mode_cfg(p_hw_spi, AMHW_SPI_MODE_1);
    
    /* ����CS����ΪƬѡ��� */
    amhw_spi_cs_cfg(p_hw_spi, AMHW_SPI_CS_SPI_OUT);

    /* ���ô����ٶ� */
    spi_speed_cfg(p_hw_spi, 3000000);

    /* ʹ��SPIģ�� */
    amhw_spi_enable(p_hw_spi);
}

/** [src_hw_spi_slave_irq] */

/* end of file */
