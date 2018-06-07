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
 * \brief GPIO ���������������������
 *
 * ʵ�����󣺳������ص�demo����õ��߽�PTC0�ӵأ�demo���ϵ�LED0������
 *
 * \par Դ����
 * \par Դ����
 * \snippet demo_std_gpio.c src_std_gpio
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-16  wxj, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_std_gpio
 * \copydoc demo_std_gpio.c
 */
 
/** [src_std_gpio] */
#include "ametal.h"
#include "am_board.h"

/**
 * \brief ���������
 */
int main (void)
{
    int pin_val=0;
    

    /* �弶��ʼ��  */
    am_board_init();

    /* ���� PORTC 0�Źܽ�ΪGPIO����,����ģʽ,�ڲ����� */
    am_gpio_pin_cfg(PIOE_1, AM_GPIO_INPUT | AM_GPIO_PULLDOWN);

    /* ���� PORTC 4�Źܽ�ΪGPIO����,���ģʽ */
    am_gpio_pin_cfg(PIOE_1, AM_GPIO_OUTPUT);

    /* ����GPIOC 4�Źܽ�Ϊ���Ϊ�� */
    am_gpio_set(PIOE_1, 0);
     

    while (1) {

        am_mdelay(500);

        /* ��ȡ PIOC_0 ���ŵ�ƽ */
        pin_val = am_gpio_get(PIOC_0);

        if (pin_val == 0) {

            /* ��ת PIOC_4 �����ƽ */
            am_gpio_toggle(PIOE_1);
        }
    }
}

/** [src_std_gpio] */
 
/* end of file */
