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
 * \brief NVIC�����жϲ������̣������жϣ�ͨ����׼��ʵ��
 *
 * - �������裺
 *   1. ������J14������KEY-PTC0������һ��
 *   2. ͨ���л�RST�������۲�LE0�仯��
 *
 * - ʵ������
 *   1. ÿ��һ�ΰ�����LED0ȡ��һ�Ρ�
 *
 * \par Դ����
 * \snippet demo_std_int_keyscan.c src_std_int_keyscan
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-16  xjc, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_std_int_keyscan
 * \copydoc demo_std_int_keyscan.c
 */
 
/** [src_std_int_keyscan] */
#include "ametal.h"
#include "am_board.h"

/** \brief ��ť�¼� */
void key_event (void * p_arg)
{
    uint32_t key;
    /* ���жϱ�־ */
    amhw_port_pin_isf_clr (AMHW_PORT, PIOC_0);

    key = am_int_cpu_lock();
    am_led_toggle(0);
    am_int_cpu_unlock(key);
}

/** \brief ������ʼ�� */
void key_init (void)
{
    am_gpio_pin_cfg(PIOC_0, AM_GPIO_INPUT);
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
    /* �弶��ʼ��  */
    am_board_init();
    
    key_init();

    am_led_toggle(0);

    while (1) {
        
        
    }
}

/** [src_std_int_keyscan] */
 
/* end of file */

