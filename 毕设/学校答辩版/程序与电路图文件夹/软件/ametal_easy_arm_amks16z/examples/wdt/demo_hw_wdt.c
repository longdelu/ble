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
 * \brief WDT ι���͸�λ���ӣ�ͨ��Ӳ����ʵ��
 *
 * - ʵ������
 *   1. ���Ź����ó�ʱʱ��Ϊ256ms����ʱ50ms��ι������������ִ�У�
 *      ���ڴ�ӡι����Ϣ��
 *   2. ����ִ�е���ʱ1sʱ��ι����ʱ��ϵͳ��λ���������¿�ʼִ�С�
 *
 *
 * \par Դ����
 * \snippet demo_hw_wdt.c src_hw_wdt
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-16  wxj, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_hw_wdt
 * \copydoc demo_hw_wdt.c
 */
 
/** [src_hw_wdt] */
#include "ametal.h"
#include "am_board.h"

/**
 * \brief ���������
 */
int main (void)
{
    am_uart_handle_t uhandle;          /**< \brief ���ڱ�׼������     */

    /* �弶��ʼ��  */
    am_board_init();

    /* ƽ̨��ʼ��  */
    amhw_plfm_uart1_init();

    /* UART��ʼ��  */
    uhandle = amdr_uart_init(&g_uart1_dev, &g_uart1_devinfo);

    /* ����Ϊ��ͨģʽ��ʱ��Ϊ�ڲ�1KHZ����ʱʱ��256ms */
    amhw_sim_cop_cfg(AMHW_SIM_COP_MODE_NORMAL,
                     AMHW_SIM_COP_CLK_INTERNAL,
                     AMHW_SIM_COP_TIMOUT_VALUE2);

    am_uart_poll_send(uhandle,
                     (uint8_t *)("�ȴ�ι��...\r\n"),
                      sizeof("�ȴ�ι��...\r\n"));

    am_mdelay(50);

    amhw_sim_srvcop_set(0x55);     /* ι������д0x55 */
    amhw_sim_srvcop_set(0XAA);     /* ι������д0xAA */

    am_uart_poll_send(uhandle,
                     (uint8_t *)("50ms��ι���ɹ���\r\n"),
                      sizeof("50ms��ι���ɹ���\r\n"));

    am_uart_poll_send(uhandle,
                     (uint8_t *)("�ȴ�ι��...\r\n"),
                      sizeof("�ȴ�ι��...\r\n"));

    am_mdelay(1000);

    /* ���ι����ʱ��������佫����ִ�� */
    amhw_sim_srvcop_set(0x55);     /* ι������д0x55 */
    amhw_sim_srvcop_set(0XAA);     /* ι������д0xAA */

    am_uart_poll_send(uhandle,
                     (uint8_t *)("1s��ι���ɹ���\r\n"),
                      sizeof("1s��ι���ɹ���\r\n"));

    while(1) {
        ;
    }
}

/** [src_hw_wdt] */
 
/* end of file */
