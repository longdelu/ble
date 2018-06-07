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
 * \brief UART�жϷ�ʽ�½��շ����������̣�ͨ����׼�ӿ�ʵ��
 *
 * - ʵ������
 *   1. �������"STD example---UART test in interrupt mode:"��
 *   2. ����������յ����ַ�����
 *
 * \note ����û��ʹ�ô��仺����������ջ����������λ���������ֻʹ����Ϊ���պ�
 *       ���Ͷ����õ��û����ݻ����������ÿ�η��ͻ��������Ҫ���ϴη��ͻ������
 *       ����ɺ���ܽ��С�
 *
 * \par Դ����
 * \snippet demo_std_uart_int.c src_std_uart_int
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-27  xym, first implementation.
 * \endinternal
 */


/**
 * \addtogroup demo_if_std_uart_int
 * \copydoc demo_std_uart_int.c
 */

 /** [src_std_uart_int] */
#include "ametal.h"
#include "am_board.h"

/** \brief ���崮�ڣ��ж�ģʽ���豸�ṹ�壬���ڷ�����ؿռ� */
am_uart_int_dev_t  g_uart1_int_dev;

const uint8_t ch[] = {"STD example---UART test in interrupt mode:\r\n"};
/**
 * \brief ���������
 */
int main (void)
{
    uint8_t              uart1_buf[5];  /**< \brief ���ݻ�����           */
    am_uart_handle_t     uart_handle;   /**< \brief ���ڱ�׼������     */
    am_uart_int_handle_t handle;        /**< \brief �����ն˱�׼������ */

    /* �弶��ʼ��  */
    am_board_init();

    /* ƽ̨��ʼ�� */
    amhw_plfm_uart1_init();

    /* UART��ʼ�� */
    uart_handle = amdr_uart_init(&g_uart1_dev, &g_uart1_devinfo);

    /* UART��ʼ��Ϊ�ж�ģʽ */
    handle      = am_uart_int_init(&g_uart1_int_dev, uart_handle);

    am_uart_int_send(handle,ch,sizeof(ch));

    while (1) {
        /* ����һ���ַ� */
        am_uart_int_receive(handle, uart1_buf, 1);

        /* ���͸ոս��յ��ַ� */
        am_uart_int_send(handle, uart1_buf, 1);
    }
}

 /** [src_std_uart_int] */

/* end of file */
