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
 * \brief DAC驱动，服务DAC标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 15-07-14  xiongyanming, first implementation.
 * \endinternal
 */
 
#ifndef  __AMDR_DAC_H
#define  __AMDR_DAC_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "am_types.h"
#include "am_dac.h"
#include "amhw_dac.h"

/**
 * \addtogroup amdr_if_dac
 * \copydoc amdr_dac.h
 * @{
 */
/**
 * \brief DAC 设备信息 
 */
typedef struct amdr_dac_devinfo {
    
    /** \brief 指向DAC寄存器块的指针 */
    amhw_dac_t *p_hw_dac;

    /** \brief DAC中断号 */
    uint8_t    inum;

    /** \brief DAC转换精度 */
    uint8_t     bits;

    /** 
     * \brief DAC参考电压，单位：mV 
     * 
     * \note 该参考电压由具体的电路决定 
     *
     */
    uint32_t    vref;
    /**
     * \brief DAC参考电压源选择
     *
     * \note 该参考电压由具体的电路决定
     *
     */
    amhw_dac_vref_sel_t    vref_source;


} amdr_dac_devinfo_t;

/**
 * \brief DAC设备实例 
 */
typedef struct amdr_dac_dev {
    
    /** \brief DAC标准服务 */
    am_dac_serv_t             dac_serve;

    /** \brief 指向DAC设备信息的指针 */
    const amdr_dac_devinfo_t *p_devinfo;


    /** \brief 当前转换的通道 */
    uint32_t                 chan;
    
} amdr_dac_dev_t;

/**
 * \brief DAC初始化 
 *
 * 默认初始化序列A 
 *
 * \param[in] p_dev     : 指向DAC设备的指针 
 * \param[in] p_devinfo : 指向DAC设备信息的指针 
 *
 * \return DAC标准服务操作句柄 如果为 NULL，表明初始化失败 
 */
am_dac_handle_t amdr_dac_init(amdr_dac_dev_t           *p_dev, 
                              const amdr_dac_devinfo_t *p_devinfo);

/**
 * \brief DAC去初始化 
 *
 * 默认去初始化序列A 
 *
 * \param[in] p_dev : 指向DAC设备的指针 
 *
 * \return 无 
 */
void amdr_dac_deinit(amdr_dac_dev_t *p_dev);                              
                              
/**
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /*__AMDR_DAC_H  */

/* end of file */
