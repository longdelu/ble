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
 * \brief ks16 UART �û������ļ�
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

/** \brief ����0 �豸��Ϣ */
const amdr_uart_devinfo_t g_uart0_devinfo = {

    AMHW_UART0,                     /**< \brief ����0                    */
    INUM_UART0,                     /**< \brief ����0���жϱ��          */

    AMHW_UART_C1_M_8BIT      |      /**< \brief 8λ����                  */
    AMHW_UART_C1_PARITY_NO   |      /**< \brief �޼���                   */
    AMHW_UART_BDH_SBNS_STOP_1,      /**< \brief 1��ֹͣλ                */


    57600,                         /**< \brief ���õĲ�����             */

    0,                              /**< \brief �������ж�               */
};

amdr_uart_dev_t  g_uart0_dev;       /**< \brief ���崮��0 �豸           */

/** \brief ����0ƽ̨��ʼ�� */
void amhw_plfm_uart0_init (void)
{

    /* ���ų�ʼ��      PIOA_1_UART0_RX  PIOA_2_UART0_TX     */
    am_gpio_pin_cfg (PIOA_1,PIOA_1_UART0_RX);
    am_gpio_pin_cfg (PIOA_2,PIOA_2_UART0_TX);
    /* UART0ʱ��Դѡ��                  */
    amhw_sim_uart0_src_set(AMHW_SIM_UART0SRC_PLLFLLCLK);
    /* ����UART0ʱ��                  */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_UART0);


}

/** \brief �������0 ƽ̨��ʼ�� */
void amhw_plfm_uart0_deinit (void)
{
    /* �ر�UART0ʱ��                  */
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_UART0);
}

/** \brief ����1 �豸��Ϣ */
const amdr_uart_devinfo_t g_uart1_devinfo = {

    AMHW_UART1,                     /**< \brief ����1                    */
    INUM_UART1,                     /**< \brief ����1���жϱ��          */

    AMHW_UART_C1_M_8BIT      |      /**< \brief 8λ����                  */
    AMHW_UART_C1_PARITY_NO   |      /**< \brief �޼���                   */
    AMHW_UART_BDH_SBNS_STOP_1,      /**< \brief 1��ֹͣλ                */

    57600,                          /**< \brief ���õĲ�����             */

    0,                              /**< \brief �������ж�               */
};

amdr_uart_dev_t  g_uart1_dev;       /**< \brief ���崮��1 �豸           */

/** \brief ����1ƽ̨��ʼ�� */
void amhw_plfm_uart1_init (void)
{
     /* ���ų�ʼ��      PIOE_1_UART1_RX  PIOE_0_UART1_TX     */
     am_gpio_pin_cfg (PIOE_1,AM_GPIO_PULLUP);
     am_gpio_pin_cfg (PIOE_0,AM_GPIO_PULLUP );
     am_gpio_pin_cfg (PIOE_1,AM_GPIO_PULLUP | PIOE_1_UART1_RX);
     am_gpio_pin_cfg (PIOE_0,AM_GPIO_PULLUP | PIOE_0_UART1_TX);

     /* ����UART1ʱ��                  */
     amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_UART1);

}

/** \brief �������1 ƽ̨��ʼ�� */
void amhw_plfm_uart1_deinit (void)
{
     /* �ر�UART1ʱ��                  */
     amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_UART1);
}

/** \brief ����2 �豸��Ϣ */
const amdr_uart_devinfo_t g_uart2_devinfo = {

    AMHW_UART2,                    /**< \brief ����2                    */
    INUM_UART2,                    /**< \brief ����2���жϱ��          */

    AMHW_UART_C1_M_8BIT      |     /**< \brief 8λ����                  */
    AMHW_UART_C1_PARITY_NO   |     /**< \brief �޼���                   */
    AMHW_UART_BDH_SBNS_STOP_1,     /**< \brief 1��ֹͣλ                */


    57600,                        /**< \brief ���õĲ�����             */

    0,                            /**< \brief �������ж�               */
};

amdr_uart_dev_t  g_uart2_dev;     /**< \brief ���崮��2 �豸              */

/** \brief ����2ƽ̨��ʼ�� */
void amhw_plfm_uart2_init (void)
{

    /* ���ų�ʼ��      PIOE_17_UART2_RX  PIOE_16_UART2_TX     */
     am_gpio_pin_cfg (PIOE_17,PIOE_17_UART2_RX);
     am_gpio_pin_cfg (PIOE_16,PIOE_16_UART2_TX);

     /* ����UART2ʱ��                  */
     amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_UART2);
}

/** \brief �������2 ƽ̨��ʼ�� */
void amhw_plfm_uart2_deinit (void)
{
    /* �ر�UART2ʱ��                  */
    amhw_sim_periph_clock_disable(AMHW_SIM_SCGC_UART2);
}

/**
 * @}
 */

/* end of file */



