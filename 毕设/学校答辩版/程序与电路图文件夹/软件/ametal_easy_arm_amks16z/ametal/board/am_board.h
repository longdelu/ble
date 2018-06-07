/*******************************************************************************
*                                 Apollo
*                       ---------------------------
*                       innovating embedded systems
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief EasyARM-54000M4 ���İ�
 *
 * \internal
 * \par Modification history
 * - 1.00 14-12-01  tee, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_prj_config.h"

#include "am_delay.h"
#include "am_led.h"
#include "am_buzzer_pwm.h"

/** \brief LED ����ID�� */
#define LED0          0

/** \brief LED ����ID�� */
#define LED1          1

/**
 * \brief �弶��ʼ��(EasyARM-54000M4)
 * \return ��
 * \note �ú������ʼ��ϵͳʱ�ӡ�GPIO���жϡ���������LED����ʱ����
 */
void am_board_init(void);

/**
 *\brief ��ʱ����
 */
void led_init(uint32_t led_num);
void led_on(uint32_t led_num);
void led_off(uint32_t led_num);
void led_toggle(uint32_t led_num);
    
/* end of file */
