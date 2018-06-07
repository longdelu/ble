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
 * \brief DAC驱动实现
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-10-28  xiongyanming, first implementation.
 * \endinternal
 */

#include "amdr_dac.h"
#include "am_int.h"

/*******************************************************************************
* 私有定义
*******************************************************************************/

#define __DAC_HW_DECL(p_hw_dac, p_drv)    \
             amhw_dac_t *p_hw_dac =       \
            ((amdr_dac_dev_t *)p_drv)->p_devinfo->p_hw_dac

#define __DAC_BITS_GET(p_drv)             \
        (((amdr_dac_dev_t *)p_drv)->p_devinfo->bits)

#define __DAC_VREF_GET(p_drv)             \
        (((amdr_dac_dev_t *)p_drv)->p_devinfo->vref)

/*******************************************************************************
* 函数声明
*******************************************************************************/
static uint32_t __dac_get_bits (void *p_drv);

static uint32_t __dac_get_vref (void *p_drv);

static int __dac_val_set (void *p_drv, int chan, uint32_t value);

static int __dac_enable (void *p_drv, int chan );

static int __dac_disable (void *p_drv, int chan );

/**
 * \brief DAC服务函数 
 */
static const struct am_dac_drv_funcs __g_dac_drvfuncs = {
       __dac_get_bits,
       __dac_get_vref,
       __dac_val_set,
       __dac_enable,
       __dac_disable
};

/******************************************************************************/
/**
 * \brief 获取DAC转换精度。
 */
static uint32_t __dac_get_bits (void *p_drv)
{
    return (uint32_t)__DAC_BITS_GET(p_drv);
}

/**
 * \brief 获取DAC参考电压。
 */
static uint32_t __dac_get_vref (void *p_drv)
{
    return (uint32_t)__DAC_VREF_GET(p_drv);
}

/**
 * \brief 设置通道的DAC转换值。
 */
int __dac_val_set (void *p_drv, int chan, uint32_t value)
{
    __DAC_HW_DECL(p_hw_dac, p_drv);
    amdr_dac_dev_t *p_dev = (amdr_dac_dev_t *)p_drv;

    if (p_dev == NULL ) {
        return -AM_EINVAL;
    }
    if (chan != 0) {
        return -AM_ENXIO;       /* 无效的通道值      */
    }
    p_dev->chan = chan;
    amhw_dac_dat_set(p_hw_dac,AMHW_DAC_DAT_0,value);
    return AM_OK;
}
/**
 * \brief 启动DAC转换
 */
static int __dac_enable (void *p_drv, int chan )
{
    
    __DAC_HW_DECL(p_hw_dac, p_drv);
    amdr_dac_dev_t *p_dev = (amdr_dac_dev_t *)p_drv;

    if (p_dev == NULL ) {
        return -AM_EINVAL;
    }

    if (chan != 0) {
        return -AM_ENXIO;       /* 无效的通道值       */
    }
    
    p_dev->chan           = chan;
   
    amhw_dac_enable(p_hw_dac);  /* 使能DAC            */
    
    return AM_OK;
}

/**
 * \brief 禁止DAC转换
 */
static int __dac_disable (void *p_drv, int chan )
{
    
    __DAC_HW_DECL(p_hw_dac, p_drv);
    amdr_dac_dev_t *p_dev = (amdr_dac_dev_t *)p_drv;

    if (p_dev == NULL ) {
        return -AM_EINVAL;
    }

    if (chan != 0) {
        return -AM_ENXIO;         /* 无效的通道值 */
    }
    
    p_dev->chan           = chan;
   
    amhw_dac_disable(p_hw_dac);  /* 禁能DAC        */
    
    return AM_OK;
}

/**
 * \brief DAC初始化
 */
am_dac_handle_t amdr_dac_init (amdr_dac_dev_t           *p_dev, 
                               const amdr_dac_devinfo_t *p_devinfo)
{

    if ((p_devinfo == NULL) || (p_dev == NULL)) {
        return NULL;
    }
    
    p_dev->p_devinfo         = p_devinfo;
    p_dev->dac_serve.p_funcs = &__g_dac_drvfuncs;
    p_dev->dac_serve.p_drv   = p_dev; 
    
    p_dev->chan              = AMHW_DAC0_CHAN_0;
    /*
     * 配置DAC相关功能
     */
    amhw_dac_c0_cfg(p_devinfo->p_hw_dac,
                    AMHW_DAC_C0_VREF_SEL(p_devinfo->vref_source) |
                    AMHW_DAC_C0_TRG_SOFT_EN                      |
                    AMHW_DAC_C0_POWER_SEL(AMHW_DAC_POWER_LOW));
    amhw_dac_disable(p_devinfo->p_hw_dac);
    return (am_dac_handle_t)(&(p_dev->dac_serve));
}

/**
 * \brief DAC去初始化
 */
void amdr_dac_deinit (amdr_dac_dev_t *p_dev)
{
    p_dev->dac_serve.p_funcs    = NULL;
    p_dev->dac_serve.p_drv      = NULL;
    amhw_dac_disable(p_dev->p_devinfo->p_hw_dac);
}

/* end of file */
