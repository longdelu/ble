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
* e-mail:      apollo.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief MCMģ���Ӳ����ӿ�
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-16  zsp, first implementation.
 * \endinternal
 */

#ifndef __AMHW_MCM_H
#define __AMHW_MCM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"

/**
 * \addtogroup amhw_if_mcm
 * \copydoc amhw_mcm.h
 * @{
 */

#define AMHW_MCM_ESFC       (1UL << 16)  /**< \brief FLASHæ��ʱ��־λ      */

#define AMHW_MCM_CFCC       (1UL << 10)  /**< \brief FLASH���ٻ������λ    */

#define AMHW_MCM_ARB        (1UL << 9 )  /**< \brief �ٲ÷�ʽѡ��λ         */

#define AMHW_MCM_CPOWOI     (1UL << 2 )  /**< \brief �жϻ��Ѽ������ʹ��λ */

#define AMHW_MCM_CPOACK     (1UL << 1 )  /**< \brief �������Ӧ���־λ     */

#define AMHW_MCM_CPOREQ     (1UL << 1 )  /**< \brief ������������־λ     */

/**
 *  \name �ٲ�ģʽ
 *  @{
 */
#define FIX_PRIO            (0)          /**< \brief �̶�����Ȩ */
#define ROUND_ROBIN         (1)          /**< \brief ѭ��ģʽ   */
/**
 *  @}
 */

/**
 *  \name Ԥ�⻺��ģʽ
 *  @{
 */
#define SEECULATE_CODE      (0)         /**< \brief Ԥ�⻺�����       */
#define SPECULATE_CODE_DATA (1)         /**< \brief Ԥ�⻺���������� */
#define SPECULATE_NONE      (2)         /**< \brief ��Ԥ�⻺��         */
/**
 *  @}
 */

/**
 *  \brief MCM �Ĵ����ṹ�嶨��
 */
typedef struct amhw_mcm {
    __I  uint32_t reserve0[2];      /**< \brief ����                        */
    __I  uint16_t plasc;            /**< \brief ���濪�شӻ����üĴ��� (RO) */
    __I  uint16_t plamc;            /**< \brief ���濪���������üĴ��� (RO) */
    __IO uint32_t placr;            /**< \brief ƽ̨���ƼĴ���              */
    __I  uint32_t reserve1[51];     /**< \brief ����                        */
    __IO uint32_t cpo;              /**< \brief ��������Ĵ���              */
} amhw_mcm_t;

/**
 *  \brief ��ȡ����axbs���������
 *
 *  \param p_hw_mcm ָ��MCMģ��Ĵ�����ָ��
 *  \return ��
 */
am_static_inline
uint16_t amhw_mcm_axbs_slave_config_get (amhw_mcm_t *p_hw_mcm)
{
    return p_hw_mcm->plasc & 0xfful;
}

/**
 *  \brief ��ȡ����axbs���������
 *
 *  \param p_hw_mcm ָ��MCMģ��Ĵ�����ָ��
 *  \return ��
 */
am_static_inline
uint16_t amhw_mcm_axbs_master_config_get (amhw_mcm_t *p_hw_mcm)
{
    return p_hw_mcm->plamc & 0xfful;
}

/**
 *  \brief �ر�flashæʱ��ʱ
 *
 *  \param p_hw_mcm ָ��MCMģ��Ĵ�����ָ��
 *  \return ��
 */
am_static_inline
void amhw_mcm_flash_stalling_enable (amhw_mcm_t *p_hw_mcm)
{
    p_hw_mcm->placr |= AMHW_MCM_ESFC;
}

/**
 *  \brief �ر�flashæʱ��ʱ
 *
 *  \param p_hw_mcm ָ��MCMģ��Ĵ�����ָ��
 *  \return ��
 */
am_static_inline
void amhw_mcm_flash_stalling_disable (amhw_mcm_t *p_hw_mcm)
{
    p_hw_mcm->placr &= ~AMHW_MCM_ESFC;
}

/**
 *  \brief ����FLASHԤ�⻺��
 *
 *  \param p_hw_mcm ָ��MCMģ��Ĵ�����ָ��
 *  \param config ����ģʽ
 *      - SEECULATE_CODE:      ����ָ��Ԥ�⻺�浫�ر�����Ԥ�⻺��
 *      - SPECULATE_CODE_DATA: ����ָ�������Ԥ�⻺��
 *      - SPECULATE_NONE:      �ر�Ԥ�⻺��
 *  \return ��
 */
am_static_inline
void amhw_mcm_speculation_buffer_config (amhw_mcm_t *p_hw_mcm, uint32_t config)
{
    p_hw_mcm->placr = (((p_hw_mcm->placr &
                      (~(0x3 << 14)))))  |
                      (config << 14);
}

/**
 *  \brief ����FLASH���ٻ���
 *
 *  \param p_hw_mcm ָ��MCMģ��Ĵ�����ָ��
 *  \param config ����ģʽ
 *      - 0: ����ָ�������
 *      - 1: ����ָ�����������
 *      - 2: ������ָ���������
 *      - 3: ָ������ݶ�������
 *      - 4: �رջ���
 *  \return ��
 */
am_static_inline
void amhw_mcm_cache_config (amhw_mcm_t *p_hw_mcm, uint32_t config)
{
    p_hw_mcm->placr = ((p_hw_mcm->placr &
                      (~(0x7 << 11))))  |
                      (config << 11);
}

/**
 *  \brief ������ٻ���(cache)
 *
 *  \param p_hw_mcm ָ��MCMģ��Ĵ�����ָ��
 *  \return ��
 */
am_static_inline
void amhw_mcm_cache_clear (amhw_mcm_t *p_hw_mcm)
{
    p_hw_mcm->placr |= AMHW_MCM_CFCC;
}

/**
 *  \brief ѡ�񽻲濪�������ٲ÷�ʽ
 *
 *  \param p_hw_mcm ָ��MCMģ��Ĵ�����ָ��
 *  \param sel �ٲ÷�ʽ
 *          - FIX_PRIO �̶����ȼ�
 *          - ROUND_ROBIN ѭ��ģʽ
 *  \return ��
 */
am_static_inline
void amhw_mcm_arbiration_select (amhw_mcm_t *p_hw_mcm, uint8_t sel)
{
    p_hw_mcm->placr = ((p_hw_mcm->placr &
                      (~AMHW_MCM_ARB))) |
                      (sel << 9);
}

/**
 *  \brief �жϻ��Ѽ������ʹ��
 *
 *  \param p_hw_mcm ָ��MCMģ��Ĵ�����ָ��
 *  \return ��
 */
am_static_inline
void amhw_mcm_compute_operation_enable (amhw_mcm_t *p_hw_mcm)
{
    p_hw_mcm->cpo |= AMHW_MCM_CPOWOI;
}

/**
 *  \brief �жϻ��Ѽ����������
 *
 *  \param p_hw_mcm ָ��MCMģ��Ĵ�����ָ��
 *  \return ��
 */
am_static_inline
void amhw_mcm_compute_operation_disable (amhw_mcm_t *p_hw_mcm)
{
    p_hw_mcm->cpo &= ~AMHW_MCM_CPOWOI;
}

/**
 *  \brief �������״̬��ȡ
 *
 *  \param p_hw_mcm ָ��MCMģ��Ĵ�����ָ��
 *
 *  \retval 0: �������δ������Խ���
 *  \retval AMHW_MCM_CPOACK: �ѽ��������������δ����
 */
am_static_inline
uint8_t amhw_mcm_compute_status_get (amhw_mcm_t *p_hw_mcm)
{
    return p_hw_mcm->cpo & AMHW_MCM_CPOACK;
}

/**
 *  \brief �������״̬����״̬��ȡ
 *
 *  \param p_hw_mcm ָ��MCMģ��Ĵ�����ָ��
 *
 *  \retval 0: �������
 *  \retval AMHW_MCM_CPOREQ: ����������
 */
am_static_inline
uint8_t amhw_mcm_compute_request_get (amhw_mcm_t *p_hw_mcm)
{
    return p_hw_mcm->cpo & AMHW_MCM_CPOREQ;
}

/**
 *  \brief ����������
 *
 *  \param p_hw_mcm ָ��MCMģ��Ĵ�����ָ��
 *
 *  \return ��
 */
am_static_inline
void amhw_mcm_compute_request (amhw_mcm_t *p_hw_mcm)
{
    p_hw_mcm->cpo |= AMHW_MCM_CPOREQ;
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /** __AMHW_MCM_H */
