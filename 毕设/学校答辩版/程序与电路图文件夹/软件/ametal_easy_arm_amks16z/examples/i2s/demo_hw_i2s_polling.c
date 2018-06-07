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
 * \brief I2S 发送（查询方式）例程，通过HW层接口实现
 *
 * - 操作步骤：
 *   1. 按照I2S 相关引脚接线TX_D0、TX_FS、TX_BCLK到逻辑分析仪。
 *   2. 调试程序。
 *
 * - 实验现象：
 *   1. 可以在逻辑分析仪上看到TX_D0、TX_FS、TX_BCLK的数据；
 *   2. 其中TX_D0为发送数据的补码，TX_FS是帧时钟，TX_BCLK是位时钟。
 *
 * \par 源代码
 * \snippet demo_hw_i2s_polling.c src_hw_i2s_polling
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-28 link , first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_hw_i2s_polling
 * \copydoc demo_hw_i2s_polling.c
 */

/** [src_hw_i2s_polling] */
#include "ametal.h"
#include "am_board.h"


/** \brief 使用I2S0 */
#define I2S_CH    AMHW_I2S0


/** \brief I2S时钟配置 */
amhw_i2s_clock_cfg_t g_i2s0_clk;

/** \brief I2S数据发送缓冲区  */
uint8_t g_send_buf[5] = {0,1,2,3,4};

/*
 * \brief I2S 相关引脚初始化
 */
void __i2s_pin_init (void)
{
    /** \brief 配置I2S 功能引脚 */
    am_gpio_pin_cfg (PIOC_1,  PIOC_1_I2S0_TXD0);     /**< \brief TX_D0   */
    am_gpio_pin_cfg (PIOC_2,  PIOC_2_I2S0_TX_FS);    /**< \brief TX_FS   */
    am_gpio_pin_cfg (PIOC_3,  PIOC_3_I2S0_TX_BCLK);  /**< \brief TX_BCLK */
    am_gpio_pin_cfg (PIOC_4,  PIOC_4_I2S0_MCLK);     /**< \brief MCLK    */
    am_gpio_pin_cfg (PIOC_5,  PIOC_5_I2S0_RXD0);     /**< \brief RX_D0   */
    am_gpio_pin_cfg (PIOC_6,  PIOC_6_I2S0_RX_BCLK);  /**< \brief RX_BCLK */
    am_gpio_pin_cfg (PIOC_7,  PIOC_7_I2S0_RX_FS);    /**< \brief RX_FS   */
}


/**
 * \brief I2S 设备初始化
 */
void __i2s_init (amhw_i2s_t *p_hw_i2s)
{
    /** \brief 使能I2S 时钟门控 */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_I2S);

    /** \brief 配置I2S时钟 */
    g_i2s0_clk.bclk = 256000UL ;
    g_i2s0_clk.bclk_src = AMHW_I2S_BCLK_SRC_MCLK;
    g_i2s0_clk.bclk_src_freq = 4096000UL;
    g_i2s0_clk.mclk = 4096000UL;
    g_i2s0_clk.mclk_src = AMHW_I2S_MCLK_SRC_SYSCLK;
    g_i2s0_clk.mclk_src_freq = g_core_clkfreq;

	/* 发送寄存器初始化 */
	amhw_i2s_tx_init(p_hw_i2s);
	amhw_i2s_tx_clock_setup(p_hw_i2s, &g_i2s0_clk);
	amhw_i2s_tx_sync_mode_set(p_hw_i2s, AMHW_I2S_SYNC_DISABLED);
	amhw_i2s_tx_mode_set(p_hw_i2s, AMHW_I2S_MODE_MASTER);
	amhw_i2s_tx_protocol_set(p_hw_i2s, AMHW_I2S_PROTOCOL_TYPE);
	amhw_i2s_tx_channel_mode_set(p_hw_i2s, AMHW_I2S_CHANNEL_MODE_MONO);
	amhw_i2s_tx_channel_enable(p_hw_i2s);
	amhw_i2s_tx_word_width_cfg(p_hw_i2s, AMHW_I2S_PROTOCOL_TYPE, 8);

    amhw_i2s_tx_int_disable(p_hw_i2s, AMHW_I2S_INT_REQ_FIFO_WARN |
                                      AMHW_I2S_INT_REQ_FIFO_ERROR);
    amhw_i2s_tx_enable(p_hw_i2s);

#if 0
    /* 接收寄存器初始化 */
    amhw_i2s_rx_init(p_hw_i2s);
    amhw_i2s_rx_clock_setup(p_hw_i2s, &g_i2s0_clk);
    amhw_i2s_tx_int_disable(p_hw_i2s, AMHW_I2S_INT_REQ_ALL);
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

/**
 * \brief main 函数
 */
int main (void)
{
    /* 板级初始化 */
    am_board_init();

    __i2s_pin_init();

    __i2s_init(I2S_CH);

    printf("The I2S polling Demo \r\n");

    while(1){

        /* 循环发送缓冲区的数据 */
        amhw_i2s_senddata_polling(I2S_CH, g_send_buf , 5);

    }
}
/** [src_hw_i2s_polling] */

/* end of file */
