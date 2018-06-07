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
 * \brief CLK������ʱ�ӱ�׼����ӿ�ʵ��
 *
 * \internal
 * \par Modification history
 * - 1.00 15-12-01  lnk, first implementation.
 * \endinternal
 */
 
#ifndef __AMDR_CLK_H
#define __AMDR_CLK_H

#include "ametal.h"
#include "am_clk.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup amdr_if_clk
 * \copydoc amdr_clk.h
 * @{
 */


/**
  * \brief ʱ�ӹ���ģʽ
  */
typedef enum amdr_clk_mode {
    AMDR_CLK_MODE_FEI   = 0,   /**< \brief FEI  */
    AMDR_CLK_MODE_FBI   = 1,   /**< \brief FBI  */
    AMDR_CLK_MODE_BLPI  = 2,   /**< \brief BLPI */
    AMDR_CLK_MODE_FEE   = 3,   /**< \brief FEE  */
    AMDR_CLK_MODE_FBE   = 4,   /**< \brief FBE  */
    AMDR_CLK_MODE_BLPE  = 5,   /**< \brief BLPE */
    AMDR_CLK_MODE_PBE   = 6,   /**< \brief PBE  */
    AMDR_CLK_MODE_PEE   = 7,   /**< \brief PEE  */
    AMDR_CLK_MODE_STOP  = 8,   /**< \brief STOP  */
    AMDR_CLK_MODE_ERROR = 9    /**< \brief ERROR  */
} amdr_clk_mode_t;

/**
 * \brief CLK �豸��Ϣ�����ṹ��
 */

typedef struct amdr_clk_devinfo {
    /* ʱ�ӹ���ģʽ */
    const amdr_clk_mode_t clk_mode;

    /* �ⲿ����Ƶ�� */
    const uint32_t xtal_rate;

} amdr_clk_devinfo_t;

/**
 * \brief CLK �豸�ṹ��
 */
typedef struct amdr_clk_dev {

    /** \brief ָ��CLK �豸��Ϣ��ָ��   */
    const amdr_clk_devinfo_t *p_devinfo;

    /* PLL��Ƶϵ���� ʹ��PLL ����Ƶ����2-4MHz ��*/
    uint8_t pll_div;

    /* PLL��Ƶϵ����MCGPLLCLK = 2MHz * (24 + pll_div), MCGPLLCLK upto 100MHz*/
    uint8_t pll_mul;

    /* FLL��Ƶϵ����ʹ��FLL ����Ƶ����32.768K ���� */
    uint8_t fll_div;

    /* FLL��ƵƵ�ʣ�MCGFLLCLK = 24MHz��48MHz��72MHz��96MHz */
    uint32_t fll_rate;

    /* �ں�ʱ��core_clk��Ƶϵ��(1-16),
     * core_clk = MCGOUTCLK / outdiv1 ,core_clk upto 48MHz
     */
    uint8_t outdiv1;
    /* ����ʱ��bus_clk��Ƶϵ��(2-8)��
     * bus_clk = core_clk / outdiv4, bus_clk upto 24MHz
     */
    uint8_t outdiv4;
} amdr_clk_dev_t;

/**
 * \brief CLK ��ʼ��
 *
 * \param[in] p_dev     : ָ��CLK�豸��ָ��
 * \param[in] p_devinfo : ָ��CLK�豸��Ϣ��ָ��
 *
 * \retval AM_OK : �����ɹ�
 */
int amdr_clk_init (amdr_clk_dev_t           *p_dev,
                   const amdr_clk_devinfo_t *p_devinfo);

/**
 * \brief �õ�core/platform/systemʱ��Ƶ��
 * \return core/platform/systemʱ��Ƶ��
 */
uint32_t amdr_clk_core_clkrate_get(void);

/**
 * \brief �õ�bus/flashʱ��Ƶ��
 * \return bus/flashʱ��Ƶ��
 */
uint32_t amdr_clk_bus_clkrate_get(void);

/**
 * \brief �õ����������Ƶ��
 *
 * \param[in] p_periph : ָ������ļĴ�����
 *
 * \return ���������Ƶ��
 */
uint32_t amdr_clk_periph_rate_get(void *p_periph);
/**
 * @} 
 */

/**
 * \brief ��ȡʱ�ӹ���ģʽ
 */
amdr_clk_mode_t amdr_clk_mode_get (void);

/**
 * \brief ����ʱ�ӹ���ģʽPEE��BLPEģʽ���͹���ģʽ��
 * \return ��
 * \note BLPEģʽ��CORE_CLK = 4MHZ
 *                 BUS_CLK = 1MHZ
 */
int amdr_clk_mode_pee2blpe (void);

/**
 * \brief ����ʱ�ӹ���ģʽBLPE��PEEģʽ
 * \return ��
 * \note PEEģʽ,CORE_CLK = 48MHZ
 *               BUS_CLK = 24MHZ
 */
int amdr_clk_mode_blpe2pee (void);

/**
 * \brief ����ʱ�ӹ���ģʽFEI��BLPIģʽ���͹���ģʽ��
 * \return ��
 * \note BLPIģʽ,
 *       Core clock = 4MHz
 *       Bus clock  = 1MHz
 */
int amdr_clk_mode_fei2blpi (void);

/**
 * \brief ����ʱ�ӹ���ģʽBLPI��FEIģʽ
 * \return ��
 * \note FBIģʽ
 *       Core clock = 20.97152MHz
 *       Bus clock  = 20.97152MHz
 */
int amdr_clk_mode_blpi2fei (void);


/**
 * \brief ��ȡFLL CLK��ʱ��
 * \return ��
 * \note FEIģʽ
 *       fll clock = 20.97152MHz

 */
uint32_t amdr_clk_mcgfllclk_rate_get (void) ;

/**
 * \brief ��ȡ MCG CLKOUT ��ʱ��
 * \return ��
 * \note FBIģʽ
 *       MCG CLKOUT = 20.97152MHz

 */
uint32_t amdr_clk_mcgoutclk_rate_get (void);

#ifdef __cplusplus
}
#endif

#endif /* __AMDR_CLK_H */

/* end of file */
