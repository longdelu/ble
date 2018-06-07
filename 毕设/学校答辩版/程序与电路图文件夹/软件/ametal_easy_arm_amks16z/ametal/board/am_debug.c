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
 * \brief AMKS16Z ���ڵ��Զ˿�
 * - ע�⣺
 *   1.��ʵ�ִ��ڴ�ӡ����,����ʹ��AM_DBG_INFO()���������Ϣ�������ʽ�� printf ���ƣ�
 *   2.ʹ��UART0ʵ�֣���λ��TX�ܽŽ���λ��UART0��RX�ܽţ���λ��RX�ܽŽ���λ��TX�ܽţ�
 *   3.AMKS16Z UART0��Ĭ�ϵ�TX�ܽ�ΪPIOA_2��Ĭ�ϵ�RX�ܽ�Ϊ PIOA_1;
 *   4.���ⲿ����Ϊ8Mʱ�������ʹ�����115200��
 *
 * \internal
 * \par Modification history
 * - 1.00 15-11-09  link, first implementation.
 * \endinternal
 */
 
#include "am_prj_config.h"
#include "am_system.h"
#include "am_debug.h"

/**
 * \brief ����֧�ֱ�׼����:printf
 */
struct __FILE {
    int handle;      /**< \brief ������� */
}; 

/**
 * \brief FILE ���� ��stdio.h�ļ��ж���
 * @{
 */
FILE __stdout;       /**< \brief ��׼��� */
FILE __stdin;        /**< \brief ��׼���� */
/** @} */

/**
 * \brief ���� _sys_exit, ���������ģʽ
 */
void _sys_exit (int x)
{ 
//    x = x;
}

/**
 * \brief fputc�����ض���
 */
int fputc (int ch, FILE *f)
{  
    while(!amhw_uart_stat1_tdre_get(AMHW_UART0)) {};
    amhw_uart_txdata_write(AMHW_UART0, (uint8_t)(ch));

    return ch;
}

/**
 * \brief fgetc�����ض���
 */
int fgetc (FILE *f)
{    
    uint8_t data  = 0;
    
    while(!amhw_uart_stat1_rdre_get(AMHW_UART0)) {};
    data = amhw_uart_rxdata_read(AMHW_UART0);

    return (int)data;
}

/**
 * \brief �����ַ���
 */
int am_debug_output_str (char *buf, size_t nbytes)
{
    uint32_t len = nbytes;

    while (len--) {
         while(!amhw_uart_stat1_tdre_get(AMHW_UART0)) {};
         amhw_uart_txdata_write(AMHW_UART0, (uint8_t)(*buf++));
    }

    return nbytes;
}

/**
 * \brief debug��ʼ��
 */
void am_debug_init (void)
{
    /* ѡ��UART0ʱ��Դ  */
    amhw_sim_uart0_src_set(AMHW_SIM_UART0SRC_PLLFLLCLK);
    /* ʹ���ſ�  */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_UART0);
    /* ���ų�ʼ��      PIOA_1_UART0_RX  PIOA_2_UART0_TX     */
    am_gpio_pin_cfg (PIOA_1,PIOA_1_UART0_RX);
    am_gpio_pin_cfg (PIOA_2,PIOA_2_UART0_TX);
    /* ���ô��ڲ����� */
    amhw_uart_baudrate_set(AMHW_UART0,
                           amdr_clk_periph_rate_get((void *)(AMHW_UART0)),
                           AM_CFG_DEBUG_BAUDRATE);
    amhw_uart_stop_bit_set (AMHW_UART0, AMHW_UART_BDH_SBNS_STOP_1);
    amhw_uart_data_mode_set(AMHW_UART0, AMHW_UART_C1_M_8BIT);
    amhw_uart_parity_set(AMHW_UART0,  AMHW_UART_C1_PARITY_NO);

    /* ʹ�ܴ��� */
    amhw_uart_enable(AMHW_UART0);
}

int am_kprintf (const char *fmt, ...)
{
    int     len;
    va_list args;

    va_start(args, fmt);
    len = vprintf(fmt, args);
    va_end(args);

    return len;
}

/* end of file */
