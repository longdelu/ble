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
 * \brief UART ����ʵ��
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-10-20 xym, first implementation.
 * \endinternal
 */
#include "amdr_uart.h"
#include "amdr_clk.h"
#include "am_int.h"

/*******************************************************************************
* Functions declaration
*******************************************************************************/

/**
 * \brief ����ģʽ����ѯ���жϣ�����
 */
int __uart_mode_set (amdr_uart_dev_t *p_dev, uint32_t new_mode);

/**
 * \brief ����Ӳ������
 */
int __uart_opt_set (amdr_uart_dev_t *p_dev, uint32_t opts);


/* KS16 ���������������� */
static int __uart_ioctl (void *p_drv, int, void *);

static int __uart_tx_startup (void *p_drv);

static int __uart_callback_set (void *p_drv,
                                int   callback_type,
                                int  (*pfn_callback)(void *),
                                void  *callback_arg);

static int __uart_poll_getchar (void *p_drv, char *p_char);

static int __uart_poll_putchar (void *p_drv, char outchar);

static int __uart_connect (void *p_drv);

/** \brief ��׼��ӿں���ʵ�� */
static const struct am_uart_drv_funcs __g_uart_drv_funcs = {
         __uart_ioctl,
         __uart_tx_startup,
         __uart_callback_set,
         __uart_poll_getchar,
         __uart_poll_putchar,
         __uart_connect,
};

/******************************************************************************/

/**
 * \brief ����IO����
 *
 * ������/��ȡ�����ʡ�ģʽ���жϻ���ѯ��������ѡ�У�飬����λ����ֹͣλ��
 * �Լ��͵��ƿ��ƣ�RTS/CTS����
 *
 * \param[in] p_drv   : ָ������ƵĴ����豸�ṹ��
 * \param[in] request : ����ָ��
   - AM_UART_BAUD_SET : ���ò����ʣ� p_argΪuint32_t���ͣ�ֵΪ������
   - AM_UART_BAUD_GET : ��ȡ�����ʣ� p_argΪuint32_tָ������
   - AM_UART_OPTS_SET : ����Ӳ��������p_arg Ϊuint32_t���ͣ��磺#AM_UART_CS8��
   - AM_UART_OPTS_GET : ��ȡ��ǰ��Ӳ���������ã�p_argΪuint32_tָ������
   - AM_UART_MODE_SET : ����ģʽ��p_argֵΪAM_UART_MODE_POLL����AM_UART_MODE_INT
   - AM_UART_MODE_GET : ��ȡ��ǰģʽ�� p_argΪuint32_tָ������
   - AM_UART_AVAIL_MODES_GET : ��ȡ��ǰ���õ�ģʽ�� p_argΪuint32_tָ������
 * \param[in,out] p_arg      : ��ָ���Ӧ�Ĳ���
 *
 * \retval        AM_OK      : ����ָ��ִ�гɹ�
 * \retval       -AM_EIO     : ָ�����
 */
static int __uart_ioctl (void *p_drv, int request, void *p_arg)
{
    amdr_uart_dev_t *p_dev     = (amdr_uart_dev_t *)p_drv;
    int              status    = AM_OK;
    amhw_uart_t     *p_hw_uart = p_dev->p_devinfo->p_hw_uart;

    switch (request) {

    /* ���������� */
    case AM_UART_BAUD_SET:
        p_hw_uart = p_dev->p_devinfo->p_hw_uart;
        /* �ȴ�����*/
        while (!(amhw_uart_intstat_get(p_hw_uart) & AMHW_UART_INTSTAT_S1_IDLE));

        status = amhw_uart_baudrate_set(p_dev->p_devinfo->p_hw_uart,
                      amdr_clk_periph_rate_get((void *)(p_hw_uart)),
                                       (uint32_t)p_arg);

        if (status > 0) {
            p_dev->baud_rate = status;
            status = AM_OK;
        } else {
            status = -AM_EIO;
        }
        break;

     /* �����ʻ�ȡ */
    case AM_UART_BAUD_GET:
        *(int *)p_arg = p_dev->baud_rate;
        break;

    /* ģʽ���� */
    case AM_UART_MODE_SET:
        status = (__uart_mode_set(p_dev, (int)p_arg) == AM_OK)
                 ? AM_OK : -AM_EIO;
        break;

    /* ģʽ��ȡ */
    case AM_UART_MODE_GET:
        *(int *)p_arg = p_dev->channel_mode;
        break;

    /* ��ȡ���ڿ����õ�ģʽ */
    case AM_UART_AVAIL_MODES_GET:
        *(int *)p_arg = AM_UART_MODE_INT | AM_UART_MODE_POLL;
        break;

    /* ����ѡ������ */
    case AM_UART_OPTS_SET:
        status = (__uart_opt_set(p_dev, (int)p_arg) == AM_OK) ? AM_OK : -AM_EIO;
        break;

    /* ����ѡ���ȡ */
    case AM_UART_OPTS_GET:
        *(int *)p_arg = p_dev->options;
        break;
    default:
        status = -AM_EIO;
        break;
    }

    return (status);
}

/**
 * \brief �������ڷ���(�����ж�ģʽ)
 *
 * \param p_drv  : ָ������ƵĴ����豸�ṹ��
 *
 * \return AM_OK : �����ɹ�
 */
int __uart_tx_startup (void *p_drv)
{
    amdr_uart_dev_t *p_dev     = (amdr_uart_dev_t *)p_drv;
    amhw_uart_t     *p_hw_uart = p_dev->p_devinfo->p_hw_uart;

    /* ʹ�ܷ����ж� */
    amhw_uart_int_enable(p_hw_uart, AMHW_UART_INT_C2_TCIE);

    return AM_OK;
}

/**
 * \brief �����жϷ���ص�����
 *
 * ���ô��ڷ��ͺͽ����жϻص������е�
 * ��ȡ�����ַ���get���ʹ�Ž����ַ���put���Ļص�����
 *
 * \param     p_drv         : ָ������ƵĴ����豸�ṹ��
 * \param[in] callback_type : ָ�����õĺ��ֻص�����
 *            - AM_UART_CALLBACK_GET_TX_CHAR  : ��ȡһ�������ַ�����
 *            - AM_UART_CALLBACK_PUT_RCV_CHAR : �ύһ�����յ����ַ���Ӧ�ó���
 *            - AM_UART_CALLBACK_ERROR        : ����ص�����
 * \param[in] pfn_callback  : ָ��ص�������ָ��
 * \param[in] callback_arg  : �ص��������û�����
 *
 * \retval  AM_OK           : �ص��������óɹ�
 * \retval -AM_ENOTSUP      : ��֧�ָûص���������
 */
static int __uart_callback_set ( void  *p_drv,
                                 int    callback_type,
                                 int  (*pfn_callback)(void *),
                                 void  *callback_arg)
{
    amdr_uart_dev_t *p_dev = (amdr_uart_dev_t *)p_drv;

    switch (callback_type) {

    /* ���÷��ͻص������еĻ�ȡ�����ַ��ص����� */
    case AM_UART_CALLBACK_GET_TX_CHAR:
        p_dev->pfn_txchar_get = (pfn_uart_txchar_get_t)pfn_callback;
        p_dev->txget_arg      = callback_arg;
        return (AM_OK);

    /* ���ý��ջص������еĴ�Ž����ַ��ص����� */
    case AM_UART_CALLBACK_PUT_RCV_CHAR:
        p_dev->pfn_rxchar_put = (pfn_uart_rxchar_put_t)pfn_callback;
        p_dev->rxput_arg      = callback_arg;
        return (AM_OK);

    /* ���ô����쳣�ص����� */
    case AM_UART_CALLBACK_ERROR:
        p_dev->pfn_err = (pfn_uart_err_t)pfn_callback;
        p_dev->err_arg = callback_arg;
        return (AM_OK);

    default:
        return (-AM_ENOTSUP);
    }
}

/**
 * \brief ��ѯģʽ�·���һ���ַ�
 *
 * \param[in] p_dev     : ָ������ƵĴ����豸�ṹ��
 * \param[in] outchar   : �������ַ�
 *
 * \retval    AM_OK     : �������ݳɹ�
 * \retval   -AM_EAGAIN : ��������δ����������ʧ��
*/
static int __uart_poll_putchar (void *p_drv, char outchar)
{
    amdr_uart_dev_t *p_dev     = (amdr_uart_dev_t *)p_drv;
    amhw_uart_t     *p_hw_uart = p_dev->p_devinfo->p_hw_uart;
    bool_t          idle_stat  = (bool_t)(amhw_uart_intstat_get(p_hw_uart)>>4);

   // amhw_uart_poll_send(p_hw_uart,((uint8_t *)(&outchar)),1);

    /* ����ģ���Ƿ���� */
    if((idle_stat & amhw_uart_stat1_tdre_get(p_hw_uart)) == 0) {
        return (-AM_EAGAIN);
    } else {
      /* ����һ���ַ� */
      amhw_uart_txdata_write(p_hw_uart, outchar);
    }

    return (AM_OK);
}

/**
 * \brief ��ѯģʽ�½����ַ�
 * \param[in]  p_dev  : ָ������ƵĴ����豸�ṹ��
 * \param[out] p_char : ָ�����Ž������ݵĴ洢��Ԫ
 *
 * \retval  AM_OK     : �������ݳɹ�
 * \retval -AM_EAGAIN : ��������δ����������ʧ��
 */
static int __uart_poll_getchar (void *p_drv, char *p_char)
{
    amdr_uart_dev_t *p_dev     = (amdr_uart_dev_t *)p_drv;
    amhw_uart_t     *p_hw_uart = p_dev->p_devinfo->p_hw_uart;
    uint8_t         *p_inchar  = (uint8_t *)p_char;
    bool_t          idle_stat  = (bool_t)(amhw_uart_intstat_get(p_hw_uart)>>4);
    //amhw_uart_poll_receive(p_hw_uart,p_inchar,1);

    /* ����ģ���Ƿ���� */
    if((idle_stat & amhw_uart_stat1_rdre_get(p_hw_uart)) == 0) {
        return (-AM_EAGAIN);
      }else {
          /* ����һ���ַ� */
          *p_inchar = amhw_uart_rxdata_read(p_hw_uart);
      }

    return (AM_OK);
}

/**
 * \brief ���ô���ģʽ
 *
 * \param[in] p_dev    : ָ������ƵĴ����豸�ṹ��
 * \param[in] new_mode : ����ģʽ
 *
 * \retval    AM_OK    : ģʽ���óɹ�
 * \retval    AM_ERROR : ģʽ��֧�֣�����ʧ��
 */
int __uart_mode_set (amdr_uart_dev_t *p_dev, uint32_t new_mode)
{
    amhw_uart_t *p_hw_uart = p_dev->p_devinfo->p_hw_uart;

    /* ��֧������ģʽ */
    if ((new_mode != AM_UART_MODE_POLL) && (new_mode != AM_UART_MODE_INT)) {
        return (AM_ERROR);
    }

    if (new_mode == AM_UART_MODE_INT) {
        /* ʹ��RDRF����׼�ж� */
        amhw_uart_int_enable(p_hw_uart, AMHW_UART_INT_C2_IRIE);
    } else {
        /* �ر����д����ж� */
        amhw_uart_int_disable(p_hw_uart, AMHW_UART_INT_ALL);
    }

    p_dev->channel_mode = new_mode;

    return (AM_OK);
}

/**
 * \brief ����ѡ������
 *
 * \param[in] p_dev     : ָ������ƵĴ����豸�ṹ��
 * \param[in] options   : ����ѡ�����
 *
 * \retval    AM_OK     : ���óɹ�
 * \retval   -AM_EINVAL : �豸ΪNULL������ʧ��
 *
 * \note ֧�����ݳ��ȣ�У�鷽ʽ��ֹͣλ����
 */
int __uart_opt_set (amdr_uart_dev_t *p_dev, uint32_t options)
{
    amhw_uart_t *p_hw_uart = p_dev->p_devinfo->p_hw_uart;
    uint8_t      cfg_flags = 0;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    p_hw_uart = p_dev->p_devinfo->p_hw_uart;

    /* �ڸı�UART�Ĵ���ֵǰ ���շ��ͽ��� */
    amhw_uart_disable(p_hw_uart);

    /* �������ݳ��� */
    switch (options & AM_UART_CSIZE) {

    case AM_UART_CS7:
        cfg_flags &= ~(1 << 4);
        cfg_flags |= AMHW_UART_C1_M_8BIT;
        break;

    case AM_UART_CS8:
        cfg_flags &= ~(1 << 4);
        cfg_flags |= AMHW_UART_C1_M_9BIT;
        break;

    default:
        break;
    }

    /* ����ֹͣλ */
    if (options & AM_UART_STOPB) {
        cfg_flags &= ~(0x01 << 5);
        cfg_flags |= AMHW_UART_BDH_SBNS_STOP_2;
    } else {
        cfg_flags &= ~(0x01 << 5);
        cfg_flags |= AMHW_UART_BDH_SBNS_STOP_1;
    }

    /* ���ü��鷽ʽ */
    if (options & AM_UART_PARENB) {
        cfg_flags &= ~(0x03 << 0);

        if (options & AM_UART_PARODD) {
            cfg_flags |= AMHW_UART_C1_PARITY_ODD;
        } else {
            cfg_flags |= AMHW_UART_C1_PARITY_EVEN;
        }
    } else {
        cfg_flags &= ~(0x03 << 0);
        cfg_flags |= AMHW_UART_C1_PARITY_NO;
    }

    /* �������Ч���� */

    amhw_uart_stop_bit_set (p_hw_uart, (cfg_flags & 0x20));
    amhw_uart_data_mode_set(p_hw_uart, (cfg_flags & 0x10));
    amhw_uart_parity_set(p_hw_uart,    (cfg_flags & 0x03));

    amhw_uart_enable(p_hw_uart);

    p_dev->options = options;

    return (AM_OK);
}



/******************************************************************************
  UART interrupt request handler
******************************************************************************/
/**
 * \brief ���ڽ����жϷ���
 * \param[in] p_dev �� ָ������ƵĴ����豸�ṹ��
 */
void __uart_irq_rx_handler (amdr_uart_dev_t *p_dev)
{
    amhw_uart_t *p_hw_uart = p_dev->p_devinfo->p_hw_uart;
    uint32_t      int_stat   = amhw_uart_intstat_get(p_hw_uart);

    char data;

    /* �Ƿ�Ϊ����Rx�ж� */
    if ((int_stat & AMHW_UART_INTSTAT_S1_RDRF) != 0) {

        /* ��ȡ�½������� */
        data = amhw_uart_rxdata_read(p_hw_uart);

        /* ����½������� */
        p_dev->pfn_rxchar_put(p_dev->rxput_arg, data);
    }
}

/**
 * \brief ���ڷ����жϷ���
 * \param[in] p_dev : ָ������ƵĴ����豸�ṹ��
 */
void __uart_irq_tx_handler (amdr_uart_dev_t *p_dev)
{
    amhw_uart_t *p_hw_uart = p_dev->p_devinfo->p_hw_uart;
    uint32_t    int_stat   = amhw_uart_intstat_get(p_hw_uart);

    char data;

    if (((int_stat & AMHW_UART_INTSTAT_S1_TDRE) != 0) || /* �Ƿ�Ϊ����Tx�ж� */
        ((int_stat & AMHW_UART_INTSTAT_S1_IDLE) != 0)) {

        /* ��ȡ�������ݲ����� */
        if ((p_dev->pfn_txchar_get(p_dev->txget_arg, &data)) == AM_OK) {
            amhw_uart_txdata_write(p_hw_uart, data);
        } else {
            /* û�����ݴ��;͹رշ����ж� */
            amhw_uart_int_disable(p_hw_uart, AMHW_UART_INT_C2_TCIE);
        }

    }
}

/**
 * \brief �����жϷ�����
 * \param[in] p_arg : ָ������ƵĴ����豸�ṹ�� ����am_int_connect()ע��
 */
void __uart_irq_handler (void *p_arg)
{
    amdr_uart_dev_t     *p_dev      = (amdr_uart_dev_t *)p_arg;
    amhw_uart_t         *p_hw_uart = p_dev->p_devinfo->p_hw_uart;

    uint32_t uart_int_stat = amhw_uart_intstat_get(p_hw_uart);


    if (uart_int_stat & AMHW_UART_INTSTAT_S1_RDRF) {
         __uart_irq_rx_handler(p_dev);
    } else if (uart_int_stat & AMHW_UART_INTSTAT_S1_TDRE) {
        __uart_irq_tx_handler(p_dev);
    } else {

    }
    /* �����ж� */
    if ((p_dev->other_int_enable & uart_int_stat) != 0) {

        uart_int_stat &= p_dev->other_int_enable;

        if (p_dev->pfn_err != NULL) {
            p_dev->pfn_err(p_dev->err_arg,
                           AMDR_UART_ERRCODE_UART_OTHER_INT,
                           (void *)uart_int_stat,
                           1);
        }
    }

}

/**
 * \brief UART�жϺ������ӣ���ʹ���ж�ģʽʱ��Ҫ���ô˺���
 *
 * \param[in] p_dev : ָ������ƵĴ����豸�ṹ��
 *
 * \retval    AM_OK : ���ӳɹ�
 */
int __uart_connect (void *p_drv)
{
    amdr_uart_dev_t *p_dev = (amdr_uart_dev_t *)p_drv;

    /* �����ж������ţ������ж� */
    am_int_connect(p_dev->p_devinfo->inum, __uart_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);
    amhw_uart_int_enable(p_dev->p_devinfo->p_hw_uart,
                          p_dev->other_int_enable);

    return AM_OK;
}

/**
 * \brief Ĭ�ϻص�����
 *
 * \returns AW_ERROR
 */
static int __uart_dummy_callback (void *p_arg, char *p_outchar)
{
    return (AM_ERROR);
}

/**
 * \brief ���ڳ�ʼ������
 *
 * \param[in] p_dev     : ָ������ƵĴ����豸�ṹ��
 * \param[in] p_devinfo : ָ�򴮿��豸����Ϣ�ṹ�峣��
 *
 * \return ���ڱ�׼������
 */
am_uart_handle_t amdr_uart_init (amdr_uart_dev_t           *p_dev,
                                  const amdr_uart_devinfo_t *p_devinfo)
{
    amhw_uart_t           *p_hw_uart;
    uint32_t                tmp;

    if (p_devinfo == NULL || p_devinfo->p_hw_uart == NULL) {
        return NULL;
    }

    /* ��ȡ���ò��� */
    p_hw_uart               = p_devinfo->p_hw_uart;
    p_dev->p_devinfo         = p_devinfo;
    p_dev->uart_serv.p_funcs = (struct am_uart_drv_funcs *)&__g_uart_drv_funcs;
    p_dev->uart_serv.p_drv   = p_dev;
    p_dev->baud_rate         = p_devinfo->baud_rate;
    p_dev->options           = 0;

    /* ��ʼ��Ĭ�ϻص����� */
    p_dev->pfn_txchar_get    = (int (*) (void *, char*))__uart_dummy_callback;
    p_dev->txget_arg         = NULL;
    p_dev->pfn_rxchar_put    = (int (*) (void *, char ))__uart_dummy_callback;
    p_dev->rxput_arg         = NULL;
    p_dev->pfn_err           =
                     (int (*) (void *, int, void*, int))__uart_dummy_callback;

    p_dev->err_arg           = NULL;

    p_dev->other_int_enable = p_devinfo->other_int_enable &
                                  ~(AMHW_UART_INT_C2_TCIE | 
                                  AMHW_UART_INT_C2_IRIE   |
                                  AMHW_UART_INT_C2_ILIE   |
                                  AMHW_UART_INT_C2_TIE);

    /* ��ȡ�������ݳ�������ѡ�� */
    tmp = p_devinfo->cfg_flags;
    tmp = (tmp >> 4) & 0x01;

    switch (tmp) {

    case 0:
        p_dev->options |= AM_UART_CS7;
        break;

    case 1:
        p_dev->options |= AM_UART_CS8;
        break;

    default:
        p_dev->options |= AM_UART_CS8;
        break;
    }

    /* ��ȡ���ڼ��鷽ʽ����ѡ�� */
    tmp = p_devinfo->cfg_flags;
    tmp = (tmp >> 0) & 0x03;

    if (tmp == 2) {
        p_dev->options |= AM_UART_PARENB;
    } else if (tmp == 3) {
        p_dev->options |= (AM_UART_PARENB | AM_UART_PARODD);
    }else{
    }
    /* ��ȡ����ֹͣλ����ѡ�� */
    if (p_devinfo->cfg_flags & AMHW_UART_BDH_SBNS_STOP_2) {
        p_dev->options |= AM_UART_STOPB;
    }else{

    }
    __uart_opt_set (p_dev, p_dev->options);

    /* ���ò����� */
    p_dev->baud_rate = amhw_uart_baudrate_set(p_hw_uart,
                                              amdr_clk_periph_rate_get((void *)(p_hw_uart)),
                                              p_devinfo->baud_rate);
    /* Ĭ����ѯģʽ  */
    __uart_mode_set(p_dev, AM_UART_MODE_POLL);

    /* uartʹ�� */
    amhw_uart_enable(p_hw_uart);

    return &(p_dev->uart_serv);
}

/**
 * \brief ����ȥ��ʼ��
 *
 * \param[in] p_dev : ָ������ƵĴ����豸�ṹ��
 *
 */
void amdr_uart_deinit (amdr_uart_dev_t *p_dev)
{
    amhw_uart_t *p_hw_uart   = p_dev->p_devinfo->p_hw_uart;
    p_dev->uart_serv.p_funcs = NULL;
    p_dev->uart_serv.p_drv   = NULL;

    if (p_dev->channel_mode == AM_UART_MODE_INT) {
        /* Ĭ��Ϊ��ѯģʽ */
        __uart_mode_set(p_dev, AM_UART_MODE_POLL);
    }
    /* �رմ��� */
    amhw_uart_enable(p_hw_uart);

    am_int_disable(p_dev->p_devinfo->inum);
}

/* end of file */
