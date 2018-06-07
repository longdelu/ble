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
 * \brief UART0ͨ��DMA��ʽ�������ݣ�ͨ��HW��Ľӿ�ʵ��
 *
 * - �������裺
 *   1. PIOE_0 ��������PC���ڵ�TXD;
 *   2. PIOE_1 ��������PC���ڵ�RXD;
 *   3. ͨ��PCһ������MCU����5���ַ���
 *
 * - ʵ������
 *   1. ���ڴ�ӡ�����յ������ݣ�
 *   2. Ȼ�󴮿ڴ�ӡ����UART DMA RX IS OK! DATA IS:����
 *   3. ��ӡ���ոմ��ڽ��յ�5�����ݡ�
 *
 * \par Դ����
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

#define UART_CH            AMHW_UART1  /**< \brief UARTͨ��     */
#define UART_BAUDRATE      (115200)    /**< \brief ������       */

amhw_dma_xfer_desc_t    g_desc;     /**< \brief DMA ���������� */
bool_t  g_trans_done  = FALSE;      /**< \brief ������ɱ�־ */
uint8_t g_buf_dst[10] = {0};        /**< \brief Ŀ������ݻ����� */

/**
 * \brief DMA�жϷ������
 *
 * \param[in] p_arg : �û��Զ��������ͨ�� amdr_dma_isr_connect() �������ݡ�
 * \param[in] flag  : DMA�жϱ�־���ɵײ��������룬�ò����Ŀ���ȡֵ��
 *                    (#AMDR_DMA_INT_ERROR) �� (#AMDR_DMA_INT_NORMAL)��

 * \return    �ޡ�
 */
void dma_isr (void *p_arg , uint8_t flag)
{
    int flag_chan  = (int)p_arg;
    if (flag == AMDR_DMA_INT_NORMAL) {
        if(flag_chan == DMA_CHAN_0) {
            g_trans_done = TRUE;
           }
     } else {
        /* �û��Զ���ִ�д��� */
       }
}

/**
 * \brief DMA���䣺ƹ��ģʽ��
 */
int dma_tran_test (void)
{

    uint32_t flags;
    uint32_t i;

    for (i = 0; i < 10; i++) {
        g_buf_dst[i] = 0;
    }

    /* ����DMA�жϷ����� */
    amdr_dma_isr_connect(DMA_CHAN_0, dma_isr, (void *)0);

        /* DMA�������� */
    flags = AMHW_DMA_DCR_PER_REQUEST_ENABLE     |  /* ��������Դʹ��    */
            AMHW_DMA_DCR_SINGLE_TRANSFERS       |  /* ���δ���          */
            AMHW_DMA_DCR_AUTO_ALIGN_DISABLE     |  /* �Զ��������      */
            AMHW_DMA_DCR_SOURCE_SIZE_8_BIT      |  /* Դ��ַ1�ֽڶ�ȡ   */
            AMHW_DMA_DCR_DESTINATION_SIZE_8_BIT |  /* Ŀ�ĵ�ַ1�ֽ�д�� */
            AMHW_DMA_DCR_REQUEST_NO_AFFECTED    |  /* ������Ӱ��        */
            AMHW_DMA_DCR_NO_LINKING             |  /* ��ͨ������        */
            AMHW_DMA_DCR_INTERRUTP_ENABLE       |  /* DMA�ж�ʹ��       */
            AMHW_DMA_DCR_START_DISABLE    ;        /* DMA��ʼ����ʹ��   */

    /* ����ͨ�������� */
    amdr_dma_xfer_desc_build(&g_desc,                        /* ͨ��������   */
                            (uint32_t)(AMHW_UART1_DAT_ADDR), /* Դ�����ݻ��� */
                            (uint32_t)(g_buf_dst),           /* Ŀ�����ݻ��� */
                            (uint32_t)5,                     /* �����ֽ���   */
                             flags);                         /* ��������     */

    /* ����DMA���䣬���Ͽ�ʼ���� */
    if (amdr_dma_chan_start(&g_desc,
                            AMHW_DMA_PER_TO_MER,     /* ���赽 �ڴ�   */
                           (uint8_t)DMA_CHAN_0) == AM_ERROR) {
        return AM_ERROR;
    }

    amdr_dma_chan_cfg(DMA_CHAN_0,
                      AMHW_DMA_TRIGGER_DISABLE |     /**< \brief DMA����ģʽ */
                      DMA_REQUEST_MUX0_UART1RX);     /**< \brief ����Դ      */

    return AM_OK;
}

/**
 * \brief UART��ʼ��
 */
void uart_init (void)
{
    /* ���ų�ʼ�� */
    am_gpio_pin_cfg (PIOE_1,PIOE_1_UART1_RX);
    am_gpio_pin_cfg (PIOE_0,PIOE_0_UART1_TX);
    /* ����UART1ʱ�� */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_UART1);
    /* ���ô��ڲ����� */
    amhw_uart_baudrate_set(UART_CH,
                           amhw_clock_periph_freq_get((void *)(UART_CH)),
                           UART_BAUDRATE);
    amhw_uart_stop_bit_set (UART_CH, AMHW_UART_BDH_SBNS_STOP_1);
    amhw_uart_data_mode_set(UART_CH, AMHW_UART_C1_M_8BIT);
    amhw_uart_parity_set(UART_CH,  AMHW_UART_C1_PARITY_NO);
    /* ʹ�ܴ��� */
    amhw_uart_dma_tx_enable(UART_CH);
    amhw_uart_dma_rx_enable(UART_CH);
    /* ʹ�ܽ����жϣ��ж��źŴ���DMA���� */
    amhw_uart_int_enable(UART_CH, AMHW_UART_INT_C2_IRIE);
    amhw_uart_enable(UART_CH);

}

/**
 * \brief ���������
 */
int main (void)
{

    /* �弶��ʼ�� */
    am_board_init();

    /* ƽ̨��ʼ�� */
    amhw_plfm_dma_init();

    /* UART��ʼ�� */
    uart_init();

    /* DMA��ʼ�� */
    amdr_dma_init(&g_dma_dev, &g_dma_devinfo);

    amhw_uart_poll_send(UART_CH,(uint8_t *)"UART DMA test:\r\n",
                           (uint32_t)sizeof("UART DMA test:\r\n"));

    /* ��ʼDMA���� */
    dma_tran_test();

    while(g_trans_done == FALSE);/* �ȴ��������  */

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
