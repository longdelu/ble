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
 * \brief WDT ι���͸�λ���ӣ�ͨ��������ʵ��
 *
 * - ʵ������
 *   1. ���Ź����ó�ʱʱ��Ϊ256ms����ʱ50ms��ι������������ִ�У�
 *      ���ڴ�ӡι����Ϣ��
 *   2. ����ִ�е���ʱ1sʱ��ϵͳ��λ���������¿�ʼִ�С�
 *
 *
 * \par Դ����
 * \snippet demo_std_wdt.c src_std_wdt
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-16  wxj, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_std_wdt
 * \copydoc demo_std_wdt.c
 */
 
/** [src_std_wdt] */
#include "ametal.h"
#include "am_board.h"

/**
 * \brief ���������
 */
int main (void)
{
    am_wdt_handle_t  handle = NULL;     /**< \brief ���Ź���׼������ */
    am_wdt_info_t    info;
    am_uart_handle_t uhandle;           /**< \brief ���ڱ�׼������ */

    /* �弶��ʼ��  */
    am_board_init();

    /* ƽ̨��ʼ��  */
    amhw_plfm_uart1_init();

    /* UART��ʼ��  */
    uhandle = amdr_uart_init(&g_uart1_dev, &g_uart1_devinfo);

    /* ��ʼ��WDT */
    handle = amdr_wdt_init(&g_wdt_dev, &g_wdt_devinfo);

    am_wdt_info_get(handle, &info);

    am_wdt_enable(handle, 256);

    am_uart_poll_send(uhandle,
                     (uint8_t *)("�ȴ�ι��...\r\n"),
                      sizeof("�ȴ�ι��...\r\n"));

    am_mdelay(50);
    am_wdt_feed(handle);

    am_uart_poll_send(uhandle,
                     (uint8_t *)("50ms��ι���ɹ���\r\n"),
                      sizeof("50ms��ι���ɹ���\r\n"));

    am_uart_poll_send(uhandle,
                     (uint8_t *)("�ȴ�ι��...\r\n"),
                      sizeof("�ȴ�ι��...\r\n"));

    am_mdelay(1000);

    /* ���ι����ʱ��������佫����ִ�� */
    am_wdt_feed(handle);

    am_uart_poll_send(uhandle,
                     (uint8_t *)("1s��ι���ɹ���\r\n"),
                      sizeof("1s��ι���ɹ���\r\n"));

    while(1) {
        ;
    }
}

/** [src_std_wdt] */
 
/* end of file */
