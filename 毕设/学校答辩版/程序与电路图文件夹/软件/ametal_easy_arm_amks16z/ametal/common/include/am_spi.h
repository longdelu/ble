/*******************************************************************************
*                                 Apollo
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/
/**
 * \file
 * \brief SPI��׼�ӿ�
 *
 * \internal
 * \par Modification history
 * - 1.00 14-11-01  jon, first implementation.
 * - 1.01 15-08-17  tee, modified some interface.
 * \endinternal
 */

#ifndef __AM_SPI_H
#define __AM_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"

/**
 * \addtogroup am_if_spi
 * \copydoc am_spi.h
 * @{
 */

/**
 * \name SPIģʽ��־������aw_spi_mkdev()������mode����
 * @{
 */

#define AM_SPI_CPHA        0x01  /**< \brief ʱ����λ������                   */
#define AM_SPI_CPOL        0x02  /**< \brief ʱ�Ӽ��Կ�����                   */
#define AM_SPI_CS_HIGH     0x04  /**< \brief Ƭѡ����Ч                       */
#define AM_SPI_LSB_FIRST   0x08  /**< \brief ����λ�ȳ��ķ�ʽ��������֡       */
#define AM_SPI_3WIRE       0x10  /**< \brief 3��ģʽ��SI/SO �ź��߹���        */
#define AM_SPI_LOOP        0x20  /**< \brief �ػ�ģʽ                         */
#define AM_SPI_NO_CS       0x40  /**< \brief ���豸����, ��Ƭѡ               */
#define AM_SPI_READY       0x80  /**< \brief READY�ź�,�ӻ����ʹ��ź���ͣ���� */

#define AM_SPI_MODE_0       (0 | 0)                     /**< \brief SPIģʽ0  */
#define AM_SPI_MODE_1       (0 | AM_SPI_CPHA)           /**< \brief SPIģʽ1  */
#define AM_SPI_MODE_2       (AM_SPI_CPOL | 0)           /**< \brief SPIģʽ2  */
#define AM_SPI_MODE_3       (AM_SPI_CPOL | AM_SPI_CPHA) /**< \brief SPIģʽ3  */

/** @} */

/**
 * \name SPI���������������ñ�־������aw_spi_mktrans()������flags����
 * @{
 */

/** \brief SPI�ڶ�ȡ������MOSI��������ߵ�ƽ��Ĭ��Ϊ�͵�ƽ��    */
#define AM_SPI_READ_MOSI_HIGH    0x01

/** \brief ���ô�����һϵ�д�������һ������ʱ��Ӧ�����øñ�־λ */
#define AM_SPI_TRANS_EOT         0x02

/** @} */


/** 
 * \brief SPI��������Ϣ�ṹ��
 */
typedef struct am_spi_info {
    uint32_t  min_speed;     /**< \brief ��С����Ƶ��                        */
    uint32_t  max_speed;     /**< \brief �����Ƶ��                        */ 
    uint32_t  features;      /**< \brief SPI����������,֧�ֵļ���SPI���ñ�־�� */
} am_spi_info_t;

struct am_spi_device;        /**< \brief ����SPI�ӻ��豸�Ľṹ������         */
struct am_spi_transfer;      /**< \brief ����SPI����Ľṹ������             */
/**
 * \brief SPI���������ṹ��
 */
struct am_spi_drv_funcs {

    /** \brief ��ȡSPI��������Ϣ */
    int (*pfn_spi_info_get) (void *p_drv, am_spi_info_t *p_info);
    
    /** \brief ����SPI�ӻ��豸   */
    int (*pfn_spi_setup) (void *p_drv, struct am_spi_device *p_dev);
    
    /** \brief SPI����           */
    int (*pfn_spi_transfer)(void                   *p_drv,
                            struct am_spi_device   *p_dev,
                            struct am_spi_transfer *p_trans);
    
    /** \brief SPI�ж����Ӻ��� */
    int (*pfn_spi_connect)(void *p_drv);
};

/**
 * \brief SPI ��׼����ṹ��
 */
typedef struct am_spi_serv {
    struct am_spi_drv_funcs   *p_funcs;    /**< \brief SPI  SPI��������      */
    void                      *p_drv;      /**< \brief SPI  SPI������������  */
} am_spi_serv_t;

/** \brief SPI ��׼�������������� */
typedef am_spi_serv_t *am_spi_handle_t;

/**
 * \brief SPI ����
 *
 * - SPI ����д�ĸ������ǵ��ڶ��ĸ�����Э������Ӧ�������ṩ \a rx_buf ��/��
 * \a tx_buf ����ĳЩ����£�����ͬ���������ṩ���ݴ����DMA��ַ�������Ļ�����
 * ����������ʹ��DMA���Խ������ݴ���Ĵ��ۡ�
 *
 * - ������仺���� \a tx_buf ΪNULL��������� \a rx_buf ʱ��0���ᱻ�Ƴ��������ϡ�
 * ������ջ����� \a rx_buf ΪNULL�������������������ݽ��ᱻ������ֻ�� \a len
 * ���ֽڻᱻ�Ƴ������롣�����Ƴ��������Ǵ���Ĳ��������磬�Ƴ�3���ֽڶ��ִ�С
 * �� 16 �� 20 bit��ǰ��ÿ����ʹ��2���ֽڣ�������ʹ��4���ֽڡ�
 *
 * - ���ڴ��У��������ǰ�CPU�ĵı����ֽ�˳���ţ������ֽ�˳���� (����ˣ�����
 * �豸ģʽ�����־�б������� AW_SPI_LSB_FIRST)�����磬��\a bits_per_word Ϊ
 * 16��������Ϊ 2N �ֽڳ� (\a len = 2N) ������CPU�ֽ�˳�㱣��N��16-bit�֡�
 *
 * - �� SPI ������ִ�С����2���ݴα�8-bitʱ����Щ�ڴ��е��ֽ����������bit����
 * ���У����������Ҷ���ģ���ˣ�δ���� (rx) ��δʹ�� (tx) ��λ���������Чλ��
 
 * - SPI���俪ʼʱ��Ƭѡ�����Ч�����俪ʼ������һֱ������Ч������Ƭѡ����Ч����
 *   ���������
 *  -# cs_change = 0 ��Ĭ�������
 *      - ������������ϵ�д�������һ�����䣨�����˱�־��AM_SPI_TRANS_EOT��ʱ,
 *        �ŻὫƬѡ��Ϊ��Ч������Ƭѡ��Ч״̬���ı䡣
 *  -# cs_change = 1
 *      - �����䲻��ϵ�д�������һ�����䣨δ���ñ�־��AM_SPI_TRANS_EOT��,��
 *        ���δ����������ƬѡΪ��Ч��
 *      - ��������ϵ�д�������һ�����䣨�����˱�־��AM_SPI_TRANS_EOT������
 *        ���δ��������Ƭѡ״̬���������ı䣬ֱ����һ���µĴ��俪ʼʱ���Ż�
 *        �����������:
 *          - �µĴ�������ͬ�豸����Ƭѡ�������ı䣬һֱ������Ч���µĴ�����ɣ�
 *            �ٸ����µĴ�����ر�־����Ƭѡ��״̬��
 *          - �µĴ������µĲ�ͬ�豸������Ƚ����豸��Ƭѡ��Ϊ��Ч�����Ž��µ�
 *            �豸Ƭѡ��Ϊ��Ч��ʼ���䣬����ͬ�����������������Ƭѡ�ı仯��
 */
typedef struct am_spi_transfer {
    const void *p_txbuf;         /**< \brief ָ�������ݻ����ָ��   */
    void       *p_rxbuf;         /**< \brief ָ��������ݻ����ָ��   */
    uint32_t    nbytes;          /**< \brief ���ݳ���(�ֽ���)         */
    
    /** \brief �ִ�С����Ϊ0����ʹ�� am_spi_device_t �����õ�ֵ */
    uint8_t     bits_per_word;   
    
    /** \brief ��������ɺ��Ƿ�Ӱ��Ƭѡ: 1-Ӱ�� 0-��Ӱ��     */
    uint8_t     cs_change;      

    /** \brief �ڱ�����֮���ڸı�Ƭѡ״̬֮ǰ��ʱ��΢����    */
    uint16_t    delay_usecs;

    /**
     * \brief Ϊ���δ���ѡ��һ���� aw_spi_device_t �е�Ĭ�����ò�ͬ���ٶȣ�
     * 0 ��ʾʹ��Ĭ��ֵ
     */
    uint32_t    speed_hz;

    /**
     * \brief ���δ�����̵�һЩ��������
     *
     * �����ڶ�ȡ����MOSI���ŵ�״̬���Լ��Ƿ���һϵ�д�������һ������
     */
    uint32_t     flags;

    void      (*pfn_callback)(void *,int); /**< \brief ���δ�����ɻص�����   */
    void       *p_arg;                     /**< \brief �ص���������ڲ���     */
} am_spi_transfer_t;

/** \brief SPI���豸�����ṹ */
typedef struct am_spi_device {

    am_spi_handle_t  handle;       /**< \brief ���豸������SPI��׼����handle  */
    
    /**
     * \brief ���ݴ������1�������֣��Ƚϳ������ִ�СΪ8-bit��12-bit�����ڴ�
     * �У��ִ�СӦ����2���ݴη������磬20-bit�Դ�СӦʹ��32-bit�ڴ档�����ÿ���
     * ���豸�����ı䣬��������Ϊ0��ʾʹ��Ĭ�ϵġ��ִ�СΪ8-bit����
     *�����䡱�� am_spi_transfer_t.bits_per_word �������ش����á�
     */
    uint8_t          bits_per_word;

    /** \brief ���豸��SPIģʽ��־����ο���SPIģʽ��־��*/
    uint16_t         mode;

    /**
     * \brief �豸֧�ֵ�����ٶ�
     *
     * �����䡱�� am_spi_transfer_t.speed_hz �����ش����á�
     */
    uint32_t    max_speed_hz;

    /**
     * \brief Ƭѡ���ű��
     *
     * �� aw_spi_device.pfunc_cs ΪNULLʱ����ʹ�ô����á�Ĭ�ϵ͵�ƽ��Ч����
     * aw_spi_device.mode �� AW_SPI_CS_HIGH ��־�����ã���ߵ�ƽ��Ч��
     */
    int         cs_pin;

    /**
     * \brief �Զ���Ƭѡ���ƺ���
     *
     * ʹ�ô˺��������豸��Ƭѡ: ���� \a state Ϊ1ʱƬѡ��Ч��Ϊ0ʱƬѡ��Ч��
     * ��������ΪNULLʱ����ʹ�� aw_spi_device.cs_pin ��ΪƬѡ���ţ�����������
     * ��������һ�����ʵ�Ƭѡ���ƺ�����
     *
     * \param p_dev
     */
    void      (*pfunc_cs)(struct am_spi_device *p_dev, int state);

} am_spi_device_t;

/** 
 * \brief ��ȡSPI����������Ϣ
 * 
 * \param[in]  handle : SPI��׼����������
 * \param[out] p_info : ���ڻ�ȡSPI��������Ϣ��ָ��
 *
 * \retval  AM_OK     : ��ȡ��Ϣ�ɹ�
 * \retval -AM_EINVAL : ��ȡ��Ϣʧ��, ��������
 */
am_static_inline
int am_spi_info_get (am_spi_handle_t handle, am_spi_info_t *p_info)
{
    return handle->p_funcs->pfn_spi_info_get(handle->p_drv,p_info);
}

/**
 * \brief SPI�ж����Ӻ���
 * \param[in] handle : SPI��׼����������
 * \retval  AM_OK    : ���ӳɹ�
 */
am_static_inline
int am_spi_connect (am_spi_handle_t handle)
{
    return handle->p_funcs->pfn_spi_connect(handle->p_drv);
}

/**
 * \brief ����SPI�豸�ṹ�����
 *
 * \param[in] p_dev         : SPI�ӻ��豸������ָ��
 * \param[in] handle        : SPI��׼����������
 * \param[in] bits_per_word : �ִ�С��Ϊ0ʱʹ��Ĭ�ϵġ�8-bit�ִ�С��
 * \param[in] mode          : �豸ģʽ��־������SPIģʽ��־��
 * \param[in] max_speed_hz  : �豸֧�ֵ�����ٶ�
 * \param[in] cs_pin        : Ƭѡ���ű��(\a pfunc_cs ΪNULLʱ����������Ч)
 * \param[in] pfunc_cs      : Ƭѡ���ƺ���(������ΪNULLʱ��\a cs_pin������Ч)
 *
 * \par ����
 * �� aw_spi_setup()
 */
am_static_inline
void am_spi_mkdev (am_spi_device_t *p_dev,
                   am_spi_handle_t  handle,
                   uint8_t          bits_per_word,
                   uint16_t         mode,
                   uint32_t         max_speed_hz,
                   int              cs_pin,
                   void           (*pfunc_cs)(am_spi_device_t *p_dev, int state))
{
    p_dev->handle        = handle;
    p_dev->bits_per_word = bits_per_word;
    p_dev->mode          = mode;
    p_dev->max_speed_hz  = max_speed_hz;
    p_dev->cs_pin        = cs_pin;
    p_dev->pfunc_cs      = pfunc_cs;
}

/**
 * \brief SPI����ṹ���������
 *
 * \param[in] p_trans       : ָ��SPI����ṹ���ָ��
 * \param[in] p_txbuf       : ָ�������ݻ����ָ��,������ʱӦ����ΪNULL
 * \param[in] p_rxbuf       : ָ��������ݻ����ָ��,������ʱӦ����ΪNULL
 * \param[in] nbytes        : ��ǰ�������ݳ��ȣ��ֽ�����16λ����ʱ���ֽ���Ӧ����2 * N�����ݸ�������
 * \param[in] cs_change     : ��������ɺ��Ƿ�Ӱ��Ƭѡ: 1-Ӱ�� 0-��Ӱ��
 * \param[in] bits_per_word : �ִ�С��Ϊ0ʱʹ��Ĭ�ϵġ�8-bit�ִ�С��
 * \param[in] delay_usecs   : �ڱ�����֮���ڸı�Ƭѡ״̬֮ǰ��ʱ��΢����
 * \param[in] speed_hz      : ��������
 * \param[in] flags         : �����־
 * \param[in] pfn_callback  : �ص�����
 * \param[in] p_arg         : �ص���������ڲ���
 *
 * \retval  AM_OK     : ����ṹ������������
 *
 * \note ������ͺͽ������ݻ���������Ч�����ȷ�����ǵ����ݳ�����һ�µġ�
 *       ���ڻ����������ͺ����ݴ����ֽ�����
 *        - 1  ~ 8 λ��ʱ��uint8_t ���ͣ�nbytes = ���ݴ���ĸ���
 *        - 9  ~ 16λ��ʱ��uint16_t���ͣ�nbytes = 2 * ���ݴ���ĸ���
 *        - 17 ~ 31λ��ʱ��uint32_t���ͣ�nbytes = 4 * ���ݴ���ĸ���
 */
am_static_inline
void am_spi_mktrans (am_spi_transfer_t *p_trans,
                     const void        *p_txbuf,
                     void              *p_rxbuf,
                     uint32_t           nbytes,
                     uint8_t            cs_change,
                     uint8_t            bits_per_word,
                     uint16_t           delay_usecs,
                     uint32_t           speed_hz,
                     uint32_t           flags,
                     void             (*pfn_callback)(void *,int),
                     void              *p_arg)
{
    p_trans->p_txbuf        = p_txbuf;
    p_trans->p_rxbuf        = p_rxbuf;
    p_trans->nbytes         = nbytes;
    p_trans->cs_change      = cs_change;
    p_trans->bits_per_word  = bits_per_word;
    p_trans->delay_usecs    = delay_usecs;
    p_trans->speed_hz       = speed_hz;
    p_trans->flags          = flags;
    p_trans->pfn_callback   = pfn_callback; 
    p_trans->p_arg          = p_arg;
}

/**
 * \brief ����SPI�ӻ��豸
 *
 * \attention ��SPI�ӻ��豸��ʹ��ǰ�������ȵ��ñ������������ã����籾��������
 *            ������һ��������ʹ�ô�SPI�ӻ�
 *
 * \param[in]   p_dev   SPI�ӻ��豸
 *
 * \retval   AM_OK           : ���óɹ�
 * \retval  -AM_ENOTSUP      : ����ʧ�ܣ���֧�ֵ�λ��ģʽ��
 *
 * \par ʾ��
 * \code
 * am_spi_device_t spi_dev;     // �豸�����ṹ
 *
 * // ��ʼ���豸�����ṹ
 * am_spi_mkdev(&spi_dev,
 *              handle,         // SPI��׼����handle
 *              8��             // �ִ�СΪ8-bit
 *              AW_SPI_MODE_0,  // SPI ģʽ0
 *              500000,         // ֧�ֵ�����ٶ� 500000 Hz
 *              PIO0_3,         // Ƭѡ����Ϊ PIO0_3
 *              NULL);          // ���Զ����Ƭѡ���ƺ���
 *
 * // �����豸
 * am_spi_setup(&spi_dev);
 *
 * \endcode
 */
am_static_inline
int am_spi_setup (am_spi_device_t *p_dev)
{
    return p_dev->handle->p_funcs->pfn_spi_setup(p_dev->handle->p_drv, p_dev);
}

/**
 * \brief SPI���亯��
 *
 * \param[in] p_dev   : SPI�ӻ��豸
 * \param[in] p_trans : ָ��SPI����ṹ���ָ��
 *
 * \retval   AM_OK     : �����������д���
 * \retval -AM_EINVAL : �����������
 *  ��������Լ����״̬��ͨ������ص�������ȡ״̬λ
 *    \li  AW_OK  : �������
 *    \li -AM_EIO : �������
 */
am_static_inline
int am_spi_transfer (am_spi_device_t   *p_dev,
                     am_spi_transfer_t *p_trans)
{
    return p_dev->handle->p_funcs->pfn_spi_transfer(p_dev->handle->p_drv, 
                                                    p_dev, 
                                                    p_trans);
}


/**
 * \brief ��д���
 *
 * �������Ƚ���SPIд�������ٽ��ж�������
 * ��ʵ��Ӧ���У�д����������Ϊ��ַ����������ΪҪ�Ӹõ�ַ��ȡ�����ݡ�
 *
 * \param[in]   p_dev   SPI�ӻ��豸
 * \param[in]   p_txbuf ���ݷ��ͻ�����
 * \param[in]   n_tx    Ҫ���͵������ֽڸ���
 * \param[out]  p_rxbuf ���ݽ��ջ�����
 * \param[in]   n_rx    Ҫ���յ������ֽڸ���
 *
 * \retval AM_OK        ��Ϣ����ɹ�
 * \retval -AM_EINVAL   ��������
 * \retval -AM_EIO      �������
 */
int am_spi_write_then_read (am_spi_device_t *p_dev,
                            const uint8_t   *p_txbuf,
                            size_t           n_tx,
                            uint8_t         *p_rxbuf,
                            size_t           n_rx);

/**
 * \brief ִ������д����
 *
 * ����������ִ������д���������η������ݻ�����0�ͻ�����1�е����ݡ�
 * ��ʵ��Ӧ���У�������0����Ϊ��ַ��������1ΪҪд��õ�ַ�����ݡ�
 *
 * \param[in]   p_dev    SPI�ӻ��豸
 * \param[in]   p_txbuf0 ���ݷ��ͻ�����0
 * \param[in]   n_tx0    ������0���ݸ���
 * \param[in]   p_txbuf1 ���ݷ��ͻ�����1
 * \param[in]   n_tx1    ������1���ݸ���
 *
 * \retval AM_OK        ��Ϣ����ɹ�
 * \retval -AM_EINVAL   ��������
 * \retval -AM_EIO      �������
 */
int am_spi_write_then_write (am_spi_device_t *p_dev,
                             const uint8_t   *p_txbuf0,
                             size_t           n_tx0,
                             const uint8_t   *p_txbuf1,
                             size_t           n_tx1);
/** 
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_SPI_H */

/*end of file */
