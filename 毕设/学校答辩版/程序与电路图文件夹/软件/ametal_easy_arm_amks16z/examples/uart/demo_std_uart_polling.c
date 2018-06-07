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
 * \brief USART��ѯ��ʽ�½��շ����������̣�ͨ����׼�ӿ�ʵ��
 *
 * - ʵ������
 *   1. UART1_RX->PTE1  UART1_TX->PTE0 
 *   2. �������"USART test in polling mode:"��
 *   3. ����������յ����ַ�����
 *
 * \note ������Դ���ʹ���뱾������ͬ����Ӧ�ں�������ʹ�õ�����Ϣ�������
 *      ���磺AM_DBG_INFO()��
 *
 * \par Դ����
 * \snippet demo_std_uart_polling.c src_std_uart_polling 
 *
 * \internal
 * \par Modification History
 * - 1.00 15-07-06  xym, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_std_uart_polling
 * \copydoc demo_std_uart_polling.c
 */
 
 /** [src_std_uart_polling] */
#include "ametal.h"
#include "am_board.h"
const uint8_t ch[]={"STD-UART test in polling mode:\r\n"};
/**
 * \brief ���������
 */

int main (void)
{
    uint8_t          uart1_buf[5];    /**< \brief ���ݻ�����           */
    am_uart_handle_t handle;          /**< \brief ���ڱ�׼������     */
    /* �弶��ʼ��  */
    am_board_init();
    /* ƽ̨��ʼ��  */
    amhw_plfm_uart1_init();
    /* UART��ʼ��  */
    handle = amdr_uart_init(&g_uart1_dev, &g_uart1_devinfo);

    am_uart_poll_send(handle,ch,sizeof(ch));

    while (1) {

        /* �����ַ� */
        am_uart_poll_receive(handle, uart1_buf, 1);
        /* ���͸ոս��յ��ַ� */
        am_uart_poll_send(handle, uart1_buf, 1);

    }
}


 /** [src_std_uart_polling] */
 
/* end of file */

