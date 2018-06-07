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
 * \brief SPI drivers implementation
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-10-20  liqing, first implementation.
 * \endinternal
 */


#include "amdr_spi.h"
#include "am_gpio.h"
#include "am_common.h"
#include "amdr_clk.h"

/*******************************************************************************
* �궨��                                                                       *
*******************************************************************************/

/* ��ȡSPI������Ƶ�� */
#define __SPI_FRQIIN_GET(p_hw_spi)    amdr_clk_periph_rate_get(p_hw_spi)

/* ��ȡSPI��֧�ֵ�����ٶ� */
#define __SPI_MAXSPEED_GET(p_hw_spi) (__SPI_FRQIIN_GET(p_hw_spi) / 2)

/* ��ȡSPI֧�ֵ���С�ٶ� */
#define __SPI_MINSPEED_GET(p_hw_spi) (__SPI_FRQIIN_GET(p_hw_spi) / 4096)


#define __SPI_CS_HIGH       1   /* CS�ߵ�ƽΪ��Ч״̬ */
#define __SPI_CS_LOW        0   /* CS�͵�ƽΪ��Ч״̬ */

#define __SPI_CS_ACTIVE     1   /* CSΪ��Ч״̬       */
#define __SPI_CS_INACTIVE   0   /* CSΪ��Ч״̬       */

#define __SPI_CS_ACTIVE_LEV_GET(f_dev) (((f_dev)->mode & AM_SPI_CS_HIGH) ?     \
                                                        __SPI_CS_HIGH :        \
                                                        __SPI_CS_LOW)
/* �����豸��CS����Ƭѡ״̬
 * f_dev       �� �豸
 * activelevel �� ��Ч״̬��ƽ
 * state       :   ״̬
 */
#define __SPI_DEV_CS_CFG(f_dev,activlevel,state)                               \
{                                                                              \
    if ((f_dev)->pfunc_cs == NULL) {                                           \
        am_gpio_pin_cfg((f_dev)->cs_pin, AM_GPIO_OUTPUT);                      \
        am_gpio_set((f_dev)->cs_pin, (state) ? (activlevel) : (!activlevel));  \
    } else {                                                                   \
        (f_dev)->pfunc_cs((f_dev), (state));                                   \
    }                                                                          \
}

/*******************************************************************************
* ״̬���¼�����                                                               *
*******************************************************************************/

/* SPI����״̬ */
#define __SPI_ST_IDLE               0       /* ����״̬            */
#define __SPI_ST_TRANS_START        1       /* ׼������״̬        */
#define __SPI_ST_M_SEND_DATA        2       /* ������������״̬    */
#define __SPI_ST_M_RECV_DATA        3       /* ������������״̬    */

/* �¼��л� */
#define __SPI_NEXT_STATE(s, e)      \
            do {                    \
                p_dev->state = (s); \
                new_event = (e);    \
            } while(0)


/* 32-bits, bit[15: 0] : �¼���
 *          bit[31:16] ���¼�����
 */
#define __SPI_EVT(evt_num, evt_par) (((evt_num) & 0xFFFF) | ((evt_par) << 16))
#define __SPI_EVT_NUM_GET(event)    ((event) & 0xFFFF)
#define __SPI_EVT_PAR_GET(event)    ((event >> 16) & 0xFFFF)

/* SPI �����¼� */
#define __SPI_EVT_NONE           __SPI_EVT(0, 0) /* ���¼�       */
#define __SPI_EVT_TRANS_LAUNCH   __SPI_EVT(1, 0) /* �����¼�     */
#define __SPI_EVT_M_SEND_DATA    __SPI_EVT(2, 0) /* ���������¼� */
#define __SPI_EVT_M_RECV_DATA    __SPI_EVT(3, 0) /* ���������¼� */

/*******************************************************************************
* Functions declaration                                                        *
*******************************************************************************/

/* SPI������������ */

static int __spi_info_get (void *p_drv, am_spi_info_t *p_info);
static int __spi_spi_setup (void *p_drv, struct am_spi_device *p_f_dev);
static int __spi_connect (void *p_drv);
static int __spi_transfer (void                   *p_drv,
                           struct am_spi_device   *p_f_dev,
                           struct am_spi_transfer *p_trans);

/* �ڲ��������� */

static void __spi_irq_handler (void *p_arg);
static void __spi_trans_complete (amdr_spi_dev_t *p_dev, int state);
static int __spi_event_trans (amdr_spi_dev_t *p_dev, uint32_t event);
static void __spi_data_read (amdr_spi_dev_t *p_dev);
static void __spi_data_write (amdr_spi_dev_t *p_dev);

static void __spi_dev_config (amdr_spi_dev_t *p_dev, am_spi_device_t *p_f_dev);
static void __spi_cs_cfg (amdr_spi_dev_t *p_dev, am_spi_device_t *p_f_dev);
static uint32_t __spi_speed_cfg (amdr_spi_dev_t *p_dev, uint32_t target_baud);


/*******************************************************************************
* ȫ�ֱ�������                                                                 *
*******************************************************************************/

/**
 * \brief SPI���������ṹ��
 */
static am_const struct am_spi_drv_funcs __g_spi_drv_funcs = {
    __spi_info_get,
    __spi_spi_setup,
    __spi_transfer,
    __spi_connect
};

/*******************************************************************************
* ����ʵ��                                                                     *
*******************************************************************************/

/**
 * \brief ��ȡSPI֧����Ϣ
 */
int __spi_info_get (void *p_drv, am_spi_info_t *p_info)
{
    amdr_spi_dev_t   *p_dev     = (amdr_spi_dev_t *)p_drv;
    amhw_spi_t       *p_hw_spi  = p_dev->p_devinfo->p_hw_spi;

    p_info->max_speed = __SPI_MAXSPEED_GET(p_hw_spi);
    p_info->min_speed = __SPI_MINSPEED_GET(p_hw_spi);
    p_info->features  = AM_SPI_CPHA      | /* ʱ����λ������     */
                        AM_SPI_CPOL      | /* ʱ�Ӽ��Կ�����     */
                        AM_SPI_CS_HIGH   | /* Ƭѡ������Ч       */
                        AM_SPI_LSB_FIRST | /* ֧�ֵ�λ���ȷ���   */
                        AM_SPI_NO_CS;      /* ֧�ֲ�ʹ��CSƬѡ   */

    return AM_OK;
}

/**
 * \brief �����豸�������豸�Ƿ�֧��
 */
int __spi_spi_setup (void *p_drv, struct am_spi_device *p_f_dev)
{
    amdr_spi_dev_t   *p_dev     = (amdr_spi_dev_t *)p_drv;
    amhw_spi_t       *p_hw_spi  = p_dev->p_devinfo->p_hw_spi;

    if(p_hw_spi == NULL) {
        return -AM_EINVAL;
    }

    /* ��֧�����¹��� */
    if (p_f_dev->bits_per_word > 16                                  ||
       (p_f_dev->mode & (AM_SPI_LOOP | AM_SPI_READY | AM_SPI_3WIRE)) ||
        p_f_dev->max_speed_hz > __SPI_MAXSPEED_GET(p_hw_spi)) {
        return -AM_ENOTSUP;
    }

    __spi_dev_config(p_dev, p_f_dev); /* ���������õ���ǰ�豸 */

    return AM_OK;
}

/**
 * \brief SPI��������
 */
int __spi_transfer (void              *p_drv,
                    am_spi_device_t   *p_f_dev,
                    am_spi_transfer_t *p_trans)
{
    amdr_spi_dev_t *p_dev    = (amdr_spi_dev_t *)p_drv;
    amhw_spi_t     *p_hw_spi = p_dev->p_devinfo->p_hw_spi;

    if(p_hw_spi == NULL ||
       p_f_dev  ==NULL  ||
       p_trans  == NULL ||
      (p_trans->p_txbuf == NULL &&  p_trans->p_rxbuf == NULL)) {

        return -AM_EINVAL;
    }

    /* ���ڽ��д��� */
    if (p_dev->state != __SPI_ST_IDLE) {
        return -AM_EBUSY;
    }

    /* ��ǰ�豸��Ŀ���豸��ͬ�������������� */
    if ( p_f_dev != p_dev->p_f_dev) {
        __spi_dev_config(p_dev, p_f_dev);
    }

    /* p_trans���ݿ�ȸ����豸���ݿ�� */
    if (p_trans->bits_per_word != 0) {
        uint8_t bits_per_word = p_trans->bits_per_word;

        amhw_spi_feature_cfg(p_hw_spi,
                            (bits_per_word > 9) ? AMHW_SPI_CFG_16BIT_WIDTH :
                                                  AMHW_SPI_CFG_8BIT_WIDTH);
        p_dev->cur_bpw = bits_per_word;
    }

    /* p_trans�ٶȸ����豸�ٶ� */
    if (p_trans->speed_hz != 0) {
        uint32_t speed = p_trans->speed_hz;

        if ((speed > (p_dev->cur_speed + p_dev->speed_diff)) ||
            (speed < p_dev->cur_speed)) {
            __spi_speed_cfg(p_dev, speed);
            /* �ٶȺ��ٶ������Ѿ����µ��豸�� */
        }
    }

    /* ����Ƭѡ��Ч���ǵ��豸���ߣ� */
    if ((AM_SPI_NO_CS & p_f_dev->mode) == 0) {
        __spi_cs_cfg(p_dev, p_f_dev);
    }

    p_dev->p_trans  = p_trans;
    p_dev->send_ptr = 0;
    p_dev->recv_ptr = 0;

    __spi_event_trans(p_dev, __SPI_EVT_TRANS_LAUNCH);

    return AM_OK;
}

/**
 * \brief SPI�жϿ�������
 */
int __spi_connect (void *p_drv)
{
    amdr_spi_dev_t *p_dev = (amdr_spi_dev_t *)p_drv;

    am_int_connect(p_dev->p_devinfo->inum, __spi_irq_handler, (void *)p_drv);
    am_int_enable(p_dev->p_devinfo->inum);
    
    return AM_OK;
}

/**
 * \brief SPI ��ʼ����������ʼ��������ʹ��SPI,���ñ�׼��û�ж�������ԣ�
 */
am_spi_handle_t amdr_spi_init (amdr_spi_dev_t           *p_dev,
                               const amdr_spi_devinfo_t *p_devinfo)
{
    amhw_spi_t *p_hw_spi = p_devinfo->p_hw_spi;

    p_dev->spi_serve.p_funcs = (struct am_spi_drv_funcs *)&__g_spi_drv_funcs;
    p_dev->spi_serve.p_drv   = p_dev;
    p_dev->p_devinfo         = p_devinfo;

    p_dev->p_trans  = NULL;
    p_dev->send_ptr = 0;
    p_dev->recv_ptr = 0;

    p_dev->p_f_dev  = NULL;

    if (0 == __spi_speed_cfg(p_dev, p_devinfo->ini_speed)) {
        return NULL;
    }

    /* ����SPIģʽ��ʱ����λ�ͼ��ԣ� */
    amhw_spi_mode_cfg(p_hw_spi, p_devinfo->mode_flag << 2);

    /* ���������㶨������� */
    amhw_spi_feature_cfg(p_hw_spi, p_devinfo->flags      |
                                   AMHW_SPI_CFG_TDMA_DIS | /* ����DMA���� */
                                   AMHW_SPI_CFG_RDMA_DIS); /* ����DMA���� */

    /* ��ʹ���Դ�CSƬѡ */
    amhw_spi_cs_cfg(p_hw_spi, AMHW_SPI_CS_GPIO);

    /* ����Ϊ����ģʽ */
    amhw_spi_workmode_cfg(p_hw_spi, AMHW_SPI_WMODE_MASTER);

    amhw_spi_enable(p_hw_spi);

    p_dev->cs_state = FALSE;
    p_dev->state    = __SPI_ST_IDLE;

    return &(p_dev->spi_serve);
}

/**
 * \brief SPI ȥ��ʼ������
 */
void amdr_spi_deinit (amdr_spi_dev_t *p_dev)
{
    amhw_spi_t *p_hw_spi = p_dev->p_devinfo->p_hw_spi;

    p_dev->spi_serve.p_funcs = NULL;
    p_dev->spi_serve.p_drv   = NULL;
    p_dev->p_trans           = NULL;
    p_dev->p_f_dev           = NULL;

    amhw_spi_disable(p_hw_spi);
    am_int_disable(p_dev->p_devinfo->inum);
    am_int_disconnect(p_dev->p_devinfo->inum, __spi_irq_handler, (void *)p_dev);
}

/**
 * \brief SPI�жϻص�����
 * \param [in] p_arg  : �ص�����
 * \return ��
 */
void __spi_irq_handler (void *p_arg)
{
    amdr_spi_dev_t   *p_dev    = (amdr_spi_dev_t *)p_arg;
    amhw_spi_t       *p_hw_spi = p_dev->p_devinfo->p_hw_spi;

    uint8_t sta_flag = amhw_spi_stat_get(p_hw_spi);

    /* �������� */
    if((sta_flag & AMHW_SPI_STAT_R_FULL) != 0) {
        __spi_event_trans(p_dev, __SPI_EVT_M_RECV_DATA);
    }

    /* �������� */
    if((sta_flag & AMHW_SPI_STAT_T_EMPTY) && (p_hw_spi->c1 & AMHW_SPI_IRQ_T)) {
        __spi_event_trans(p_dev, __SPI_EVT_M_SEND_DATA);
    }
}

/**
 * \brief ���ݴ����¼�����
 *        ��������״̬���¼�����첽��������
 * \param [in] p_dev  : ָ��SPI�豸��ָ��
 * \param [in] event  : �����¼�
 * \return ��
 */
int __spi_event_trans (amdr_spi_dev_t *p_dev, uint32_t event)
{
    amhw_spi_t  *p_hw_spi = p_dev->p_devinfo->p_hw_spi;

    volatile uint32_t new_event = __SPI_EVT_NONE;

    while (1) {

        if (new_event != __SPI_EVT_NONE) {
            event      = new_event;
            new_event  = __SPI_EVT_NONE;
        }

        switch (p_dev->state) {

        case __SPI_ST_IDLE:          /* ����״̬ */
        {
            if (event != __SPI_EVT_TRANS_LAUNCH) {
                return -AM_EINVAL;
            }
            /* �л�����ʼ״̬������break */
        }

        case __SPI_ST_TRANS_START:   /* ��ʼ״̬ */
        {

            if (event != __SPI_EVT_TRANS_LAUNCH) {
                return -AM_EINVAL;
            }

            p_dev->send_ptr = 0;
            p_dev->recv_ptr = 0;

            __SPI_NEXT_STATE(__SPI_ST_M_SEND_DATA, __SPI_EVT_M_SEND_DATA);

            break;
        }

        case __SPI_ST_M_SEND_DATA:  /* ����״̬ */
        {
            if (event != __SPI_EVT_M_SEND_DATA) {
                return -AM_EINVAL;
            }

            /* û�и����������Ҫ���� */
            if (p_dev->send_ptr >= p_dev->p_trans->nbytes) {

                /* �ر������ж� */
                amhw_spi_int_disable(p_hw_spi, AMHW_SPI_IRQ_T  |
                                               AMHW_SPI_IRQ_R_MODDEF);

                /* ���ݽ�����ɴ��� */
                __spi_trans_complete(p_dev, AM_OK);

                __SPI_NEXT_STATE(__SPI_ST_IDLE, __SPI_EVT_NONE);

            /* ��Ҫ�������� */
            } else {

                if ((amhw_spi_stat_get(p_hw_spi) & AMHW_SPI_STAT_T_EMPTY) != 0) {

                    amhw_spi_int_disable(p_hw_spi, AMHW_SPI_IRQ_T);

                    __spi_data_write(p_dev);

                    /* �ȴ����� */
                    __SPI_NEXT_STATE(__SPI_ST_M_RECV_DATA, __SPI_EVT_NONE);

                    amhw_spi_int_enable(p_hw_spi, AMHW_SPI_IRQ_R_MODDEF);

                /* ʹ�ܷ����ж� */
                } else {
                    amhw_spi_int_enable(p_hw_spi, AMHW_SPI_IRQ_T);
                }
            }
            break;
        }

        case __SPI_ST_M_RECV_DATA:     /* ����״̬ */
        {

            if (event != __SPI_EVT_M_RECV_DATA) {
                return -AM_EINVAL;
            }

            __spi_data_read(p_dev);

            /* ���յ������ݵ��ڷ��͵����� */
            if (p_dev->send_ptr == p_dev->recv_ptr) {

                __SPI_NEXT_STATE(__SPI_ST_M_SEND_DATA, __SPI_EVT_M_SEND_DATA);

                amhw_spi_int_disable(p_hw_spi, AMHW_SPI_IRQ_R_MODDEF |
                                               AMHW_SPI_IRQ_T);

            } else {
                /* ��Ӧ��ִ�е��� */
            }

            break;
        }

        default:
            break;
        } /* switch */

        /* û���¼����˳��¼�ѭ�� */
        if (new_event == __SPI_EVT_NONE) {
            break;
        }
    } /* while */

    return AM_OK;
}

/**
 * \brief SPI��������
 * \param[in] p_dev  : ָ��SPI�豸��ָ��
 * \return ��
 */
void __spi_data_write (amdr_spi_dev_t *p_dev)
{
    amhw_spi_t  *p_hw_spi = p_dev->p_devinfo->p_hw_spi;

    if (p_dev->p_trans->bits_per_word > 9) {
        if (p_dev->p_trans->p_txbuf == NULL) {
            amhw_spi_data16_wirte(p_hw_spi, 0x0000);
        } else {
            amhw_spi_data16_wirte(
                  p_hw_spi,
               *((uint16_t*)((uint32_t)p_dev->p_trans->p_txbuf +p_dev->send_ptr)));
        }

        p_dev->send_ptr += 2;

    } else {
        if (p_dev->p_trans->p_txbuf == NULL) {
            amhw_spi_data8_wirte(p_hw_spi, 0x00);
        } else {
            amhw_spi_data8_wirte(
                  p_hw_spi,
               *((uint8_t*)((uint32_t)p_dev->p_trans->p_txbuf +p_dev->send_ptr)));
        }

        p_dev->send_ptr += 1;
    }
}

/**
 * \brief SPI��������
 * \param [in] p_dev  : ָ��SPI�豸��ָ��
 * \return ��
 */
void __spi_data_read (amdr_spi_dev_t *p_dev)
{
    amhw_spi_t  *p_hw_spi = p_dev->p_devinfo->p_hw_spi;

    if (p_dev->p_trans->bits_per_word > 9) {
        if (p_dev->p_trans->p_rxbuf == NULL) {
            amhw_spi_data16_read(p_hw_spi);
        } else {
            *(uint16_t*)((uint32_t)p_dev->p_trans->p_rxbuf + p_dev->recv_ptr) =
                         amhw_spi_data16_read(p_hw_spi);
        }

        p_dev->recv_ptr += 2;

    } else {
        if (p_dev->p_trans->p_rxbuf == NULL) {
            amhw_spi_data8_read(p_hw_spi);
        } else {
            *((uint8_t*)((uint32_t)p_dev->p_trans->p_rxbuf + p_dev->recv_ptr)) =
                         amhw_spi_data8_read(p_hw_spi);
        }

        p_dev->recv_ptr += 1;
    }
}

/**
 * \brief ������ɴ�����
 *
 * \param [in] p_dev  : ָ��SPI�豸��ָ��
 * \param [in] state ��������
 *
 * \return ��
 */
void __spi_trans_complete (amdr_spi_dev_t *p_dev, int state)
{
    int active_level  = __SPI_CS_ACTIVE_LEV_GET(p_dev->p_f_dev);

    bool_t trans_end = (p_dev->p_trans->flags & AM_SPI_TRANS_EOT) ? TRUE : FALSE;
    bool_t cs_change = (p_dev->p_trans->cs_change) ? TRUE : FALSE;

    //�ı�Ƭѡ״̬�ӳ٣�

    /* ��������Ƭѡ��Ϊ��Ч */
    if(state != AM_OK) {
        __SPI_DEV_CS_CFG(p_dev->p_f_dev, active_level, __SPI_CS_INACTIVE);
        p_dev->cs_state = FALSE;

    } else {
        if((!cs_change && trans_end) || (cs_change && !trans_end)) {
            __SPI_DEV_CS_CFG(p_dev->p_f_dev, active_level, __SPI_CS_INACTIVE);
            p_dev->cs_state = FALSE;
        }
    }

    /* �ص� */
    if(p_dev->p_trans->pfn_callback != NULL) {
        p_dev->p_trans->pfn_callback(p_dev->p_trans->p_arg, state);
    }
}

/**
 * \brief ����SPI�豸����
 *
 * \param [in] p_dev   : ָ��SPI�豸��ָ��
 * \param [in] p_f_dev ��ָ��SPI�����豸������ָ��
 *
 * \return ��
 */
void  __spi_dev_config (amdr_spi_dev_t    *p_dev,
                        am_spi_device_t   *p_f_dev)
{
    amhw_spi_t *p_hw_spi = p_dev->p_devinfo->p_hw_spi;

    uint32_t speed         = p_f_dev->max_speed_hz;
    uint8_t  bits_per_word = p_f_dev->bits_per_word;

    /* �ٶȲ���������Χ�ڲ���Ҫ�������� */
    if ((speed > (p_dev->cur_speed + p_dev->speed_diff)) ||
        (speed < p_dev->cur_speed)) {

        __spi_speed_cfg(p_dev, p_f_dev->max_speed_hz);
    }

    /* �������ݿ�ȶ� */
    if (bits_per_word != p_dev->cur_bpw) {
        amhw_spi_feature_cfg(p_hw_spi,
                            (bits_per_word >9) ? AMHW_SPI_CFG_16BIT_WIDTH :
                                                 AMHW_SPI_CFG_8BIT_WIDTH);
        p_dev->cur_bpw = bits_per_word;
    }

    /* �������� */
    if (p_f_dev->mode != p_dev->p_f_dev->mode) {

        if (p_f_dev->mode & AM_SPI_LSB_FIRST) {
            amhw_spi_feature_cfg(p_hw_spi, AMHW_SPI_CFG_LSB);
        } else {
            amhw_spi_feature_cfg(p_hw_spi, AMHW_SPI_CFG_MSB);
        }

        /* ����ʱ����λ�ͼ��� */
        amhw_spi_mode_cfg(p_hw_spi, p_f_dev->mode << 2);
    }

    p_dev->p_f_dev = p_f_dev;
}

/**
 * \brief Ƭѡ����(ʹ�豸Ƭѡ��Ч)
 *
 * \param [in] p_dev   : ָ��SPI�豸��ָ��
 * \param [in] p_f_dev ��ָ��SPI�����豸������ָ��
 *
 * \return ��
 */
void __spi_cs_cfg (amdr_spi_dev_t *p_dev, am_spi_device_t *p_f_dev)
{
        int active_level_o;  /* ���豸��Ч״̬   */
        int active_level_n;  /* ���豸����Ч״̬ */

        active_level_o = __SPI_CS_ACTIVE_LEV_GET(p_dev->p_f_dev);
        active_level_n = __SPI_CS_ACTIVE_LEV_GET(p_f_dev);

        /* ���豸Ƭѡ��Ч���������豸Ƭѡ��Ч */
        if (p_dev->cs_state == FALSE) {
            __SPI_DEV_CS_CFG(p_f_dev, active_level_n, __SPI_CS_ACTIVE);

        /*  ���豸Ƭѡ��Ч����Ҫ�Ƚ��Ƿ���ͬһ���豸
         * - ��ͬ�豸��ʹ���豸Ƭѡ��Ч�����豸��Ч��
         * - ͬһ�豸������������
         */
        } else {

            /* �豸��ͬ */
            if (p_dev->p_f_dev != p_f_dev) {
                __SPI_DEV_CS_CFG(p_dev->p_f_dev, active_level_o, __SPI_CS_INACTIVE);
                __SPI_DEV_CS_CFG(p_f_dev, active_level_n, __SPI_CS_ACTIVE);
            }

        }

        p_dev->cs_state = TRUE;
}


/**
 * \brief SPI�����ٶ�����
 *
 * \param[in]  p_dev        : ָ��SPI�豸��ָ��
 * \param[in]  target_speed : Ŀ���ٶ�
 *
 * \retval 0 ����֧��Ŀ���ٶ�,û�����óɹ�
 * \return �������õ��ٶȣ�������Ŀ���ٶȲ�ͬ
 *
 * \note   ���óɹ��������SPI�豸�е�cur_speed��min_diff
 */
uint32_t __spi_speed_cfg (amdr_spi_dev_t *p_dev, uint32_t target_speed)
{

    uint32_t real_speed, best_speed;  /* ��������ٶ�    */
    uint32_t pdiv, best_pdiv;         /* Ԥ��Ƶֵ        */
    uint32_t div, best_div;           /* ����ֵ          */
    uint32_t diff, min_diff;          /* �ٶȲ�ֵ        */
    uint32_t max_speed, min_speed;    /* ֧���ٶȷ�Χ    */
    uint32_t spi_freq;                /* SPI����ʱ��Ƶ�� */

    amhw_spi_t *p_hw_spi = p_dev->p_devinfo->p_hw_spi;

    spi_freq  = __SPI_FRQIIN_GET(p_hw_spi);
    max_speed = __SPI_MAXSPEED_GET(p_hw_spi);
    min_speed = __SPI_MINSPEED_GET(p_hw_spi);

    if (target_speed > max_speed || target_speed < min_speed) {
        return 0;
    }

   min_diff = 0xFFFFFFFFU;

   /* ����Ϊ���ķ�Ƶֵ,�ٶ�Ϊ��С */
   best_pdiv  = 7;
   best_div   = 8;
   best_speed = min_speed;

   /* ��������ʵ��ٶ� */
   for (pdiv = 0; (pdiv <= 7) && min_diff; pdiv++)
   {
       for (div = 0; (div <= 8) && min_diff; div++)
       {
           /* all_div = (pdiv+1) * 2^(div+1) */
           real_speed = (spi_freq / ((pdiv + 1) << (div+1)));

           /* ʵ���ٶ�ӦС��Ŀ���ٶ� */
           if (target_speed >= real_speed)
           {
               diff = target_speed-real_speed;

               if (min_diff > diff)
               {
                   /* ����ʵ��ٶ� */
                   min_diff   = diff;
                   best_pdiv  = pdiv;
                   best_div   = div;
                   best_speed = real_speed;
               }
           }
       }
   }

   amhw_spi_div_cfg(p_hw_spi, best_pdiv, best_div);

   p_dev->cur_speed  = best_speed; /* ���浱ǰ�ٶ�                       */
   p_dev->speed_diff = min_diff;   /* ����������������Χ�ڲ��ټ����ٶ�   */

   return p_dev->cur_speed;
}

/* end of file */
