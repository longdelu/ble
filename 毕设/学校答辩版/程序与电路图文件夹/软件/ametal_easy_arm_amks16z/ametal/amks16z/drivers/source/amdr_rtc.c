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

/** ƽ��ÿ�µ���ʼ���� */
static const uint32_t __non_leap_year[] =
{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

/** ����ÿ�µ���ʼ���� */
static const uint32_t __leap_year[] =
{0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};


/** \brief ʱ���ȡ���� */
static int __rtc_time_get (void *p_drv, am_tm_t *p_tm);

/** \brief ʱ�����ú��� */
static int __rtc_time_set (void *p_drv, am_tm_t *p_tm);

/** \brief RTC �������� */
static const struct am_rtc_drv_funcs __g_rtc_drv_funcs = {
    __rtc_time_set,
    __rtc_time_get,
};

/**
 *  \brief ��ת��Ϊtmʱ������
 *
 *  \param p_time ָ��tmʱ�����ݵ�ָ��
 *  \param  seconds ������
 *  \return ��
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
    day             = sec / (24 * 60 * 60);  // 4���еĵڼ���
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
 *  \brief tmʱ������ת��Ϊ��
 *
 *  \param p_time ָ��tmʱ�����ݵ�ָ��
 *
 *  \return ������
 *
 *  \note ʱ���Ǵ�2000�꿪ʼ��
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

    /** ����������һ�� */
    sec             += ((p_time->tm_year - 100) + 3) / 4 * 24 * 60 * 60;
    p_time->tm_isdst = 0;
    return sec;
}

/**
 *  \brief Brief
 *
 *  \param[in] p_drv ָ��RTC�豸��ָ��
 *  \param[in,out] p_tm ָ��ʱ����Ϣ�ṹ���ָ��
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

    /** �رռ����� */
    amhw_rtc_time_counter_status_clr(p_hw_rtc);

    /** ����Ԥ��Ƶ������ */
    amhw_rtc_prescaler_set(p_hw_rtc, 0x00);

    /** ����������� */
    amhw_rtc_second_set(p_hw_rtc, sec);

    /** ���������� */
    amhw_rtc_time_counter_status_set(p_hw_rtc);

    return AM_OK;
}


/**
 * \brief ��ʼ��RTC
 *
 * \param[in] p_dev     : ָ��RTC�豸
 * \param[in] p_devinfo : ָ��RTC�豸��Ϣ
 *
 * \return RTC��׼���������������Ϊ NULL��������ʼ��ʧ�ܡ�
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
     *  �豸��ʼ��
     */

    /** �ر�������� */
    amhw_rtc_time_counter_status_clr(p_hw_rtc);

    /** ��Ƶ�����������������У׼�Ĵ�����0 */
    amhw_rtc_prescaler_set(p_hw_rtc, 0x00);
    amhw_rtc_second_set(p_hw_rtc, 0x00);
    amhw_rtc_alarm_set(p_hw_rtc, 0x00);
    amhw_rtc_compensate_interval_set(p_hw_rtc, 0x00);
    amhw_rtc_compensate_value_set(p_hw_rtc, 0x00);
    amhw_rtc_current_compensate_counter_set(p_hw_rtc, 0x00);
    amhw_rtc_current_compensate_value_set(p_hw_rtc, 0x00);

    /** ʹ��������� */
    amhw_rtc_time_counter_status_set(p_hw_rtc);

    return &(p_dev->rtc_serv);
}

/**
 *  \brief ���RTC��ʼ��
 *
 *  \param p_dev ָ��RTC�豸��ָ��
 *
 *  \return ��
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
 *  \brief ����RTC��У׼����
 *
 *  \param p_dev ָ��RTC�豸��ָ��
 *
 *  \param frq RTCʱ�ӵ�ʵ��Ƶ�ʣ���Hz��
 *
 *  \return ʵ��У׼���Ƶ��
 */
int amdr_rtc_compensation_set (amdr_rtc_dev_t *p_dev, float frq)
{
    int i, j;             /** ѭ������         */
    int m, n;             /** ���������ݴ�     */
    float err_aim;        /** Ŀ��ƫ��         */
    float err, err_pre;   /** ������ľ���ƫ�� */
    amhw_rtc_t *p_hw_rtc;
    p_hw_rtc = p_dev->p_devinfo->p_hw_rtc;

    if (NULL == p_dev) {
        return -AM_EINVAL;
    }

    err_aim = 32768.0 - frq;
    err_pre = err = err_aim > 0 ? err_aim : -err_aim;

    /** ������������ */
    if (frq < (32768 - 127)) {
        m = -127;
        n = 1;
        amhw_rtc_current_interval_and_value_set(p_hw_rtc, n - 1, m);
        return (32768.0 * frq) / (32768.0 - m * 1.0);
    }
    /** ������������ */
    else if (frq > (32768 + 128)) {
        m = 128;
        n = 1;
        amhw_rtc_current_interval_and_value_set(p_hw_rtc, n - 1, m);
        return (32768.0 * frq) / (32768.0 - m * 1.0);
    }
    /** ǡ�������� */
    else if (0 == (frq - (int)frq)) {
        m = 32768 - frq;
        n = 1;
    }
    /** С������ */
    else {
        m = (int)err_aim;
        n = 1;

        for (j = 1; j < 256; j ++) {
            /** �ӿ� */
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

                        /** �õ���ƫ���������ѭ�� */
                        if (0 == err_pre) {
                            goto out;
                        }
                    }
                }

                /** i������������ѭ�� */
                if (i > 127) {
                    break;
                }
            }
            /** ���� */
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

                        /** �õ���ƫ���������ѭ�� */
                        if (0 == err_pre) {
                            goto out;
                        }
                    }
                }

                /** i������������ѭ�� */
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
 *  \brief ����RTC����ʱ��
 *
 *  \param p_dev ָ��RTC�豸��ָ��
 *  \param p_tm ��Ҫ���õ�ʱ��
 *
 *  \return ��
 */
void amdr_rtc_alarm_set (amdr_rtc_dev_t *p_dev, am_tm_t *p_tm)
{
    uint32_t sec;
    sec = __tm2sec(p_tm);
    amhw_rtc_second_set(p_dev->p_devinfo->p_hw_rtc, sec);
}

/**
 *  \breif �������ڼ���һ���еĵڼ���
 *
 *  \param day
 *  \param month
 *  \param year
 *
 *  \return һ���еĵڼ��� (1��1�ռ�Ϊ��0��)
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
