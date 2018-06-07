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
 * \brief 数模转换（DAC）操作接口
 * 1. 片上可编程输出产生器(电压输出从 1/4096Vin 到 Vin Vin 步长是 1/4096Vin)
 * 2. Vin 可以从两个参考电源中选择
 * 3. 在正常停机模式的静态操作
 * 4. 支持 16 位的数据缓冲区
 * 5. 支持 DMA
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-22  xiongyanming, first implementation.
 * \endinternal
 */

#ifndef __AMHW_DAC_H
#define __AMHW_DAC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"

/**
 * \addtogroup amhw_if_dac
 * \copydoc amhw_dac.h
 * @{
 */

/**
 * \brief dac 寄存器块结构体
 */
typedef struct amhw_dac {
    struct{
        __IO uint8_t datl;                /**< \brief DAC 数据（低）寄存器   */
        __IO uint8_t dath;                /**< \brief DAC 数据（高）寄存器   */
    } dat[2];                             /**< \brief DAC 数据寄存器数组     */
    __IO uint8_t reserved[28];            /**< \brief 保留                   */
     __IO uint8_t sr;                     /**< \brief DAC 状态寄存器         */
     __IO uint8_t c0;                     /**< \brief DAC控制寄存器0         */
     __IO uint8_t c1;                     /**< \brief DAC控制寄存器 1        */
     __IO uint8_t c2;                     /**< \brief DAC控制寄存器 2        */
} amhw_dac_t;


/**
 * \brief DAC 通道编号
 */
#define AMHW_DAC0_CHAN_0          0      /**< \brief DAC 通道 0              */

/**
 * \brief DAC 缓冲区大小
 */
#define AMHW_DAC_DATL_COUNT       2      /**< \brief DAC 缓存区大小          */


/**
 * \name DAC数据寄存器设置位宏定义
 * @{
 */

/**
 * \brief DAC 数据通道索引
 */
typedef enum amhw_dac_dat_index {
    AMHW_DAC_DAT_0 = 0,                  /**< \brief  数据通道0              */
    AMHW_DAC_DAT_1                       /**< \brief  数据通道1              */
} amhw_dac_dat_index_t;

/** \brief 数据寄存器掩码                                                    */
#define AMHW_DAC_DAT_MASK     0xFFu
/** \brief 数据寄存器设置                                                    */
#define AMHW_DAC_DAT(x)       (((uint8_t)(((uint8_t)(x))<<0))&AMHW_DAC_DAT_MASK)

/** @} */

/**
 * \name DAC 控制寄存器C0 位宏定义
 * \anchor grp_amhw_dac_c0
 * @{
 */
 
/**
 * \brief DAC 参考电压选择索引
 */
typedef enum amhw_dac_vref_sel {
    AMHW_DAC_VREF_1,                    /**< \brief  DACREF_1 作为参考电压 */
    AMHW_DAC_VREF_2,                    /**< \brief  DACREF_2 作为参考电压 */
} amhw_dac_vref_sel_t;

 /**
 * \brief DAC触发源选择索引
 */
typedef enum amhw_dac_trg_sel {
    AMHW_DAC_TRG_HARD,                  /**< \brief  DAC 硬件触发         */
    AMHW_DAC_TRG_SOFT,                  /**< \brief  DAC 软件触发         */
} amhw_dac_trg_sel_t;

 /**
 * \brief DAC功耗模式索引
 */
typedef enum amhw_dac_power_sel {
    AMHW_DAC_POWER_HIGH,                  /**< \brief  DAC 硬件触发       */
    AMHW_DAC_POWER_LOW                    /**< \brief  DAC 软件触发       */
} amhw_dac_power_sel_t;

/** \brief DAC使能                                                        */
#define AMHW_DAC_C0_EN                  ((uint8_t)(1<<7))
/** \brief DAC禁能                                                        */
#define AMHW_DAC_C0_DISEN               ((uint8_t)(0<<7))   
/** \brief DAC参考电压选择                                                */
#define AMHW_DAC_C0_VREF_SEL(x)         ((uint8_t)(((uint8_t)(x))<<6))
/** \brief  DAC 触发源选择                                                */
#define AMHW_DAC_C0_TRG_SEL(x)          ((uint8_t)(((uint8_t)(x))<<5)) 
/** \brief DAC软件触发使能                                                */
#define AMHW_DAC_C0_TRG_SOFT_EN         ((uint8_t)(1<<4))  
/** \brief DAC软件触发禁能                                                */
#define AMHW_DAC_C0_TRG_SOFT_DISEN      ((uint8_t)(0<<4))  
/** \brief 使能DAC低功耗模式                                              */
#define AMHW_DAC_C0_POWER_SEL(x)        ((uint8_t)(((uint8_t)(x))<<3)) 

/** \brief DAC 缓冲读取顶指针中断使能                                     */
#define AMHW_DAC_C0_BTI_EN              ((uint8_t)(1<<1))
/** \brief DAC 缓冲读取顶指针中断禁能                                     */
#define AMHW_DAC_C0_BTI_DISEN           ((uint8_t)(0<<1))
/** \brief DAC 缓冲读取底指针中断使能                                     */
#define AMHW_DAC_C0_BBI_EN              ((uint8_t)(1<<0))
/** \brief DAC 缓冲读取底指针中断禁能                                     */
#define AMHW_DAC_C0_BBI_DISEN           ((uint8_t)(0<<0))

/** @} */

/**
 * \name DAC 控制寄存器C1 位宏定义
 * @{
 */
 
 /**
 * \brief DAC缓冲溢出模式选择索引
 */
typedef enum amhw_dac_buf_mode {
    AMHW_DAC_BUFMODE_NORMAL,              /**< \brief  DAC 普通模式         */
    AMHW_DAC_BUFMODE_SCAN,                /**< \brief  DAC 单次扫描模式     */
} amhw_dac_buf_mode_t;

/** \brief DAC的DMA使能                                                     */
#define AMHW_DAC_C1_DMA_EN              ((uint8_t)(1<<7))   
/** \brief DAC 缓冲溢出模式                                                 */
#define AMHW_DAC_C1_BUFMODE_SEL(x)      ((uint8_t)(((uint8_t)(x))<<2)) 
/** \brief DAC 缓冲使能                                                     */
#define AMHW_DAC_C1_BUF_EN              ((uint8_t)(1<<0))

/** @} */

/**
 * \name DAC 状态寄存器SR位宏定义
 * @{
 */
 
/** \brief DAC 缓冲顶指针溢出掩码                                            */
#define AMHW_DAC_SR_BFRP_TOPF_MASK       ((uint8_t)(1<<1)) 
/** \brief DAC 缓冲底指针溢出掩码                                            */
#define AMHW_DAC_SR_BFRP_BOTF_MASK       ((uint8_t)(1<<0))

/** @} */

/**
 * \brief DAC 缓冲区配置参数结构体
 */
typedef struct amhw_dac_buffer_config
{
    bool_t buffer_enable;          /**< \brief  DAC 硬件触发               */
    amhw_dac_trg_sel_t trig_mode;  /**< \brief  DAC 硬件触发               */
    bool_t start_int_enable;       /**< \brief  DAC 缓冲读取底指针中断使能 */
    bool_t upper_int_enable;       /**< \brief  DAC 缓冲读取顶指针中断使能 */
    bool_t dma_enable;             /**< \brief  DAC DMA使能位              */
    amhw_dac_buf_mode_t buf_mode;  /**< \brief  DAC BUF工作模式选择位      */
    uint8_t upperidx;              /**< \brief  DAC 上限值                 */
} amhw_dac_buffer_config_t;

/**
 * \brief dac 中断标志位枚举定义
 */
typedef enum amhw_dac_flag_t
{
    AMHW_DAC_BUF_FLAG_START = 1U, /**< \brief 缓冲指针开始事件标志位 */
    AMHW_DAC_BUF_FLAG_UPPER = 2U  /**< \brief 缓存指针到达上限标志位 */
} amhw_dac_flag_t;

/**
 * \brief DAC控制寄存器0配置函数
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 * \param[in] flags    : 配置参数 参见\ref grp_amhw_dac_c0
                          AMHW_DAC_C0_*宏值或AMHW_DAC_C0_*多个宏值的或值
 *
 * \return 无
 */
am_static_inline
void amhw_dac_c0_cfg (amhw_dac_t *p_hw_dac, uint8_t flags)
{
    p_hw_dac->c0 = flags;
}

/**
 * \brief 写数据到DAC 低数据寄存器
 *
 * \param[in] p_hw_dac  : 指向DAC寄存器块的指针
 * \param[in] dat_index : AMHW_DAC_DAT_* 枚举类型
 * \param[in] datl      : datl值
 *
 * \return 无
 */
am_static_inline
void amhw_dac_datl_set (amhw_dac_t *p_hw_dac, amhw_dac_dat_index_t dat_index, 
                        uint8_t datl)
{
    p_hw_dac->dat[dat_index].datl = AMHW_DAC_DAT(datl);
}

/**
 * \brief 写数据到DAC 高数据寄存器
 *
 * \param[in] p_hw_dac  : 指向DAC寄存器块的指针
 * \param[in] dat_index : AMHW_DAC_DAT_* 枚举类型
 * \param[in] dath      : dath值
 *
 * \return 无
 */
am_static_inline
void amhw_dac_dath_set (amhw_dac_t *p_hw_dac, amhw_dac_dat_index_t dat_index,
                        uint8_t dath)
{
    p_hw_dac->dat[dat_index].dath = AMHW_DAC_DAT(dath);
}

/**
 * \brief 写数据到DAC 数据寄存器
 *
 * \param[in] p_hw_dac  : 指向DAC寄存器块的指针
 * \param[in] dat_index : AMHW_DAC_DAT_* 枚举类型
 * \param[in] dat       : dat值
 *
 * \return 无
 */
am_static_inline
void amhw_dac_dat_set (amhw_dac_t *p_hw_dac, amhw_dac_dat_index_t dat_index,
                       uint16_t dat)
{
    p_hw_dac->dat[dat_index].datl = AMHW_DAC_DAT(dat);
    p_hw_dac->dat[dat_index].dath = AMHW_DAC_DAT(dat>>8);
}


/**
 * \brief 使能DAC功能函数
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_dac_enable (amhw_dac_t *p_hw_dac )
{
    p_hw_dac->c0 |= AMHW_DAC_C0_EN;
}

/**
 * \brief 禁能DAC功能函数
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_dac_disable (amhw_dac_t *p_hw_dac )
{
    p_hw_dac->c0 &= ~AMHW_DAC_C0_EN;
}

/**
 * \brief DAC参考电压设置函数
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 * \param[in] vref     : amhw_dac_vref_sel_t枚举类型
 *                       AMHW_DAC_VREF_* 宏值
 *
 * \return 无
 */
am_static_inline
void amhw_dac_refv_sel (amhw_dac_t *p_hw_dac ,amhw_dac_vref_sel_t vref)
{
    p_hw_dac->c0 &= ~(1<<6);
    p_hw_dac->c0 |= AMHW_DAC_C0_VREF_SEL(vref);
}

/**
 * \brief DAC触发源设置函数
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 * \param[in] trg      : amhw_dac_trg_sel_t枚举类型
 *                       AMHW_DAC_TRG_* 宏值
 *
 * \return 无
 */
am_static_inline
void amhw_dac_trg_sel (amhw_dac_t *p_hw_dac ,amhw_dac_trg_sel_t trg)
{
    p_hw_dac->c0 &= ~(1<<5);
    p_hw_dac->c0 |= AMHW_DAC_C0_TRG_SEL(trg);
}

/**
 * \brief DAC软件触发使能函数
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_dac_soft_trg_enable (amhw_dac_t *p_hw_dac )
{
    p_hw_dac->c0 |= AMHW_DAC_C0_TRG_SOFT_EN;
}

/**
 * \brief DAC软件触发禁能函数
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_dac_soft_trg_disable (amhw_dac_t *p_hw_dac )
{
    p_hw_dac->c0 &= ~AMHW_DAC_C0_TRG_SOFT_EN;
}

/**
 * \brief DAC功耗模式设置函数
 *
 * \param[in] p_hw_dac   : 指向DAC寄存器块的指针
 * \param[in] power_mode : amhw_dac_power_sel_t枚举类型
 *                         AMHW_POWER_* 宏值
 * \return 无
 */
am_static_inline
void amhw_dac_power_set (amhw_dac_t *p_hw_dac ,amhw_dac_power_sel_t power_mode)
{
    p_hw_dac->c0 &= ~(1<<3);
    p_hw_dac->c0 |= AMHW_DAC_C0_POWER_SEL(power_mode);
}

/**
 * \brief 缓冲读取顶指针中断使能函数
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_dac_buftop_int_enable (amhw_dac_t *p_hw_dac )
{
    p_hw_dac->c0 |= AMHW_DAC_C0_BTI_EN;
}

/**
 * \brief 缓冲读取顶指针中断禁能函数
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_dac_buftop_int_disable (amhw_dac_t *p_hw_dac )
{
    p_hw_dac->c0 &= ~AMHW_DAC_C0_BTI_EN;
}

/**
 * \brief 缓冲读取底指针中断使能函数
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_dac_bufbot_int_enable (amhw_dac_t *p_hw_dac )
{
    p_hw_dac->c0 |= AMHW_DAC_C0_BBI_EN;
}

/**
 * \brief 缓冲读取底指针中断禁能函数
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_dac_bufbot_int_disable (amhw_dac_t *p_hw_dac )
{
    p_hw_dac->c0 &= ~AMHW_DAC_C0_BBI_EN;
}

/**
 * \brief DAC缓冲溢出模式设置函数
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 * \param[in] bf_mode  : amhw_dac_buf_mode_t枚举类型
 *                       AMHW_DAC_BUFMODE_* 宏值
 *
 * \return 无
 */
am_static_inline
void amhw_dac_bufmode_set(amhw_dac_t *p_hw_dac ,amhw_dac_buf_mode_t bf_mode)
{
    p_hw_dac->c1 &= ~(1<<2);
    p_hw_dac->c1 |= AMHW_DAC_C1_BUFMODE_SEL(bf_mode);
}

/**
 * \brief 使能DAC的DMA功能
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_dac_dma_enable (amhw_dac_t *p_hw_dac )
{
    p_hw_dac->c1 |= AMHW_DAC_C1_DMA_EN;
}

/**
 * \brief 禁能DAC的DMA功能
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_dac_dma_disable (amhw_dac_t *p_hw_dac )
{
    p_hw_dac->c1 &= ~AMHW_DAC_C1_DMA_EN;
}

/**
 * \brief 使能DAC的缓冲区功能
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_dac_buf_enable (amhw_dac_t *p_hw_dac )
{
    p_hw_dac->c1 |= AMHW_DAC_C1_BUF_EN;
}

/**
 * \brief 禁能DAC的缓冲区功能
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_dac_buf_disable (amhw_dac_t *p_hw_dac )
{
    p_hw_dac->c1 &= ~AMHW_DAC_C1_BUF_EN;
}

/**
 * \brief 设置DAC缓存区上限值
 *
 * \param[in] p_hw_dac   : 指向DAC寄存器块的指针
 * \param[in] buf_up_val : 缓存区上限值 buf_up_val
 *
 * \return 无
 */
am_static_inline
void amhw_dac_c2_buf_up (amhw_dac_t *p_hw_dac , uint8_t buf_up_val)
{
    p_hw_dac->c2 |= ((uint8_t)(buf_up_val&0x01));
}

/**
 * \brief 获取DAC缓冲区标志位
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 * \param[in] flag     : 想要得到的中断标志位 amhw_dac_flag_t 枚举类型
 *                                            AMHW_DAC_BUF_FLAG_* 宏值
 * \return    TRUE     : 标志位置位
 *            FALSE    : 标志位没有置位
 */
am_static_inline
bool_t amhw_dac_bufint_flag_get (amhw_dac_t *p_hw_dac ,amhw_dac_flag_t flag)
{


    bool_t b_ret = TRUE;
    switch (flag)
    {
    case AMHW_DAC_BUF_FLAG_START:
        b_ret = (bool_t)((p_hw_dac->sr & AMHW_DAC_SR_BFRP_BOTF_MASK));
        break;
    case AMHW_DAC_BUF_FLAG_UPPER:
        b_ret = (bool_t)(p_hw_dac->sr & AMHW_DAC_SR_BFRP_TOPF_MASK);
        break;
    default:
        b_ret = FALSE;
        break;
    }
    return b_ret;
}

/**
 * \brief 清除DAC缓冲区中断标志位
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 * \param[in] flag     : 想要得到的中断标志位 amhw_dac_flag_t 枚举类型
 *                                            AMHW_DAC_BUF_FLAG_* 宏值
 * \return 无
 */
am_static_inline
void amhw_dac_bufint_flag_clr (amhw_dac_t *p_hw_dac ,amhw_dac_flag_t flag)
{
    switch (flag)
    {
    case AMHW_DAC_BUF_FLAG_START:
        p_hw_dac->sr &= ~AMHW_DAC_SR_BFRP_BOTF_MASK;
        break;
    case AMHW_DAC_BUF_FLAG_UPPER:
        p_hw_dac->sr &= ~AMHW_DAC_SR_BFRP_TOPF_MASK;
        break;
    default:
        p_hw_dac->sr &= ~AMHW_DAC_SR_BFRP_BOTF_MASK;
        p_hw_dac->sr &= ~AMHW_DAC_SR_BFRP_TOPF_MASK;
        break;
    }
}

/**
 * \brief 设置缓冲区值
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 * \param[in] start    : 想要发送值的起始位置
 * \param[in] offset   : 每次偏移值
 * \param[in] arr[]    : 存放将要写入缓存区的值
 *                                            
 * \return    AM_ERROR : 设置buf值错误
 *            AM_OK    : 设置buf值成功
 */
am_static_inline
int amhw_dac_buf_val_set (amhw_dac_t *p_hw_dac ,uint8_t start, uint8_t offset,
                          uint16_t arr[])
{
    uint8_t i;

    if (  (!arr) || (start + offset > AMHW_DAC_DATL_COUNT)){
        return AM_ERROR;
    }

    for (i = 0; i < offset; i++) {
        amhw_dac_dat_set(p_hw_dac,(amhw_dac_dat_index_t)(start+i),arr[i]);
    }

    return AM_OK;
   
}
/**
 * \brief DAC数据缓存区初始化
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 * \param[in] p_buf    : 指向DAC缓存区配置结构体
 *
 * \return 无
 */
void amhw_dac_dat_buf_init (amhw_dac_t *p_hw_dac,amhw_dac_buffer_config_t *p_buf);

/**
 * @} amhw_if_dac
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_DAC_H */

/* end of file */





