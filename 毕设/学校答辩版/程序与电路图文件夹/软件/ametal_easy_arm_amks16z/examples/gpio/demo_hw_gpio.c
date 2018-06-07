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
 * \brief KS16xx GPIO��PORT HW������
 *
 * ʵ�����󣺳������ص�demo����õ��߽�PTC0�ӵأ�demo���ϵ�LED0������
 *
 * \par Դ����
 * \par Դ����
 * \snippet demo_hw_gpio.c src_hw_gpio
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-16  wxj, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_hw_gpio
 * \copydoc demo_hw_gpio.c
 */
 
/** [src_hw_gpio] */
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

    /* ���� PORTC 0,4�Źܽ�ΪGPIO���� */
    amhw_port_pin_func_cfg(AMHW_PORT, PIOC_4, 0x1);
    amhw_port_pin_func_cfg(AMHW_PORT, PIOC_0, 0x1);

    /* �ܽ���/����ʹ�� */
    amhw_port_pin_pull_enable(AMHW_PORT, PIOC_0);

    /* ������������ */
    amhw_port_pin_pull_up (AMHW_PORT, PIOC_0);

    /* ����GPIOC 4 �Źܽų�ʼ��ƽ */
    amhw_gpio_pin_init_out_low (AMHW_GPIO, PIOC_4);

    /* ����GPIOC 4�Źܽ�Ϊ������� */
    amhw_gpio_pin_dir_output (AMHW_GPIO, PIOC_4);

    /* ����GPIOC 0�Źܽ�Ϊ���뷽�� */
    amhw_gpio_pin_dir_input (AMHW_GPIO, PIOC_0);

    /* ����GPIOC 4�Źܽ�Ϊ���Ϊ��,8�Źܽ����Ϊ�� */
    amhw_gpio_pin_out_high (AMHW_GPIO, PIOC_4);

    while (1) {

        am_mdelay(1000);

        /* ��ȡ���������ƽ */
        pin_val = amhw_gpio_pin_input_get(AMHW_GPIO, PIOC_0);

        if (pin_val == 0) {

            /* ��ת���������ƽ */
            amhw_gpio_pin_out_toggle (AMHW_GPIO, PIOC_4);
        }
    }
}

/** [src_hw_gpio] */
 
/* end of file */
