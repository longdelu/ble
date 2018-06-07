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
 * \brief UART中断方式下接收发送数据例程，通过HW层的接口实现
 *
 * - 操作步骤：
 *   1. PIOE_1 引脚连接PC串口的TXD;
 *   2. PIOE_0 引脚连接PC串口的RXD。
 *    
 * - 实验现象：
 *   1. 串口输出"HW example---UART test in interrupt mode:"；
 *   2. 串口输出接收到的字符。
 *
 * \par 源代码
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

/** \brief UART通道*/
#define UART_CH            AMHW_UART1

/** \brief 波特率 */
#define UART_BAUDRATE     (115200)

uint8_t g_buf_src[256];          /**< \brief 数据缓冲区       */
const uint8_t ch[]={"HW example---UART test in interrupt mode:\r\n"};
/**
 * \brief UART时钟使能初始化
 * \param[in] p_hw_uart : 指向串口设备寄存器结构体, 如AMHW_UART1.
 */
void uart_clock_init (void *p_hw_uart)
{
    uint32_t base_addr = (uint32_t)(p_hw_uart);

    switch (base_addr) {

    /* 串口0时钟初始化 */
    case AMHW_UART0_BASE:
         amhw_sim_uart0_src_set(AMHW_SIM_UART0SRC_PLLFLLCLK);
         /* 开启UART0时钟                  */
         amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_UART0);
        break;

    /* 串口1时钟初始化 */
    case AMHW_UART1_BASE:
         amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_UART1);
        break;

    /* 串口2时钟初始化 */
    case AMHW_UART2_BASE:
         amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_UART2);
        break;
    }
}
/**
 * \brief UART引脚初始化
 * \param[in] p_hw_uart : 指向串口设备寄存器结构体, 如AMHW_UART1.
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
        /* 引脚初始化      PIOE_1_UART1_RX  PIOE_0_UART1_TX     */
         am_gpio_pin_cfg (PIOE_1,PIOE_1_UART1_RX);
         am_gpio_pin_cfg (PIOE_0,PIOE_0_UART1_TX);
        break;

    case AMHW_UART2_BASE:
        /* 引脚初始化      PIOE_17_UART2_RX  PIOE_16_UART2_TX     */
         am_gpio_pin_cfg (PIOE_17,PIOE_17_UART2_RX);
         am_gpio_pin_cfg (PIOE_16,PIOE_16_UART2_TX);
        break;
    }
}

volatile uint8_t g_tx_index  = 0;    /**< \brief 当前发送数据索引        */
volatile uint8_t g_tx_nbytes = 0;    /**< \brief 需要发送的总字节数      */
const uint8_t   *gp_tx_buf   = NULL; /**< \brief 发送缓冲区              */

/** 
 * \brief 串口发送数据（中断模式下，该函数不会阻塞）
 *
 * \param[in] p_hw_uart : 指向需控制的串口设备结构体
 * \param[in] p_buf     : 指向发送数据的缓冲区
 * \param[in] len       : 发送的字节数
 * 
 * \retval    1         : 发送就绪
 * \retval    0         : 发送模块忙碌，无法发送
 * 
 * \note 发送缓冲区最好是全局的，保证在所有数据发送完成前，缓冲区没有被释放掉 
 */
uint8_t uart_int_send(amhw_uart_t *p_hw_uart, const uint8_t *p_buf, uint32_t len) 
{
    if (g_tx_index >= g_tx_nbytes)  {   /* 上次发送已经发送完成     */
        gp_tx_buf   = p_buf;            /* 重新定义发送缓冲器区     */
        g_tx_index  = 0;                /* 从缓存区起始字节开始发送 */
        g_tx_nbytes = len;              /* 发送g_tx_nbytes个字节    */
        amhw_uart_int_enable(p_hw_uart, AMHW_UART_INT_C2_TCIE);
        return 1;
    } 
    return 0;
}
/** 
 * \brief 串口中断服务函数
 * \param[in] p_arg : 指向需控制的串口设备结构体 ，在am_int_connect()注册
 */
void uart_irq_handler (void *p_arg)
{
    amhw_uart_t *p_hw_uart = (amhw_uart_t *)p_arg;
    uint32_t uart_int_stat =  amhw_uart_intstat_get(p_hw_uart);
    uint8_t data;
    
    if (uart_int_stat & AMHW_UART_INTSTAT_S1_RDRF) {
         /* 获取新接收数据，并发送出去 */
        data = amhw_uart_rxdata_read(p_hw_uart);
        amhw_uart_txdata_write(p_hw_uart, data);
    } else if (uart_int_stat & AMHW_UART_INTSTAT_S1_TDRE) {
        /* 发送中断 */
        if(g_tx_index < g_tx_nbytes) {
            /* 缓冲区还有待发送数据 */
            amhw_uart_txdata_write(p_hw_uart, gp_tx_buf[g_tx_index++]);
        } else {
            /* 缓冲区没有发送数据，关闭发送中断 */
            amhw_uart_int_disable(p_hw_uart, AMHW_UART_INT_C2_TCIE);
        } 
    } 
}

/**
 * \brief UART初始化
 */
void uart_init (void)
{

    uint8_t inum = 0;
    
    /* 使能串口时钟模块 */
    uart_clock_init(UART_CH);
    
    /* 配置串口输入输出引脚 */
    uart_pin_init(UART_CH);

    /* 设置串口波特率 */
    amhw_uart_baudrate_set(UART_CH,
                           amhw_clock_periph_freq_get((void *)(UART_CH)),
                           UART_BAUDRATE);

    amhw_uart_stop_bit_set (UART_CH, AMHW_UART_BDH_SBNS_STOP_1);
    amhw_uart_data_mode_set(UART_CH, AMHW_UART_C1_M_8BIT);
    amhw_uart_parity_set(UART_CH,  AMHW_UART_C1_PARITY_NO);

    /* 计算出不同串口对应的中断向量号 */
    inum = INUM_UART0 + (((uint32_t)UART_CH - AMHW_UART0_BASE)>>12) ;
    
    /* 关联中断向量号，开启中断 */
    am_int_connect(inum, uart_irq_handler, (void *)UART_CH);
    am_int_enable(inum); 
    
    /* 使能接收中断 */
    amhw_uart_int_enable(UART_CH, AMHW_UART_INT_C2_IRIE);
    
    /* 使能串口 */ 
    amhw_uart_enable(UART_CH);
}

/**
 * \brief 主函数入口
 */
int main (void)
{
    /* 板级初始化 */
    am_board_init();

    /* UART初始化 */
    uart_init();
    
    uart_int_send(UART_CH, ch,sizeof(ch));
    
    while (1) {

    }
}
/** [src_hw_uart_int] */

/* end of file */
