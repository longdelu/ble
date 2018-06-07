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
 * \brief UART�жϷ�ʽ�½��շ����������̣�ͨ��HW��Ľӿ�ʵ��
 *
 * - �������裺
 *   1. PIOE_1 ��������PC���ڵ�TXD;
 *   2. PIOE_0 ��������PC���ڵ�RXD��
 *    
 * - ʵ������
 *   1. �������"HW example---UART test in interrupt mode:"��
 *   2. ����������յ����ַ���
 *
 * \par Դ����
 * \snippet demo_hw_uart_int.c src_hw_uart_int 
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-20  xym, first implementation.
 * \endinternal
 */
 
 /**
 * \addtogroup demo_if_hw_uart_int
 * \copydoc demo_hw_uart_int.c
 */ 
/** [src_hw_uart_int] */
#include "ametal.h"
#include "am_board.h"

/** \brief UARTͨ��*/
#define UART_CH            AMHW_UART1

/** \brief ������ */
#define UART_BAUDRATE     (115200)

uint8_t g_buf_src[256];          /**< \brief ���ݻ�����       */
const uint8_t ch[]={"HW example---UART test in interrupt mode:\r\n"};
/**
 * \brief UARTʱ��ʹ�ܳ�ʼ��
 * \param[in] p_hw_uart : ָ�򴮿��豸�Ĵ����ṹ��, ��AMHW_UART1.
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
 * \brief UART���ų�ʼ��
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

volatile uint8_t g_tx_index  = 0;    /**< \brief ��ǰ������������        */
volatile uint8_t g_tx_nbytes = 0;    /**< \brief ��Ҫ���͵����ֽ���      */
const uint8_t   *gp_tx_buf   = NULL; /**< \brief ���ͻ�����              */

/** 
 * \brief ���ڷ������ݣ��ж�ģʽ�£��ú�������������
 *
 * \param[in] p_hw_uart : ָ������ƵĴ����豸�ṹ��
 * \param[in] p_buf     : ָ�������ݵĻ�����
 * \param[in] len       : ���͵��ֽ���
 * 
 * \retval    1         : ���;���
 * \retval    0         : ����ģ��æµ���޷�����
 * 
 * \note ���ͻ����������ȫ�ֵģ���֤���������ݷ������ǰ��������û�б��ͷŵ� 
 */
uint8_t uart_int_send(amhw_uart_t *p_hw_uart, const uint8_t *p_buf, uint32_t len) 
{
    if (g_tx_index >= g_tx_nbytes)  {   /* �ϴη����Ѿ��������     */
        gp_tx_buf   = p_buf;            /* ���¶��巢�ͻ�������     */
        g_tx_index  = 0;                /* �ӻ�������ʼ�ֽڿ�ʼ���� */
        g_tx_nbytes = len;              /* ����g_tx_nbytes���ֽ�    */
        amhw_uart_int_enable(p_hw_uart, AMHW_UART_INT_C2_TCIE);
        return 1;
    } 
    return 0;
}
/** 
 * \brief �����жϷ�����
 * \param[in] p_arg : ָ������ƵĴ����豸�ṹ�� ����am_int_connect()ע��
 */
void uart_irq_handler (void *p_arg)
{
    amhw_uart_t *p_hw_uart = (amhw_uart_t *)p_arg;
    uint32_t uart_int_stat =  amhw_uart_intstat_get(p_hw_uart);
    uint8_t data;
    
    if (uart_int_stat & AMHW_UART_INTSTAT_S1_RDRF) {
         /* ��ȡ�½������ݣ������ͳ�ȥ */
        data = amhw_uart_rxdata_read(p_hw_uart);
        amhw_uart_txdata_write(p_hw_uart, data);
    } else if (uart_int_stat & AMHW_UART_INTSTAT_S1_TDRE) {
        /* �����ж� */
        if(g_tx_index < g_tx_nbytes) {
            /* ���������д��������� */
            amhw_uart_txdata_write(p_hw_uart, gp_tx_buf[g_tx_index++]);
        } else {
            /* ������û�з������ݣ��رշ����ж� */
            amhw_uart_int_disable(p_hw_uart, AMHW_UART_INT_C2_TCIE);
        } 
    } 
}

/**
 * \brief UART��ʼ��
 */
void uart_init (void)
{

    uint8_t inum = 0;
    
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

    /* �������ͬ���ڶ�Ӧ���ж������� */
    inum = INUM_UART0 + (((uint32_t)UART_CH - AMHW_UART0_BASE)>>12) ;
    
    /* �����ж������ţ������ж� */
    am_int_connect(inum, uart_irq_handler, (void *)UART_CH);
    am_int_enable(inum); 
    
    /* ʹ�ܽ����ж� */
    amhw_uart_int_enable(UART_CH, AMHW_UART_INT_C2_IRIE);
    
    /* ʹ�ܴ��� */ 
    amhw_uart_enable(UART_CH);
}

/**
 * \brief ���������
 */
int main (void)
{
    /* �弶��ʼ�� */
    am_board_init();

    /* UART��ʼ�� */
    uart_init();
    
    uart_int_send(UART_CH, ch,sizeof(ch));
    
    while (1) {

    }
}
/** [src_hw_uart_int] */

/* end of file */
