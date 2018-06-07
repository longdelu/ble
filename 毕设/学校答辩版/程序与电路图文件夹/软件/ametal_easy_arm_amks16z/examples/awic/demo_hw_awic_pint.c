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
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief ͨ�������첽���Ѵ���˯��״̬��MCU
 *
 * - �������裺
 *   1. PIOA_1��������PC���ڵ�TXD��
 *   2. PIOA_2��������PC���ڵ�RXD��
 *   3. ������J14������KEY-PTC0������һ��
 *   4. ���������ִ�ӡ�ꡰsleep...��֮�󣬽�ȥ˯��ģʽ���л�RST���������˳�˯��ģʽ��
 *
 * - ʵ������
 *   1. ���ڴ�ӡ��sleep...���󣬽������˯��ģʽ��muc�ĵ��͡�
 *   2. RST�������º󣬴��ڼ�����ӡ��wake up...����mcu�����
 *
 * \par Դ����
 * \snippet demo_hw_awic_pint.c src_hw_awic_pint
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-16  xjc, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_hw_awic_pint
 * \copydoc demo_hw_awic_pint.c
 */
 
/** [src_hw_awic_pint] */
#include "ametal.h"
#include "am_board.h"
/** \brief ���崮�ڣ��ж�ģʽ���豸�ṹ�壬���ڷ�����ؿռ� */
am_uart_int_dev_t  g_uart1_int_dev;


/** \brief ��ť�¼� */
void key_event (void * p_arg)
{
    /* ���жϱ�־ */
    amhw_port_pin_isf_clr (AMHW_PORT, PIOC_0);

    am_led_toggle(0);
}

/** \brief ������ʼ�� */
void key_init (void)
{
    am_gpio_pin_cfg(PIOC_0, AM_GPIO_INPUT | AM_GPIO_PULLUP);
    am_gpio_trigger_cfg(PIOC_0, AM_GPIO_TRIGGER_FALL);

    am_int_disable(INUM_PORTC_PORTD);
    am_int_connect(INUM_PORTC_PORTD, key_event, NULL);
    am_int_enable(INUM_PORTC_PORTD);
}
/**
 * \brief ���������
 */
int main (void)
{  
    int step = 0;

    /* �弶��ʼ��  */
    am_board_init();

    key_init();

    printf("AWIC Demo:\r\n");

    while (1) {
        switch (step) {

        case 0:
            step = 1;
            printf("runing.\r\n");
            break;

        case 1:
            step = 2;
            printf("runing..\r\n");
            break;

        case 2:
            step = 0;
            printf("runing...\r\n");
            printf("sleep...\r\n");
        
            am_udelay(300);
        
            if (amdr_pmu_mode_into(AMDR_PMU_MODE_VLPS) != AM_OK) {
                printf("Enter STOP_MODE Failed !\r\n");
            }           
            am_udelay(50);
        
            printf("wake up...\r\n");
            break;

        default:break;
        }
        am_mdelay(500);
    }
}

/** [src_hw_awic_pint] */
 
/* end of file */

