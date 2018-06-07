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
 * \brief
 *
 * - �������裺
 *      - PTC1�������32.786kHz��ʱ������
 *      - ʹ�õ��Դ��ڴ�ӡ
 *
 *
 * - ʵ������
 *      - ���ڴ�ӡ�����õ�ʱ��
 *      - �Ժ�ÿ���ӡһ��ʱ��
 *
 * \par Դ����
 * \snippet demo_std_rtc.c src_std_rtc
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-28  zsp, first implementation.
 * \endinternal
 */

/**
* \addtogroup demo_if_std_rtc
* \copydoc demo_std_rtc.c
*/

/** [src_std_rtc] */
#include "ametal.h"
#include "am_board.h"
#include "am_debug.h"
#include "am_delay.h"
#include "am_prj_config.h"

int main ()
{
    uint32_t tmp;

    /** RTC ������ */
    am_rtc_handle_t rtc_handler;

    /** ����ʱ��ı��� */
    am_tm_t time;

    /** ��ʼʱ������ */
    am_tm_t current_time = {
        58,   /** ��             */
        30,   /** ��             */
        15,   /** ʱ             */
        28,   /** ��             */
        9,    /** ��             */
        115,  /** ��             */
        3,    /** ��             */
        300,  /** �� ����ĵ�n�� */
        0,    /** ����ʱ         */
    };


    /** �弶��ʼ�� */
    am_board_init();

    /** RTCƽ̨��ʼ�� */
    amhw_plfm_rtc_init();

    /** ��ʼ��RTCģ�飬��÷����� */
    rtc_handler = amdr_rtc_init(&g_rtc_dev, &g_rtc_devinfo);

    /** ʱ��ԴУ�� 32766.6Hz ��ʱ������ʱ�ӵ�Ƶ�� */
    amdr_rtc_compensation_set(&g_rtc_dev, 32766.6);

    /** ����RTC��ʱ�� */
    am_rtc_time_set(rtc_handler, &current_time);

    /** ��ӡ���õ�ʱ�� */
    printf("the time you set is :%d-%d-%d %d:%d:%d\n",
           current_time.tm_year + 1900,
           current_time.tm_mon + 1,
           current_time.tm_mday,
           current_time.tm_hour,
           current_time.tm_min,
           current_time.tm_sec);

    /** ���б仯��ʱ���ӡʱ�� */
    while (1) {
        am_rtc_time_get(rtc_handler, &time);

        if (time.tm_sec != tmp) {
            printf("current time: ");

            printf("%d-%d-%d %d:%d:%d\n",
                   time.tm_year + 1900,
                   time.tm_mon + 1,
                   time.tm_mday,
                   time.tm_hour,
                   time.tm_min,
                   time.tm_sec);
        }

        tmp = time.tm_sec;
    }
}

/** [src_std_rtc] */


/** end of the file */

