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
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief AMKS16 I2C���ӻ�������������I2C�ӻ���׼�ӿ�
 *
 * \internal
 * \par Modification History
 * - 1.00 15-12-18  mem, first implementation.
 * \endinternal
 */

/*******************************************************************************
  includes
*******************************************************************************/
#include "am_i2c_slv.h"
#include "amdr_i2c_slv.h"
#include "amdr_clk.h"

/*******************************************************************************
  �궨��
*******************************************************************************/
#define __I2C_SLV_STATE_IDLE        1     /* ����״̬           */
#define __I2C_SLV_STATE_RECV        2     /* ��������״̬       */
#define __I2C_SLV_STATE_SEND        3     /* ��������״̬       */
#define __I2C_SLV_STATE_SENDING     4     /* ����������         */

#define __I2C_SLV_EVT_ADDR_MATCHED  1     /* ��ַƥ���¼�       */
#define __I2C_SLV_EVT_RXD           2     /* �����¼�           */
#define __I2C_SLV_EVT_TXD           3     /* �����¼�           */
#define __I2C_SLV_EVT_STOPPED       4     /* ���յ�ֹͣ�¼�     */

/*******************************************************************************
  ��������
*******************************************************************************/

/* I2C �������� */

/** \brief ע��һ��I2C�ӻ� */
static int __i2c_slv_slv_register(void *p_drv, am_i2c_slv_dev_t *p_dev);

/** \brief ȡ����ע���һ��I2C�ӻ�*/
static int __i2c_slv_slv_unregister(void *p_drv, am_i2c_slv_dev_t *p_dev);

/* I2C Ӳ����غ��� */
static int __i2c_slv_speed_set(amdr_i2c_slv_dev_t *p_this, uint32_t bus_speed);
static int __i2c_slv_send_data(amdr_i2c_slv_dev_t *p_this);
static int __i2c_slv_recv_data(amdr_i2c_slv_dev_t *p_this);

/* ��ʱ���ʹ����� */
static uint32_t __i2c_slv_timeout_cal (amdr_i2c_slv_dev_t *p_this, 
                                       uint32_t            timeout_ms);
static int      __i2c_slv_timeout_set (amdr_i2c_slv_dev_t *p_this,
                                       uint32_t            timeout_ms);
static void     __timeout_process (amdr_i2c_slv_dev_t *p_this);

/* ��ʼ��ֹͣ���� */
static void __start_process (amdr_i2c_slv_dev_t *p_this, bool_t is_restart);
static void __stop_process (amdr_i2c_slv_dev_t *p_this);

/* I2C �ж���غ��� */
static void __i2c_slv_irq_handler(void *p_arg);
static void __i2c_slv_sm_event (amdr_i2c_slv_dev_t *p_this, uint32_t i2c_event);

/*******************************************************************************
  ���������ṹ�嶨��
*******************************************************************************/

/**
 * \brief I2C ������������
 */
static am_const struct am_i2c_slv_drv_funcs __g_i2c_slv_drv_funcs = {
    __i2c_slv_slv_register,
    __i2c_slv_slv_unregister,
};

/**
 * \brief I2Cʱ�����ýṹ��
 */
typedef struct __i2c_div_table {
    uint8_t   icr;         /**< \brief I2C ��������        */
    uint16_t  scl_div;     /**< \brief I2C ��Ƶ����        */
} __i2c_div_table_t;

/*******************************************************************************
  ȫ�ֱ���
*******************************************************************************/

/* I2C ʱ�ӷ�Ƶ�����б�  */
const __i2c_div_table_t __i2c_div_table[50] = {
    { 0x00, 20 },   { 0x01, 22 },   { 0x02, 24 },   { 0x03, 26 },
    { 0x04, 28 },   { 0x05, 30 },   { 0x09, 32 },   { 0x06, 34 },
    { 0x0a, 36 },   { 0x07, 40 },   { 0x0c, 44 },   { 0x10, 48 },
    { 0x11, 56 },   { 0x12, 64 },   { 0x0f, 68 },   { 0x13, 72 },
    { 0x14, 80 },   { 0x15, 88 },   { 0x19, 96 },   { 0x16, 104 },
    { 0x1a, 112 },  { 0x17, 128 },  { 0x1c, 144 },  { 0x1d, 160 },
    { 0x1e, 192 },  { 0x22, 224 },  { 0x1f, 240 },  { 0x23, 256 },
    { 0x24, 288 },  { 0x25, 320 },  { 0x26, 384 },  { 0x2a, 448 },
    { 0x27, 480 },  { 0x2b, 512 },  { 0x2c, 576 },  { 0x2d, 640 },
    { 0x2e, 768 },  { 0x32, 896 },  { 0x2f, 960 },  { 0x33, 1024 },
    { 0x34, 1152 }, { 0x35, 1280 }, { 0x36, 1536 }, { 0x3a, 1792 },
    { 0x37, 1920 }, { 0x3b, 2048 }, { 0x3c, 2304 }, { 0x3d, 2560 },
    { 0x3e, 3072 }, { 0x3f, 3840 }
};

/*******************************************************************************
  �ڲ�ʹ�ú���
*******************************************************************************/
static void __i2c_slv_delay(void)
{
  volatile uint32_t j;

  for(j=0U; j< 0xC8UL; j++){}
}

/* I2C �����������ã�Ĭ������Ϊ10����������� */
static int __i2c_slv_speed_set (amdr_i2c_slv_dev_t *p_dev,
                                uint32_t            max_bus_speed)
{
    amhw_i2c_t *p_hw_i2c  = p_dev->p_devinfo->p_hw_i2c;
    
    uint8_t  mult , i , multiplier;
    uint32_t rate , abs_error;
    uint32_t min_error = 0xffffffff;
    uint32_t best_mult = 0;
    uint32_t best_icr;

    /* ֧�ֵ�ʱ��Ϊ400KHz */
    if (max_bus_speed > 400000) {
        return -AM_EINVAL;
    }
    
    /* �ҵ����ʺϵķ�Ƶֵ */
    for (mult = 0 ; mult <= 2 ; mult++) {
        multiplier = 1 << mult;
        for (i = 0 ; i < 50 ; i++) {
            rate = p_dev->clk_rate / (multiplier * __i2c_div_table[i].scl_div);
            abs_error = max_bus_speed > rate ? max_bus_speed - rate
                                             : rate - max_bus_speed;
            if (abs_error < min_error) {
                best_mult = mult;
                best_icr  =  __i2c_div_table[i].icr;
                min_error = abs_error;
            }
            if (min_error == 0) {
                break;
            }
        }
    }

    amhw_i2c_clk_div_set(p_hw_i2c , (best_mult << 6) | best_icr);
    p_dev->clk_rate = rate;

    return AM_OK;
}

/* ���㳬ʱʱ��ֵ */
static uint32_t __i2c_slv_timeout_cal (amdr_i2c_slv_dev_t *p_this, 
                                       uint32_t            timeout_ms)
{
    uint32_t ticks;
    uint8_t  div = 1;
    
    /* ������Ҫ��ʱ��ticks */
    ticks = p_this->clk_in / 1000 * timeout_ms;
    
    /* ���tickΪ65536 */
    if (ticks > 65535) {

        div = 64;
        ticks >>= 6;

        /* 64��Ƶ�󻹴���65535 */
        if (ticks > 65535) {
            ticks = 65535;
        }
    }
    
    /* ʵ�ʵĳ�ʱʱ��ֵ */
    timeout_ms = (ticks * 1000 * div) / p_this->clk_in;
    
    return timeout_ms;
}

static int __i2c_slv_timeout_set (amdr_i2c_slv_dev_t *p_this, 
                                  uint32_t            timeout_ms)
{
    uint32_t    value;
    amhw_i2c_t *p_hw_i2c = p_this->p_devinfo->p_hw_i2c;
    
    /* ����Ҫ��ʱ */
    if (timeout_ms == 0) {

        /* ���ó�ʱʱ��Ϊ0 */
        amhw_i2c_scl_timeout_val_set(p_hw_i2c, 0);

        /* ����SDA�͵�ƽ��ʱ�ж� */
        amhw_i2c_int_disable(p_hw_i2c, AMHW_I2C_INT_SHTF2IE);

    }  else {

        /* ������Ҫ��ʱ��ticks */
        value = p_this->clk_in / 1000 * timeout_ms;

        if (value < 65536) {
            amhw_i2c_timeout_clk_sel(p_hw_i2c, AMHW_I2C_MODULE_CLOCK_1);

        /* I2Cģ��ʱ��64��Ƶ */
        } else {
            value >>= 6; /* (ֵ����64) */
            amhw_i2c_timeout_clk_sel(p_hw_i2c, AMHW_I2C_MODULE_CLOCK_64);
        }

        amhw_i2c_scl_timeout_val_set(p_hw_i2c, value);
        amhw_i2c_int_enable(p_hw_i2c, AMHW_I2C_INT_SHTF2IE);
    }

    return AM_OK;
}

/** \brief I2C �������� */
static int __i2c_slv_send_data (amdr_i2c_slv_dev_t *p_this)
{
    amhw_i2c_t *p_hw_i2c    = p_this->p_devinfo->p_hw_i2c;

    if (p_this->data_ptr >= p_this->p_trans->length) {

        /* ���ûص����� */
        if (p_this->p_trans->pfn_complete_cb) {
            p_this->p_trans->pfn_complete_cb(p_this->p_trans->p_arg,
                                             AM_OK,
                                             p_this->p_trans->length);
        }

        p_this->p_trans  = NULL;
        p_this->data_ptr = 0;
    }

    if (p_this->p_trans == NULL) {
        am_i2c_slv_dev_t *p_slv_dev =
             (am_i2c_slv_dev_t *)p_this->p_devinfo->p_slv_list[p_this->cur_indx];

        /* ��ȡһ��trans */
        p_slv_dev->pfn_trans_get(p_slv_dev->p_arg_trans_get,
                                 AM_I2C_S_DIR_WRITE,
                                 &p_this->p_trans);
        if (p_this->p_trans == NULL || p_this->p_trans->length == 0) {
            amhw_i2c_data_write(p_hw_i2c, 0xFF);
            __i2c_slv_delay();
            p_this->p_trans = NULL;
            p_this->data_ptr = 0;
            return -AM_ENXIO;
        }

        /* ���trans,����ָ�� */
        p_this->data_ptr = 0;
    }

    amhw_i2c_data_write(p_hw_i2c, p_this->p_trans->p_buf[p_this->data_ptr++]);
    __i2c_slv_delay();

    return AM_OK;
}

/**
 * \brief I2C �ӻ�������������������
 */
static int __i2c_slv_recv_data (amdr_i2c_slv_dev_t *p_this)
{
    amhw_i2c_t            *p_hw_i2c    = p_this->p_devinfo->p_hw_i2c;
    am_i2c_slv_transfer_t *p_cur_trans = NULL;
    am_i2c_slv_dev_t      *p_slv_dev   = NULL;
 
    p_cur_trans = p_this->p_trans;
    p_slv_dev = (am_i2c_slv_dev_t *)p_this->p_devinfo->p_slv_list[p_this->cur_indx];

    if (p_cur_trans == NULL) {

        /* ��ȡһ��trans */
        p_slv_dev->pfn_trans_get(p_slv_dev->p_arg_trans_get,
                                 AM_I2C_S_DIR_READ,
                                 &p_this->p_trans);
        if (p_this->p_trans == NULL || p_this->p_trans->length == 0) {
            amhw_i2c_nak_respond(p_hw_i2c);
            __i2c_slv_delay();
            amhw_i2c_data_read(p_hw_i2c);
            __i2c_slv_delay();
            return -AM_ENXIO;
        } else {
            p_cur_trans      = p_this->p_trans;
            p_this->data_ptr = 0;
        }
    }

    /* ���һ������ */
    if (p_this->data_ptr >= (p_cur_trans->length -1)) {
        if (p_cur_trans->trans_flags & AM_I2C_S_TRANS_LAST_RD_NACK) {
            amhw_i2c_nak_respond(p_hw_i2c);
        } else {
            amhw_i2c_ack_respond(p_hw_i2c);
        }
    } else {
        if (p_cur_trans->trans_flags & AM_I2C_S_TRANS_RD_NACK) {
            amhw_i2c_nak_respond(p_hw_i2c);
        } else {
            amhw_i2c_ack_respond(p_hw_i2c);
        }
    }
    __i2c_slv_delay();

    p_cur_trans->p_buf[p_this->data_ptr++] = amhw_i2c_data_read(p_hw_i2c);

    if(p_this->data_ptr == p_cur_trans->length) {

        /* �˴�����ɣ����ûص����� */
        if (p_cur_trans->pfn_complete_cb) {
            p_cur_trans->pfn_complete_cb(p_cur_trans->p_arg,
                                         AM_OK,
                                         p_this->data_ptr);
        }
        p_this->p_trans  = NULL;
        p_this->data_ptr = 0;
    }

    __i2c_slv_delay();

    return AM_OK;
}

/*******************************************************************************
  I2C״̬����غ��������ģ�
*******************************************************************************/
volatile bool_t stop_occur = FALSE;
static int __i2c_slv_internal_reset (amdr_i2c_slv_dev_t *p_this)
{
    p_this->p_trans   = NULL;
    p_this->data_ptr  = 0;
    p_this->state     = __I2C_SLV_STATE_IDLE;
    stop_occur = TRUE;

    /* ��λ�ڲ��߼� */
    __i2c_slv_timeout_set(p_this, 0);

    return AM_OK;
}

uint8_t my_count = 0;
static void __start_process (amdr_i2c_slv_dev_t *p_this, bool_t is_restart)
{
    am_i2c_slv_dev_t *p_slv_dev = NULL;
    amhw_i2c_t       *p_hw_i2c = p_this->p_devinfo->p_hw_i2c;
    uint8_t           dir;
    bool_t            isack;

    p_slv_dev = (am_i2c_slv_dev_t *)p_this->p_devinfo->p_slv_list[p_this->cur_indx];
    
    if (p_slv_dev == NULL) {
        return;
    }

    /* ��ȡ���ݴ��䷽�� */
    dir = (amhw_i2c_stat_get(p_hw_i2c) & AMHW_I2C_STAT_SRW) ?
                                         AM_I2C_S_DIR_WRITE :
                                         AM_I2C_S_DIR_READ;


    __i2c_slv_timeout_set(p_this, p_slv_dev->timeout_ms);
    
    /* �ظ���ʼ�ź�,������һ�δ���  */
    if (is_restart && p_this->p_trans && p_this->p_trans->pfn_complete_cb) {
         p_this->p_trans->pfn_complete_cb(p_this->p_trans->p_arg,
                                          -AM_EIO,
                                          p_this->data_ptr);
    }

    /* Ĭ��Ϊ����Ӧ�� */
    isack = TRUE;

    /* ���õ�ַƥ�亯�� */
    if (p_slv_dev->pfn_matched) {
        p_slv_dev->pfn_matched(p_slv_dev->p_arg_matched, dir, &isack);
    }

    if (isack == TRUE) {
        amhw_i2c_ack_respond(p_hw_i2c);
    } else {
        amhw_i2c_nak_respond(p_hw_i2c);
    }

    if (dir == AM_I2C_S_DIR_WRITE) {
        amhw_i2c_transmode_set(p_hw_i2c, AMHW_I2C_TRANSMODE_SEND);
        __i2c_slv_delay();
    } else {

        __i2c_slv_delay();
        amhw_i2c_transmode_set(p_hw_i2c, AMHW_I2C_TRANSMODE_RECV);
        amhw_i2c_data_read(p_hw_i2c);
        __i2c_slv_delay();
    }

    p_this->p_trans  = NULL;
    p_this->data_ptr = 0;

    /* �ı�״̬ */
    if (dir == AM_I2C_S_DIR_READ) {
        p_this->state = __I2C_SLV_STATE_RECV;
    } else {
        p_this->state = __I2C_SLV_STATE_SEND;
    }
    my_count++;
    if (my_count> 1) {
        my_count = 0;
    }

}

/* ����ֹͣ�¼� */
static void __stop_process (amdr_i2c_slv_dev_t *p_this)
{

    /* ��ǰ��������δ������ɣ�ǿ�ƽ���  */
    if (p_this->p_trans &&
        p_this->p_trans->pfn_complete_cb) {
        p_this->p_trans->pfn_complete_cb(p_this->p_trans->p_arg,
                                         -AM_EIO,
                                         p_this->data_ptr);
    }
    __i2c_slv_internal_reset(p_this);
}

/* ��ʱ�����¼�����λ�ڲ�״̬���߼���Ӳ�� & ����� */
static void __timeout_process (amdr_i2c_slv_dev_t *p_this)
{
    /* ��ǰ��������δ������ɣ�ǿ�ƽ��� */
    if (p_this->p_trans &&
        p_this->p_trans->pfn_complete_cb) {
        p_this->p_trans->pfn_complete_cb(p_this->p_trans->p_arg,
                                         -AM_ETIME,
                                         p_this->data_ptr);
    }

    __i2c_slv_internal_reset(p_this);
}

/* ״̬������ */
static void __i2c_slv_sm_event (amdr_i2c_slv_dev_t *p_this, uint32_t i2c_event)
{   
    switch(p_this->state) {
        
    case __I2C_SLV_STATE_IDLE: /* ����״̬  */
        
        /* ��ַƥ���¼� */
        if (i2c_event == __I2C_SLV_EVT_ADDR_MATCHED) {
            __start_process(p_this, FALSE);

        /* �����¼�(��ʱ�ͷ����ߣ��������ڷ�������) */
        } else if (i2c_event == __I2C_SLV_EVT_RXD) {
            amhw_i2c_transmode_set(p_this->p_devinfo->p_hw_i2c, AMHW_I2C_TRANSMODE_RECV);
            amhw_i2c_nak_respond(p_this->p_devinfo->p_hw_i2c);
            __i2c_slv_delay();
            amhw_i2c_data_read(p_this->p_devinfo->p_hw_i2c);
            __i2c_slv_delay();

        /* �����¼�(��ʱ�ͷ����ߣ���������ʱ��) */
        } else if (i2c_event == __I2C_SLV_EVT_TXD) {
            amhw_i2c_transmode_set(p_this->p_devinfo->p_hw_i2c, AMHW_I2C_TRANSMODE_RECV);
            amhw_i2c_data_write(p_this->p_devinfo->p_hw_i2c, 0xFF);
            __i2c_slv_delay();
        }
        break;

    case __I2C_SLV_STATE_RECV: /* ���ݽ���״̬ */
        
        /* �����¼� */
        if (i2c_event == __I2C_SLV_EVT_RXD ) {
            __i2c_slv_recv_data(p_this);

        /* ֹͣ�¼� */
        } else if (i2c_event == __I2C_SLV_EVT_STOPPED) {
            __stop_process(p_this);

        /* ��ַƥ���¼����������� */
        } else if (i2c_event == __I2C_SLV_EVT_ADDR_MATCHED) {
            __start_process(p_this, TRUE);
        }
        break;

    case __I2C_SLV_STATE_SEND: /* ����׼������״̬������δ��ʼд������ */
        
        /* ���������¼� */
        if (i2c_event == __I2C_SLV_EVT_TXD) {

            p_this->state = __I2C_SLV_STATE_SENDING;
            __i2c_slv_send_data(p_this);

        /* ֹͣ�¼� */
        } else if (i2c_event == __I2C_SLV_EVT_STOPPED) {
            __stop_process(p_this);

        /* ��ַƥ���¼����������� */
        } else if (i2c_event == __I2C_SLV_EVT_ADDR_MATCHED) {
            __start_process(p_this, TRUE);
        }
        break;
        
    case __I2C_SLV_STATE_SENDING: /* �������ڷ���״̬ */
        
        /* ���������¼� */
        if (i2c_event == __I2C_SLV_EVT_TXD) {
            __i2c_slv_send_data(p_this);
            
        /* ֹͣ�¼� */
        } else if (i2c_event == __I2C_SLV_EVT_STOPPED) {
            if (p_this->p_trans != NULL) {
                p_this->data_ptr--;
            }
            __stop_process(p_this);

       /* ��ַƥ���¼����������� */
        } else if (i2c_event == __I2C_SLV_EVT_ADDR_MATCHED) {

            /* �ϴ�д��Ĵ��������ݲ�δ���ͳɹ�  */
            if (p_this->p_trans != NULL) {
                p_this->data_ptr--;
            }
            __start_process(p_this, TRUE);
        }
        break;

    default: 
        break;
    }
}

/** \brief I2C �жϴ����� */
static void __i2c_slv_irq_handler (void *p_arg)
{
    uint8_t i;
    uint8_t i2c_event;
    int     slot = -1;

    amdr_i2c_slv_dev_t *p_this    = (amdr_i2c_slv_dev_t *)p_arg;
    amhw_i2c_t         *p_hw_i2c  = p_this->p_devinfo->p_hw_i2c;

    bool_t  stop_occur = amhw_i2c_stop_is_dec(p_hw_i2c);
    uint8_t stat       = amhw_i2c_stat_get(p_hw_i2c);
    uint8_t tmout      = amhw_i2c_tmout_flag_get(p_hw_i2c);

    if ((tmout & (AMHW_I2C_TMOUT_FLAG_SCL_LOW | AMHW_I2C_TMOUT_FLAG_SDA_LOW))) {
        amhw_i2c_tmout_flag_clr(p_hw_i2c, tmout);
        amhw_i2c_stat_clr(p_hw_i2c, AMHW_I2C_STAT_ARBL | AMHW_I2C_STAT_IICIF);
        __timeout_process(p_this);
        return;
    }

    if (stop_occur) {
        amhw_i2c_stop_dec_clr(p_hw_i2c);
        amhw_i2c_stat_clr(p_hw_i2c, AMHW_I2C_STAT_ARBL | AMHW_I2C_STAT_IICIF);
        i2c_event = __I2C_SLV_EVT_STOPPED;
        __i2c_slv_sm_event(p_this, i2c_event);

        /* �鿴�Ƿ��Ѿ����ֵ�ַƥ�䣬��Ҫ���� */
        if ((stat & AMHW_I2C_STAT_IAAS) == 0) {
            return;
        }
    }

    amhw_i2c_stat_clr(p_hw_i2c, stat);

    /* ��ַƥ���ж� */
    if (stat & AMHW_I2C_STAT_IAAS) {

        uint8_t addr;

        am_i2c_slv_dev_t *p_slv_dev = NULL;
        i2c_event = __I2C_SLV_EVT_ADDR_MATCHED;

        addr = amhw_i2c_data_read(p_hw_i2c) >> 1;

        /* ����ƥ����豸 */
        for (i = 0; i < p_this->p_devinfo->dev_num; i++) {

            p_slv_dev = (am_i2c_slv_dev_t*)p_this->p_devinfo->p_slv_list[i];

            if((p_slv_dev != NULL) && (addr == p_slv_dev->slv_addr)) {
                slot = i;
                break;
            }
        }

        if (slot != -1) {
            p_this->cur_indx = slot;
            __i2c_slv_sm_event(p_this, i2c_event);

            /* ����ģʽ */
            if (stat & AMHW_I2C_STAT_SRW) {

                /* ����д��һ������ */
                i2c_event = __I2C_SLV_EVT_TXD;
                __i2c_slv_sm_event(p_this, i2c_event);
            }
        }
    } else {

        /* �����¼� */
        if (stat & AMHW_I2C_STAT_SRW) {

            /* �յ���Ӧ���ź� */
            if (stat & AMHW_I2C_STAT_RXAK) {

                /* ����ģʽ���ͷ����� */
                amhw_i2c_transmode_set(p_hw_i2c, AMHW_I2C_TRANSMODE_RECV);
                amhw_i2c_nak_respond(p_hw_i2c);
                amhw_i2c_data_read(p_hw_i2c);
            } else {
                i2c_event = __I2C_SLV_EVT_TXD;
                __i2c_slv_sm_event(p_this, i2c_event);
            }

        /* �����¼� */
        } else {
            i2c_event = __I2C_SLV_EVT_RXD;
            __i2c_slv_sm_event(p_this, i2c_event);
        }
    }
}

/*******************************************************************************
  ��׼������������ʵ��
*******************************************************************************/

/** \brief ע��һ��I2C�ӻ� */
static int __i2c_slv_slv_register (void *p_drv, am_i2c_slv_dev_t *p_dev)
{
    amdr_i2c_slv_dev_t *p_this    = (amdr_i2c_slv_dev_t *)p_drv;
    am_i2c_slv_dev_t   *p_slv_dev = NULL;
    amhw_i2c_t         *p_hw_i2c  = NULL;
    
    uint8_t i;
    
    if ((p_drv == NULL) || (p_dev == NULL) || (p_dev->pfn_trans_get == NULL)) {
        return -AM_EINVAL;
    }
    
    /* �ݲ�֧�� 10bit ��ַ */
    if (p_dev->addr_flags & AM_I2C_S_ADDR_10BIT) {
        return -AM_ENOTSUP;
    }

    for (i = 0; i < p_this->p_devinfo->dev_num; i++) {

        p_slv_dev = (am_i2c_slv_dev_t*)p_this->p_devinfo->p_slv_list[i];
        
        if (p_slv_dev != NULL) {

            /* �õ�ַ�Ѿ�ע�� */
            if (p_slv_dev == p_dev) {
                return -AM_EPERM;
            }

        /* �ҵ�������Դ */
        } else {
            break;
        }
    }
    
    if (i >= p_this->p_devinfo->dev_num) {
        return -AM_EFULL;
    }
    
    p_this->p_devinfo->p_slv_list[i] = (uint32_t)p_dev;
    p_hw_i2c                         = p_this->p_devinfo->p_hw_i2c;
    
    if (p_dev->timeout_ms != 0) {
        p_dev->timeout_ms = __i2c_slv_timeout_cal(p_this, p_dev->timeout_ms);
    }
    
    switch (i) {

    /* ʹ��addr0 */
    case 0:
        amhw_i2c_addr1_set(p_hw_i2c,
                           (uint8_t)(p_dev->slv_addr),
                           AMHW_I2C_ADDRMODE_BIT7);
        break;

   /* ʹ��addr2 */
    case 1:
        amhw_i2c_addr_range_set(p_hw_i2c, (uint8_t)(p_dev->slv_addr));
        break;

    /* ʹ��addra */
    case 2:
        amhw_i2c_addr2_set(p_hw_i2c, (uint8_t)(p_dev->slv_addr));
        amhw_i2c_addr2_match_enable(p_hw_i2c);
        break;
    default:
        break;
    }
    
    return AM_OK;
}

/** \brief ȡ����ע���һ��I2C�ӻ�    */
static int __i2c_slv_slv_unregister (void *p_drv, am_i2c_slv_dev_t *p_dev)
{
    amdr_i2c_slv_dev_t            *p_this     = (amdr_i2c_slv_dev_t *)p_drv;
    const amdr_i2c_slv_devinfo_t  *p_devinfo  = NULL;
    amhw_i2c_t                    *p_hw_i2c   = NULL;
    
    int i;
    
    if (p_drv == NULL || p_dev == NULL) {
        return -AM_EINVAL;
    }
    
    p_devinfo = p_this->p_devinfo;
    p_hw_i2c  = p_devinfo->p_hw_i2c;

    for (i = 0; i < p_devinfo->dev_num; i++) {
        
        if (p_this->p_devinfo->p_slv_list[i] == (uint32_t)p_dev) {
            break;
        }
    }
    
    /* δ�ҵ����豸���������豸δע�ᣬ����ȡ��δע����豸 */
    if (i >= p_this->p_devinfo->dev_num) {
        return -AM_EPERM;
    }

    switch (i) {

    /* ʹ��addr0 */
    case 0:
        amhw_i2c_addr1_set(p_hw_i2c, 0x00, AMHW_I2C_ADDRMODE_BIT7);
        break;

   /* ʹ��addr2 */
    case 1:
        amhw_i2c_addr_range_set(p_hw_i2c, 0x00);
        break;

    /* ʹ��addra */
    case 2:
        amhw_i2c_addr2_set(p_hw_i2c, 0x00);
        amhw_i2c_addr2_match_disable(p_hw_i2c);
        break;
    default:
        break;
    }

    p_this->p_devinfo->p_slv_list[i] = 0;

    return AM_OK;
}

/**
 * \brief I2C �ӻ���ʼ��
 */
am_i2c_slv_handle_t amdr_i2c_slv_init (amdr_i2c_slv_dev_t           *p_dev,
                                       const amdr_i2c_slv_devinfo_t *p_devinfo)
{
    int         i;
    amhw_i2c_t *p_hw_i2c = NULL;
    
    if (p_dev               == NULL ||
        p_devinfo           == NULL ||
        p_devinfo->p_hw_i2c == NULL ||
        p_devinfo->dev_num  >  3) { /* ��ʱֻ֧��3���豸 */
        return NULL;
    }

    p_hw_i2c = p_devinfo->p_hw_i2c;

    /* ��Ա��ʼ�� */
    p_dev->i2c_serv.p_funcs = (struct am_i2c_slv_drv_funcs *)&__g_i2c_slv_drv_funcs;
    p_dev->i2c_serv.p_drv   = p_dev; 
    p_dev->p_devinfo        = p_devinfo;
    p_dev->clk_in           = amdr_clk_periph_rate_get(p_hw_i2c);
    p_dev->p_trans          = NULL;
    p_dev->data_ptr         = 0;
    p_dev->cur_indx         = 0;
    p_dev->state            = __I2C_SLV_STATE_IDLE;
    
    /* �豸��ַ��ʼ�� */
    for (i = 0; i < p_devinfo->dev_num; i++) {
        p_dev->p_devinfo->p_slv_list[i] = 0;
    }
    
    /* Ӳ����ʼ�� */
    amhw_i2c_disable(p_hw_i2c);

    /* ���ò�����  */
    __i2c_slv_speed_set(p_dev, p_devinfo->max_bus_speed);

    /* �յ�һ�����ݺ�Ӧ�� */
    amhw_i2c_fast_ack_enable(p_hw_i2c);

    /* ��ʼ����ַ�Ĵ��� */
    amhw_i2c_addr1_set(p_hw_i2c, 0x00, AMHW_I2C_ADDRMODE_BIT7);
    amhw_i2c_addr_range_set(p_hw_i2c, 0x00);
    amhw_i2c_addr2_set(p_hw_i2c, 0x00);
    amhw_i2c_addr2_match_disable(p_hw_i2c);

    amhw_i2c_int_enable(p_hw_i2c, AMHW_I2C_INT_IICIE | AMHW_I2C_INT_STOPIE);

    am_int_connect(p_devinfo->inum, __i2c_slv_irq_handler, (void*)p_dev);
    am_int_enable(p_devinfo->inum);
    
    amhw_i2c_enable(p_hw_i2c);

    return &(p_dev->i2c_serv);
}

/**
 * \brief I2C �ӻ� ���ʼ��
 */
void amdr_i2c_slv_deinit (amdr_i2c_slv_dev_t *p_dev)
{
    amhw_i2c_t *p_hw_i2c    = p_dev->p_devinfo->p_hw_i2c;
    p_dev->i2c_serv.p_funcs = NULL;
    p_dev->i2c_serv.p_drv   = NULL;

    am_int_connect(p_dev->p_devinfo->inum, __i2c_slv_irq_handler, (void*)p_dev);
    am_int_disable(p_dev->p_devinfo->inum);
    amhw_i2c_disable(p_hw_i2c);
}

/* end of file */
