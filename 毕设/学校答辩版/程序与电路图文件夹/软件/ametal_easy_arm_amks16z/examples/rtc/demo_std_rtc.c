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
 * - 操作步骤：
 *      - PTC1引脚外接32.786kHz的时钟输入
 *      - 使用调试串口打印
 *
 *
 * - 实验现象：
 *      - 串口打印出设置的时间
 *      - 以后每秒打印一次时间
 *
 * \par 源代码
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

    /** RTC 服务句柄 */
    am_rtc_handle_t rtc_handler;

    /** 缓存时间的变量 */
    am_tm_t time;

    /** 初始时间设置 */
    am_tm_t current_time = {
        58,   /** 秒             */
        30,   /** 分             */
        15,   /** 时             */
        28,   /** 日             */
        9,    /** 月             */
        115,  /** 年             */
        3,    /** 周             */
        300,  /** 天 当年的第n天 */
        0,    /** 夏令时         */
    };


    /** 板级初始化 */
    am_board_init();

    /** RTC平台初始化 */
    amhw_plfm_rtc_init();

    /** 初始化RTC模块，获得服务句柄 */
    rtc_handler = amdr_rtc_init(&g_rtc_dev, &g_rtc_devinfo);

    /** 时钟源校正 32766.6Hz 是时间输入时钟的频率 */
    amdr_rtc_compensation_set(&g_rtc_dev, 32766.6);

    /** 设置RTC的时间 */
    am_rtc_time_set(rtc_handler, &current_time);

    /** 打印设置的时间 */
    printf("the time you set is :%d-%d-%d %d:%d:%d\n",
           current_time.tm_year + 1900,
           current_time.tm_mon + 1,
           current_time.tm_mday,
           current_time.tm_hour,
           current_time.tm_min,
           current_time.tm_sec);

    /** 秒有变化的时候打印时间 */
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

