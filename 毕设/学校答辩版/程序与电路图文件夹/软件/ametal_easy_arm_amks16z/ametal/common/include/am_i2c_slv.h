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
 * \brief I2C���ӻ�����׼�ӿ�
 *
 * \internal
 * \par Modification History
 * - 1.00 15-09-26  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_I2C_SLV_H
#define __AM_I2C_SLV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"

/**
 * @addtogroup am_if_i2c_slv
 * @copydoc am_i2c_slv.h
 * @{
 */

/**
 * \name I2C �ӻ���ַ��ر�־������ am_i2c_slv_device_t �� addr_flags ��Ա
 * @{ 
 */

/** \brief �ӻ�Ϊ7-bit��ַ��Ĭ�ϣ�     */
#define AM_I2C_S_ADDR_7BIT          0x0000u

/** \brief �ӻ�Ϊ10-bits ��ַ          */
#define AM_I2C_S_ADDR_10BIT         0x0001u

/** \brief I2C�ӻ���ַ���Ʊ�־����     */
#define AM_I2C_S_ADDR_MASK          0x000Fu

/** @} */
 
/**
 * \name I2C���ӻ���������Ʊ�ʶ������am_i2c_slv_transfer �� flags��Ա
 * @{ 
 */
 
/** \brief ��������������Ӧ�𣨵���������������ʱ��                  */
#define AM_I2C_S_TRANS_IGNORE_NAK         0x0010u

/** \brief �ڶ������У���������ʱ�����˴�������һ�����ݲ�����Ӧ��  */
#define AM_I2C_S_TRANS_LAST_RD_NACK       0x0020u

/** \brief �ڶ������У���������ʱ�����������ݾ�������Ӧ��            */
#define AM_I2C_S_TRANS_RD_NACK            0x0040u

/** @} */

/**
 * \name �ӻ������ݴ��䷽����Щ��ֵ��ʾ�˵�ַƥ��ص������� dir ����ֵ�ĺ���
 * @{
 */

#define AM_I2C_S_DIR_READ   0  /**< \brief �ӻ������ݣ����䷽������ -> �ӻ� */
#define AM_I2C_S_DIR_WRITE  1  /**< \brief �ӻ�д���ݣ����䷽�򣺴ӻ� -> ���� */

/** @} */

/** \brief �ӻ��豸��������     */
struct am_i2c_slv_dev;

/** \brief �ӻ����ݴ����������� */
struct am_i2c_slv_transfer;
   
/**
 * \name �ص��������Ͷ���
 * @{
 */

/**
 * \brief ��ȡһ�����䲢��ʼ���ݴ��䣬������ɺ󣬵��ô�����ɻص�����
 *
 *     ����Ҫ���ջ�������ʱ�������øá������ȡ����������ȡһ�����䡣����ķ���
 * �ɲ��� dir ָ�����û����������趨���䷽��
 *
 *     ���� p_arg ΪӦ�ó����Զ��������
 *
 *     ���� dir �����˱��λ�ȡ�Ĵ�������ݴ��䷽��
 *       - AM_I2C_S_DIR_READ  �ӻ������ݣ����䷽��Ϊ������ -> �ӻ�
 *       - AM_I2C_S_DIR_WRITE �ӻ�д���ݣ����䷽��Ϊ: �ӻ� -> ����
 *
 *     ���� pp_trans ���ڻ�ȡһ�����䣬���ڸô���Ҫ��ȡһ������ĵ�ַ����
 * ������Ϊ��άָ�롣��ȡ��ʽ�ܼ򵥣��磺 *pp_trans = &trans;
 *
 * \note ���ȷ�������䡱�ĵ�ַ�Լ������еġ����ݻ���������������������о�����Ч�ġ�
 *       ����ʹ�þ�̬��ȫ�ֱ�����
 */
typedef void (*am_i2c_slv_trans_get_cb_t) (void                        *p_arg,
                                           uint8_t                      dir,
                                           struct am_i2c_slv_transfer **pp_trans);
 
/** 
 * \brief ��ַƥ��ص��������Ͷ���
 *
 *     ������START�źź󣬷��͵Ĵӻ���ַ��ע��Ĵӻ���ַһ��ʱ����Ϊ��ַƥ�䡣
 * ����ú������ڣ������øú�����ָ�������������ݴ���ķ���
 *  
 *     ���� p_arg ΪӦ�ó����Զ��������
 *
 *     ���� dir ��ʾ���������źź�ĵ�ַ��Ϣ�еķ���λ
 *       - AM_I2C_S_DIR_READ  �ӻ������ݣ����䷽��Ϊ������ -> �ӻ�
 *       - AM_I2C_S_DIR_WRITE �ӻ�д���ݣ����䷽��Ϊ: �ӻ� -> ����
 *
 *     ���� p_isack �����û��趨�Ƿ���ҪӦ��õ�ַƥ�䡣
 *        \li TRUE   Ӧ��
 *        \li FALSE  ��Ӧ��
 *
 * \note ���������£����ǽ��յ���ַƥ�������Ӧ�𣬴�ʱ�������������󣬿���
 *       ���趨�ú��������������Զ��Ե�ַƥ���¼�����Ӧ���źš�
 */
typedef void (*am_i2c_slv_addr_mat_cb_t) (void    *p_arg,
                                          uint8_t  dir,
                                          bool_t  *p_isack);


/** 
 * \brief ������ɻص��������Ͷ���
 * 
 * ����p_argΪӦ�ó����Զ������
 *
 * ���� stat �����˴�����ɵ�״̬
 *    \li  AM_OK     �������
 *    \li -AM_EIO    ���ݴ�����ִ��������ݻ�δ��/д��Ͼ�����ֹͣ/�����źŵȡ�
 *    \li -AM_ETIME  ��ʱ���أ���ʱʱ��������� am_i2c_slv_mkdev() �����á�
 *                    - ��/д���ݻ�δ���ָ�����ֽ���������ʱʱ���ڣ�δ�ȵ�����
 *                      ����������Ӧ������
 *                    - д����ʱ����Ҫ������Ӧ���źţ�����ʱʱ����δ�ȵ�������
 *                      Ӧ���źš�
 *                    - �򵥵�˵���ȴ�I2C���κ��¼���ֻҪ�ȴ�ʱ�䳬����ʱʱ����
 *                      ��ֵ������Ϊ��ʱ��
 *
 * ���� nbytes ������ʵ�ʴ���ɹ����ֽ�������ʹ״̬��ΪAM_OK���Ѿ��ɹ���ɵ���
 * �ݻ�����Ч�ģ��紫���趨Ϊ����10���ֽڣ������յ�5���ֽں������޷����յ�����
 * ����ʱ���أ���ʱ���Ѿ��յ���5���ֽ����ݻ�����Ч�ġ�
 */
typedef void (*am_i2c_slv_trans_complete_cb_t) (void     *p_arg,
                                                int       stat,
                                                uint32_t  nbytes);

/**
 * \brief I2C���ӻ������������ṹ��
 */
struct am_i2c_slv_drv_funcs {
    
    /** \brief ע��һ��I2C�ӻ�            */
    int (*pfn_i2c_slv_register) (void *p_drv, struct am_i2c_slv_dev *p_dev);
    
    /** \brief ȡ����ע���һ��I2C�ӻ�    */
    int (*pfn_i2c_slv_unregister) (void *p_drv, struct am_i2c_slv_dev *p_dev);
};

/**
 * \brief I2C���ӻ�����׼����ṹ�� 
 */
typedef struct am_i2c_slv_serv {
    struct am_i2c_slv_drv_funcs   *p_funcs;   /**< \brief I2C��������         */
    void                          *p_drv;     /**< \brief I2C����������ں��� */
} am_i2c_slv_serv_t;

/** \brief I2C���ӻ�����׼�������������� */
typedef am_i2c_slv_serv_t *am_i2c_slv_handle_t;

/**
 * \brief I2C�ӻ��豸�����Ϣ�ṹ��
 */
typedef struct am_i2c_slv_dev {
    am_i2c_slv_handle_t        handle;          /**< \brief I2C�ӻ�������   */
    uint16_t                   slv_addr;        /**< \brief �ӻ���ַ          */ 
    uint16_t                   addr_flags;      /**< \brief �ӻ���ַ��־      */
    uint32_t                   timeout_ms;      /**< \brief ��ʱʱ��          */
    am_i2c_slv_trans_get_cb_t  pfn_trans_get;   /**< \brief ��ȡ����ص�����  */
    void                      *p_arg_trans_get; /**< \brief �ص���������      */
    am_i2c_slv_addr_mat_cb_t   pfn_matched;     /**< \brief ��ַƥ��ص�����  */
    void                      *p_arg_matched;   /**< \brief �ص���������      */
} am_i2c_slv_dev_t;

/**
 * \brief I2C�ӻ����ݴ���ṹ��
 */
typedef struct am_i2c_slv_transfer {
    uint16_t                  trans_flags;      /**< \brief �����־          */
    uint8_t                  *p_buf;            /**< \brief �������ݻ���ָ��  */
    uint32_t                  length;           /**< \brief ���ݴ��䳤��      */
    
    /** \brief ������ɻص�����  */
    am_i2c_slv_trans_complete_cb_t  pfn_complete_cb;  
    
    /**< \brief �ص�������ڲ���  */
    void                           *p_arg;            
} am_i2c_slv_transfer_t;

/**
 * \brief I2C�ӻ��豸�����Ϣ��������
 *
 *     ��ʹ��I2C�ӻ�ǰ��Ӧʹ�øú������ú�һ���ӻ��豸��Ϣ�������ӻ���ַ�ȡ���
 * �ú���ز����󣬿���ʹ�� am_i2c_slv_register()��I2C��������ע��һ���ӻ��豸��
 *
 * \param[in] p_dev           : ָ��ӻ��豸�Ľṹ���ָ��
 * \param[in] handle          : I2C�ӻ���׼����������
 * \param[in] slv_addr        : �ӻ���ַ
 * \param[in] addr_flags      : �ӻ���ַ��־
 * \param[in] timeout_ms      : ��ʱʱ�䣨��λ: ms����0��ʾ��ʹ�ó�ʱ����
 * \param[in] pfn_trans_get   : ��ȡ�����䡱�ص�����������ΪNULL��
 * \param[in] p_arg_trans_get : ��ȡ�����䡱�ص������Ĳ���  
 * \param[in] pfn_matched     : ��ַƥ��ص���������ΪNULL��
 * \param[in] p_arg_matched   : ��ַƥ��ص���������
 *
 * \return ��
 *
 * \note ���ڳ�ʱʱ�䣬��ʱ���ƿ��Ա���I2C�ӻ�������״��������������ݣ����ͻ�
 * ���գ����ʱ�䳬���˳�ʱʱ��ֵ�����ڳ�ʱʱ���ھ�����Ч���ݣ�����Ϊ��ʱ�¼���
 * δ�յ���ʼ�ź�ʱ��I2C���ڿ���״̬�����ᵼ��I2C��ʱ����ʱ��I2C�ӻ�����λ��
 * ���߼����ص���ʼ̬�������ٴ�ͨ�ţ�����Ҫ�µ������źš�ʵ�ʵĳ�ʱֵ������˴�
 * ���õĳ�ʱֵ���ڲ��죬ע����豸��timeout_msֵ����ӳ��ʵ�ʵĳ�ʱֵ��һ��
 * ����£�10ms���Ҽ��ɡ�
 */
am_static_inline
void am_i2c_slv_mkdev (am_i2c_slv_dev_t           *p_dev,
                       am_i2c_slv_handle_t         handle,
                       uint16_t                    slv_addr,
                       uint16_t                    addr_flags,
                       uint32_t                    timeout_ms,
                       am_i2c_slv_trans_get_cb_t   pfn_trans_get,
                       void                       *p_arg_trans_get,
                       am_i2c_slv_addr_mat_cb_t    pfn_matched,
                       void                       *p_arg_matched)

{
    p_dev->handle          = handle;
    p_dev->slv_addr        = slv_addr;
    p_dev->addr_flags      = addr_flags;
    p_dev->timeout_ms      = timeout_ms;
    p_dev->pfn_trans_get   = pfn_trans_get;
    p_dev->p_arg_trans_get = p_arg_trans_get;
    p_dev->pfn_matched     = pfn_matched;
    p_dev->p_arg_matched   = p_arg_matched;
}

/**
 * \brief I2C�ӻ�����ṹ����Ϣ��������
 *
 * \param[in] p_trans      : ָ����ṹ���ָ��
 * \param[in] addr         : �����Ĵӻ��豸��ַ
 * \param[in] flags        : ��������еĿ��Ʊ�ʶλ������������Ʊ�־��ֵΪ0
 * \param[in] p_buf        : ָ���ͻ��߽������ݻ���
 * \param[in] nbytes       : ��ǰ���������/��ַ����
 * \param[in] pfn_callback : ������ɻص�����
 * \param[in] p_arg        : �ص�������ڲ���
 *
 * \retval  AM_OK     : ����ṹ������������
 * \retval -AM_EINVAL : ��������
 */
am_static_inline
void am_i2c_slv_mktrans (am_i2c_slv_transfer_t           *p_trans, 
                         uint16_t                         flags, 
                         void                            *p_buf, 
                         uint32_t                         nbytes,
                         am_i2c_slv_trans_complete_cb_t   pfn_callback,
                         void                            *p_arg)
{ 
    p_trans->trans_flags     = flags;
    p_trans->p_buf           = p_buf; 
    p_trans->length          = nbytes;
    p_trans->pfn_complete_cb = pfn_callback; 
    p_trans->p_arg           = p_arg;
}

/**
 * \brief ע��һ��I2C�ӻ�
 *
 *     һ��I2C�����������ܹ��ṩ���I2C�ӻ����񣬴�ʱ����ʹ�øú���ע����I2C
 * �ӻ���Ҫȷ��ÿ��I2C�ӻ���ַ����Ψһ�ġ���ע��һ���ӻ��豸ǰ�����ȷ���Ѿ�ʹ��
 * am_i2c_slv_mkdev()��ʼ�����豸��ص���Ϣ��
 *
 * \param[in] p_dev    : ָ��ӻ��豸�Ľṹ���ָ��
 *
 * \retval  AM_OK      : �ӻ�ע��ɹ�����������ʹ��
 * \retval -AM_EINVAL  : ��������
 * \retval -AM_ENOTSUP : �ӻ�ĳЩ���Բ�֧�֣����е��豸��֧��10-bit��ַģʽ
 * \retval -AM_EPERM   : ���������������õ�ַ�Ѿ����ڣ������ظ�ע��
 * \retval -AM_EFULL   : �ӻ�ע�����������豸ֻ֧��4���ӻ�����ע���5���ӻ�ʱ
 *                       ���᷵�ظ�ֵ������ʹ�� am_i2c_slv_unregister()ȡ��һЩ
 *                       ���õĴӻ��豸��������ע�ᡣ
 *
 * \note �ɹ�ע��󣬽��ں��ʵ�ʱ��������Ӧ�Ļص�������
 */
am_static_inline
int am_i2c_slv_register (am_i2c_slv_dev_t *p_dev)
{
    return p_dev->handle->p_funcs->pfn_i2c_slv_register(p_dev->handle->p_drv,
                                                        p_dev);
}

/**
 * \brief ȡ��һ���Ѿ�ע���I2C�ӻ�
 *
 *     һ��I2C�����������ܹ��ṩ���I2C�ӻ����񣬴�ʱ����ʹ�øú���ע����I2C
 * �ӻ���Ҫȷ��ÿ��I2C�ӻ���ַ����Ψһ�ġ�
 *
 * \param[in] p_dev   : ָ��ӻ��豸��Ϣ�Ľṹ���ָ��
 *
 * \retval  AM_OK      : ȡ���ɹ�
 * \retval -AM_EINVAL  : ��������
 * \retval -AM_EPERM   : �����������ôӻ���δע��
 */
am_static_inline
int am_i2c_slv_unregister (am_i2c_slv_dev_t *p_dev)
{
    return p_dev->handle->p_funcs->pfn_i2c_slv_unregister(p_dev->handle->p_drv,
                                                          p_dev);
}

/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_I2C_SLV_H */

/* end of file */
