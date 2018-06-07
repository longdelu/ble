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
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief I2S ���ͣ��жϷ�ʽ�����̣�ͨ��HW��ӿ�ʵ��
 *
 * - �������裺
 *   1. ����I2S ������Ž���TX_D0��TX_FS��TX_BCLK���߼������ǡ�
 *   2. ���Գ���
 *
 * - ʵ������
 *   1. �������߼��������Ͽ���TX_D0��TX_FS��TX_BCLK�����ݣ�
 *   2. ����TX_D0Ϊ�������ݵĲ��룬TX_FS��֡ʱ�ӣ�TX_BCLK��λʱ�ӡ�
 *
 * \par Դ����
 * \snippet demo_hw_i2s_int.c src_hw_i2s_int
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-28 link, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_hw_i2s_int
 * \copydoc demo_hw_i2s_int.c
 */

/** [src_hw_i2s_int] */
#include "ametal.h"
#include "am_board.h"

/** \brief ʹ��I2S0 */
#define I2S_CH    AMHW_I2S0


/** \brief I2Sʱ������ */
amhw_i2s_clock_cfg_t g_i2s0_clk;

/** \brief Ҫ���͵�����   */
uint8_t g_dat = 0;

/*
 * \brief I2S ������ų�ʼ��
 */
void __i2s_pin_init (void)
{
    /** \brief ����I2S �������� */
    am_gpio_pin_cfg (PIOC_1,  PIOC_1_I2S0_TXD0);     /**< \brief TX_D0   */
    am_gpio_pin_cfg (PIOC_2,  PIOC_2_I2S0_TX_FS);    /**< \brief TX_FS   */
    am_gpio_pin_cfg (PIOC_3,  PIOC_3_I2S0_TX_BCLK);  /**< \brief TX_BCLK */
    am_gpio_pin_cfg (PIOC_4,  PIOC_4_I2S0_MCLK);     /**< \brief MCLK    */
    am_gpio_pin_cfg (PIOC_5,  PIOC_5_I2S0_RXD0);     /**< \brief RX_D0   */
    am_gpio_pin_cfg (PIOC_6,  PIOC_6_I2S0_RX_BCLK);  /**< \brief RX_BCLK */
    am_gpio_pin_cfg (PIOC_7,  PIOC_7_I2S0_RX_FS);    /**< \brief RX_FS   */
}


/**
 * \brief I2S �豸��ʼ��
 */
void __i2s_init (amhw_i2s_t *p_hw_i2s)
{
    /** \brief ʹ��I2S ʱ���ſ� */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_I2S);

    /** \brief ����I2Sʱ�� */
    g_i2s0_clk.bclk = 256000UL ;
    g_i2s0_clk.bclk_src = AMHW_I2S_BCLK_SRC_MCLK;
    g_i2s0_clk.bclk_src_freq = 4096000UL;
    g_i2s0_clk.mclk = 4096000UL;
    g_i2s0_clk.mclk_src = AMHW_I2S_MCLK_SRC_SYSCLK;
    g_i2s0_clk.mclk_src_freq = g_core_clkfreq;

	/* ���ͼĴ�����ʼ�� */
	amhw_i2s_tx_init(p_hw_i2s);
	amhw_i2s_tx_clock_setup(p_hw_i2s, &g_i2s0_clk);
	amhw_i2s_tx_sync_mode_set(p_hw_i2s, AMHW_I2S_SYNC_DISABLED);
	amhw_i2s_tx_mode_set(p_hw_i2s, AMHW_I2S_MODE_MASTER);
	amhw_i2s_tx_protocol_set(p_hw_i2s, AMHW_I2S_PROTOCOL_TYPE);
	amhw_i2s_tx_channel_mode_set(p_hw_i2s, AMHW_I2S_CHANNEL_MODE_MONO);
	amhw_i2s_tx_channel_enable(p_hw_i2s);
	amhw_i2s_tx_word_width_cfg(p_hw_i2s, AMHW_I2S_PROTOCOL_TYPE, 8);

	amhw_i2s_tx_int_enable(p_hw_i2s, AMHW_I2S_INT_REQ_FIFO_WARN |
	                                 AMHW_I2S_INT_REQ_FIFO_ERROR);
    amhw_i2s_tx_enable(p_hw_i2s);

#if 0
    /* ���ռĴ�����ʼ�� */
    amhw_i2s_rx_init(p_hw_i2s);
    amhw_i2s_rx_clock_setup(p_hw_i2s, &g_i2s0_clk);
    amhw_i2s_rx_int_disable(p_hw_i2s, AMHW_I2S_INT_REQ_ALL);
    amhw_i2s_rx_sync_mode_set(p_hw_i2s, AMHW_I2S_SYNC_SELF);
    amhw_i2s_rx_mode_set(p_hw_i2s, AMHW_I2S_MODE_MASTER);
    amhw_i2s_rx_protocol_set(p_hw_i2s, AMHW_I2S_PROTOCOL_TYPE);
    amhw_i2s_rx_channel_mode_set(p_hw_i2s, AMHW_I2S_CHANNEL_MODE_MONO);
    amhw_i2s_rx_channel_enable(p_hw_i2s);
    amhw_i2s_rx_word_width_cfg(p_hw_i2s, AMHW_I2S_PROTOCOL_TYPE, 16);
    amhw_i2s_rx_state_flag_clr(p_hw_i2s, AMHW_I2S_STATE_FLAG_ALL);
    amhw_i2s_rx_enable(p_hw_i2s);
#endif
}

void __i2s_isr_handler (void *p_var)
{
    amhw_i2s_t *p_hw_i2s = (amhw_i2s_t *)(p_var);

    /* �����жϴ��� */
    if ((amhw_i2s_tcsr_get(p_hw_i2s) & AMHW_I2S_TCSR_FEF_MASK)) {
        amhw_i2s_tx_state_flag_clr(I2S_CH, AMHW_I2S_STATE_FLAG_FIFO_ERROR);
        printf("fifo error interrupt take care !!\r\n");
    }
	if ((amhw_i2s_tcsr_get(p_hw_i2s) & AMHW_I2S_TCSR_FWF_MASK)) {
	    /* �������� */
	    amhw_i2s_txdata_write (p_hw_i2s, g_dat);
	    g_dat = g_dat + 1;
	    g_dat %= 127;
        printf("fifo warning interrupt take care !!\r\n");
	}
	/* �����жϴ��� */

}


int main (void)
{
    /* �弶��ʼ�� */
    am_board_init();

    am_int_connect(INUM_I2S, __i2s_isr_handler, I2S_CH);
    am_int_enable(INUM_I2S);

    __i2s_pin_init();

    __i2s_init(I2S_CH);

    printf("The I2S polling Demo\r\n");

    while(1){

        printf("The program is running!!\r\n");

        am_mdelay(200);

    }
}
/** [src_hw_i2s_int] */

/* end of file */
