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
#include "ametal.h"
#include "amdr_rtc.h"
#include "amhw_rtc.h"

/** 平年每月的起始天数 */
static const uint32_t __non_leap_year[] =
{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

/** 闰年每月的起始天数 */
static const uint32_t __leap_year[] =
{0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};


/** \brief 时间获取函数 */
static int __rtc_time_get (void *p_drv, am_tm_t *p_tm);

/** \brief 时间设置函数 */
static int __rtc_time_set (void *p_drv, am_tm_t *p_tm);

/** \brief RTC 驱动函数 */
static const struct am_rtc_drv_funcs __g_rtc_drv_funcs = {
    __rtc_time_set,
    __rtc_time_get,
};

/**
 *  \brief 秒转化为tm时间数据
 *
 *  \param p_time 指向tm时间数据的指针
 *  \param  seconds 秒数据
 *  \return 无
 */
static void __sec2tm(am_tm_t *p_time, uint32_t sec)
{
    uint32_t year, day, hour, min, i, j;
    uint32_t year_start_day[] = {
        0,
        366,
        366 + 365,
        366 + 365 + 365,
        366 + 365 + 365 + 365
    };
    year            = 4 * (sec / ((365 * 4 + 1) * 24 * 60 * 60));
    p_time->tm_wday = ((sec / (24 * 60 * 60)) + 6) % 7;
    sec             = sec % ((365 * 4 + 1) * 24 * 60 * 60);
    day             = sec / (24 * 60 * 60);  // 4年中的第几天
    sec             = sec % (24 * 60 * 60);
    hour            = sec / (60 * 60);
    sec             = sec % (60 * 60);
    min             = sec / 60;
    sec             = sec % 60;
    p_time->tm_sec  = sec;
    p_time->tm_min  = min;
    p_time->tm_hour = hour;
    i = 0;

    while (year_start_day[i] <= day ) {
        i++;
    }

    i               = i - 1;
    year           += i;
    p_time->tm_year = year + 100;
    p_time->tm_yday = day - year_start_day[i];

    if (0 == i) {
        j = 0;

        while (p_time->tm_yday >= __leap_year[j]) {
            j++;
        }

        j               = j - 1;
        p_time->tm_mon  = j;
        p_time->tm_mday = p_time->tm_yday - __leap_year[j] + 1;
    } else {
        j = 0;

        while (p_time->tm_yday >= __non_leap_year[j]) {
            j++;
        }

        j               = j - 1;
        p_time->tm_mon  = j;
        p_time->tm_mday = p_time->tm_yday - __non_leap_year[j] + 1;
    }

    p_time->tm_isdst = 0;
}

/**
 *  \brief tm时间数据转化为秒
 *
 *  \param p_time 指向tm时间数据的指针
 *
 *  \return 秒数据
 *
 *  \note 时间是从2000年开始的
 */
static uint32_t __tm2sec(am_tm_t *p_time)
{
    uint32_t sec;
    sec              = 0;
    sec             += p_time->tm_sec;
    sec             += p_time->tm_min * 60;
    sec             += p_time->tm_hour * 60 * 60;
    sec             += p_time->tm_yday * 24 * 60 * 60;
    sec             += (p_time->tm_year - 100) * 365 * 24 * 60 * 60;

    /** 加上闰年多的一天 */
    sec             += ((p_time->tm_year - 100) + 3) / 4 * 24 * 60 * 60;
    p_time->tm_isdst = 0;
    return sec;
}

/**
 *  \brief Brief
 *
 *  \param[in] p_drv 指向RTC设备的指针
 *  \param[in,out] p_tm 指向时间信息结构体的指针
 *
 *  \return AM_OK
 */
static int __rtc_time_get (void *p_drv, am_tm_t *p_tm)
{
    uint32_t sec;
    amdr_rtc_dev_t *p_dev   = (amdr_rtc_dev_t *)p_drv;
    sec                     = amhw_rtc_second_get(p_dev->p_devinfo->p_hw_rtc);
    __sec2tm(p_tm, sec);
    return AM_OK;
}

static int __rtc_time_set (void *p_drv, am_tm_t *p_tm)
{
    uint32_t sec;

    amdr_rtc_dev_t *p_dev = (amdr_rtc_dev_t *)p_drv;
    amhw_rtc_t *p_hw_rtc  = p_dev->p_devinfo->p_hw_rtc;
    sec                   = __tm2sec(p_tm);

    /** 关闭计数器 */
    amhw_rtc_time_counter_status_clr(p_hw_rtc);

    /** 设置预分频计数器 */
    amhw_rtc_prescaler_set(p_hw_rtc, 0x00);

    /** 设置秒计数器 */
    amhw_rtc_second_set(p_hw_rtc, sec);

    /** 开启计数器 */
    amhw_rtc_time_counter_status_set(p_hw_rtc);

    return AM_OK;
}


/**
 * \brief 初始化RTC
 *
 * \param[in] p_dev     : 指向RTC设备
 * \param[in] p_devinfo : 指向RTC设备信息
 *
 * \return RTC标准服务操作句柄。如果为 NULL，表明初始化失败。
 */
am_rtc_handle_t amdr_rtc_init (amdr_rtc_dev_t           *p_dev,
                               const amdr_rtc_devinfo_t *p_devinfo)
{
    amhw_rtc_t *p_hw_rtc;

    if (NULL == p_devinfo || NULL == p_devinfo->p_hw_rtc) {
        return NULL;
    }

    p_hw_rtc                = p_devinfo->p_hw_rtc;
    p_dev->p_devinfo        = p_devinfo;
    p_dev->rtc_serv.p_funcs = (struct am_rtc_drv_funcs *)&__g_rtc_drv_funcs;
    p_dev->rtc_serv.p_drv   = p_dev;

    /**
     *  设备初始化
     */

    /** 关闭秒计数器 */
    amhw_rtc_time_counter_status_clr(p_hw_rtc);

    /** 分频计数器、秒计数器、校准寄存器清0 */
    amhw_rtc_prescaler_set(p_hw_rtc, 0x00);
    amhw_rtc_second_set(p_hw_rtc, 0x00);
    amhw_rtc_alarm_set(p_hw_rtc, 0x00);
    amhw_rtc_compensate_interval_set(p_hw_rtc, 0x00);
    amhw_rtc_compensate_value_set(p_hw_rtc, 0x00);
    amhw_rtc_current_compensate_counter_set(p_hw_rtc, 0x00);
    amhw_rtc_current_compensate_value_set(p_hw_rtc, 0x00);

    /** 使能秒计数器 */
    amhw_rtc_time_counter_status_set(p_hw_rtc);

    return &(p_dev->rtc_serv);
}

/**
 *  \brief 解除RTC初始化
 *
 *  \param p_dev 指向RTC设备的指针
 *
 *  \return 无
 */
void amdr_rtc_deinit (amdr_rtc_dev_t *p_dev)
{
    amhw_rtc_time_counter_status_clr(p_dev->p_devinfo->p_hw_rtc);
    p_dev->p_devinfo           = NULL;
    p_dev->rtc_serv.p_funcs    = NULL;
    p_dev->rtc_serv.p_drv      = NULL;
    p_dev                      = NULL;
}

/**
 *  \brief 设置RTC的校准配置
 *
 *  \param p_dev 指向RTC设备的指针
 *
 *  \param frq RTC时钟的实际频率，以Hz计
 *
 *  \return 实际校准后的频率
 */
int amdr_rtc_compensation_set (amdr_rtc_dev_t *p_dev, float frq)
{
    int i, j;             /** 循环计数         */
    int m, n;             /** 最优配置暂存     */
    float err_aim;        /** 目标偏差         */
    float err, err_pre;   /** 调整后的决定偏差 */
    amhw_rtc_t *p_hw_rtc;
    p_hw_rtc = p_dev->p_devinfo->p_hw_rtc;

    if (NULL == p_dev) {
        return -AM_EINVAL;
    }

    err_aim = 32768.0 - frq;
    err_pre = err = err_aim > 0 ? err_aim : -err_aim;

    /** 超过调整下限 */
    if (frq < (32768 - 127)) {
        m = -127;
        n = 1;
        amhw_rtc_current_interval_and_value_set(p_hw_rtc, n - 1, m);
        return (32768.0 * frq) / (32768.0 - m * 1.0);
    }
    /** 超过调整上限 */
    else if (frq > (32768 + 128)) {
        m = 128;
        n = 1;
        amhw_rtc_current_interval_and_value_set(p_hw_rtc, n - 1, m);
        return (32768.0 * frq) / (32768.0 - m * 1.0);
    }
    /** 恰好是整数 */
    else if (0 == (frq - (int)frq)) {
        m = 32768 - frq;
        n = 1;
    }
    /** 小数调整 */
    else {
        m = (int)err_aim;
        n = 1;

        for (j = 1; j < 256; j ++) {
            /** 加快 */
            if (err_aim > 0) {
                for (i = (int)err_aim * j;
                     (i < 128) && (i < (int)(err_aim + 1)*j);
                     i++) {
                    err = err_aim > i * 1.0 / j ?
                          err_aim - i * 1.0 / j :
                          i * 1.0 / j - err_aim;

                    if (err < err_pre) {
                        err_pre = err;
                        m = i;
                        n = j;

                        /** 得到无偏差参数跳出循环 */
                        if (0 == err_pre) {
                            goto out;
                        }
                    }
                }

                /** i超过上限跳出循环 */
                if (i > 127) {
                    break;
                }
            }
            /** 减慢 */
            else {
                for (i = (int)err_aim * j;
                     (i > -127) && (i > (int)(err_aim - 1)*j);
                     i--) {
                    err = err_aim > i * 1.0 / j ?
                          err_aim - i * 1.0 / j :
                          i * 1.0 / j - err_aim;

                    if (err < err_pre) {
                        err_pre = err;
                        m = i;
                        n = j;

                        /** 得到无偏差参数跳出循环 */
                        if (0 == err_pre) {
                            goto out;
                        }
                    }
                }

                /** i超过下限跳出循环 */
                if (i < -128) {
                    break;
                }
            }
        }
    }

out:
    amhw_rtc_current_interval_and_value_set(p_hw_rtc, n - 1, m);
    return (int)(frq + err_pre);
}

/**
 *  \brief 设置RTC闹钟时间
 *
 *  \param p_dev 指向RTC设备的指针
 *  \param p_tm 需要设置的时间
 *
 *  \return 无
 */
void amdr_rtc_alarm_set (amdr_rtc_dev_t *p_dev, am_tm_t *p_tm)
{
    uint32_t sec;
    sec = __tm2sec(p_tm);
    amhw_rtc_second_set(p_dev->p_devinfo->p_hw_rtc, sec);
}

/**
 *  \breif 根据日期计算一年中的第几天
 *
 *  \param day
 *  \param month
 *  \param year
 *
 *  \return 一年中的第几天 (1月1日记为第0天)
 */
int amdr_rtc_date2yday (uint8_t day, uint8_t month, uint32_t year)
{
    if (((0 == year % 4) && (0 != year % 100)) || (year % 400 == 0)) {
        return __leap_year[month-1] + day - 1;
    }
    else {
        return __non_leap_year[month - 1] + day - 1;
    }
}

/** end of the file */
