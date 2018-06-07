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
 * \brief ADC驱动，服务ADC标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-23  xjc, first implementation.
 * \endinternal
 */

#ifndef  __AMDR_ADC_H
#define  __AMDR_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_adc.h"
#include "amhw_adc.h"

/**
 * \addtogroup amdr_if_adc
 * \copydoc amdr_adc.h
 * @{
 */
#define AMDR_ADC_AVG_DIS     0    /**< \brief 不使用硬件平均功能 */
#define AMDR_ADC_AVG_EN      1    /**< \brief 使用硬件平均功能 */
#define AMDR_ADC_CMP_DIS     0    /**< \brief 不使用比较功能 */
#define AMDR_ADC_CMP_EN      1    /**< \brief 使用比较功能 */

/**
 * \name ADC 输入配置标志
 * \anchor grp_amdr_adc_diff
 * @{
 */
#define AMDR_ADC_DIFF_EN      TRUE    /**< \brief 使用差分输入模式 */
#define AMDR_ADC_DIFF_DIS     FALSE   /**< \brief 使用单端输入模式 */
/** @} */

/**
 * \name ADC 触发模式配置标志
 * \anchor grp_amdr_adc_mode_trg
 * @{
 */
#define AMDR_ADC_MODE_TRG_SF      0    /**< \brief 软件触发模式标志 */
#define AMDR_ADC_MODE_TRG_HW      1    /**< \brief 硬件触发模式配置 */
/** @} */

/**
 * \name ADC 状态控制寄存器3位域定义
 * \anchor grp_amdr_cmp_mode
 * @{
 */
/** \brief 比较条件为 "res < cmp_val1" */
#define AMDR_ADC_CMP_MODE_1   0

/** \brief 比较条件为 "res >= cmp_val1" */
#define AMDR_ADC_CMP_MODE_2   1

/** \brief 比较条件为 "res < a" 或者  "res > b" （a=min(cmp_val1, cmp_val2),b=max(cmp_val1, cmp_val2)）*/
#define AMDR_ADC_CMP_MODE_3   2

/** \brief 比较条件为 " a < res < b" （a=min(cmp_val1, cmp_val2),b=max(cmp_val1, cmp_val2)）*/
#define AMDR_ADC_CMP_MODE_4   3

/** \brief 比较条件为 " a <= res <= b" （a=min(cmp_val1, cmp_val2),b=max(cmp_val1, cmp_val2)）*/
#define AMDR_ADC_CMP_MODE_5   4

/** \brief 比较条件为 "res <= a" 或者  "res >= b" （a=min(cmp_val1, cmp_val2),b=max(cmp_val1, cmp_val2)）*/
#define AMDR_ADC_CMP_MODE_6   5
/** @} */

/**
 * \brief ADC 硬件平均功能信息结构体
 */
typedef struct amdr_adc_avg_info {
    /** \brief 硬件平均使能标志 */
    uint8_t         enable;
    /** \brief 平均的采样值个数 */
    amhw_adc_avgs_t avgs_num;
} amdr_adc_avg_info_t;

/**
 * \brief ADC 比较功能信息结构体
 */
typedef struct amdr_adc_cmp_info {
    /** \brief 比较功能使能标志 */
    uint8_t  enable;

    /** \brief 比较模式 */
    uint16_t cmp_mode;

    /** \brief 比较功能阈值1 */
    uint16_t cmp_val1;

    /** \brief 比较功能阈值2 */
    uint16_t cmp_val2;

} amdr_adc_cmp_info_t;



/**
 * \brief ADC 设备信息
 */
typedef struct amdr_adc_devinfo {

    /** \brief 指向ADC寄存器块的指针 */
    amhw_adc_t *p_hw_adc;

    /**
     * \brief ADC参考电压，单位：mV
     *
     * 参考电压 Vref = (Vrefh - Vrefl)  为了在最大采样速率时
     * 稳定运行ADC，确保：Vrefh = 1.13~VDDA，Vrefl典型值为： Vss或 Vssa
     *
     * \note 该参考电压由具体的电路决定
     *
     */
    uint32_t   vref;

    /** \brief 采样精度（当is_diff=TRUE，取值为[9,11,13,16], 否则取值为[8,10,12,16]） */
    uint8_t    bits;

    /** \brief 是否使用差分模式（/ref grp_amdr_adc_diff, 当chan > 3,此值会忽略）*/
    uint8_t    is_diff;

    /** \brief ADC中断向量号 */
    uint8_t    inum;

    /** \brief 触发模式 参考/ref grp_amdr_adc_mode_trg */
    uint8_t    mode_trg;

    /** \brief 使用的序列 */
    amhw_adc_seq_index_t  seq;

    /** \brief DMA传输通道号,如DMA_CHAN_0，若为-1指不使用DMA传输 */
    int                       dma_chan;

    /** \brief 指向硬件平均信息结构体指针 */
   const amdr_adc_avg_info_t *p_adc_avg_info;

    /** \brief 指向比较功能信息结构体指针 */
   const amdr_adc_cmp_info_t *p_adc_cmp_info;
} amdr_adc_devinfo_t;

/**
 * \brief ADC设备实例
 */
typedef struct amdr_adc_dev {

    /** \brief ADC标准服务 */
    am_adc_serv_t             adc_serv;

    /** \brief 指向ADC设备信息的指针 */
    const amdr_adc_devinfo_t *p_devinfo;

    /** \brief 存放用户启动转换后的回调函数 */
    pfn_adc_seq_complete_t    pfn_callback;

    /** \brief 用户启动回调函数的回调函数参数 */
    void                    *p_arg;

    /** \brief 当前转换的序列描述符数组首地址 */
    am_adc_buf_desc_t       *p_desc;

    /** \brief 当前转换的序列描述符数量 */
    uint32_t                desc_num;

    /** \brief 转换的次数 */
    uint32_t                count;

    /** \brief 转换标志   */
    uint32_t                flags;

    /** \brief 当前转换的通道 */
    uint32_t                chan;

    /** \brief 指向描述符   */
    uint32_t                desc_pos;

    /** \brief 指向当前保存数据的位置 */
    uint32_t                index;

    /** \brief 当前转换次数 */
    uint32_t                count_pos;
} amdr_adc_dev_t;

/**
 * \brief ADC初始化
 *
 * 默认初始化序列A
 *
 * \param[in] p_dev     : 指向ADC设备的指针
 * \param[in] p_devinfo : 指向ADC设备信息的指针
 *
 * \return ADC标准服务操作句柄 如果为 NULL，表明初始化失败
 * \note   默认设置100KHz采样率（需bus总线时钟足够大）
 */
am_adc_handle_t amdr_adc_init(amdr_adc_dev_t           *p_dev,
                              const amdr_adc_devinfo_t *p_devinfo);

/**
 * \brief ADC去初始化
 *
 * 默认去初始化序列A
 *
 * \param[in] p_dev : 指向ADC设备的指针
 *
 * \return 无
 */
void amdr_adc_deinit(amdr_adc_dev_t *p_dev);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /*__AMDR_ADC_H  */

/* end of file */
