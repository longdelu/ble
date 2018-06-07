/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief ADC drivers implementation
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-23  xjc, first implementation.
 * \endinternal
 */

#include "amdr_adc.h"
#include "am_int.h"
#include "amdr_clk.h"
#include "math.h"
#include "amdr_dma.h"
/*******************************************************************************
* 私有定义
*******************************************************************************/
#define __ADC_HW_DECL(p_hw_adc, p_drv)    \
        amhw_adc_t *p_hw_adc =               \
            ((amdr_adc_dev_t *)p_drv)->p_devinfo->p_hw_adc

#define __ADC_VREF_GET(p_drv)             \
        (((amdr_adc_dev_t *)p_drv)->p_devinfo->vref)


/*******************************************************************************
* 函数声明
*******************************************************************************/

/** \brief 指向ADC中断连接函数 */
static int __amdr_adc_adc_connect (void *p_drv);

/** \brief 启动ADC转换        */
static int __amdr_adc_adc_start  (void                   *p_drv,
                             int                     chan,
                             am_adc_buf_desc_t       *p_desc,
                             uint32_t                desc_num,
                             uint32_t                count,
                             uint32_t                flags,
                             pfn_adc_seq_complete_t  pfn_callback,
                             void                   *p_arg);

/** \brief 停止转换           */
static int __amdr_adc_stop (void *p_drv, int chan);

/** \brief 获取ADC的采样率    */
static int __amdr_adc_rate_get (void       *p_drv,
                                int         chan,
                                uint32_t   *p_rate);

/** \brief 设置ADC的采样率，实际采样率可能存在差异 */
static int __amdr_adc_rate_set (void     *p_drv,
                                int       chan,
                                uint32_t  rate);

/** \brief 获取ADC转换精度 */
static uint32_t __amdr_adc_bits_get(void *p_drv, int chan);

/** \brief 获取ADC参考电压 */
static uint32_t __amdr_adc_vref_get(void *p_drv, int chan);



/**
 * \brief ADC服务函数
 */
static const struct am_adc_drv_funcs __g_adc_drvfuncs = {

    __amdr_adc_adc_connect,
    __amdr_adc_adc_start,
    __amdr_adc_stop,
    __amdr_adc_rate_get,
    __amdr_adc_rate_set,
    __amdr_adc_bits_get,
    __amdr_adc_vref_get
};

/******************************************************************************/
static void __amdr_adc_dma_isr (void *p_arg , uint8_t flag);
/**
 * \brief DMA传输配置
 */
int __amdr_adc_dma_tran_cfg ( amdr_adc_dev_t *p_dev, am_adc_buf_desc_t *p_desc)
{

    uint32_t flags;
    amhw_dma_xfer_desc_t    dma_desc;

    /* DMA传输配置 */
    flags = AMHW_DMA_DCR_PER_REQUEST_ENABLE      |  /* 外设请求源使能    */
            AMHW_DMA_DCR_SINGLE_TRANSFERS        |  /* 单次传输          */
            AMHW_DMA_DCR_AUTO_ALIGN_DISABLE      |  /* 自动对齐禁能      */
            AMHW_DMA_DCR_SOURCE_SIZE_32_BIT      |  /* 源地址4字节读取   */
            AMHW_DMA_DCR_DESTINATION_SIZE_32_BIT |  /* 目的地址4字节写入 */
            AMHW_DMA_DCR_REQUEST_NO_AFFECTED     |  /* 请求无影响        */
            AMHW_DMA_DCR_NO_LINKING              |  /* 无通道连接        */
            AMHW_DMA_DCR_INTERRUTP_ENABLE        |  /* DMA中断使能       */
            AMHW_DMA_DCR_START_DISABLE    ;         /* DMA开始传输使能   */

    /* 连接DMA中断服务函数 */
    amdr_dma_isr_connect(p_dev->p_devinfo->dma_chan, __amdr_adc_dma_isr, p_dev);

    amdr_dma_chan_cfg(p_dev->p_devinfo->dma_chan,
                      AMHW_DMA_TRIGGER_DISABLE |     /**< \brief DMA正常模式 */
                      DMA_REQUEST_MUX0_ADC0);        /**< \brief 请求源      */

    /* 建立通道描述符 */
    amdr_dma_xfer_desc_build(&dma_desc,                      /* 通道描述符   */
                            (uint32_t)&(p_dev->p_devinfo->p_hw_adc->dat[p_dev->p_devinfo->seq]), /* 源端数据缓冲 */
                            (uint32_t)(p_desc->p_buf),       /* 目标数据缓冲 */
                            (uint32_t)((p_desc->length) << 2), /* 传输字节数   */
                            flags);                    /* 传输配置     */

    /* 启动DMA传输，马上开始传输 */
    if (amdr_dma_chan_start(&dma_desc,
                            AMHW_DMA_PER_TO_MER,   /* 外设到 内存   */
                           (uint8_t)p_dev->p_devinfo->dma_chan) == AM_ERROR) {
        return AM_ERROR;
    }

    return AM_OK;
}
/**
 * \brief ADC DMA传输中断服务函数
 */
static void __amdr_adc_dma_isr (void *p_arg , uint8_t flag)
{

    amdr_adc_dev_t *p_dev = (amdr_adc_dev_t *)p_arg;

    if (flag == AMDR_DMA_INT_NORMAL) { /* 通信正常 */

        if (NULL != p_dev->p_desc[p_dev->desc_pos].pfn_complete) {
            p_dev->p_desc[p_dev->desc_pos].pfn_complete(p_dev->p_desc[p_dev->desc_pos].p_arg, AM_OK);
        }

        p_dev->desc_pos++;
        if (p_dev->desc_pos == p_dev->desc_num) { /* 已经接收完成 */
            p_dev->desc_pos = 0;

            if (NULL != p_dev->pfn_callback) {
                p_dev->pfn_callback(p_dev->p_arg, AM_OK);
            }
            p_dev->count_pos++; /* 转换序列标准 */
            if (p_dev->count != 0 && p_dev->count_pos == p_dev->count) {
                p_dev->count_pos = 0;
                __amdr_adc_stop(p_dev, p_dev->chan);  /* 关闭模块 */
                amhw_adc_dma_disable(p_dev->p_devinfo->p_hw_adc); /* 关闭DMA传输*/
                amdr_dma_chan_stop(p_dev->p_devinfo->dma_chan);
                return ; /* 返回 */
            }
        }
        /* 否则启动下一个传输 */
        amhw_adc_dma_disable(p_dev->p_devinfo->p_hw_adc); /* 关闭DMA传输*/
        __amdr_adc_dma_tran_cfg(p_dev, &(p_dev->p_desc[p_dev->desc_pos]));
        amhw_adc_dma_enable(p_dev->p_devinfo->p_hw_adc); /* 使能DMA传输*/
    } else { /* 通信错误 */

    }
}

/**
 * \brief ADC读取数据
 */
static void __adc_int(void *p_arg)
{
    amdr_adc_dev_t *p_dev = (amdr_adc_dev_t *)p_arg;
    am_adc_val_t   *p_buf = NULL;

    uint16_t dat;

    /* 获取存放数据缓冲区的指针 */
    p_buf = (am_adc_val_t *)p_dev->p_desc[p_dev->desc_pos].p_buf;
    if (p_buf == NULL) {
        return;
    }
    /* 获取数据 */
    dat   = amhw_adc_dat_get(p_dev->p_devinfo->p_hw_adc, p_dev->p_devinfo->seq);

    /* 对齐格式 */
    if (AM_ADC_DATA_ALIGN_RIGHT != p_dev->flags) {
        p_buf[p_dev->index] = (dat & 0xFFFF) << 16;
    } else {
        p_buf[p_dev->index] = (dat & 0xFFFF);
    }
    (p_dev->index)++;
    /* 完成一个描述数缓冲区的接收*/
    if (p_dev->index >= p_dev->p_desc[p_dev->desc_pos].length) {
        p_dev->index = 0;
        if (NULL != p_dev->p_desc[p_dev->desc_pos].pfn_complete) {
            p_dev->p_desc[p_dev->desc_pos].pfn_complete(p_dev->p_desc[p_dev->desc_pos].p_arg, AM_OK);
        }

        p_dev->desc_pos++;
        if (p_dev->desc_pos == p_dev->desc_num) { /* 已经接收完成 */
            p_dev->desc_pos = 0;

            if (NULL != p_dev->pfn_callback) {
                p_dev->pfn_callback(p_dev->p_arg, AM_OK);
            }
            p_dev->count_pos++; /* 转换序列标准 */
            if (p_dev->count != 0 && p_dev->count_pos == p_dev->count) {
                p_dev->count_pos = 0;
                __amdr_adc_stop(p_dev, p_dev->chan);  /* 关闭模块 */
                return ; /* 返回 */
            }
        }
    }
}


/** \brief 指向ADC中断连接函数 */
static int __amdr_adc_adc_connect (void *p_drv)
{
    return -AM_ENOTSUP; /* 该函数未实现 （已废弃该函数）*/
}



/**
 * \brief 启动ADC转换
 */
static int __amdr_adc_adc_start  (void                   *p_drv,
                                 int                     chan,
                                 am_adc_buf_desc_t       *p_desc,
                                 uint32_t                desc_num,
                                 uint32_t                count,
                                 uint32_t                flags,
                                 pfn_adc_seq_complete_t  pfn_callback,
                                 void                   *p_arg)
{
    amdr_adc_dev_t *p_dev = NULL;
    uint32_t cfg = 0;


    if (NULL == p_drv || p_desc == NULL) {
        return -AM_EINVAL;
    }

    p_dev = (amdr_adc_dev_t *)p_drv;

    p_dev->p_desc       = p_desc;
    p_dev->chan         = chan;
    p_dev->desc_num     = desc_num;
    p_dev->count        = count;
    p_dev->flags        = flags;
    p_dev->pfn_callback = pfn_callback;
    p_dev->p_arg        = p_arg;
    p_dev->index        = 0;
    p_dev->count_pos    = 0;

    cfg = 0;
    if (p_dev->p_devinfo->dma_chan == -1) { /* 非DMA模式下使能中断 */
       cfg |= AMHW_ADC_SC1_INT_EN_CFG;
    } else {
       /* DMA传输配置 */
       __amdr_adc_dma_tran_cfg(p_dev, &p_desc[0]);
       amhw_adc_dma_enable(p_dev->p_devinfo->p_hw_adc);
    }

    cfg |= AMHW_ADC_SC1_CHAN_SEL_CFG(p_dev->chan);
    if (p_dev->p_devinfo->is_diff && p_dev->chan < 3) {
        cfg |= AMHW_ADC_SC1_DIFF_EN_CFG;
    }

    /* 配置单端、差分模式，以及通道， 启动转换*/
    amhw_adc_sc1_cfg(p_dev->p_devinfo->p_hw_adc, p_dev->p_devinfo->seq, cfg);

    return AM_OK;
}

/**
 * \brief 停止转换
 */
static int __amdr_adc_stop (void *p_drv, int chan)
{
    amdr_adc_dev_t *p_dev = NULL;

    if (NULL == p_drv) {
        return -AM_EINVAL;
    }
    p_dev = (amdr_adc_dev_t *)p_drv;
    amhw_adc_chan_set(p_dev->p_devinfo->p_hw_adc,p_dev->p_devinfo->seq, AMHW_ADC_CHAN_CLOSE);
    amhw_adc_dma_disable(p_dev->p_devinfo->p_hw_adc);
    return AM_OK;
}

/**
 * \brief 获取BCT（位数对FADCK的要求）
 */
uint8_t __amdr_adc_rate_bct_get(uint8_t bits, uint8_t diff)
{
    uint8_t bct = 0;

    /* 位数影响的FADCK时钟个数 */
    switch (bits) {

    case 8:
        bct = 17;
        break;

    case 9:
        bct = 27;
        break;

    case 10:
    case 12:
        bct = 20;
        break;

    case 11:
    case 13:
        bct = 30;
        break;

    case 16:
        if (diff == AMDR_ADC_DIFF_DIS) {
            bct = 25;
        } else {
            bct = 34;
        }
        break;
    default:break;
    }

    return bct;
}

/**
 * \brief 获取ADC的采样率
 */
static int __amdr_adc_rate_get (void       *p_drv,
                                int         chan,
                                uint32_t   *p_rate)
{
    uint8_t         adc_clkdiv;
    uint32_t        bus_clk;
    amdr_adc_dev_t *p_dev = NULL;
    uint32_t        fadck;

    if (NULL == p_drv || NULL == p_rate) {
        return -AM_EINVAL;
    }

    p_dev = (amdr_adc_dev_t *)p_drv;
    bus_clk = amdr_clk_bus_clkrate_get();

    adc_clkdiv = amhw_adc_clkdiv_get(p_dev->p_devinfo->p_hw_adc);
    adc_clkdiv = 0x1 << adc_clkdiv;

    /* 获取FADC时钟 */
    bus_clk /= adc_clkdiv;

    /* 获取位数影响的FADCK个数 */
    fadck  = __amdr_adc_rate_bct_get(p_dev->p_devinfo->bits,
                                     p_dev->p_devinfo->is_diff);
    /* 获取高速模式下影响的FADCK个数 */
    if (amhw_adc_cfg2_get(p_dev->p_devinfo->p_hw_adc) &
                          AMHW_ADC_CFG2_HSC_EN_CFG) {
            fadck += 2;
    }

    /* 获取长采样影响的FADCK个数 */
    if (amhw_adc_cfg1_get(p_dev->p_devinfo->p_hw_adc) &
                          AMHW_ADC_CFG1_LSMP_EN_CFG) {

        switch (amhw_adc_cfg2_get(p_dev->p_devinfo->p_hw_adc) &
                AMHW_ADC_CFG2_LSTS_MASK) {

        case AMHW_ADC_CFG2_LSTS_CFG(AMHW_ADC_LSTS_20ADCK):
            fadck += 20;
            break;

        case AMHW_ADC_CFG2_LSTS_CFG(AMHW_ADC_LSTS_12ADCK):
            fadck += 12;
            break;

        case AMHW_ADC_CFG2_LSTS_CFG(AMHW_ADC_LSTS_6ADCK):
            fadck += 6;
            break;

        case AMHW_ADC_CFG2_LSTS_CFG(AMHW_ADC_LSTS_2ADCK):
            fadck += 2;
            break;
        default:break;
        }
    }

    /* 硬件平均影响的FADCK个数 */
    if (p_dev->p_devinfo->p_adc_avg_info != NULL) {
        if (p_dev->p_devinfo->p_adc_avg_info->enable == AMDR_ADC_AVG_EN) {
            fadck <<= ((uint8_t)(p_dev->p_devinfo->p_adc_avg_info->avgs_num) + 2);
        }
    }

    /* 首次转换的影响 */
    fadck += 5;
    if (amhw_adc_cfg1_get(p_dev->p_devinfo->p_hw_adc) &
                          AMHW_ADC_CFG1_LSMP_EN_CFG) {
        fadck -= 2;
    }

    *p_rate = bus_clk / fadck; /* 得到采样率 */

    return AM_OK;
}

/**
 * \brief 求出两个数组的乘法组合跟指定值的最佳匹配时的数组索引
 *
 * \param[in] p_nums   : 指向不同模式时ADCK的数组，数组从小到大排列
 * \param[in] nums_len : ADCK的数组元素个数
 * \param[in] p_divs   : 指向不同分频值的数组，数组从小到大排列
 * \param[in] nums_len : 不同分频值的数组元素个数
 * \param[in] rate     : 指定匹配值（bus_clk/rate）
 * \param[in] bct      : 不同位数时的ADCK数
 * \param[in] avgs     : 硬件平均参数（参数=平均个数，0=1， 2=4， 3=8，4=16，5=32）
 * \param[in] base     : 第一次转换的基础ADCK个数（长采样是为5， 短采样为3）
 * \param[out] p_nums_n : 最佳的nums数组中的索引
 * \param[out] p_nums_n : 最佳的divs数组中的索引
 *
 * \return 最小误差（相差ADCK个数）
 */
uint32_t __find_minerr (const uint32_t *p_nums, uint8_t nums_len,
                        const uint32_t *p_divs, uint8_t divs_len,
                        uint32_t rate, uint8_t bct, uint8_t avgs, uint8_t base,
                        uint8_t *p_nums_n, uint8_t *p_divs_n)
{
    uint8_t first = 0 ;
    int     last  = divs_len - 1;
    int     yerr  = 0;

    uint32_t now;
    uint32_t minerr = 0xFFFFFFFF, err;


    *p_nums_n = first;
    *p_divs_n = last;

    for (first = 0; first < nums_len && last >= 0; ) {
        now = (base + ((p_nums[first] + bct) << avgs)) * p_divs[last];
        yerr = now - rate;
        if (yerr < 0) { /* 是否为负数 */
            yerr = ~yerr + 1;      /* 取补码 */
        }
        err = yerr;
        if (err < minerr || ((err == minerr) && (now < rate))) {
            minerr = err;
            *p_nums_n  = first;
            *p_divs_n  = last;
        }
        if (now > rate) {
            last--;
        } else if (now < rate){
            first++;
        } else {
            break;
        }
    }

    first = first >= nums_len ? nums_len - 1 : first;
    last  = last  < 0 ? 0 : last;
    *p_nums_n = first;
    *p_divs_n = last;

    return minerr;
}


/**
 * \brief 设置采样率
 * \note
 *       fadck = (BUS_CLK / BUS_DIV)  //得出模块时钟
 *       num   =  FSCAdder + AVGS * (BCT + LSTAdder + HSCAdder) //一个转换需要的模块时钟个数
 *       rate  = fadck / num                                    //采样率
 *       因封装较多层，且寻址较多，影响了采集处理的速度（速度要求高，建议使用DMA传输模式）
 *       采样率设置范围为20~140kHz（对所有位数，所有模式通用），DMA模式下最高达818kHz
 */
int __amdr_adc_rate_set(void     *p_drv,
                        int       chan,
                        uint32_t  rate)
{
    const uint32_t nums1[] = {2, 4, 6, 8, 12, 14, 20, 22}; /* 长采样*/
    const uint32_t nums2[] = {0, 2};                       /* 短采样 */
    const uint32_t divs[]  = {1, 2, 4, 8, 16};             /* 分频数组 */

    uint8_t         bct = 0,   avgs = 0, sel = 0;
    uint8_t         nums_n[2], divs_n[2];
    uint32_t        bus_clk;
    uint32_t        err1, err2;
    amdr_adc_dev_t *p_dev = NULL;

    if (NULL == p_drv) {
        return -AM_ENXIO;
    }
    p_dev = (amdr_adc_dev_t *)p_drv;

    /* 采样率支持判断 */
    if ((p_dev->p_devinfo->bits) == 16) {
        if (rate < 37037 || rate > 461467) {
            return -AM_EINVAL;
        }
    } else {
        if (rate < 20000 || rate > 818330) {
            return -AM_EINVAL;
        }
    }

    /* 获取BCT */
    bct = __amdr_adc_rate_bct_get(p_dev->p_devinfo->bits,
                                 p_dev->p_devinfo->is_diff);

    /* 硬件平均影响一次采样值的时间 */
    if (p_dev->p_devinfo->p_adc_avg_info != NULL) {
       if (p_dev->p_devinfo->p_adc_avg_info->enable == AMDR_ADC_AVG_EN) {
           avgs = (uint8_t)(p_dev->p_devinfo->p_adc_avg_info->avgs_num);
           if (avgs <= 3) {
               avgs += 2;
           }
       }
    }

    /* 使用短采样，低速模式模式 */
    bus_clk = amdr_clk_bus_clkrate_get();
    rate = bus_clk / rate; /* bus_clk/div / num = rate*/
    /* 短采样模式下，采样率误差 */
    err1 = __find_minerr(nums2, sizeof(nums2)/sizeof(nums2[0]),
                         divs, sizeof(divs)/sizeof(divs[0]),
                         rate, bct, avgs, 5,
                         &nums_n[0], &divs_n[0]);
    /* 长采样模式下，采样率误差 */
    err2 = __find_minerr(nums1, sizeof(nums1)/sizeof(nums1[0]),
                         divs, sizeof(divs)/sizeof(divs[0]),
                         rate, bct, avgs, 3,
                         &nums_n[1], &divs_n[1]);

    /* 根据不同的模式设置寄存器 */
    if (err1 > err2) {
        sel = 1;
        /* 使用长采样模式 */
        amhw_adc_lsmp_enable(p_dev->p_devinfo->p_hw_adc);
        /* 求出LSTS */
        if (nums1[nums_n[1]] <= 4) {
            amhw_adc_lsts_set(p_dev->p_devinfo->p_hw_adc,AMHW_ADC_LSTS_2ADCK);
        } else if (nums1[nums_n[1]] <= 8) {
            amhw_adc_lsts_set(p_dev->p_devinfo->p_hw_adc,AMHW_ADC_LSTS_6ADCK);
        } else if (nums1[nums_n[1]] <= 14) {
            amhw_adc_lsts_set(p_dev->p_devinfo->p_hw_adc,AMHW_ADC_LSTS_12ADCK);
        } else {
            amhw_adc_lsts_set(p_dev->p_devinfo->p_hw_adc,AMHW_ADC_LSTS_20ADCK);
        }
    } else {
        sel = 0;
        /* 使用短采样模式 */
        amhw_adc_lsmp_disable(p_dev->p_devinfo->p_hw_adc);
    }

    /* 设置是否使用高速采样 */
    if (nums_n[sel] % 2 == 0) {
        amhw_adc_hsc_disable(p_dev->p_devinfo->p_hw_adc);
    } else {
        amhw_adc_hsc_enable(p_dev->p_devinfo->p_hw_adc);
    }
    if (divs[divs_n[sel]] == 16) { /* 需要选择bus/2时钟源，分频值为8 */
        amhw_adc_clksrc_set(p_dev->p_devinfo->p_hw_adc, AMHW_ADC_CLK_SRC_ALTCLK2);
        amhw_adc_clkdiv_set(p_dev->p_devinfo->p_hw_adc, AMHW_ADC_CLK_DIV_8);
    } else {
        /* 选择总线时钟源 */
        amhw_adc_clksrc_set(p_dev->p_devinfo->p_hw_adc, AMHW_ADC_CLK_SRC_BUS);
        amhw_adc_clkdiv_set(p_dev->p_devinfo->p_hw_adc, (amhw_adc_clk_div_t)divs_n[sel]);
    }
    /* 使用最高频率，必须有如下使能，此时可能也会引入误差~*/
    bus_clk = bus_clk / divs[divs_n[sel]];
    if (bus_clk == 18000000UL || (bus_clk == 12000000UL &&
        p_dev->p_devinfo->bits == 16)) {
        amhw_adc_hsc_enable(p_dev->p_devinfo->p_hw_adc);
        amhw_adc_lowpower_disable(p_dev->p_devinfo->p_hw_adc);
    }

    return AM_OK;
}

/**
 * \brief 获取ADC转换精度
 */
static uint32_t __amdr_adc_bits_get(void *p_drv, int chan)
{
    amdr_adc_dev_t *p_dev = NULL;

    if (NULL == p_drv) {
        return 0xFFFFFFFF;
    }

    p_dev = (amdr_adc_dev_t *)p_drv;

    return p_dev->p_devinfo->bits;
}

/**
 * \brief 获取ADC转换精度
 */
static int __amdr_adc_bits_set(amdr_adc_dev_t *p_dev)
{
    uint32_t bits;

    if (NULL == p_dev) {
        return -AM_EINVAL;
    }

    bits = p_dev->p_devinfo->bits;
    if (p_dev->p_devinfo->is_diff == TRUE) {
        if (bits == 9 || bits == 11 || bits == 13 || bits == 16) {
            switch ( p_dev->p_devinfo->bits ) {
            case 9:
                amhw_adc_mode_set(p_dev->p_devinfo->p_hw_adc, AMHW_ADC_BITS_9);
                break;
            case 11:
                amhw_adc_mode_set(p_dev->p_devinfo->p_hw_adc, AMHW_ADC_BITS_11);
                break;
            case 13:
                amhw_adc_mode_set(p_dev->p_devinfo->p_hw_adc, AMHW_ADC_BITS_13);
                break;
            case 16:
                amhw_adc_mode_set(p_dev->p_devinfo->p_hw_adc, AMHW_ADC_BITS_16);
                break;
            default:break;
            }
        } else {
            return -AM_EINVAL;
        }
    } else {
        if (bits == 8 || bits == 10 || bits == 12 || bits == 16) {
            switch ( p_dev->p_devinfo->bits ) {
            case 8:
                amhw_adc_mode_set(p_dev->p_devinfo->p_hw_adc, AMHW_ADC_BITS_8);
                break;
            case 10:
                amhw_adc_mode_set(p_dev->p_devinfo->p_hw_adc, AMHW_ADC_BITS_10);
                break;
            case 12:
                amhw_adc_mode_set(p_dev->p_devinfo->p_hw_adc, AMHW_ADC_BITS_12);
                break;
            case 16:
                amhw_adc_mode_set(p_dev->p_devinfo->p_hw_adc, AMHW_ADC_BITS_16);
                break;
            default:break;
            }
        } else {
            return -AM_EINVAL;
        }
    }
    return AM_OK;
}
/**
 * \brief 获取ADC参考电压
 */
static uint32_t __amdr_adc_vref_get(void *p_drv, int chan)
{
    amdr_adc_dev_t *p_dev = NULL;

    if (NULL == p_drv) {
        return 0xFFFFFFFF;
    }

    p_dev = (amdr_adc_dev_t *)p_drv;

    return p_dev->p_devinfo->vref;
}




/**
 * \brief ADC初始化
 */
am_adc_handle_t amdr_adc_init (amdr_adc_dev_t           *p_dev,
                               const amdr_adc_devinfo_t *p_devinfo)
{
    uint32_t cfg = 0;
    uint16_t cv1, cv2;

    if (NULL == p_devinfo || NULL == p_dev ||
        p_devinfo->dma_chan > 3 || p_devinfo->dma_chan < -1) { /* 参数有效性检查 */
        return NULL;
    }

    p_dev->p_devinfo         = p_devinfo;
    p_dev->adc_serv.p_funcs  = &__g_adc_drvfuncs;
    p_dev->adc_serv.p_drv    = p_dev;

    p_dev->pfn_callback      = NULL;
    p_dev->p_desc            = NULL;
    p_dev->p_arg             = NULL;
    p_dev->desc_num          = 0;
    p_dev->flags             = 0;
    p_dev->count             = 0;
    p_dev->chan              = AMHW_ADC_CHAN_CLOSE; /* 默认关闭 */
    p_dev->desc_pos          = 0;


    /* 默认100k采样率, 配置模块时钟 */
    __amdr_adc_rate_set(p_dev, 0, 1000000); /* 采样率100K */

    /* 设置ADC分辨率 */
    if (AM_OK != __amdr_adc_bits_set(p_dev)) {
        return NULL;
    }
    /* 选择序列 */
    amhw_adc_seq_set(p_dev->p_devinfo->p_hw_adc, p_dev->p_devinfo->seq);

    if (p_dev->p_devinfo->mode_trg == AMDR_ADC_MODE_TRG_HW) {
        cfg |= AMHW_ADC_SC2_HWTRG_EN_CFG;
    }
    /* 比较功能配置 */
    if (p_dev->p_devinfo->p_adc_cmp_info != NULL) {
        if (p_dev->p_devinfo->p_adc_cmp_info->enable == AMDR_ADC_CMP_EN) {
            cfg |= AMHW_ADC_SC2_CMP_EN_CFG; /* 使能比较功能 */
            switch (p_dev->p_devinfo->p_adc_cmp_info->cmp_mode) {

            case AMDR_ADC_CMP_MODE_1:
                cv1 = p_dev->p_devinfo->p_adc_cmp_info->cmp_val1;
                break;

            case AMDR_ADC_CMP_MODE_2:
                cfg |= AMHW_ADC_SC2_CMPGT_EN_CFG;
                cv1 = p_dev->p_devinfo->p_adc_cmp_info->cmp_val1;
                break;

            case AMDR_ADC_CMP_MODE_3:
                cfg |= AMHW_ADC_SC2_CMPR_EN_CFG;
                cv1 = p_dev->p_devinfo->p_adc_cmp_info->cmp_val1;
                cv2 = p_dev->p_devinfo->p_adc_cmp_info->cmp_val2;
                if (cv1 > cv2) {
                    cv2 = cv1;
                    cv1 = p_dev->p_devinfo->p_adc_cmp_info->cmp_val2;
                }
                break;

            case AMDR_ADC_CMP_MODE_4:
                cfg |= AMHW_ADC_SC2_CMPR_EN_CFG;
                cv1 = p_dev->p_devinfo->p_adc_cmp_info->cmp_val1;
                cv2 = p_dev->p_devinfo->p_adc_cmp_info->cmp_val2;
                if (cv1 < cv2) {
                    cv2 = cv1;
                    cv1 = p_dev->p_devinfo->p_adc_cmp_info->cmp_val2;
                }
                break;

            case AMDR_ADC_CMP_MODE_5:
                cfg |= AMHW_ADC_SC2_CMPR_EN_CFG;
                cfg |= AMHW_ADC_SC2_CMPGT_EN_CFG;
                cv1 = p_dev->p_devinfo->p_adc_cmp_info->cmp_val1;
                cv2 = p_dev->p_devinfo->p_adc_cmp_info->cmp_val2;
                if (cv1 > cv2) {
                    cv2 = cv1;
                    cv1 = p_dev->p_devinfo->p_adc_cmp_info->cmp_val2;
                }
                break;

            case AMDR_ADC_CMP_MODE_6:
                cfg |= AMHW_ADC_SC2_CMPR_EN_CFG;
                cfg |= AMHW_ADC_SC2_CMPGT_EN_CFG;
                cv1 = p_dev->p_devinfo->p_adc_cmp_info->cmp_val1;
                cv2 = p_dev->p_devinfo->p_adc_cmp_info->cmp_val2;
                if (cv1 < cv2) {
                    cv2 = cv1;
                    cv1 = p_dev->p_devinfo->p_adc_cmp_info->cmp_val2;
                }
                break;
            default:break;
            }/* switch(mode) */
            amhw_adc_cv1_set(p_dev->p_devinfo->p_hw_adc, cv1);
            amhw_adc_cv2_set(p_dev->p_devinfo->p_hw_adc, cv2);
        }/* if enable */
    }/* if not CMP func NULL*/
    amhw_adc_sc2_cfg(p_dev->p_devinfo->p_hw_adc, cfg); /* 配置触发模式以及比较功能*/

    if (p_dev->p_devinfo->mode_trg == AMDR_ADC_MODE_TRG_HW) {
        cfg = 0;
    } else {
        cfg = AMHW_ADC_SC3_CONT_EN_CFG;  /* 使能连续平均功能 */
    }

    /* 硬件平均功能配置 */
    if (p_dev->p_devinfo->p_adc_avg_info != NULL) {
        if (p_dev->p_devinfo->p_adc_avg_info->enable == AMDR_ADC_AVG_EN) {
            cfg |= AMHW_ADC_SC3_AVG_EN_CFG; /* 使能硬件平均功能 */
            cfg |= AMHW_ADC_SC3_AVGS_CFG(p_dev->p_devinfo->p_adc_avg_info->avgs_num);
        }
    }
    amhw_adc_sc3_cfg(p_dev->p_devinfo->p_hw_adc, cfg); /* 配置硬件平均个数或者连续模式 */

    cfg = 0;
    cfg |= AMHW_ADC_SC1_CHAN_SEL_CFG(p_dev->chan);
    if (p_dev->p_devinfo->is_diff && p_dev->chan < 3) {
        cfg |= AMHW_ADC_SC1_DIFF_EN_CFG;
    }
    if (p_dev->p_devinfo->dma_chan == -1) { /* 非DMA模式下使能中断 */
        cfg |= AMHW_ADC_SC1_INT_EN_CFG;
        am_int_connect(p_dev->p_devinfo->inum, __adc_int, p_dev);
        am_int_enable(p_dev->p_devinfo->inum);
        cfg |= AMHW_ADC_SC1_INT_EN_CFG;
    }

    /* 配置单端、差分模式，以及通道 （初始化后，通道默认关闭）*/
    amhw_adc_sc1_cfg(p_dev->p_devinfo->p_hw_adc, p_dev->p_devinfo->seq, cfg);
    return (am_adc_handle_t)(&(p_dev->adc_serv));
}

/**
 * \brief ADC去初始化
 */
void amdr_adc_deinit (amdr_adc_dev_t *p_dev)
{
    if (NULL == p_dev) {
        return ;
    }
    __amdr_adc_stop(p_dev, p_dev->chan);
    am_int_disable(p_dev->p_devinfo->inum);
    am_int_disconnect(p_dev->p_devinfo->inum, __adc_int, p_dev);

}
/* end of file */
