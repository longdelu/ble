/*******************************************************************************
*                                 AMetal
*                       ---------------------------
*                       innovating embedded systems
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief AMKS16Z ���İ�
 *
 * \internal
 * \par Modification history
 * - 1.00 15-11-01  link, first implementation.
 * \endinternal
 */
#include "am_board.h"
#include "am_common.h"
#include "am_prj_config.h"
#include "am_system.h"

/*******************************************************************************
  ����LED�����Ϣ
*******************************************************************************/

#if (AM_CFG_LED_ENABLE == 1)

#include "am_led.h"

/** \brief ����LED��GPIO�ܽź��Ƿ�͵�ƽ���� */
const am_led_info_t g_led_info[] = {
        {PIOC_4, TRUE},
        {PIOC_8, TRUE},
};

/** \brief ����LED���� */
const uint32_t g_led_num = sizeof(g_led_info) / sizeof(g_led_info[0]);

#endif

/*******************************************************************************
  ������Դ�����������Ϣ
*******************************************************************************/

#if (AM_CFG_BUZZER_ENABLE == 1)

#include "amdr_tpm_pwm.h"

#define __BUZZER_PWM_PERIOD_NS \
            (1000000000 / AM_CFG_BUZZER_PWM_FREQ)

#define __BUZZER_PWM_DUTY_NS   \
            (10000000 * AM_CFG_BUZZER_PWM_DUTY / AM_CFG_BUZZER_PWM_FREQ)
#endif 

/*******************************************************************************
  ��������
*******************************************************************************/

/**
 * \brief �弶��ʼ��
 */
void am_board_init (void)
{

#if (AM_CFG_SIM_COP == 0)

    /** \brief  �ر�COP���Ź� */
    amhw_sim_cop_cfg (AMHW_SIM_COP_MODE_NORMAL,
                      AMHW_SIM_COP_CLK_INTERNAL,
                      AMHW_SIM_COP_DISABLED);
#endif

   am_system_init();
    //�������ݴ����

    /* ʹ��GPIOǰ������GPIO��ʼ������ */
    amhw_plfm_gpio_init();
    amdr_gpio_init(&g_gpio_dev, &g_gpio_devinfo);
    
   /* ʹ���ж�ǰ�������жϳ�ʼ������ */
    amdr_int_init(&g_int_dev, &g_int_devinfo);

#if (AM_CFG_DELAY_ENABLE == 1)
    am_delay_init();
#endif

#if (AM_CFG_LED_ENABLE == 1)
    am_led_init(g_led_info,g_led_num);
#endif

#if (AM_CFG_DEBUG_ENABLE == 1)
    am_debug_init();
#endif

#if (AM_CFG_BUZZER_ENABLE == 1)
    
    amhw_plfm_tpm0_pwm_init();
    
    am_buzzer_init(amdr_tpm_pwm_init(&g_tpm0_pwm_dev,&g_tpm0_pwm_devinfo),
                   2,                    /* ʹ��ͨ��2, TPM0_CH2,PTC3 */
                   __BUZZER_PWM_DUTY_NS,
                   __BUZZER_PWM_PERIOD_NS);
#endif
}

/* end of file */
