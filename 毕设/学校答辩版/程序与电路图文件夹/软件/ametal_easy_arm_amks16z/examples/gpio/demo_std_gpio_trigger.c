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
 * \brief GPIO �����������ж�����
 *
 * ʵ�����󣺳������ص�demo����õ��߽�PTC0�ӵأ�Ȼ��ο���demo���ϵ�LED0������
 *
 * \par Դ����
 * \snippet demo_std_gpio_trigger.c src_std_gpio_trigger
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-16  wxj, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_std_gpio_trigger
 * \copydoc demo_std_gpio_trigger.c
 */
 
/** [src_std_gpio_trigger] */
#include "ametal.h"
#include "am_board.h"

/**
 * \brief �����жϷ�����
 */
void gpio_isr (void *p_arg)
{
    /* PIOC_4���ŵ�ƽ��ת */
    am_gpio_toggle(PIOC_8);
}

/**
 * \brief ���������
 */
int main (void)
{
    /* �弶��ʼ��  */
    am_board_init();

    /* PIOC_4����Ϊ��� */
    am_gpio_pin_cfg(PIOC_8, PIOC_8_GPIO | AM_GPIO_OUTPUT);

    /* PIOC_0����ΪGPIO�������ж����� */
    am_gpio_pin_cfg(PIOC_0, PIOC_0_GPIO);

    /* ���������жϴ�����ʽ */
    am_gpio_trigger_cfg(PIOC_0, AM_GPIO_TRIGGER_RISE);

    /* ���������жϷ����� */
    am_gpio_trigger_connect(PIOC_0, gpio_isr, (void *)0);

    /* ���������ж� */
    am_gpio_trigger_on(PIOC_0);

    while(1){
        ;
    }
}

/** [src_std_gpio_trigger] */
 
/* end of file */
