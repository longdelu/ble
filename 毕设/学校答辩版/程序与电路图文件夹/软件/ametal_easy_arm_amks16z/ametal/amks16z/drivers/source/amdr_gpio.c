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
 * \brief GPIO implementation
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-21  wxj, first implementation.
 * \endinternal
 */

#include "am_gpio.h"
#include "amdr_gpio.h"
#include "amdr_gpio_util.h"
#include "am_gpio_util.h"
#include "am_int.h"

/*******************************************************************************
* 私有定义
*******************************************************************************/

/** \brief 中断未连接标识             */
#define AMDR_GPIO_INVALID_PIN_MAP    0xFF

/** \brief 未配置其他功能标识         */
#define AMDR_GPIO_NO_FUNC            0xFF

/** \brief 定义指向GPIO设备信息的指针 */
#define __GPIO_DEVINFO_DECL(p_gpio_devinfo, p_dev)  \
        const amdr_gpio_devinfo_t *p_gpio_devinfo = p_dev->p_devinfo

/******************************************************************************
  全局变量
******************************************************************************/

/** \brief 指向GPIO设备的指针 */
amdr_gpio_dev_t *__gp_gpio_dev;

/*******************************************************************************
  公共函数
*******************************************************************************/

/**
 * \brief 引脚配置
 */
int am_gpio_pin_cfg (int pin, uint32_t flags)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    amhw_port_t *p_hw_port = p_gpio_devinfo->p_hw_port;
    amhw_gpio_t *p_hw_gpio = p_gpio_devinfo->p_hw_gpio;

    uint32_t func, mode;

    /* 读取标准层定义的信息 */
    func = AM_GPIO_COM_FUNC_GET(flags);
    mode = AM_GPIO_COM_MODE_GET(flags);

    /* 管脚默认为GPIO功能 */
    amhw_port_pin_func_cfg(p_hw_port, pin, AMHW_PORT_GPIO);
    /*
     * 共有功能配置（优先级高）
     */
    if (func != 0x00) {

        switch (func) {

        /* 默认管脚为GPIO功能 */
        case AM_GPIO_INPUT:           
            amhw_gpio_pin_dir_input(p_hw_gpio, pin);
            break;

        case AM_GPIO_OUTPUT:
            amhw_gpio_pin_dir_output(p_hw_gpio, pin);
            break;

        case AM_GPIO_OUTPUT_INIT_HIGH:
            amhw_gpio_pin_dir_output(p_hw_gpio, pin);
            amhw_gpio_pin_init_out_high(p_hw_gpio, pin);
            break;

        case AM_GPIO_OUTPUT_INIT_LOW:
            amhw_gpio_pin_dir_output(p_hw_gpio, pin);
            amhw_gpio_pin_init_out_low(p_hw_gpio, pin);
            break;

        default:
            break;
        }
    }

    if (mode != 0x00) {

        if (mode == AM_GPIO_MODE_INVALID_VAL) {             /* 无上/下拉 */
            amhw_port_pin_pull_disable (p_hw_port, pin);

        } else if (mode == AM_GPIO_PULL_UP_VAL) {           /* 设置上拉模式 */
            amhw_port_pin_pull_enable (p_hw_port, pin);
            amhw_port_pin_pull_up (p_hw_port, pin);

        } else if (mode == AM_GPIO_PULL_DOWN_VAL) {         /* 设置下拉模式 */
            amhw_port_pin_pull_enable (p_hw_port, pin);
            amhw_port_pin_pull_down (p_hw_port, pin);

        } else {
            return -AM_ENOTSUP;
        }
    }

    /* 平台功能配置 */

    /* 需要设置引脚功能 */
    if ((flags & AMDR_PORT_FUNEN) != 0) {

        amhw_port_pin_func_cfg (p_hw_port, pin, AMDR_GPIO_FUNC_GET(flags));
    }

    /* 需要设置引脚低转换速率     */
    if ((flags & AMDR_PORT_SLEWSLOW) != 0) {
        amhw_port_pin_slew_slow (p_hw_port, pin);
    }

    /* 需要使能滤波器 */
    if ((flags & AMDR_PORT_FILTEREN) != 0) {
        amhw_port_pin_filter_enable (p_hw_port, pin);
    }

    /* 需要设置引脚高驱动力  */
    if ((flags & AMDR_PORT_DRIVERHIGH) != 0) {
        amhw_port_pin_drive_high (p_hw_port, pin);
    }

    return AM_OK;
}

/**
 * \brief 获取引脚状态
 */
int am_gpio_get (int pin)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    amhw_gpio_t *p_hw_gpio = p_gpio_devinfo->p_hw_gpio;

    return (int)amhw_gpio_pin_input_get(p_hw_gpio, pin);
}

/**
 * \brief 设置引脚输出状态
 */
int am_gpio_set (int pin, int value)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    amhw_gpio_t *p_hw_gpio = p_gpio_devinfo->p_hw_gpio;

    if (value == 0) {
        amhw_gpio_pin_out_low(p_hw_gpio, pin);
    } else {
        amhw_gpio_pin_out_high(p_hw_gpio, pin);
    }

    return AM_OK;
}

/**
 * \brief 引脚输出翻转
 */
int am_gpio_toggle (int pin)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    amhw_gpio_t *p_hw_gpio = p_gpio_devinfo->p_hw_gpio;

    amhw_gpio_pin_out_toggle (p_hw_gpio, pin);

    return AM_OK;
}

/**
 * \brief 引脚触发形式设置
 */
int am_gpio_trigger_cfg (int pin, uint32_t flag)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    amhw_port_t *p_hw_port = p_gpio_devinfo->p_hw_port;
    amhw_gpio_t *p_hw_gpio = p_gpio_devinfo->p_hw_gpio;

    /* 设置管脚为输入方向 */
    amhw_gpio_pin_dir_input(p_hw_gpio, pin);

    /* 管脚上/下拉使能 */
    amhw_port_pin_pull_enable(p_hw_port, pin);

    /* 引脚输入上拉 */
    amhw_port_pin_pull_up(p_hw_port, pin);

    switch (flag) {

    case AM_GPIO_TRIGGER_OFF:           /* 关闭触发 */
        amhw_port_pin_irq_disable(p_hw_port, pin);
        break;

    case AM_GPIO_TRIGGER_HIGH:          /* 高电平触发 */
        amhw_port_pin_irq_cfg(p_hw_port, pin, AMHW_INT_HIGH);
        break;

    case AM_GPIO_TRIGGER_LOW:           /* 低电平触发 */
        amhw_port_pin_irq_cfg(p_hw_port, pin, AMHW_INT_LOW);
        break;

    case AM_GPIO_TRIGGER_RISE:          /* 上升沿触发 */
        amhw_port_pin_irq_cfg(p_hw_port, pin, AMHW_INT_RISING);
        break;

    case AM_GPIO_TRIGGER_FALL:          /* 下降沿触发 */
        amhw_port_pin_irq_cfg(p_hw_port, pin, AMHW_INT_FALLING);
        break;

    case AM_GPIO_TRIGGER_BOTH_EDGES:    /* 双边沿触发 */
        amhw_port_pin_irq_cfg(p_hw_port, pin, AMHW_PINT_EITHER);
        break;

    default:
        break;
    }

    /* 清引脚中断标志位 */
    amhw_port_pin_isf_clr(p_hw_port, pin);

    return AM_OK;
}

/**
 * \brief GPIO 中断服务响应函数。
 * \param[in] p_arg : 当前中断映射编号。
 * \return 无
 */
static void __gpio_eint_isr (void *p_arg)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    int          slot      = (int)p_arg;
    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;

    int         pin_ium=0;

    /* 获取有关回调函数及参数 */
    pfn_isr = p_gpio_devinfo->p_triginfo[slot].pfn_callback;
    p_arg_tmp   = p_gpio_devinfo->p_triginfo[slot].p_arg;

    /* 获得中断引脚号 */
    pin_ium = p_gpio_devinfo->p_infomap[slot];

    /* 清中断标志 */
    amhw_port_pin_isf_clr (AMHW_PORT, pin_ium);

    if (pfn_isr != NULL) {
        pfn_isr(p_arg_tmp);
    }
}

/**
 * \brief 连接引脚中断回调函数
 */
int am_gpio_trigger_connect (int           pin,
                             am_pfnvoid_t  pfn_callback,
                             void         *p_arg)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    uint32_t port;

    int          key;
    int          i;
    int          slot      = -1;
    int          inum      = 0;
    int          err       = AM_OK;
    bool_t       already   = FALSE;

    /** \brief 获得引脚对应的引脚端号 */
    port = pin >> 5;

    /* 关闭CPU中断 */
    key = am_int_cpu_lock();

    for (i = 0; i < p_gpio_devinfo->pint_count; i++) {
        if ((p_gpio_devinfo->p_infomap)[i] == AMDR_GPIO_INVALID_PIN_MAP) {
            slot = i;
            break;
        }
        if (p_gpio_devinfo->p_infomap[i] == pin) {
            already = TRUE;
            break;
        }
    }

    if (already) {

        /* 打开CPU中断 */
        am_int_cpu_unlock(key);
        return -AM_EINVAL;
    }

    if (port < 2) {     /* 判断是哪个端口配置的中断 */
        inum = 0;       /* A端口引脚产生的中断 */
    } else {
        inum = 1;       /* C\D端口产生的中断 */
    }

    if (-1 != slot) {
        err = am_int_connect((p_gpio_devinfo->inum_pin)[inum], /* 写入中断号 */
                             (am_pfnvoid_t)__gpio_eint_isr,
                             (void *)slot);

        /*  保存触发引脚及回调函数 */
        if (AM_OK == err) {
            (p_gpio_devinfo->p_infomap)[slot]               = pin;
            (p_gpio_devinfo->p_triginfo)[slot].pfn_callback = pfn_callback;
            (p_gpio_devinfo->p_triginfo)[slot].p_arg        = p_arg;
        }

        /* 打开中断 */
        am_int_cpu_unlock(key);
    }
     return AM_OK;
}


/**
 * \brief 删除引脚中断回调函数连接
 */
int am_gpio_trigger_disconnect (int           pin,
                                am_pfnvoid_t  pfn_callback,
                                void         *p_arg)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    int key;
    int i;
    int slot = -1;

    /* 关闭CPU中断 */
    key = am_int_cpu_lock();

    for (i = 0; i < p_gpio_devinfo->pint_count; i++) {
        if ((p_gpio_devinfo->p_infomap)[i] == pin) {
            slot = i;
            break;
        }
    }

    if (slot == -1) {

        /* 打开CPU中断 */
        am_int_cpu_unlock(key);
        return -AM_EINVAL;
    }

    am_int_disconnect((p_gpio_devinfo->inum_pin)[slot],
                      (am_pfnvoid_t)__gpio_eint_isr,
                      (void *)slot);

    (p_gpio_devinfo->p_infomap)[slot]               = AMDR_GPIO_INVALID_PIN_MAP;
    (p_gpio_devinfo->p_triginfo)[slot].pfn_callback = NULL;

    /* 打开CPU中断 */
    am_int_cpu_unlock(key);

    return AM_OK;
}

/**
 * \brief 使能引脚中断。
 */
int am_gpio_trigger_on (int pin)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    int key;
    int i;
    int slot = -1;

    uint32_t port, inum;

    /* 关闭CPU中断 */
    key = am_int_cpu_lock();

    for (i = 0; i < p_gpio_devinfo->pint_count; i++) {
        if ((p_gpio_devinfo->p_infomap)[i] == pin) {
            slot = i;
            break;
        }
    }

    /* 打开CPU中断 */
    am_int_cpu_unlock(key);

    if (slot == -1) {
        return -AM_ENXIO;
    }

    /** \brief 获得引脚对应的引脚端号 */
    port = pin >> 5;

    if (port < 1) {     /* 判断是哪个端口配置的中断 */
        inum = 0;       /* A端口引脚产生的中断 */
    } else {
        inum = 1;       /* C\D端口产生的中断 */
    }

    return am_int_enable((p_gpio_devinfo->inum_pin)[inum]);
}

/**
 * \brief 禁能引脚中断。
 */
int am_gpio_trigger_off (int pin)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    int key;
    int i;
    int slot = -1;

    uint32_t port, inum;

    /* 关闭CPU中断 */
    key = am_int_cpu_lock();

    for (i = 0; i < p_gpio_devinfo->pint_count; i++) {
        if ((p_gpio_devinfo->p_infomap)[i] == pin) {
            slot = i;
            break;
        }
    }

    /* 打开CPU中断 */
    am_int_cpu_unlock(key);

    if (slot == -1) {
        return -AM_ENXIO;
    }

    /** \brief 获得引脚对应的引脚端号 */
    port = pin >> 5;

    if (port < 2) {     /* 判断是哪个端口配置的中断 */
        inum = 0;       /* A端口引脚产生的中断 */
    } else {
        inum = 1;       /* C\D端口产生的中断 */
    }

    return am_int_disable((p_gpio_devinfo->inum_pin)[inum]);
}

/**
 * \brief GPIO初始化
 */
int amdr_gpio_init (amdr_gpio_dev_t           *p_dev,
                    const amdr_gpio_devinfo_t *p_devinfo)
{
    int i;

    if (p_dev == NULL || p_devinfo == NULL) {
        return -AM_EINVAL;
    }

    p_dev->p_devinfo  = p_devinfo;
    __gp_gpio_dev     = p_dev;
    p_dev->valid_flg  = TRUE;

    if ((p_devinfo->p_triginfo == NULL) ||
        (p_devinfo->p_infomap == NULL)) {
        p_dev->valid_flg = FALSE;
    }

    /* 中断表可用 */
    if (p_dev->valid_flg) {
        for (i = 0; i < p_devinfo->pint_count; i++) {
            (p_devinfo->p_infomap)[i]               = AMDR_GPIO_INVALID_PIN_MAP;
            (p_devinfo->p_triginfo)[i].pfn_callback = NULL;
        }
    }

    return AM_OK;
}

/**
 * \brief GPIO去初始化
 */
void amdr_gpio_deinit (amdr_gpio_dev_t *p_dev)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, p_dev);
    int i;

    if (p_dev->valid_flg) {
        for (i = 0; i < p_gpio_devinfo->pint_count; i++) {
            (p_gpio_devinfo->p_infomap)[i]               = 0;
            (p_gpio_devinfo->p_triginfo)[i].pfn_callback = NULL;
        }
    }

    for (i = 0; i < p_gpio_devinfo->pint_count; i++) {
        am_int_disable((p_gpio_devinfo->inum_pin)[i]);
    }
}

/* end of file */
