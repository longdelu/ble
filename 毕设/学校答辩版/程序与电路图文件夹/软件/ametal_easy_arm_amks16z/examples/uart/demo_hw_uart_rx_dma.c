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
 * \brief UART0通过DMA方式接收数据，通过HW层的接口实现
 *
 * - 操作步骤：
 *   1. PIOE_0 引脚连接PC串口的TXD;
 *   2. PIOE_1 引脚连接PC串口的RXD;
 *   3. 通过PC一次性向MCU发送5个字符。
 *
 * - 实验现象：
 *   1. 串口打印出接收到的数据；
 *   2. 然后串口打印出“UART DMA RX IS OK! DATA IS:”；
 *   3. 打印出刚刚串口接收的5个数据。
 *
 * \par 源代码
 * \snippet demo_hw_uart_rx_dma.c src_hw_uart_rx_dma
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 15-07-10  xym, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_hw_uart_rx_dma
 * \copydoc demo_hw_uart_rx_dma.c
 */

/** [src_hw_uart_rx_dma] */
#include "ametal.h"
#include "am_board.h"

#define UART_CH            AMHW_UART1  /**< \brief UART通道     */
#define UART_BAUDRATE      (115200)    /**< \brief 波特率       */

amhw_dma_xfer_desc_t    g_desc;     /**< \brief DMA 重载描述符 */
bool_t  g_trans_done  = FALSE;      /**< \brief 传输完成标志 */
uint8_t g_buf_dst[10] = {0};        /**< \brief 目标端数据缓冲区 */

/**
 * \brief DMA中断服务程序。
 *
 * \param[in] p_arg : 用户自定义参数，通过 amdr_dma_isr_connect() 函数传递。
 * \param[in] flag  : DMA中断标志，由底层驱动传入，该参数的可能取值：
 *                    (#AMDR_DMA_INT_ERROR) 或 (#AMDR_DMA_INT_NORMAL)。

 * \return    无。
 */
void dma_isr (void *p_arg , uint8_t flag)
{
    int flag_chan  = (int)p_arg;
    if (flag == AMDR_DMA_INT_NORMAL) {
        if(flag_chan == DMA_CHAN_0) {
            g_trans_done = TRUE;
           }
     } else {
        /* 用户自定义执行代码 */
       }
}

/**
 * \brief DMA传输：乒乓模式。
 */
int dma_tran_test (void)
{

    uint32_t flags;
    uint32_t i;

    for (i = 0; i < 10; i++) {
        g_buf_dst[i] = 0;
    }

    /* 连接DMA中断服务函数 */
    amdr_dma_isr_connect(DMA_CHAN_0, dma_isr, (void *)0);

        /* DMA传输配置 */
    flags = AMHW_DMA_DCR_PER_REQUEST_ENABLE     |  /* 外设请求源使能    */
            AMHW_DMA_DCR_SINGLE_TRANSFERS       |  /* 单次传输          */
            AMHW_DMA_DCR_AUTO_ALIGN_DISABLE     |  /* 自动对齐禁能      */
            AMHW_DMA_DCR_SOURCE_SIZE_8_BIT      |  /* 源地址1字节读取   */
            AMHW_DMA_DCR_DESTINATION_SIZE_8_BIT |  /* 目的地址1字节写入 */
            AMHW_DMA_DCR_REQUEST_NO_AFFECTED    |  /* 请求无影响        */
            AMHW_DMA_DCR_NO_LINKING             |  /* 无通道连接        */
            AMHW_DMA_DCR_INTERRUTP_ENABLE       |  /* DMA中断使能       */
            AMHW_DMA_DCR_START_DISABLE    ;        /* DMA开始传输使能   */

    /* 建立通道描述符 */
    amdr_dma_xfer_desc_build(&g_desc,                        /* 通道描述符   */
                            (uint32_t)(AMHW_UART1_DAT_ADDR), /* 源端数据缓冲 */
                            (uint32_t)(g_buf_dst),           /* 目标数据缓冲 */
                            (uint32_t)5,                     /* 传输字节数   */
                             flags);                         /* 传输配置     */

    /* 启动DMA传输，马上开始传输 */
    if (amdr_dma_chan_start(&g_desc,
                            AMHW_DMA_PER_TO_MER,     /* 外设到 内存   */
                           (uint8_t)DMA_CHAN_0) == AM_ERROR) {
        return AM_ERROR;
    }

    amdr_dma_chan_cfg(DMA_CHAN_0,
                      AMHW_DMA_TRIGGER_DISABLE |     /**< \brief DMA正常模式 */
                      DMA_REQUEST_MUX0_UART1RX);     /**< \brief 请求源      */

    return AM_OK;
}

/**
 * \brief UART初始化
 */
void uart_init (void)
{
    /* 引脚初始化 */
    am_gpio_pin_cfg (PIOE_1,PIOE_1_UART1_RX);
    am_gpio_pin_cfg (PIOE_0,PIOE_0_UART1_TX);
    /* 开启UART1时钟 */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_UART1);
    /* 设置串口波特率 */
    amhw_uart_baudrate_set(UART_CH,
                           amhw_clock_periph_freq_get((void *)(UART_CH)),
                           UART_BAUDRATE);
    amhw_uart_stop_bit_set (UART_CH, AMHW_UART_BDH_SBNS_STOP_1);
    amhw_uart_data_mode_set(UART_CH, AMHW_UART_C1_M_8BIT);
    amhw_uart_parity_set(UART_CH,  AMHW_UART_C1_PARITY_NO);
    /* 使能串口 */
    amhw_uart_dma_tx_enable(UART_CH);
    amhw_uart_dma_rx_enable(UART_CH);
    /* 使能接收中断，中断信号触发DMA传输 */
    amhw_uart_int_enable(UART_CH, AMHW_UART_INT_C2_IRIE);
    amhw_uart_enable(UART_CH);

}

/**
 * \brief 主函数入口
 */
int main (void)
{

    /* 板级初始化 */
    am_board_init();

    /* 平台初始化 */
    amhw_plfm_dma_init();

    /* UART初始化 */
    uart_init();

    /* DMA初始化 */
    amdr_dma_init(&g_dma_dev, &g_dma_devinfo);

    amhw_uart_poll_send(UART_CH,(uint8_t *)"UART DMA test:\r\n",
                           (uint32_t)sizeof("UART DMA test:\r\n"));

    /* 开始DMA传输 */
    dma_tran_test();

    while(g_trans_done == FALSE);/* 等待传输完成  */

    amhw_uart_poll_send(UART_CH,(uint8_t *)"UART DMA RX IS OK! DATA IS:\r\n",
                                sizeof("UART DMA RX IS OK! DATA IS:\r\n"));
    am_mdelay(10);
    amhw_uart_poll_send(UART_CH, g_buf_dst,5);

    while (1) {
        ;
    }
}

/** [src_hw_uart_rx_dma] */

/* end of file */
