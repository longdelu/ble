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
 * \brief UART���λ�������ʽ�½��շ����������̣�ͨ����׼�ӿ�ʵ��
 *
 *
 * - ʵ������
 *   1. �������"UART interrupt mode(Add ring buffer) test:"��
 *   2. ����������յ����ַ�����
 *
 * \par Դ����
 * \snippet demo_std_uart_ringbuf.c src_std_uart_ringbuf
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-27  xym, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_uart_ringbuf
 * \copydoc demo_std_uart_ringbuf.c
 */

/** [src_std_uart_ringbuf] */
#include "ametal.h"
#include "am_board.h"


#define UART1_RX_BUF_SIZE  128  /**< \brief ���ջ��λ�������С��Ӧ��Ϊ2^n  */
#define UART1_TX_BUF_SIZE  128  /**< \brief ���ͻ��λ�������С��Ӧ��Ϊ2^n  */

static char uart1_rxbuf[UART1_RX_BUF_SIZE];
static char uart1_txbuf[UART1_TX_BUF_SIZE];
const uint8_t ch[]={"UART interrupt mode(Add ring buffer) test:\r\n"};
am_uart_ringbuf_dev_t  g_uart1_ringbuf_dev;

/**
 * \brief ���������
 */
int main (void)
{
    uint8_t                  uart1_buf[5]; /**< \brief ���ݻ�����            */
    am_uart_handle_t         uart_handle;  /**< \brief ���ڱ�׼������      */
    am_uart_ringbuf_handle_t handle;       /**< \brief ���ڻ��λ�����������*/

    /* �弶��ʼ��  */
    am_board_init();

    /* ƽ̨��ʼ�� */
    amhw_plfm_uart1_init();

    /* UART��ʼ��    */
    uart_handle = amdr_uart_init(&g_uart1_dev, &g_uart1_devinfo);

    /* UART��ʼ��Ϊ���λ�����ģʽ */
    handle = am_uart_ringbuf_init(&g_uart1_ringbuf_dev,
                                   uart_handle,
                                   uart1_rxbuf,
                                   UART1_RX_BUF_SIZE,
                                   uart1_txbuf,
                                   UART1_TX_BUF_SIZE);

    am_uart_ringbuf_send(handle,ch,sizeof(ch));

    while (1) {

        /* ����һ���ַ� */
        am_uart_ringbuf_receive(handle, uart1_buf, 1);

        /* ���͸ոս��յ��ַ� */
        am_uart_ringbuf_send(handle, uart1_buf, 1);

    }
}

/** [src_std_uart_ringbuf] */

/* end of file */
