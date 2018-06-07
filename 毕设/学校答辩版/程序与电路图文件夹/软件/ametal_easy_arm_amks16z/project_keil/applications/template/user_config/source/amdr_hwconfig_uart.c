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
 * \brief ks16 UART 用户配置文件
 * \sa amdr_hwconfig_uart.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-21  xym, first implementation.
 * \endinternal
 */

#include "amdr_hwconfig_uart.h"
/**
 * \addtogroup amdr_if_hwconfig_src_uart
 * \copydoc amdr_hwconfig_uart.c
 * @{
 */

/** \brief 串口0 设备信息 */
const amdr_uart_devinfo_t g_uart0_devinfo = {

    AMHW_UART0,                     /**< \brief 串口0                    */
    INUM_UART0,                     /**< \brief 串口0的中断编号          */

    AMHW_UART_C1_M_8BIT      |      /**< \brief 8位数据                  */
    AMHW_UART_C1_PARITY_NO   |      /**< \brief 无极性                   */
    AMHW_UART_BDH_SBNS_STOP_1,      /**< \brief 1个停止位                */


    57600,                         /**< \brief 设置的波特率             */

    0,                              /**< \brief 无其他中断               */
};

amdr_uart_dev_t  g_uart0_dev;       /**< \brief 定义串口0 设备           */

/** \brief 串口0平台初始化 */
void amhw_plfm_uart0_init (void)
{

    /* 引脚初始化      PIOA_1_UART0_RX  PIOA_2_UART0_TX     */
    am_gpio_pin_cfg (PIOA_1,PIOA_1_UART0_RX);
    am_gpio_pin_cfg (PIOA_2,PIOA_2_UART0_TX);
    /* UART0时钟源选择                  */
    amhw_sim_uart0_src_set(AMHW_SIM_UART0SRC_PLLFLLCLK);
    /* 开启UART0时钟                  */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_UART0);


}

/** \brief 解除串口0 平台初始化 */
void amhw_plfm_uart0_deinit (void)
{
    /* 关闭UART0时钟                  */
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_UART0);
}

/** \brief 串口1 设备信息 */
const amdr_uart_devinfo_t g_uart1_devinfo = {

    AMHW_UART1,                     /**< \brief 串口1                    */
    INUM_UART1,                     /**< \brief 串口1的中断编号          */

    AMHW_UART_C1_M_8BIT      |      /**< \brief 8位数据                  */
    AMHW_UART_C1_PARITY_NO   |      /**< \brief 无极性                   */
    AMHW_UART_BDH_SBNS_STOP_1,      /**< \brief 1个停止位                */

    57600,                          /**< \brief 设置的波特率             */

    0,                              /**< \brief 无其他中断               */
};

amdr_uart_dev_t  g_uart1_dev;       /**< \brief 定义串口1 设备           */

/** \brief 串口1平台初始化 */
void amhw_plfm_uart1_init (void)
{
     /* 引脚初始化      PIOE_1_UART1_RX  PIOE_0_UART1_TX     */
     am_gpio_pin_cfg (PIOE_1,AM_GPIO_PULLUP);
     am_gpio_pin_cfg (PIOE_0,AM_GPIO_PULLUP );
     am_gpio_pin_cfg (PIOE_1,AM_GPIO_PULLUP | PIOE_1_UART1_RX);
     am_gpio_pin_cfg (PIOE_0,AM_GPIO_PULLUP | PIOE_0_UART1_TX);

     /* 开启UART1时钟                  */
     amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_UART1);

}

/** \brief 解除串口1 平台初始化 */
void amhw_plfm_uart1_deinit (void)
{
     /* 关闭UART1时钟                  */
     amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_UART1);
}

/** \brief 串口2 设备信息 */
const amdr_uart_devinfo_t g_uart2_devinfo = {

    AMHW_UART2,                    /**< \brief 串口2                    */
    INUM_UART2,                    /**< \brief 串口2的中断编号          */

    AMHW_UART_C1_M_8BIT      |     /**< \brief 8位数据                  */
    AMHW_UART_C1_PARITY_NO   |     /**< \brief 无极性                   */
    AMHW_UART_BDH_SBNS_STOP_1,     /**< \brief 1个停止位                */


    57600,                        /**< \brief 设置的波特率             */

    0,                            /**< \brief 无其他中断               */
};

amdr_uart_dev_t  g_uart2_dev;     /**< \brief 定义串口2 设备              */

/** \brief 串口2平台初始化 */
void amhw_plfm_uart2_init (void)
{

    /* 引脚初始化      PIOE_17_UART2_RX  PIOE_16_UART2_TX     */
     am_gpio_pin_cfg (PIOE_17,PIOE_17_UART2_RX);
     am_gpio_pin_cfg (PIOE_16,PIOE_16_UART2_TX);

     /* 开启UART2时钟                  */
     amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_UART2);
}

/** \brief 解除串口2 平台初始化 */
void amhw_plfm_uart2_deinit (void)
{
    /* 关闭UART2时钟                  */
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_UART2);
}

/**
 * @}
 */

/* end of file */



