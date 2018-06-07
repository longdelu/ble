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
 * \brief DAC����ʵ��
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-10-28  xiongyanming, first implementation.
 * \endinternal
 */

#include "amdr_dac.h"
#include "am_int.h"

/*******************************************************************************
* ˽�ж���
*******************************************************************************/

#define __DAC_HW_DECL(p_hw_dac, p_drv)    \
             amhw_dac_t *p_hw_dac =       \
            ((amdr_dac_dev_t *)p_drv)->p_devinfo->p_hw_dac

#define __DAC_BITS_GET(p_drv)             \
        (((amdr_dac_dev_t *)p_drv)->p_devinfo->bits)

#define __DAC_VREF_GET(p_drv)             \
        (((amdr_dac_dev_t *)p_drv)->p_devinfo->vref)

/*******************************************************************************
* ��������
*******************************************************************************/
static uint32_t __dac_get_bits (void *p_drv);

static uint32_t __dac_get_vref (void *p_drv);

static int __dac_val_set (void *p_drv, int chan, uint32_t value);

static int __dac_enable (void *p_drv, int chan );

static int __dac_disable (void *p_drv, int chan );

/**
 * \brief DAC������ 
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
 * \brief ��ȡDACת�����ȡ�
 */
static uint32_t __dac_get_bits (void *p_drv)
{
    return (uint32_t)__DAC_BITS_GET(p_drv);
}

/**
 * \brief ��ȡDAC�ο���ѹ��
 */
static uint32_t __dac_get_vref (void *p_drv)
{
    return (uint32_t)__DAC_VREF_GET(p_drv);
}

/**
 * \brief ����ͨ����DACת��ֵ��
 */
int __dac_val_set (void *p_drv, int chan, uint32_t value)
{
    __DAC_HW_DECL(p_hw_dac, p_drv);
    amdr_dac_dev_t *p_dev = (amdr_dac_dev_t *)p_drv;

    if (p_dev == NULL ) {
        return -AM_EINVAL;
    }
    if (chan != 0) {
        return -AM_ENXIO;       /* ��Ч��ͨ��ֵ      */
    }
    p_dev->chan = chan;
    amhw_dac_dat_set(p_hw_dac,AMHW_DAC_DAT_0,value);
    return AM_OK;
}
/**
 * \brief ����DACת��
 */
static int __dac_enable (void *p_drv, int chan )
{
    
    __DAC_HW_DECL(p_hw_dac, p_drv);
    amdr_dac_dev_t *p_dev = (amdr_dac_dev_t *)p_drv;

    if (p_dev == NULL ) {
        return -AM_EINVAL;
    }

    if (chan != 0) {
        return -AM_ENXIO;       /* ��Ч��ͨ��ֵ       */
    }
    
    p_dev->chan           = chan;
   
    amhw_dac_enable(p_hw_dac);  /* ʹ��DAC            */
    
    return AM_OK;
}

/**
 * \brief ��ֹDACת��
 */
static int __dac_disable (void *p_drv, int chan )
{
    
    __DAC_HW_DECL(p_hw_dac, p_drv);
    amdr_dac_dev_t *p_dev = (amdr_dac_dev_t *)p_drv;

    if (p_dev == NULL ) {
        return -AM_EINVAL;
    }

    if (chan != 0) {
        return -AM_ENXIO;         /* ��Ч��ͨ��ֵ */
    }
    
    p_dev->chan           = chan;
   
    amhw_dac_disable(p_hw_dac);  /* ����DAC        */
    
    return AM_OK;
}

/**
 * \brief DAC��ʼ��
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
     * ����DAC��ع���
     */
    amhw_dac_c0_cfg(p_devinfo->p_hw_dac,
                    AMHW_DAC_C0_VREF_SEL(p_devinfo->vref_source) |
                    AMHW_DAC_C0_TRG_SOFT_EN                      |
                    AMHW_DAC_C0_POWER_SEL(AMHW_DAC_POWER_LOW));
    amhw_dac_disable(p_devinfo->p_hw_dac);
    return (am_dac_handle_t)(&(p_dev->dac_serve));
}

/**
 * \brief DACȥ��ʼ��
 */
void amdr_dac_deinit (amdr_dac_dev_t *p_dev)
{
    p_dev->dac_serve.p_funcs    = NULL;
    p_dev->dac_serve.p_drv      = NULL;
    amhw_dac_disable(p_dev->p_devinfo->p_hw_dac);
}

/* end of file */
