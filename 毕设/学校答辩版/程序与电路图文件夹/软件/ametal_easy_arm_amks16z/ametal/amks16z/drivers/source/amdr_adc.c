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
* ˽�ж���
*******************************************************************************/
#define __ADC_HW_DECL(p_hw_adc, p_drv)    \
        amhw_adc_t *p_hw_adc =               \
            ((amdr_adc_dev_t *)p_drv)->p_devinfo->p_hw_adc

#define __ADC_VREF_GET(p_drv)             \
        (((amdr_adc_dev_t *)p_drv)->p_devinfo->vref)


/*******************************************************************************
* ��������
*******************************************************************************/

/** \brief ָ��ADC�ж����Ӻ��� */
static int __amdr_adc_adc_connect (void *p_drv);

/** \brief ����ADCת��        */
static int __amdr_adc_adc_start  (void                   *p_drv,
                             int                     chan,
                             am_adc_buf_desc_t       *p_desc,
                             uint32_t                desc_num,
                             uint32_t                count,
                             uint32_t                flags,
                             pfn_adc_seq_complete_t  pfn_callback,
                             void                   *p_arg);

/** \brief ֹͣת��           */
static int __amdr_adc_stop (void *p_drv, int chan);

/** \brief ��ȡADC�Ĳ�����    */
static int __amdr_adc_rate_get (void       *p_drv,
                                int         chan,
                                uint32_t   *p_rate);

/** \brief ����ADC�Ĳ����ʣ�ʵ�ʲ����ʿ��ܴ��ڲ��� */
static int __amdr_adc_rate_set (void     *p_drv,
                                int       chan,
                                uint32_t  rate);

/** \brief ��ȡADCת������ */
static uint32_t __amdr_adc_bits_get(void *p_drv, int chan);

/** \brief ��ȡADC�ο���ѹ */
static uint32_t __amdr_adc_vref_get(void *p_drv, int chan);



/**
 * \brief ADC������
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
 * \brief DMA��������
 */
int __amdr_adc_dma_tran_cfg ( amdr_adc_dev_t *p_dev, am_adc_buf_desc_t *p_desc)
{

    uint32_t flags;
    amhw_dma_xfer_desc_t    dma_desc;

    /* DMA�������� */
    flags = AMHW_DMA_DCR_PER_REQUEST_ENABLE      |  /* ��������Դʹ��    */
            AMHW_DMA_DCR_SINGLE_TRANSFERS        |  /* ���δ���          */
            AMHW_DMA_DCR_AUTO_ALIGN_DISABLE      |  /* �Զ��������      */
            AMHW_DMA_DCR_SOURCE_SIZE_32_BIT      |  /* Դ��ַ4�ֽڶ�ȡ   */
            AMHW_DMA_DCR_DESTINATION_SIZE_32_BIT |  /* Ŀ�ĵ�ַ4�ֽ�д�� */
            AMHW_DMA_DCR_REQUEST_NO_AFFECTED     |  /* ������Ӱ��        */
            AMHW_DMA_DCR_NO_LINKING              |  /* ��ͨ������        */
            AMHW_DMA_DCR_INTERRUTP_ENABLE        |  /* DMA�ж�ʹ��       */
            AMHW_DMA_DCR_START_DISABLE    ;         /* DMA��ʼ����ʹ��   */

    /* ����DMA�жϷ����� */
    amdr_dma_isr_connect(p_dev->p_devinfo->dma_chan, __amdr_adc_dma_isr, p_dev);

    amdr_dma_chan_cfg(p_dev->p_devinfo->dma_chan,
                      AMHW_DMA_TRIGGER_DISABLE |     /**< \brief DMA����ģʽ */
                      DMA_REQUEST_MUX0_ADC0);        /**< \brief ����Դ      */

    /* ����ͨ�������� */
    amdr_dma_xfer_desc_build(&dma_desc,                      /* ͨ��������   */
                            (uint32_t)&(p_dev->p_devinfo->p_hw_adc->dat[p_dev->p_devinfo->seq]), /* Դ�����ݻ��� */
                            (uint32_t)(p_desc->p_buf),       /* Ŀ�����ݻ��� */
                            (uint32_t)((p_desc->length) << 2), /* �����ֽ���   */
                            flags);                    /* ��������     */

    /* ����DMA���䣬���Ͽ�ʼ���� */
    if (amdr_dma_chan_start(&dma_desc,
                            AMHW_DMA_PER_TO_MER,   /* ���赽 �ڴ�   */
                           (uint8_t)p_dev->p_devinfo->dma_chan) == AM_ERROR) {
        return AM_ERROR;
    }

    return AM_OK;
}
/**
 * \brief ADC DMA�����жϷ�����
 */
static void __amdr_adc_dma_isr (void *p_arg , uint8_t flag)
{

    amdr_adc_dev_t *p_dev = (amdr_adc_dev_t *)p_arg;

    if (flag == AMDR_DMA_INT_NORMAL) { /* ͨ������ */

        if (NULL != p_dev->p_desc[p_dev->desc_pos].pfn_complete) {
            p_dev->p_desc[p_dev->desc_pos].pfn_complete(p_dev->p_desc[p_dev->desc_pos].p_arg, AM_OK);
        }

        p_dev->desc_pos++;
        if (p_dev->desc_pos == p_dev->desc_num) { /* �Ѿ�������� */
            p_dev->desc_pos = 0;

            if (NULL != p_dev->pfn_callback) {
                p_dev->pfn_callback(p_dev->p_arg, AM_OK);
            }
            p_dev->count_pos++; /* ת�����б�׼ */
            if (p_dev->count != 0 && p_dev->count_pos == p_dev->count) {
                p_dev->count_pos = 0;
                __amdr_adc_stop(p_dev, p_dev->chan);  /* �ر�ģ�� */
                amhw_adc_dma_disable(p_dev->p_devinfo->p_hw_adc); /* �ر�DMA����*/
                amdr_dma_chan_stop(p_dev->p_devinfo->dma_chan);
                return ; /* ���� */
            }
        }
        /* ����������һ������ */
        amhw_adc_dma_disable(p_dev->p_devinfo->p_hw_adc); /* �ر�DMA����*/
        __amdr_adc_dma_tran_cfg(p_dev, &(p_dev->p_desc[p_dev->desc_pos]));
        amhw_adc_dma_enable(p_dev->p_devinfo->p_hw_adc); /* ʹ��DMA����*/
    } else { /* ͨ�Ŵ��� */

    }
}

/**
 * \brief ADC��ȡ����
 */
static void __adc_int(void *p_arg)
{
    amdr_adc_dev_t *p_dev = (amdr_adc_dev_t *)p_arg;
    am_adc_val_t   *p_buf = NULL;

    uint16_t dat;

    /* ��ȡ������ݻ�������ָ�� */
    p_buf = (am_adc_val_t *)p_dev->p_desc[p_dev->desc_pos].p_buf;
    if (p_buf == NULL) {
        return;
    }
    /* ��ȡ���� */
    dat   = amhw_adc_dat_get(p_dev->p_devinfo->p_hw_adc, p_dev->p_devinfo->seq);

    /* �����ʽ */
    if (AM_ADC_DATA_ALIGN_RIGHT != p_dev->flags) {
        p_buf[p_dev->index] = (dat & 0xFFFF) << 16;
    } else {
        p_buf[p_dev->index] = (dat & 0xFFFF);
    }
    (p_dev->index)++;
    /* ���һ���������������Ľ���*/
    if (p_dev->index >= p_dev->p_desc[p_dev->desc_pos].length) {
        p_dev->index = 0;
        if (NULL != p_dev->p_desc[p_dev->desc_pos].pfn_complete) {
            p_dev->p_desc[p_dev->desc_pos].pfn_complete(p_dev->p_desc[p_dev->desc_pos].p_arg, AM_OK);
        }

        p_dev->desc_pos++;
        if (p_dev->desc_pos == p_dev->desc_num) { /* �Ѿ�������� */
            p_dev->desc_pos = 0;

            if (NULL != p_dev->pfn_callback) {
                p_dev->pfn_callback(p_dev->p_arg, AM_OK);
            }
            p_dev->count_pos++; /* ת�����б�׼ */
            if (p_dev->count != 0 && p_dev->count_pos == p_dev->count) {
                p_dev->count_pos = 0;
                __amdr_adc_stop(p_dev, p_dev->chan);  /* �ر�ģ�� */
                return ; /* ���� */
            }
        }
    }
}


/** \brief ָ��ADC�ж����Ӻ��� */
static int __amdr_adc_adc_connect (void *p_drv)
{
    return -AM_ENOTSUP; /* �ú���δʵ�� ���ѷ����ú�����*/
}



/**
 * \brief ����ADCת��
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
    if (p_dev->p_devinfo->dma_chan == -1) { /* ��DMAģʽ��ʹ���ж� */
       cfg |= AMHW_ADC_SC1_INT_EN_CFG;
    } else {
       /* DMA�������� */
       __amdr_adc_dma_tran_cfg(p_dev, &p_desc[0]);
       amhw_adc_dma_enable(p_dev->p_devinfo->p_hw_adc);
    }

    cfg |= AMHW_ADC_SC1_CHAN_SEL_CFG(p_dev->chan);
    if (p_dev->p_devinfo->is_diff && p_dev->chan < 3) {
        cfg |= AMHW_ADC_SC1_DIFF_EN_CFG;
    }

    /* ���õ��ˡ����ģʽ���Լ�ͨ���� ����ת��*/
    amhw_adc_sc1_cfg(p_dev->p_devinfo->p_hw_adc, p_dev->p_devinfo->seq, cfg);

    return AM_OK;
}

/**
 * \brief ֹͣת��
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
 * \brief ��ȡBCT��λ����FADCK��Ҫ��
 */
uint8_t __amdr_adc_rate_bct_get(uint8_t bits, uint8_t diff)
{
    uint8_t bct = 0;

    /* λ��Ӱ���FADCKʱ�Ӹ��� */
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
 * \brief ��ȡADC�Ĳ�����
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

    /* ��ȡFADCʱ�� */
    bus_clk /= adc_clkdiv;

    /* ��ȡλ��Ӱ���FADCK���� */
    fadck  = __amdr_adc_rate_bct_get(p_dev->p_devinfo->bits,
                                     p_dev->p_devinfo->is_diff);
    /* ��ȡ����ģʽ��Ӱ���FADCK���� */
    if (amhw_adc_cfg2_get(p_dev->p_devinfo->p_hw_adc) &
                          AMHW_ADC_CFG2_HSC_EN_CFG) {
            fadck += 2;
    }

    /* ��ȡ������Ӱ���FADCK���� */
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

    /* Ӳ��ƽ��Ӱ���FADCK���� */
    if (p_dev->p_devinfo->p_adc_avg_info != NULL) {
        if (p_dev->p_devinfo->p_adc_avg_info->enable == AMDR_ADC_AVG_EN) {
            fadck <<= ((uint8_t)(p_dev->p_devinfo->p_adc_avg_info->avgs_num) + 2);
        }
    }

    /* �״�ת����Ӱ�� */
    fadck += 5;
    if (amhw_adc_cfg1_get(p_dev->p_devinfo->p_hw_adc) &
                          AMHW_ADC_CFG1_LSMP_EN_CFG) {
        fadck -= 2;
    }

    *p_rate = bus_clk / fadck; /* �õ������� */

    return AM_OK;
}

/**
 * \brief �����������ĳ˷���ϸ�ָ��ֵ�����ƥ��ʱ����������
 *
 * \param[in] p_nums   : ָ��ͬģʽʱADCK�����飬�����С��������
 * \param[in] nums_len : ADCK������Ԫ�ظ���
 * \param[in] p_divs   : ָ��ͬ��Ƶֵ�����飬�����С��������
 * \param[in] nums_len : ��ͬ��Ƶֵ������Ԫ�ظ���
 * \param[in] rate     : ָ��ƥ��ֵ��bus_clk/rate��
 * \param[in] bct      : ��ͬλ��ʱ��ADCK��
 * \param[in] avgs     : Ӳ��ƽ������������=ƽ��������0=1�� 2=4�� 3=8��4=16��5=32��
 * \param[in] base     : ��һ��ת���Ļ���ADCK��������������Ϊ5�� �̲���Ϊ3��
 * \param[out] p_nums_n : ��ѵ�nums�����е�����
 * \param[out] p_nums_n : ��ѵ�divs�����е�����
 *
 * \return ��С�����ADCK������
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
        if (yerr < 0) { /* �Ƿ�Ϊ���� */
            yerr = ~yerr + 1;      /* ȡ���� */
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
 * \brief ���ò�����
 * \note
 *       fadck = (BUS_CLK / BUS_DIV)  //�ó�ģ��ʱ��
 *       num   =  FSCAdder + AVGS * (BCT + LSTAdder + HSCAdder) //һ��ת����Ҫ��ģ��ʱ�Ӹ���
 *       rate  = fadck / num                                    //������
 *       ���װ�϶�㣬��Ѱַ�϶࣬Ӱ���˲ɼ�������ٶȣ��ٶ�Ҫ��ߣ�����ʹ��DMA����ģʽ��
 *       ���������÷�ΧΪ20~140kHz��������λ��������ģʽͨ�ã���DMAģʽ����ߴ�818kHz
 */
int __amdr_adc_rate_set(void     *p_drv,
                        int       chan,
                        uint32_t  rate)
{
    const uint32_t nums1[] = {2, 4, 6, 8, 12, 14, 20, 22}; /* ������*/
    const uint32_t nums2[] = {0, 2};                       /* �̲��� */
    const uint32_t divs[]  = {1, 2, 4, 8, 16};             /* ��Ƶ���� */

    uint8_t         bct = 0,   avgs = 0, sel = 0;
    uint8_t         nums_n[2], divs_n[2];
    uint32_t        bus_clk;
    uint32_t        err1, err2;
    amdr_adc_dev_t *p_dev = NULL;

    if (NULL == p_drv) {
        return -AM_ENXIO;
    }
    p_dev = (amdr_adc_dev_t *)p_drv;

    /* ������֧���ж� */
    if ((p_dev->p_devinfo->bits) == 16) {
        if (rate < 37037 || rate > 461467) {
            return -AM_EINVAL;
        }
    } else {
        if (rate < 20000 || rate > 818330) {
            return -AM_EINVAL;
        }
    }

    /* ��ȡBCT */
    bct = __amdr_adc_rate_bct_get(p_dev->p_devinfo->bits,
                                 p_dev->p_devinfo->is_diff);

    /* Ӳ��ƽ��Ӱ��һ�β���ֵ��ʱ�� */
    if (p_dev->p_devinfo->p_adc_avg_info != NULL) {
       if (p_dev->p_devinfo->p_adc_avg_info->enable == AMDR_ADC_AVG_EN) {
           avgs = (uint8_t)(p_dev->p_devinfo->p_adc_avg_info->avgs_num);
           if (avgs <= 3) {
               avgs += 2;
           }
       }
    }

    /* ʹ�ö̲���������ģʽģʽ */
    bus_clk = amdr_clk_bus_clkrate_get();
    rate = bus_clk / rate; /* bus_clk/div / num = rate*/
    /* �̲���ģʽ�£���������� */
    err1 = __find_minerr(nums2, sizeof(nums2)/sizeof(nums2[0]),
                         divs, sizeof(divs)/sizeof(divs[0]),
                         rate, bct, avgs, 5,
                         &nums_n[0], &divs_n[0]);
    /* ������ģʽ�£���������� */
    err2 = __find_minerr(nums1, sizeof(nums1)/sizeof(nums1[0]),
                         divs, sizeof(divs)/sizeof(divs[0]),
                         rate, bct, avgs, 3,
                         &nums_n[1], &divs_n[1]);

    /* ���ݲ�ͬ��ģʽ���üĴ��� */
    if (err1 > err2) {
        sel = 1;
        /* ʹ�ó�����ģʽ */
        amhw_adc_lsmp_enable(p_dev->p_devinfo->p_hw_adc);
        /* ���LSTS */
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
        /* ʹ�ö̲���ģʽ */
        amhw_adc_lsmp_disable(p_dev->p_devinfo->p_hw_adc);
    }

    /* �����Ƿ�ʹ�ø��ٲ��� */
    if (nums_n[sel] % 2 == 0) {
        amhw_adc_hsc_disable(p_dev->p_devinfo->p_hw_adc);
    } else {
        amhw_adc_hsc_enable(p_dev->p_devinfo->p_hw_adc);
    }
    if (divs[divs_n[sel]] == 16) { /* ��Ҫѡ��bus/2ʱ��Դ����ƵֵΪ8 */
        amhw_adc_clksrc_set(p_dev->p_devinfo->p_hw_adc, AMHW_ADC_CLK_SRC_ALTCLK2);
        amhw_adc_clkdiv_set(p_dev->p_devinfo->p_hw_adc, AMHW_ADC_CLK_DIV_8);
    } else {
        /* ѡ������ʱ��Դ */
        amhw_adc_clksrc_set(p_dev->p_devinfo->p_hw_adc, AMHW_ADC_CLK_SRC_BUS);
        amhw_adc_clkdiv_set(p_dev->p_devinfo->p_hw_adc, (amhw_adc_clk_div_t)divs_n[sel]);
    }
    /* ʹ�����Ƶ�ʣ�����������ʹ�ܣ���ʱ����Ҳ���������~*/
    bus_clk = bus_clk / divs[divs_n[sel]];
    if (bus_clk == 18000000UL || (bus_clk == 12000000UL &&
        p_dev->p_devinfo->bits == 16)) {
        amhw_adc_hsc_enable(p_dev->p_devinfo->p_hw_adc);
        amhw_adc_lowpower_disable(p_dev->p_devinfo->p_hw_adc);
    }

    return AM_OK;
}

/**
 * \brief ��ȡADCת������
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
 * \brief ��ȡADCת������
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
 * \brief ��ȡADC�ο���ѹ
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
 * \brief ADC��ʼ��
 */
am_adc_handle_t amdr_adc_init (amdr_adc_dev_t           *p_dev,
                               const amdr_adc_devinfo_t *p_devinfo)
{
    uint32_t cfg = 0;
    uint16_t cv1, cv2;

    if (NULL == p_devinfo || NULL == p_dev ||
        p_devinfo->dma_chan > 3 || p_devinfo->dma_chan < -1) { /* ������Ч�Լ�� */
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
    p_dev->chan              = AMHW_ADC_CHAN_CLOSE; /* Ĭ�Ϲر� */
    p_dev->desc_pos          = 0;


    /* Ĭ��100k������, ����ģ��ʱ�� */
    __amdr_adc_rate_set(p_dev, 0, 1000000); /* ������100K */

    /* ����ADC�ֱ��� */
    if (AM_OK != __amdr_adc_bits_set(p_dev)) {
        return NULL;
    }
    /* ѡ������ */
    amhw_adc_seq_set(p_dev->p_devinfo->p_hw_adc, p_dev->p_devinfo->seq);

    if (p_dev->p_devinfo->mode_trg == AMDR_ADC_MODE_TRG_HW) {
        cfg |= AMHW_ADC_SC2_HWTRG_EN_CFG;
    }
    /* �ȽϹ������� */
    if (p_dev->p_devinfo->p_adc_cmp_info != NULL) {
        if (p_dev->p_devinfo->p_adc_cmp_info->enable == AMDR_ADC_CMP_EN) {
            cfg |= AMHW_ADC_SC2_CMP_EN_CFG; /* ʹ�ܱȽϹ��� */
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
    amhw_adc_sc2_cfg(p_dev->p_devinfo->p_hw_adc, cfg); /* ���ô���ģʽ�Լ��ȽϹ���*/

    if (p_dev->p_devinfo->mode_trg == AMDR_ADC_MODE_TRG_HW) {
        cfg = 0;
    } else {
        cfg = AMHW_ADC_SC3_CONT_EN_CFG;  /* ʹ������ƽ������ */
    }

    /* Ӳ��ƽ���������� */
    if (p_dev->p_devinfo->p_adc_avg_info != NULL) {
        if (p_dev->p_devinfo->p_adc_avg_info->enable == AMDR_ADC_AVG_EN) {
            cfg |= AMHW_ADC_SC3_AVG_EN_CFG; /* ʹ��Ӳ��ƽ������ */
            cfg |= AMHW_ADC_SC3_AVGS_CFG(p_dev->p_devinfo->p_adc_avg_info->avgs_num);
        }
    }
    amhw_adc_sc3_cfg(p_dev->p_devinfo->p_hw_adc, cfg); /* ����Ӳ��ƽ��������������ģʽ */

    cfg = 0;
    cfg |= AMHW_ADC_SC1_CHAN_SEL_CFG(p_dev->chan);
    if (p_dev->p_devinfo->is_diff && p_dev->chan < 3) {
        cfg |= AMHW_ADC_SC1_DIFF_EN_CFG;
    }
    if (p_dev->p_devinfo->dma_chan == -1) { /* ��DMAģʽ��ʹ���ж� */
        cfg |= AMHW_ADC_SC1_INT_EN_CFG;
        am_int_connect(p_dev->p_devinfo->inum, __adc_int, p_dev);
        am_int_enable(p_dev->p_devinfo->inum);
        cfg |= AMHW_ADC_SC1_INT_EN_CFG;
    }

    /* ���õ��ˡ����ģʽ���Լ�ͨ�� ����ʼ����ͨ��Ĭ�Ϲرգ�*/
    amhw_adc_sc1_cfg(p_dev->p_devinfo->p_hw_adc, p_dev->p_devinfo->seq, cfg);
    return (am_adc_handle_t)(&(p_dev->adc_serv));
}

/**
 * \brief ADCȥ��ʼ��
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
