/*******************************************************************************
*                                 AMeatal
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
 * \brief PMU�����ӿ�����
 *
 *  1. ϵͳģʽ�л���
 *  2. ���������ã�
 *  3. LLWU����Դ���ü�LLWU�ж����ӣ�
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-28  liqing, first implementation.
 * \endinternal
 */

#ifndef __AMDR_PMU_H
#define __AMDR_PMU_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "am_types.h"
#include "amhw_pmc.h"
#include "amhw_smc.h"
#include "amhw_rcm.h"
#include "amhw_llwu.h"


/**
 * \addtogroup amdr_if_pmu
 * \copydoc amdr_pmu.h
 * @{
 */

/**
 * \name PMU�豸��ʼ������
 *  
 * ���ڳ�ʼ���豸��Ϣ��flags����
 * @{
 */

/*
 *  bit[ 0 ] : ��λ����stopģʽLPO�˲�ʹ��
 *  bit[1-5] : ��λ����run��waitģʽ�˲�ʱ�Ӹ�����BUSʱ�ӣ���0-32��
 *  bit[ 6 ] : VLPxģʽ�Ƿ�ʹ���ܴ�϶
 *  bit[ 7 ] : �Ƿ�ʹ���ܴ�϶����
 *  bit[ 8 ] : VLSS0ģʽ�Ƿ����POR
 */

/** \brief ��λ����stopģʽLPO�˲�ʹ�� */
#define AMDR_PMU_ARG_REST_PIN_STOP_FILT_ENABLE    AM_BIT(0)

/** \brief ��λ����runģʽ�˲�ʱ������ */
#define AMDR_PMU_ARG_REST_PIN_RUN_FILT_C(count)   AM_SBF((count),1

/** \brief VLPxģʽ�ܴ�϶ʹ�� */
#define AMDR_PMU_ARG_VLPx_BANDGAP_ENABLE          AM_BIT(6)

/** \brief �ܴ�϶�����Ƿ�ʹ�� */
#define AMDR_PMU_ARG_BANDGAP_BUF_ENABLE           AM_BIT(7)

/** \brief VLSS0ģʽ����POR�����͹��� */
#define AMDR_PMU_ARG_VLLS0_POR_DISABLE            AM_BIT(8)

/** \brief �Ƿ����LLWU���Ѹ�λ��ACK��־(����������ͷ�) */
#define AMDR_PMU_ARG_VLLSx_ISOACK_CLEAR           AM_BIT(9)

/** @}*/

/**
 * \brief PMU �豸��Ϣ�ṹ��
 */
typedef struct amdr_pmu_devinfo {
    amhw_smc_t       *p_hw_smc;   /**< \brief ָ��SMC�Ĵ������ָ��  */
    amhw_pmc_t       *p_hw_pmc;   /**< \brief ָ��PMC�Ĵ������ָ��  */
    amhw_rcm_t       *p_hw_rcm;   /**< \brief ָ��RCM�Ĵ������ָ��  */
    amhw_llwu_t      *p_hw_llwu;  /**< \brief ָ��LLWU�Ĵ������ָ�� */
    uint32_t          flags;      /**< \brief ��ʼ�����ò���         */
} amdr_pmu_devinfo_t;

/**
 * \brief PMU �豸�ṹ��
 */
typedef struct amdr_pmu_dev {

    /** \brief ָ��PMU�豸��Ϣָ�� */
    const amdr_pmu_devinfo_t *p_devinfo;

    /** \brief �����жϻص�����ָ�� */
    am_pfnvoid_t              p_warn_func;

    /** \brief �����жϻص��������� */
    void                     *p_warn_arg;
} amdr_pmu_dev_t;

/**
 * \brief PMU ϵͳģʽ
 *
 *        RUN <--> VPLRʱ��ģʽ�л���PEE <--> BLPE
 *                                   FEI <--> BLPI
 */
typedef enum amdr_pmu_mode {
    AMDR_PMU_MODE_RUN = 0,       /**< \brief ����ģʽ          */
    AMDR_PMU_MODE_VLPR,          /**< \brief ���͹�������ģʽ  */
    AMDR_PMU_MODE_WAIT,          /**< \brief �ȴ�ģʽ          */
    AMDR_PMU_MODE_VLPW,          /**< \brief ���͹��ĵȴ�ģʽ  */
    AMDR_PMU_MODE_STOP2,         /**< \brief ֹͣģʽ2         */
    AMDR_PMU_MODE_STOP1,         /**< \brief ֹͣģʽ1         */
    AMDR_PMU_MODE_STOP,          /**< \brief ����ֹͣģʽ      */
    AMDR_PMU_MODE_VLPS,          /**< \brief ���͹���ֹͣģʽ  */
    AMDR_PMU_MODE_LLS,           /**< \brief ��©��ֹͣģʽ    */
    AMDR_PMU_MODE_VLLS3,         /**< \brief ����©��ֹͣģʽ3 */
    AMDR_PMU_MODE_VLLS1,         /**< \brief ����©��ֹͣģʽ1 */
    AMDR_PMU_MODE_VLLS0          /**< \brief ����©��ֹͣģʽ0 */
} amdr_pmu_mode_t;

/** \brief ���籨����ѹ��ֵ��������ѹ���ڸ�λ��ѹ�� */
typedef enum amhw_pmu_warn_v_sel {
    AMDR_PMU_WARN_V1 = 0,              /**< \brief ѡ�񱨾���ֵ1(1.8v)\(2.7V) */
    AMDR_PMU_WARN_V2,                  /**< \brief ѡ�񱨾���ֵ2(1.9V)\(2.8V) */
    AMDR_PMU_WARN_V3,                  /**< \brief ѡ�񱨾���ֵ3(2.0V)\(2.9V) */
    AMDR_PMU_WARN_V4                   /**< \brief ѡ�񱨾���ֵ4(2.1V)\(3.0V) */
} amhw_pmu_warn_v_sel_t;

/** \brief ���縴λ��ѹ��ֵ */
typedef enum amdr_pmu_rest_v_sel {
    AMDR_PMU_REST_VL = 0,              /**< \brief ѡ�����ֵ���(1.6V ) */
    AMDR_PMU_REST_VH                   /**< \brief ѡ�����ֵ���(2.56V) */
} amdr_pmu_rest_v_sel_t;

/** \brief PMU ������Ϣ���� */
typedef struct amdr_pmu_brownout_info {
    amhw_pmu_warn_v_sel_t  warn_v;      /**< \brief ������ѹѡ�� */
    amdr_pmu_rest_v_sel_t  rest_v;      /**< \brief ��λ��ѹѡ�� */
    am_pfnvoid_t           p_callfunc;  /**< \brief �����ص����� */
    void                  *p_arg;       /**< \brief �ص�����     */
    bool_t                 enable;      /**< \brief �Ƿ�ʹ��     */
} amdr_pmu_brownout_info_t;

/** \brief LLWU����Դ��Ϣ
 *  
 * Ӧʹ�ú���amdr_pmu_llwu_src_x_mkinfo()��ʼ���ýṹ��
 */
typedef struct amdr_pmu_llwu_src_info {
    uint32_t src;         /**< \brief ����Դ         */
    uint32_t extra_info;  /**< \brief ����Դ������Ϣ */
} amdr_pmu_llwu_src_info_t;

/**
 * \brief PMU��ʼ��
 *
 * \param[in] p_dev     : ָ��PMU�豸�ṹ���ָ��
 * \param[in] p_devinfo : ָ��PMU�豸��Ϣ�ṹ�峣����ָ��
 *
 * \retval AM_OK      : ��ʼ���ɹ�
 * \retval -AM_EINVAL : ��������
 */
int amdr_pmu_init (amdr_pmu_dev_t *p_dev, const amdr_pmu_devinfo_t *p_devinfo);

/**
 * \brief PMUȥ��ʼ��
 * \param[in] p_dev     : ָ��PMU�豸�ṹ���ָ��
 * \return ��
 */
void amdr_pmu_deinit (amdr_pmu_dev_t *p_dev);

/**
 * \brief ����ϵͳģʽ
 *
 *  1.ģʽ�л����������ģ��磺VLPR��WAIT��Ӧ���л���RUNģʽ�����л���WAITģʽ\n
 *  2.���ģʽ�л��ȴ���ֹͣģʽ�����ڴ˺����ڲ�ֹͣ���Ǹ�λ���Ѻ���˳�
 *    ������AM_OK;\n
 *  3.���ģʽ�л����ɹ���������󣬸ú����᷵��-AM_EINVAL��AM_ERROR��
 *    ����ʱ���ö����ж���\n
 *  4.VLLSxģʽ���Ѻ�ִ�и�λ������
 *
 * \param[in] mode : ϵͳģʽ����
 *
 * \retval AM_OK      ��ģʽ�л���Ч
 * \retval AM_ERROR   ��ģʽ�л�����
 * \retval -AM_EINVAL ��ģʽ��������
 */
int amdr_pmu_mode_into (amdr_pmu_mode_t mode);

/**
 * \brief ��ȡϵͳģʽ
 * \retval AMDR_PMU_MODE_RUN  : ����ģʽ
 * \retval AMDR_PMU_MODE_VLPR : �͹�������ģʽ
 */
amdr_pmu_mode_t amdr_pmu_mode_get (void);

/**
 * \brief ���õ�������Ϣ
 *
 * \param[in] p_info : ָ�������Ϣ�ṹ��ָ��
 *
 * \retval AM_OK      : ���óɹ�
 * \retval -AM_EINVAL : ��������
 * \retval AM_ERROR   : ����ʧ��
 *
 * \note VLPx,LLS��VLLSxģʽ��֧�ֵ����⣬����AM_ERROR
 */
int amdr_pmu_brownout_cfg (amdr_pmu_brownout_info_t *p_info);

/**
 * \brief �����������Ϣ�ṹ��
 *
 * \param[in] p_info : ָ�������Ϣ�ṹ��ָ��
 * \param[in] warn_v : ������ѹ
 * \param[in] rest_v : ��λ��ѹ��������ѹ���ڸ�λ��ѹ��
 * \param[in] p_func : �����ص�����
 * \param[in] p_arg  : �ص���������
 * \param[in] enable : �Ƿ�ʹ��
 *      \arg TURE  : ���������жϺ͸�λʹ��
 *      \arg FALSE : ��ʹ�õ����⹦��
 *
 * \retval AM_OK ����ɹ�
 */
am_static_inline
int amdr_pmu_brownout_mkinfo (amdr_pmu_brownout_info_t *p_info,
                              amhw_pmu_warn_v_sel_t     warn_v,
                              amdr_pmu_rest_v_sel_t     rest_v,
                              am_pfnvoid_t              p_func,
                              void                     *p_arg,
                              bool_t                    enable)
{
    p_info->warn_v     = warn_v;
    p_info->rest_v     = rest_v;
    p_info->p_callfunc = p_func;
    p_info->p_arg      = p_arg;
    p_info->warn_v     = warn_v;

    return AM_OK;
}

/**
 * \brief ����LLWU����Դ��Ϣ
 * \param[in] p_info : ָ������Ϣ�ṹ��ָ��
 * \retval AM_OK ����ɹ�
 */
int amdr_pmu_llwu_src_cfg (amdr_pmu_llwu_src_info_t *p_info);
        
/**
 * \brief �������ŵĻ�����Ϣ�ṹ��
 *
 * \param[in] p_info : ָ������Ϣ�ṹ��ָ��
 * \param[in] pin    : ��������
 * \param[in] arg    : ���Ų���
 *
 * \retval AM_OK ����ɹ�
 *
 * \note ��������Ӧ����Ϊ����ģʽ���ܲ�������
 */
am_static_inline
int amdr_pmu_llwu_src_pin_mkinfo (amdr_pmu_llwu_src_info_t *p_info,
                                  amhw_llwu_pin_t           pin,
                                  amhw_llwu_pin_arg_t       arg)
{
    p_info->src        = pin | AM_SBF(0x00, 24);
    p_info->extra_info = arg;

    return AM_OK;
}

/**
 * \brief ��������ģ��Ļ�����Ϣ�ṹ��
 *
 * \param[in] p_info : ָ������Ϣ�ṹ��ָ��
 * \param[in] module : ����ģ��
 * \param[in] arg    : ģ�����
 *
 * \retval AM_OK ����ɹ�
 */
am_static_inline
int amdr_pmu_llwu_src_module_mkinfo (amdr_pmu_llwu_src_info_t *p_info,
                                     amhw_llwu_module_t        module,
                                     amhw_llwu_module_arg_t    arg)
{
    p_info->src        = module | AM_SBF(0x01, 24);
    p_info->extra_info = arg;

    return AM_OK;
}

/**
 * \brief �����˲�ͨ���Ļ�����Ϣ�ṹ��
 *
 * \param[in] p_info : ָ������Ϣ�ṹ��ָ��
 * \param[in] chan   : �˲�ͨ��
 * \param[in] pin    : ��������
 * \param[in] arg    : ͨ������
 *
 * \retval AM_OK ����ɹ�
 */
am_static_inline
int amdr_pmu_llwu_src_filt_mkinfo (amdr_pmu_llwu_src_info_t *p_info,
                                   amhw_llwu_filt_chan_t     chan,
                                   amhw_llwu_pin_t           pin,
                                   amhw_llwu_filt_chan_arg_t arg)
{
    p_info->src         = chan | AM_SBF(0x02, 24);
    p_info->extra_info  = pin;
    p_info->extra_info |= arg << 16;

    return AM_OK;
}

/**
 * \brief LLWU�жϴ�����
 *
 * ��ȡ����Դ��־���������Դ��־���ú������жϵ���
 *
 * \retval ��
 */
void amdr_pmu_llwu_handle (void);

/** 
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMDR_PMU_H */

/*end of file */
