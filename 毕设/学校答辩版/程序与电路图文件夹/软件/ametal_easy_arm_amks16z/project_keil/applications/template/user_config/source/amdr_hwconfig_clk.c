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
 * \brief KS16XX CLK �û������ļ���
 * \sa amdr_hwconfig_clk.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-22  lnk, first implementation.
 * \endinternal
 */

#include "amdr_hwconfig_clk.h"

/**
 * \addtogroup amdr_if_hwconfig_src_clk
 * \copydoc amdr_hwconfig_clk.c
 * @{
 */

/** \brief ʱ�ӹ���ģʽ
 *
 *   �ɶ���ΪAMDR_CLK_MODE_FEI �� AMDR_CLK_MODE_PEE
 *
 *  - FEI mode: ʹ���ڲ����� IRC(32.768KHz)��FLL�ο�ʱ�� ����FLL��Ƶ������ʱ�ӣ�
 *              Core clock = 43.97MHz��
 *              Bus clock  = 23.97152MHz
 *
 *  - PEE mode: ʹ���ⲿʱ��XTAL��PLL�ο�ʱ�� ����PLL��Ƶ������ʱ�ӣ�
 *              Core clock = 48MHz��
 *              Bus clock  = 24Hz
 *
 */
#define __CLK_MODE    AMDR_CLK_MODE_FEI


/** \brief CLK�豸��Ϣ     */
const amdr_clk_devinfo_t g_clk_devinfo = {
    
    /* ʱ�ӹ���ģʽ */
    __CLK_MODE,

    /* AMDR_CLK_MODE_PEEģʽ���ⲿ����Ƶ�ʣ�����8-32MHz, 4�������� */
    (8*1000*1000)
};

/** \brief CLK�豸ʵ��   */
amdr_clk_dev_t g_clk_dev;

/** \brief CLKƽ̨��ʼ�� */
void amhw_plfm_clk_init (void)
{
    /** \brief  �����ⲿ����OSC���Ź��� */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_PORTA);
    amhw_port_pin_func_cfg(AMHW_PORT,PIOA_18,PIOA_18_EXTAL0);
    amhw_port_pin_func_cfg(AMHW_PORT,PIOA_19,PIOA_19_XTAL0);
}

/* \brief CLK ƽ̨ȥ��ʼ�� */
void amhw_plfm_clk_deinit (void)
{
    /* ʱ����ȥ��ʼ������*/
    return;
}

/**
 * @}
 */

/* end of file */
