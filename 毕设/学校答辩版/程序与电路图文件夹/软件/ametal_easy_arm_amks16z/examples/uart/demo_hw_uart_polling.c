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
 * \brief UART��ѯ��ʽ�½��շ����������̣�ͨ��HW��Ľӿ�ʵ��
 *
 * - �������裺
 *   1. PIOE_1 ��������PC���ڵ�TXD;
 *   2. PIOE_0 ��������PC���ڵ�RXD��
 *
 * - ʵ������
 *   1. �������"HW example---UART test in polling mode:"��
 *   2. ����������յ����ַ�����
 *
 * \par Դ����
 * \snippet demo_hw_uart_polling.c src_hw_uart_polling
 *
 * \internal
 * \par Modification History
 * - 1.00 15-07-07  xym, first implementation.
 * \endinternal
 */
 
 /**
 * \addtogroup demo_if_hw_uart_polling
 * \copydoc demo_hw_uart_polling.c
 */
/** [src_hw_uart_polling] */
#include "ametal.h"
#include "am_board.h"

/** \brief UARTͨ��*/
#define UART_CH            AMHW_UART1

/** \brief ������ */
#define UART_BAUDRATE     (115200)
const uint8_t ch[]={"HW example---UART test in polling mode:\r\n"};
/**
 * \brief UARTʱ��ʹ�ܳ�ʼ��
 * \param[in] p_hw_uart : ָ�򴮿��豸�Ĵ����ṹ��, ��AMHW_UART0.
 */
void uart_clock_init (void *p_hw_uart)
{
    uint32_t base_addr = (uint32_t)(p_hw_uart);

    switch (base_addr) {

    /* ����0ʱ�ӳ�ʼ�� */
    case AMHW_UART0_BASE:
         amhw_sim_uart0_src_set(AMHW_SIM_UART0SRC_PLLFLLCLK);
         /* ����UART0ʱ��                  */
         amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_UART0);
        break;

    /* ����1ʱ�ӳ�ʼ�� */
    case AMHW_UART1_BASE:
         amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_UART1);
        break;

    /* ����2ʱ�ӳ�ʼ�� */
    case AMHW_UART2_BASE:
         amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_UART2);
        break;
    }
}
/**
 * \brief UART���ų�ʼ
 * \param[in] p_hw_uart : ָ�򴮿��豸�Ĵ����ṹ��, ��AMHW_UART1.
 */
void uart_pin_init (void *p_hw_uart)
{
    uint32_t base_addr = (uint32_t)(p_hw_uart);

    switch (base_addr) {

    case AMHW_UART0_BASE:
        am_gpio_pin_cfg (PIOA_1,PIOA_1_UART0_RX);
        am_gpio_pin_cfg (PIOA_2,PIOA_2_UART0_TX);
        break;

    case AMHW_UART1_BASE:
        /* ���ų�ʼ��      PIOE_1_UART1_RX  PIOE_0_UART1_TX     */
         am_gpio_pin_cfg (PIOE_1,PIOE_1_UART1_RX);
         am_gpio_pin_cfg (PIOE_0,PIOE_0_UART1_TX);
        break;

    case AMHW_UART2_BASE:
        /* ���ų�ʼ��      PIOE_17_UART2_RX  PIOE_16_UART2_TX     */
         am_gpio_pin_cfg (PIOE_17,PIOE_17_UART2_RX);
         am_gpio_pin_cfg (PIOE_16,PIOE_16_UART2_TX);
        break;
    }
}
/**
 * \brief UART��ʼ��
 */
void uart_init (void)
{
    /* ʹ�ܴ���ʱ��ģ�� */
    uart_clock_init(UART_CH);

    /* ���ô�������������� */
    uart_pin_init(UART_CH);

    /* ���ô��ڲ����� */
    amhw_uart_baudrate_set(UART_CH,
            amhw_clock_periph_freq_get((void *)(UART_CH)),
                            UART_BAUDRATE);

    amhw_uart_stop_bit_set (UART_CH, AMHW_UART_BDH_SBNS_STOP_1);
    amhw_uart_data_mode_set(UART_CH, AMHW_UART_C1_M_8BIT);
    amhw_uart_parity_set(UART_CH,  AMHW_UART_C1_PARITY_NO);

    /* ʹ�ܴ��� */
    amhw_uart_enable(UART_CH);
}

/**
 * \brief ���������
 */
int main (void)
{
    uint8_t buf_src[256];          /**< \brief ���ݻ�����       */

    /* �弶��ʼ�� */
    am_board_init();

    /* UART��ʼ�� */
    uart_init();

    amhw_uart_poll_send(UART_CH,ch, sizeof(ch));

    while (1) {
        /* �����ַ� */
        amhw_uart_poll_receive(UART_CH, buf_src, 1);
        /* ���͸ոս��յ��ַ� */
        amhw_uart_poll_send(UART_CH, buf_src, 1);
    }
}
/** [src_hw_uart_polling] */

/* end of file */
