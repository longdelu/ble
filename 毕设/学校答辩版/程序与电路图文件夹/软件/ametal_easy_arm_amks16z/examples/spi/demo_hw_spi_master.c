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
 * \brief SPI������ʾ���̣�ͨ��HW��Ľӿ�ʵ��
 *
 * - ��������:
 *   1. ��SPI��MOSI���ź�MISO��������������ģ��ӻ��豸���ػ����ԣ�
 *
 * - ʵ������:
 *   1. ����ͨ��MOSI�������ݣ����������ݴ�MOSI���أ�
 *   2. �ȽϷ����Ͷ��ص����ݣ������ͬLED0һֱ����,������200msʱ������˸��
 *
 * \par Դ����
 * \snippet demo_hw_spi_master.c src_hw_spi_master
 *
 * \internal
 * \par History
 * - 1.00 15-10-26  liqing, first implementation.
 * \endinternal
 */ 
 
/**
 * \addtogroup demo_if_hw_spi_master
 * \copydoc demo_hw_spi_master.c
 */
 
/** [src_hw_spi_master] */
#include "ametal.h"
#include "am_board.h"
#include "am_prj_config.h"

#define SPI_CS_PIN          PIOE_16     /**< \brief Ƭѡ����             */

/**
 * \name SPI����ṹ�����ò���
 */

#define SPI_CFG_LSB         AM_BIT(0)   /**< \brief ��λ���ȷ���         */
#define SPI_CFG_16BIT       AM_BIT(1)   /**< \brief �������ݿ��Ϊ16λ   */

#define SPI_CFG_MODE_0                 0            /**< \brief ģʽ0    */
#define SPI_CFG_MODE_1      (0         | AM_BIT(2)) /**< \brief ģʽ1    */
#define SPI_CFG_MODE_2      (AM_BIT(3) | 0)         /**< \brief ģʽ2    */
#define SPI_CFG_MODE_3      (AM_BIT(2) | AM_BIT(3)) /**< \brief ģʽ3    */

/** @} */

/** 
 * \brief SPI����ṹ��
 */
typedef struct spi_transfer {
    const void  *p_txbuf;           /**< \brief ָ�������ݻ����ָ��   */
    void        *p_rxbuf;           /**< \brief ָ��������ݻ����ָ��    */
    uint32_t     nbytes;            /**< \brief ���ݳ���                  */
    uint32_t     flags;             /**< \brief SPI������Ʊ�־λ         */
} spi_transfer_t;

spi_transfer_t g_spi0_transfer;     /* ����һ��SPI����ṹ��ʵ��          */

/**
 * \brief SPI����ṹ���������
 *
 * \param[in] p_trans : ָ��SPI����ṹ��
 * \param[in] p_txbuf : ָ�������ݻ����ָ��
 * \param[in] p_rxbuf : ָ��������ݻ����ָ��
 * \param[in] nbytes  : ��ǰ�������ݳ���
 * \param[in] flags   : SPI������Ʊ�־λ
 *
 * \retval  AM_OK     : �������
 */
int spi_mktrans (spi_transfer_t *p_trans,
                 const void     *p_txbuf,
                 void           *p_rxbuf,
                 uint32_t        nbytes,
                 uint16_t        flags);


/**
 * \brief SPI�ٶ�����
 *
 * \param[in] p_hw_spi     : ָ��SPI�Ĵ������ָ��
 * \param[in] target_speed : Ŀ���ٶ�
 *
 * \return  ��
 */
void spi_speed_cfg (amhw_spi_t *p_hw_spi, uint32_t target_speed);

/**
 * \brief SPI����
 *
 * \param[in] p_hw_spi : ָ��SPI�Ĵ������ָ��
 * \param[in] flags    : SPI���ñ�ʶ��AMHW_SPI_CFG_*��ֵ
 *
 * \return  ��
 */
void spi_master_cfg (amhw_spi_t *p_hw_spi, uint32_t flags);

/**
 * \brief SPIƬѡ����
 *
 * \param[in] p_hw_spi     : ָ��SPI�Ĵ������ָ��
 * \param[in] pin          : CS���ź�
 * \param[in] active_level : ������Ч״̬��ƽ
 * \param[in] state        : ״̬
 *
 * \return  ��
 */
void spi_master_cs_ctr (amhw_spi_t *p_hw_spi,
                        int         pin,
                        uint8_t     active_level,
                        bool_t      state);

/**
 * \brief SPI�ػ��������
 */
void spi_loop_trans (amhw_spi_t      *p_hw_spi,
                     spi_transfer_t  *p_trans);

/**
 * \brief ������
 */
int main (void)
{
    spi_transfer_t *p_trans = &g_spi0_transfer;

    const uint32_t length =32;
    uint16_t       spi_send_buf[16];
    uint16_t       spi_recv_buf[16];

    uint8_t i;

    bool_t error_ocur = FALSE;

    /* �弶��ʼ�� */
    am_board_init();

    /* ����ʱ�� */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_SPI0);

    /* �������� */
    am_gpio_pin_cfg(SPI_CS_PIN, AM_GPIO_OUTPUT_INIT_HIGH | AM_GPIO_PULLUP);
    am_gpio_pin_cfg(PIOE_17, PIOE_17_SPI0_SCK  | AM_GPIO_PULLUP);
    am_gpio_pin_cfg(PIOE_18, PIOE_18_SPI0_MOSI | AM_GPIO_PULLUP);
    am_gpio_pin_cfg(PIOE_19, PIOE_19_SPI0_MISO | AM_GPIO_PULLUP);

    amhw_spi_workmode_cfg(AMHW_SPI0, AMHW_SPI_WMODE_MASTER);
    spi_speed_cfg(AMHW_SPI0, 3000000);
    amhw_spi_enable(AMHW_SPI0);

    /* �������� */
    for (i = 0; i < (length / 2); i++) {
        spi_send_buf[i] = i;
    }
    spi_mktrans(p_trans,
               &spi_send_buf,
               &spi_recv_buf,
                length,
               (SPI_CFG_MODE_0 | SPI_CFG_16BIT));



    /* ���ݴ��� */
    spi_loop_trans(AMHW_SPI0, p_trans);

    /* ���ݼ��� */
    for (i = 0; (i < length / 2); i++) {
        if (spi_recv_buf[i] != spi_send_buf[i]) {
            error_ocur = TRUE;
            break;
        }
    }

    while (1) {
        if (error_ocur) {
            am_led_on(LED0);
            am_mdelay(200);
            am_led_off(LED0);
            am_mdelay(200);
        } else {
            am_led_on(LED0);
        }
    }
}


int spi_mktrans (spi_transfer_t *p_trans, 
                 const void     *p_txbuf,
                 void           *p_rxbuf,
                 uint32_t        nbytes,
                 uint16_t        flags)
                    
{
    if (p_trans == NULL) {
        return -AM_EINVAL;
    }
    
    p_trans->p_txbuf  = p_txbuf;
    p_trans->p_rxbuf  = p_rxbuf;
    p_trans->nbytes   = nbytes;
    p_trans->flags    = flags;

    return AM_OK;
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

void spi_master_cfg (amhw_spi_t *p_hw_spi, uint32_t flags)
{
    if (flags & SPI_CFG_LSB) {
        amhw_spi_feature_cfg(p_hw_spi, AMHW_SPI_CFG_LSB);
    } else {
        amhw_spi_feature_cfg(p_hw_spi, AMHW_SPI_CFG_MSB);
    }

    if (flags & SPI_CFG_16BIT) {
        amhw_spi_feature_cfg(p_hw_spi, AMHW_SPI_CFG_16BIT_WIDTH);
    } else {
        amhw_spi_feature_cfg(p_hw_spi, AMHW_SPI_CFG_8BIT_WIDTH);
    }

    /* ����ʱ����λ�ͼ��� */
    amhw_spi_mode_cfg(p_hw_spi, flags);
}

void spi_master_cs_ctr (amhw_spi_t *p_hw_spi,
                        int         pin,
                        uint8_t     active_level,
                        bool_t      state)
{
    /* Ƭѡ��Ч */
    if (state) {
        am_gpio_set(pin, active_level);
    
    /* Ƭѡ��Ч */
    } else {
        am_gpio_set(pin, !active_level);
    }
}

/**
 * \brief SPI�ػ��������
 */
void spi_loop_trans (amhw_spi_t      *p_hw_spi,
                     spi_transfer_t  *p_trans)
{
    uint32_t pos = 0;

    bool_t  width_16bit  = (p_trans->flags & SPI_CFG_16BIT) ? TRUE : FALSE;
    
    spi_master_cfg(p_hw_spi, p_trans->flags);

    spi_master_cs_ctr(p_hw_spi, SPI_CS_PIN, AM_GPIO_LEVEL_LOW, TRUE);

    while(pos < p_trans->nbytes) {

        /* �ȴ����Է��� */
        while ((amhw_spi_stat_get(p_hw_spi) & AMHW_SPI_STAT_T_EMPTY) == 0);

        if (width_16bit) {
            amhw_spi_data16_wirte(p_hw_spi, *(uint16_t*)((uint32_t)p_trans->p_txbuf + pos));
        } else {
            amhw_spi_data8_wirte(p_hw_spi, *(uint8_t*)((uint32_t)p_trans->p_txbuf + pos));
        }

        /* �ȴ����Խ��� */
        while ((amhw_spi_stat_get(p_hw_spi) & AMHW_SPI_STAT_R_FULL) == 0);

        if (width_16bit) {
            *(uint16_t*)((uint32_t)p_trans->p_rxbuf + pos) = amhw_spi_data16_read(p_hw_spi);
            pos +=2;
        } else {
            *(uint8_t*)((uint32_t)p_trans->p_rxbuf + pos) = amhw_spi_data8_read(p_hw_spi);
            pos += 1;
        }
    }
    
    /* ����Ƭѡ */
    spi_master_cs_ctr(p_hw_spi, SPI_CS_PIN, AM_GPIO_LEVEL_LOW, FALSE);
}

/** [src_hw_spi_master] */

/* end of file */
