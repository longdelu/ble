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
 * \brief TPM驱动，服务PWM标准接口
 *
 * 1. TPM支持提供如下三种标准服务，本驱动提供的是服务PWM标准服务的驱动。
 *     - 定时
 *     - PWM输出
 *     - 捕获
 * 2. 在本驱动中，TPM相关资源使用如下：
 *    - 匹配寄存器0用于产生PWM周期，与事件0相关联
 *    - 匹配寄存器1用于产生PWM输出通道0(TPM_OUT0)的脉宽
 *    - 匹配寄存器2用于产生PWM输出通道1(TPM_OUT1)的脉宽
 *    - 匹配寄存器3用于产生PWM输出通道2(TPM_OUT2)的脉宽
 *    - 匹配寄存器4用于产生PWM输出通道3(TPM_OUT3)的脉宽
 *    - 匹配寄存器5用于产生PWM输出通道4(TPM_OUT4)的脉宽
 *    - 匹配寄存器6用于产生PWM输出通道5(TPM_OUT5)的脉宽
 *    - 匹配寄存器7用于产生PWM输出通道6(TPM_OUT6)的脉宽
 *    - 匹配寄存器8用于产生PWM输出通道7(TPM_OUT7)的脉宽
 *
 * \note 一个TPM输出的所有PWM共享周期值，也就是说，该TPM输出的所有PWM周期相同，
 *       频率相同
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-21  cuishengen, first implementation.
 * \endinternal
 */


#ifndef __AMDR_TPM_PWM_H
#define __AMDR_TPM_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_pwm.h"
#include "amhw_tpm.h"

/**
 * \addtogroup amdr_if_tpm_pwm
 * \copydoc amdr_tpm_pwm.h
 * @{
 */

/**
 * \brief TPM PWM输出功能相关的GPIO信息
 */
typedef struct amdr_tpm_pwm_ioinfo {
    uint32_t gpio;              /**< \brief PWM 所用的GPIO                    */
    uint32_t func;              /**< \brief PWM功能的GPIO功能设置值           */
    uint32_t dfunc;             /**< \brief 禁能PWM模式后，默认GPIO功能设置值 */
} amdr_tpm_pwm_ioinfo_t;

/**
 * \brief TPM PWM输出功能相关的设备信息
 */
typedef struct amdr_tpm_pwm_devinfo {
    amhw_tpm_t            *p_hw_tpm;     /**< \brief 指向TPM寄存器块的指针    */
    uint8_t                channels_num; /**< \brief 使用的通道数，最大为8    */
    amdr_tpm_pwm_ioinfo_t *p_ioinfo;     /**< \brief 指向PWM输出管脚信息结构体*/
} amdr_tpm_pwm_devinfo_t;
 
/**
 * \brief TPM PWM输出功能设备结构体
 */
typedef struct amdr_tpm_pwm_dev {
    
    am_pwm_serv_t                 pwm_serv; /**< \brief 标准PWM服务           */
    
    /** \brief 指向TPM(PWM输出功能)设备信息常量的指针 */
    const amdr_tpm_pwm_devinfo_t  *p_devinfo;

} amdr_tpm_pwm_dev_t;

/**
 * \brief 初始化TPM为PWM输出功能
 *
 * \param[in] p_dev     : 指向TPM(PWM输出功能)设备的指针
 * \param[in] p_devinfo : 指向TPM(PWM输出功能)设备信息常量的指针
 *
 * \return PWM标准服务操作句柄，值为NULL时表明初始化失败
 */
am_pwm_handle_t amdr_tpm_pwm_init(amdr_tpm_pwm_dev_t           *p_dev,
                                  const amdr_tpm_pwm_devinfo_t *p_devinfo);

/**
 * \brief 不使用TPM PWM输出功能时，解初始化TPM PWM输出功能，释放相关资源
 * \param[in] p_dev : 指向TPM(PWM输出功能)设备的指针
 * \return 无
 */
void amhw_tpm_pwm_deinit(amdr_tpm_pwm_dev_t *p_dev);

/** 
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMDR_TPM_PWM_H */

/* end of file */
