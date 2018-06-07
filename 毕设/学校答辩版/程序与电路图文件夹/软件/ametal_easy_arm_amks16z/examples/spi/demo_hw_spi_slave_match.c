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
 * \brief SPI�ӻ�ƥ���ж���ʾ���̣�ͨ��HW��Ľӿ�ʵ��
 *
 * - ��������:
 *   1. ��SPI�ӻ��ӿں������ӿڶ�Ӧ����������
 *
 * - ʵ������:
 *   1. SPI0�ӻ�����SPI1�������ݣ�������ݱ�ƥ�䣬���ʹӻ��������������
 *   2. SPI1���������յ����������Ƿ���ȷ�������ȷ����LED������LED��˸������
 *
 * \par Դ����
 * \snippet demo_hw_spi_slave_match.c src_hw_spi_slave_match
 *
 * \internal
 * \par History
 * - 1.00 15-10-30  liqing, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_hw_spi_slave_match
 * \copydoc demo_hw_spi_slave_match.c
 */
 
/** [src_hw_spi_slave_match] */
#include "ametal.h"
#include "am_board.h"
#include "am_prj_config.h"

#define SLAVE_MATCH_DATA    0x55            /**< \brief ƥ������      */
#define DATA_LENGTH         0x5             /**< \brief �����볤��    */

/** \brief �ӻ�������    */
const uint8_t g_slv_data[DATA_LENGTH] = {  
   1, 2, 1, 3, 5
};

/** \brief �������ͻ����� */
uint8_t g_master_tx_buf[6] = {SLAVE_MATCH_DATA, 0, 0, 0, 0, 0};

/** \brief �������ջ����� */
uint8_t g_master_rx_buf[6] = {0};

void spi0_pin_init (void);
void spi1_pin_init (void);
void spi_slve_init (amhw_spi_t *p_hw_spi);
void spi_master_init (amhw_spi_t *p_hw_spi);

/**
 * \brief SPI�жϴ�����
 */
void spi_irq_handle (void *p_arg)
{
    static volatile uint8_t pos = 0;               /* ����λ��          */
    static volatile bool_t  match_irq  = TRUE;     /* ƥ���жϴ򿪱�־   */
    static volatile bool_t  send_irq   = FALSE;    /* �����жϴ򿪱�־   */

    amhw_spi_t *p_hw_spi = (amhw_spi_t*)p_arg;

    /* ƥ���ж� */
    if (match_irq && ((amhw_spi_stat_get(p_hw_spi) & AMHW_SPI_STAT_MATCH) != 0 )) {
        
        /* ��ʼ�������������� */
        p_hw_spi->s = 0x40;
        pos = 0;
        
        amhw_spi_int_disable(p_hw_spi, AMHW_SPI_IRQ_MATCH); /* �ر�ƥ���ж� */
        match_irq = FALSE;
        
        amhw_spi_int_enable(p_hw_spi, AMHW_SPI_IRQ_T);      /* ���������ж� */
        send_irq  = TRUE;
    }

    /* �����ж� */
    if (send_irq && (amhw_spi_stat_get(p_hw_spi) & AMHW_SPI_STAT_T_EMPTY) != 0 ) {
        
        /* �������� */
        if (pos < DATA_LENGTH) {
            amhw_spi_data8_read(p_hw_spi);
            amhw_spi_data8_wirte(p_hw_spi, g_slv_data[pos++]);
            
        /* �ص����״̬ */
        } else {
            p_hw_spi->s = 0x40;
            amhw_spi_matchdata16_wirte(p_hw_spi, 0x55);
            
            amhw_spi_int_enable(p_hw_spi, AMHW_SPI_IRQ_MATCH);
            match_irq = TRUE;
            
            amhw_spi_int_disable(p_hw_spi, AMHW_SPI_IRQ_T);
            send_irq  = FALSE;
        }
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
    
    /* SPI���ų�ʼ�� */
    spi0_pin_init();
    spi1_pin_init();
    
    /* ����SPIʱ�� */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_SPI0);
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_SPI1);
    
    /* SPI �ӻ���ʼ�� */
    spi_slve_init(AMHW_SPI0);
    spi_master_init(AMHW_SPI1);

    /* ����ƥ������(���������8bit������ֻ�е�8λ��Ч ) */
    amhw_spi_matchdata16_wirte(AMHW_SPI0, SLAVE_MATCH_DATA);

    /* ���ƥ���־ */
    AMHW_SPI0->s = 0x40;
    
    /* ����ƥ���ж� */
    amhw_spi_int_enable(AMHW_SPI0, AMHW_SPI_IRQ_MATCH);
    am_int_connect(INUM_SPI0, spi_irq_handle, (void*)AMHW_SPI0);
    am_int_enable(INUM_SPI0);
    
    /* �������������룬������������ */
    for (i = 0; i < (DATA_LENGTH + 1); i++) {
        while((amhw_spi_stat_get(AMHW_SPI1) & AMHW_SPI_STAT_T_EMPTY) == 0);
        amhw_spi_data8_wirte(AMHW_SPI1,g_master_tx_buf[i]);
        
        while((amhw_spi_stat_get(AMHW_SPI1) & AMHW_SPI_STAT_R_FULL) == 0);
        g_master_rx_buf[i] = amhw_spi_data8_read(AMHW_SPI1);
    }
    
    am_mdelay(1000);
    
    /* �����յ��������� */
    for (i = 0; i < DATA_LENGTH ; i++) {
        if (g_master_rx_buf[i+1] != g_slv_data[i]) {
            error_ocur = TRUE;
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
 * \brief SPI0�ӻ����ų�ʼ��
 */
void spi0_pin_init (void)
{
    /* ����ʹ��Ƭѡ,�͵�ƽ��Ч */
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
    /* ����SPIΪ�ӻ� */
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

/** [src_hw_spi_slave_match] */

/* end of file */
