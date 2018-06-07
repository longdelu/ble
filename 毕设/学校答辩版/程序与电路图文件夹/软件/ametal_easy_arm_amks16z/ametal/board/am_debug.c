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
 * \brief AMKS16Z 串口调试端口
 * - 注意：
 *   1.仅实现串口打印功能,可以使用AM_DBG_INFO()输出调试信息，输出格式与 printf 类似；
 *   2.使用UART0实现，上位机TX管脚接下位机UART0的RX管脚，上位机RX管脚接下位机TX管脚；
 *   3.AMKS16Z UART0的默认的TX管脚为PIOA_2，默认的RX管脚为 PIOA_1;
 *   4.当外部晶振为8M时，波特率工作在115200。
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
 * \brief 用于支持标准函数:printf
 */
struct __FILE {
    int handle;      /**< \brief 操作句柄 */
}; 

/**
 * \brief FILE 类型 在stdio.h文件中定义
 * @{
 */
FILE __stdout;       /**< \brief 标准输出 */
FILE __stdin;        /**< \brief 标准输入 */
/** @} */

/**
 * \brief 定义 _sys_exit, 避免半主机模式
 */
void _sys_exit (int x)
{ 
//    x = x;
}

/**
 * \brief fputc函数重定义
 */
int fputc (int ch, FILE *f)
{  
    while(!amhw_uart_stat1_tdre_get(AMHW_UART0)) {};
    amhw_uart_txdata_write(AMHW_UART0, (uint8_t)(ch));

    return ch;
}

/**
 * \brief fgetc函数重定义
 */
int fgetc (FILE *f)
{    
    uint8_t data  = 0;
    
    while(!amhw_uart_stat1_rdre_get(AMHW_UART0)) {};
    data = amhw_uart_rxdata_read(AMHW_UART0);

    return (int)data;
}

/**
 * \brief 发送字符串
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
 * \brief debug初始化
 */
void am_debug_init (void)
{
    /* 选择UART0时钟源  */
    amhw_sim_uart0_src_set(AMHW_SIM_UART0SRC_PLLFLLCLK);
    /* 使能门控  */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_UART0);
    /* 引脚初始化      PIOA_1_UART0_RX  PIOA_2_UART0_TX     */
    am_gpio_pin_cfg (PIOA_1,PIOA_1_UART0_RX);
    am_gpio_pin_cfg (PIOA_2,PIOA_2_UART0_TX);
    /* 设置串口波特率 */
    amhw_uart_baudrate_set(AMHW_UART0,
                           amdr_clk_periph_rate_get((void *)(AMHW_UART0)),
                           AM_CFG_DEBUG_BAUDRATE);
    amhw_uart_stop_bit_set (AMHW_UART0, AMHW_UART_BDH_SBNS_STOP_1);
    amhw_uart_data_mode_set(AMHW_UART0, AMHW_UART_C1_M_8BIT);
    amhw_uart_parity_set(AMHW_UART0,  AMHW_UART_C1_PARITY_NO);

    /* 使能串口 */
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
