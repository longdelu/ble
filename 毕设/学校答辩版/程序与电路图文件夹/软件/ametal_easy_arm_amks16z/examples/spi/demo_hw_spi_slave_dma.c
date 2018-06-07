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
 * \brief SPI�ӻ�ͨ��DMA�������������ݣ�ͨ��HW��Ľӿ�ʵ��
 *
 * - ��������:
 *   1. ��SPI�ӻ��ӿں�SPI�����ӿ�����������
 *   2. ���������ص�demo�壬�����ϵ���߸�λ��
 *
 * - ʵ������:
 *   1. �����������ݸ��ӻ����ӻ�ͨ��DMA�������ݣ�
 *   2. �ӻ�������յ����ݣ������ȷ������LED������LED��˸������
 *
 * \par Դ����
 * \snippet demo_hw_spi_slave_dma.c src_hw_spi_slave_dma
 *
 * \internal
 * \par History
 * - 1.00 15-10-30  liqng, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_hw_spi_slave_dma
 * \copydoc demo_hw_spi_slave_dma.c
 */
 
/** [src_hw_spi_slave_dma] */
#include "ametal.h"
#include "am_board.h"
#include "am_prj_config.h"

#define BUF_LEN     32           /**< \brief ����������      */
uint8_t g_buf_recv[BUF_LEN];     /**< \brief ���ջ�����      */
uint8_t g_buf_send[BUF_LEN];     /**< \brief ���ͻ�����      */

bool_t g_dma_err_occur = FALSE;  /**< \brief DMA��������ʶ */
bool_t g_recv_done     = FALSE;  /**< \brief ������ɱ�־    */

void spi0_pin_init (void);
void spi1_pin_init (void);
void spi_slve_init (amhw_spi_t *p_hw_spi);
void spi_master_init (amhw_spi_t *p_hw_spi);

/**
 * \brief DMA�жϷ������
 */
void dma_isr (void *p_arg , uint8_t flag)
{
    if(flag == AMDR_DMA_INT_ERROR) {
        g_dma_err_occur = TRUE;     /* DMA����     */
    } else if(flag == AMDR_DMA_INT_NORMAL) {
        g_recv_done = TRUE;         /* DMA������� */
    }
}

/**
 * \brief SPI0�ӻ�DMA��������
 */
int spi0_dma_recv_cfg(amhw_spi_t *p_hw_spi)
{
    uint32_t flags;

    int state;

    amhw_dma_xfer_desc_t xfer_desc; /* DAMͨ�������� */

    amdr_dma_chan_cfg(DMA_CHAN_0,
                      AMHW_DMA_TRIGGER_DISABLE |    /**< \brief DMA����ģʽ   */
                      DMA_REQUEST_MUX0_SPI0RX);     /**< \brief ����Դ        */


    /* ����DMA�жϷ����� */
    amdr_dma_isr_connect(DMA_CHAN_0, dma_isr, (void *)0);

    /* DMA�������� */
    flags = AMHW_DMA_DCR_INTERRUTP_ENABLE        |  /* DMA�ж�ʹ��            */
            AMHW_DMA_DCR_PER_REQUEST_ENABLE      |  /* ��������ʹ��           */
            AMHW_DMA_DCR_SINGLE_TRANSFERS        |  /* ���δ���               */
            AMHW_DMA_DCR_SOURCE_SIZE_8_BIT       |  /* Դ��ַ1�ֽڶ�ȡ        */
            AMHW_DMA_DCR_DESTINATION_SIZE_8_BIT  |  /* Ŀ�ĵ�ַ1�ֽ�д��      */
            AMHW_DMA_DCR_REQUEST_AFFECTED        |  /* ������Ӱ��             */
            AMHW_DMA_DCR_START_DISABLE    ;         /* DMA�������ʹ��        */

    /* ����ͨ�������� */
    amdr_dma_xfer_desc_build(&xfer_desc,                  /* ͨ��������       */
                             (uint32_t)(&(p_hw_spi->dl)), /* Դ�������׵�ַ   */
                             (uint32_t)(g_buf_recv),      /* Ŀ�Ļ������׵�ַ */
                              BUF_LEN,                    /* �����ֽ���       */
                              flags);                     /* ��������         */

    /* ����DMA���䣬���Ͽ�ʼ���� */
    state = amdr_dma_chan_start(&xfer_desc, AMHW_DMA_PER_TO_MER, DMA_CHAN_0);

    if ( state == AM_ERROR) {
        return AM_ERROR;
    }

    return AM_OK;
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
    
    /* DMA��س�ʼ�� */
    amhw_plfm_dma_init();
    amdr_dma_init(&g_dma_dev, &g_dma_devinfo);
    
    /* SPI���ų�ʼ�� */
    spi0_pin_init();
    spi1_pin_init();
    
    /* ����SPIʱ�� */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_SPI0);
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_SPI1);
    
    /* SPI��ʼ�� */
    spi_slve_init(AMHW_SPI0);
    spi_master_init(AMHW_SPI1);
    
    /* SPI0����DMA���� */
    spi0_dma_recv_cfg(AMHW_SPI0);
    
    /* ������������ */
    for (i = 0; i < BUF_LEN; i++) {
        
        g_buf_send[i] = i;
        while((amhw_spi_stat_get(AMHW_SPI1) & AMHW_SPI_STAT_T_EMPTY) == 0);
        amhw_spi_data8_wirte(AMHW_SPI1,g_buf_send[i]);
    }
    
    am_mdelay(1000);
    
    while (1) {

        /* ������� */
        if (g_recv_done) {
            
            /* �����յ��������� */
            for (i = 0; i < BUF_LEN; i++) {
                if (g_buf_send[i] != g_buf_recv[i]) {
                    error_ocur = TRUE;
                    break;
                }
            }
            
            break;
        }

        /* DMA ���� */
        if (g_dma_err_occur) {
            error_ocur = FALSE;
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
 * \brief SPI0�ӻ���ʼ��(8bit���ݿ�� SPI_MODE1,ʹ��DMA����)
 */
void spi_slve_init (amhw_spi_t *p_hw_spi)
{
    /* ����SPIΪ�ӻ� */
    amhw_spi_workmode_cfg(p_hw_spi, AMHW_SPI_WMODE_SLAVE);

    /* ����8bit���ݿ�ȣ�ʹ��DMA���� */
    amhw_spi_feature_cfg(p_hw_spi, AMHW_SPI_CFG_8BIT_WIDTH | AMHW_SPI_CFG_RDMA_EN);

    /* ����ʱ����λ�ͼ��� */
    amhw_spi_mode_cfg(p_hw_spi, AMHW_SPI_MODE_1);

    /* ʹ��SPIģ�� */
    amhw_spi_enable(p_hw_spi);
}

/**
 * \brief SPI1�ӻ���ʼ��(8bit���ݿ�� SPI_MODE1)
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

/** [src_hw_spi_slave_dma] */

/* end of file */
