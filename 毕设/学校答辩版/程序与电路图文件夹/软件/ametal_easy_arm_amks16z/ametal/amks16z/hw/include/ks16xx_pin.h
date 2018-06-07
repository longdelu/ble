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
 * \brief KS16xx GPIO 引脚配置参数定义
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-17  wxj, first implementation.
 * \endinternal
 */

#ifndef __KS16XX_PIN_H
#define __KS16XX_PIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "amdr_gpio_util.h"

 /**
 * \addtogroup amhw_if_pin
 * \copydoc ks16xx_pin.h
 * @{
 */

/**
 * \name KS16xx GPIO 引脚编号
 * @{
 */

#define PIOA_0          0         /**< \brief PIOA_0 引脚号  */
#define PIOA_1          1         /**< \brief PIOA_1 引脚号  */
#define PIOA_2          2         /**< \brief PIOA_2 引脚号  */
#define PIOA_3          3         /**< \brief PIOA_3 引脚号  */
#define PIOA_4          4         /**< \brief PIOA_4 引脚号  */
#define PIOA_5          5         /**< \brief PIOA_5 引脚号  */
/** \brief  PIOA_6~ PIOA_11 RESERVED */
#define PIOA_12         12        /**< \brief PIOA_12引脚号  */
#define PIOA_13         13        /**< \brief PIOA_13引脚号  */
/** \brief  PIOA_14~ PIOA_17 RESERVED */
#define PIOA_18         18        /**< \brief PIOA_18引脚号  */
#define PIOA_19         19        /**< \brief PIOA_19引脚号  */
#define PIOA_20         20        /**< \brief PIOA_20引脚号 */
/** \brief  PIOA_21~ PIOA_31 RESERVED */

#define PIOB_0          32       /**< \brief PIOB_0 引脚号 */
#define PIOB_1          33       /**< \brief PIOB_1 引脚号 */
#define PIOB_2          34       /**< \brief PIOB_2 引脚号 */
#define PIOB_3          35       /**< \brief PIOB_3 引脚号 */
/** \brief  PIOB_4~ PIOA_15 RESERVED */
#define PIOB_16         48       /**< \brief PIOB_16引脚号 */
#define PIOB_17         49       /**< \brief PIOB_17引脚号 */
#define PIOB_18         50       /**< \brief PIOB_18引脚号 */
#define PIOB_19         51       /**< \brief PIOB_19引脚号 */
/** \brief  PIOB_20~ PIOB_31 RESERVED */

#define PIOC_0          64       /**< \brief PIOC_0 引脚号 */
#define PIOC_1          65       /**< \brief PIOC_1 引脚号 */
#define PIOC_2          66       /**< \brief PIOC_2 引脚号 */
#define PIOC_3          67       /**< \brief PIOC_3 引脚号 */
#define PIOC_4          68       /**< \brief PIOC_4 引脚号 */
#define PIOC_5          69       /**< \brief PIOC_5 引脚号 */
#define PIOC_6          70       /**< \brief PIOC_6 引脚号 */
#define PIOC_7          71       /**< \brief PIOC_7 引脚号 */
#define PIOC_8          72       /**< \brief PIOC_8 引脚号 */
#define PIOC_9          73       /**< \brief PIOC_9 引脚号 */
#define PIOC_10         74       /**< \brief PIOC_10引脚号 */
#define PIOC_11         75       /**< \brief PIOC_11引脚号 */
/** \brief  PIOC_12~ PIOC_31 RESERVED */

#define PIOD_0          96       /**< \brief PIOD_0 引脚号 */
#define PIOD_1          97       /**< \brief PIOD_1 引脚号 */
#define PIOD_2          98       /**< \brief PIOD_2 引脚号 */
#define PIOD_3          99       /**< \brief PIOD_3 引脚号 */
#define PIOD_4          100       /**< \brief PIOD_4 引脚号 */
#define PIOD_5          101       /**< \brief PIOD_5 引脚号 */
#define PIOD_6          102       /**< \brief PIOD_6 引脚号 */
#define PIOD_7          103       /**< \brief PIOD_7 引脚号 */
/** \brief  PIOD_8~ PIOD_31 RESERVED */

#define PIOE_0          128       /**< \brief PIOE_0 引脚号 */
#define PIOE_1          129       /**< \brief PIOE_1 引脚号 */
/** \brief  PIOE_3~ PIOE_15 RESERVED */
#define PIOE_16         144       /**< \brief PIOE_16引脚号 */
#define PIOE_17         145       /**< \brief PIOE_17引脚号 */
#define PIOE_18         146       /**< \brief PIOE_18引脚号 */
#define PIOE_19         147       /**< \brief PIOE_19引脚号 */
#define PIOE_20         148       /**< \brief PIOE_20引脚号 */
#define PIOE_21         149       /**< \brief PIOE_21引脚号 */
#define PIOE_22         150       /**< \brief PIOE_22引脚号 */
#define PIOE_23         151       /**< \brief PIOE_23引脚号 */
#define PIOE_24         152       /**< \brief PIOE_24引脚号 */
#define PIOE_25         153       /**< \brief PIOE_25引脚号 */
/** \brief  PIOE_26~ PIOE_28 RESERVED */
#define PIOE_29         157       /**< \brief PIOE_29引脚号 */
#define PIOE_30         158       /**< \brief PIOE_30引脚号 */
#define PIOE_31         159       /**< \brief PIOE_31引脚号 */

/** @} */

/**
 * \brief 引脚数目为29
 */
#define PIN_NUM         54

/**
 * \brief 支持最大引脚中断数是2
 */
#define PIN_INT_MAX     2

/**
 * \name KS16xx 可转移功能定义(每个引脚都具有以下定义)
 * @{
 */

/******************************************************************************/

/**
 * \name PIOA_0引脚功能
 * @{
 */

#define PIOA_0_TSI0_CH1     AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief TSI0_CH1 */
#define PIOA_0_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOA_0_TPM0_CH5     AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM0_CH5 */
#define PIOA_0_SWD_CLK      AMDR_GPIO_FUNC_CODE(0x7)   /**< \brief SWD_CLK */

/** @} */

/**
 * \name PIOA_0功能模式
 * @{
 */

#define PIOA_0_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief (无上拉/下拉)*/
#define PIOA_0_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOA_0_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOA_0 驱动回转速率
 * @{
 */

#define PIOA_0_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOA_0_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 该管脚没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOA_1引脚功能
 * @{
 */

#define PIOA_1_TSI0_CH2     AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief TSI0_CH1 */
#define PIOA_1_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOA_1_UART0_RX     AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief UART0_RX */
#define PIOA_1_TPM2_CH0     AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM2_CH0 */

/** @} */

/**
 * \name PIOA_1功能模式
 * @{
 */

#define PIOA_1_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief (无上拉/下拉)*/
#define PIOA_1_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOA_1_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOA_1 驱动回转速率
 * @{
 */

#define PIOA_1_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOA_1_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOA_2引脚功能
 * @{
 */

#define PIOA_2_TSI0_CH3     AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief TSI0_CH3 */
#define PIOA_2_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOA_2_UART0_TX     AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief UART0_TX */
#define PIOA_2_TPM2_CH1     AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM2_CH1 */

/** @} */

/**
 * \name PIOA_2功能模式
 * @{
 */

#define PIOA_2_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief(无上拉/下拉)*/
#define PIOA_2_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOA_2_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOA_2 驱动回转速率
 * @{
 */

#define PIOA_2_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOA_2_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOA_3引脚功能
 * @{
 */

#define PIOA_3_TSI0_CH4     AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief TSI0_CH4 */
#define PIOA_3_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOA_3_I2C1_SCL     AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief I2C_SCL */
#define PIOA_3_TPM0_CH0     AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM0_CH0 */
#define PIOA_3_SWD_DIO      AMDR_GPIO_FUNC_CODE(0x7)   /**< \brief SWD_DIO */

/** @} */

/**
 * \name PIOA_3功能模式
 * @{
 */

#define PIOA_3_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉)*/
#define PIOA_3_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOA_3_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOA_3 驱动回转速率
 * @{
 */

#define PIOA_3_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOA_3_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOA_4引脚功能
 * @{
 */

#define PIOA_4_TSI0_CH5     AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief TSI0_CH5 */
#define PIOA_4_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOA_4_I2C1_SDA     AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief I2C_SDA */
#define PIOA_4_TPM0_CH1     AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM0_CH1 */
#define PIOA_4_NMI_B        AMDR_GPIO_FUNC_CODE(0x7)   /**< \brief NMI_B */

/** @} */

/**
 * \name PIOA_4功能模式
 * @{
 */

#define PIOA_4_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOA_4_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOA_4_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOA_4 驱动回转速率
 * @{
 */

#define PIOA_4_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOA_4_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/**
 * \name PIOA_4 输入干扰滤波(无源滤波器)
 * @{
 */

#define PIOA_4_FILT_DIS     AMDR_GPIO_PFE_CODE(0x0)    /**< \brief 滤波禁能 */
#define PIOA_4_FILT_EN      AMDR_GPIO_PFE_CODE(0x1)    /**< \brief 滤波使能 */

/** @} */

/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOA_5引脚功能
 * @{
 */

#define PIOA_5_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOA_5_TPM0_CH2     AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM0_CH2 */
#define PIOA_5_I2S0_TX_BCLK AMDR_GPIO_FUNC_CODE(0x6)   /**< \brief I2S0_TX_BCLK */

/** @} */

/**
 * \name PIOA_5功能模式
 * @{
 */

#define PIOA_5_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOA_5_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOA_5_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOA_5 驱动回转速率
 * @{
 */

#define PIOA_5_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOA_5_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOA_12引脚功能
 * @{
 */

#define PIOA_12_GPIO        AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOA_12_TPM1_CH0    AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM1_CH0 */
#define PIOA_12_I2S0_TXD0   AMDR_GPIO_FUNC_CODE(0x6)   /**< \brief I2S0_TXD0 */

/** @} */

/**
 * \name PIOA_12功能模式
 * @{
 */

#define PIOA_12_INACTIVE    AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOA_12_PULLDOWN    AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOA_12_PULLUP      AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOA_12 驱动回转速率
 * @{
 */

#define PIOA_12_SLEW_FAST   AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOA_12_SLEW_SLOW   AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOA_13引脚功能
 * @{
 */

#define PIOA_13_GPIO        AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOA_13_TPM1_CH1    AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM1_CH1 */
#define PIOA_13_I2S0_TX_FS  AMDR_GPIO_FUNC_CODE(0x6)   /**< \brief I2S0_TX_FS */

/** @} */

/**
 * \name PIOA_13功能模式
 * @{
 */

#define PIOA_13_INACTIVE    AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOA_13_PULLDOWN    AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOA_13_PULLUP      AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOA_13 驱动回转速率
 * @{
 */

#define PIOA_13_SLEW_FAST   AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOA_13_SLEW_SLOW   AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOA_18引脚功能
 * @{
 */

#define PIOA_18_EXTAL0      AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief EXTAL0 */
#define PIOA_18_GPIO        AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOA_18_UART1_RX    AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief UART1_RX */
#define PIOA_18_TPM_CLKIN0  AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief TPM_CLKIN0 */

/** @} */

/**
 * \name PIOA_18功能模式
 * @{
 */

#define PIOA_18_INACTIVE    AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOA_18_PULLDOWN    AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOA_18_PULLUP      AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOA_18 驱动回转速率
 * @{
 */

#define PIOA_18_SLEW_FAST   AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOA_18_SLEW_SLOW   AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOA_19引脚功能
 * @{
 */

#define PIOA_19_XTAL0       AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief XTAL0 */
#define PIOA_19_GPIO        AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOA_19_UART1_TX    AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief UART1_TX */
#define PIOA_19_TPM_CLKIN1  AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief TPM_CLKIN1 */
#define PIOA_19_LPTMR0_ALT1 AMDR_GPIO_FUNC_CODE(0x6)   /**< \brief LPTMR_ALT1 */
/** @} */

/**
 * \name PIOA_19功能模式
 * @{
 */

#define PIOA_19_INACTIVE    AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOA_19_PULLDOWN    AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOA_19_PULLUP      AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOA_19 驱动回转速率
 * @{
 */

#define PIOA_19_SLEW_FAST   AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOA_19_SLEW_SLOW   AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOA_20引脚功能
 * @{
 */

#define PIOA_20_GPIO        AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOA_20_REST_B      AMDR_GPIO_FUNC_CODE(0x7)   /**< \brief REST_B */

/** @} */

/**
 * \name PIOA_20功能模式
 * @{
 */

#define PIOA_20_INACTIVE    AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOA_20_PULLDOWN    AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOA_20_PULLUP      AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOA_20 驱动回转速率
 * @{
 */

#define PIOA_20_SLEW_FAST   AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOA_20_SLEW_SLOW   AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOB_0引脚功能
 * @{
 */

#define PIOB_0_ADC0_SE8     AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_SE8 */
#define PIOB_0_TSI0_CH0     AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief TSI0_CH0 */
#define PIOB_0_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOB_0_LLWU_P5      AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief LLWU_P5 */
#define PIOB_0_I2C0_SCL     AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief I2C0_SCL */
#define PIOB_0_TPM1_CH0     AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM1_CH0 */

/** @} */

/**
 * \name PIOB_0功能模式
 * @{
 */

#define PIOB_0_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOB_0_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOB_0_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOB_0 驱动回转速率
 * @{
 */

#define PIOB_0_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOB_0_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/**
 * \name PIOB_0 管脚驱动能力
 * @{
 */

#define PIOB_0_DRIVE_NORMAL AMDR_GPIO_DES_CODE(0x0)    /**< \brief 普通驱动力 */
#define PIOB_0_DRIVE_HIGH   AMDR_GPIO_DES_CODE(0x1)    /**< \brief 强驱动力*/

/* 该管脚没有配置管脚滤波功能 */

/******************************************************************************/

/**
 * \name PIOB_1引脚功能
 * @{
 */

#define PIOB_1_ADC0_SE9     AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_SE9 */
#define PIOB_1_TSI0_CH6     AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief TSI0_CH6 */
#define PIOB_1_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOB_1_I2C0_SDA     AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief I2C0_SDA */
#define PIOB_1_TPM1_CH1     AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM1_CH1 */

/** @} */

/**
 * \name PIOB_1功能模式
 * @{
 */

#define PIOB_1_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOB_1_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOB_1_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOB_1 驱动回转速率
 * @{
 */

#define PIOB_1_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOB_1_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/**
 * \name PIOB_1 管脚驱动能力
 * @{
 */

#define PIOB_1_DRIVE_NORMAL AMDR_GPIO_DES_CODE(0x0)    /**< \brief 普通驱动力 */
#define PIOB_1_DRIVE_HIGH   AMDR_GPIO_DES_CODE(0x1)    /**< \brief 强驱动力*/

/* 该管脚没有配置管脚滤波功能 */

/******************************************************************************/

/**
 * \name PIOB_2引脚功能
 * @{
 */

#define PIOB_2_ADC0_SE12    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_SE12 */
#define PIOB_2_TSI0_CH7     AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief TSI0_CH7 */
#define PIOB_2_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOB_2_I2C0_SCL     AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief I2C0_SCL */
#define PIOB_2_TPM2_CH0     AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM2_CH0 */

/** @} */

/**
 * \name PIOB_2功能模式
 * @{
 */

#define PIOB_2_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOB_2_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOB_2_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOB_2 驱动回转速率
 * @{
 */

#define PIOB_2_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOB_2_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */
/******************************************************************************/

/**
 * \name PIOB_3引脚功能
 * @{
 */

#define PIOB_3_ADC0_SE13    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_SE13 */
#define PIOB_3_TSI0_CH8     AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief TSI0_CH8 */
#define PIOB_3_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOB_3_I2C0_SDA     AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief I2C0_SDA */
#define PIOB_3_TPM2_CH1     AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM2_CH1 */

/** @} */

/**
 * \name PIOB_3功能模式
 * @{
 */

#define PIOB_3_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOB_3_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOB_3_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOB_3 驱动回转速率
 * @{
 */

#define PIOB_3_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOB_3_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */
/******************************************************************************/

/**
 * \name PIOB_16引脚功能
 * @{
 */

#define PIOB_16_TSI0_CH9    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief TSI0_CH9 */
#define PIOB_16_GPIO        AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOB_16_SPI1_MOSI   AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief SPI1_MOSI */
#define PIOB_16_UART0_RX    AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief UART0_RX */
#define PIOB_16_TPM_CLKIN0  AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief TPM_CLKIN0 */
#define PIOB_16_SPI1_MISO   MDR_GPIO_FUNC_CODE(0x5)    /**< \brief SPI1_MISO */

/** @} */

/**
 * \name PIOB_16功能模式
 * @{
 */

#define PIOB_16_INACTIVE    AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOB_16_PULLDOWN    AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOB_16_PULLUP      AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOB_16 驱动回转速率
 * @{
 */

#define PIOB_16_SLEW_FAST   AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOB_16_SLEW_SLOW   AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */
/******************************************************************************/

/**
 * \name PIOB_17引脚功能
 * @{
 */

#define PIOB_17_TSI0_CH10   AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief TSI0_CH10 */
#define PIOB_17_GPIO        AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOB_17_SPI1_MISO   AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief SPI1_MISO */
#define PIOB_17_UART0_TX    AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief UART0_TX */
#define PIOB_17_TPM_CLKIN1  AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief TPM_CLKIN1 */
#define PIOB_17_SPI1_MOSI   AMDR_GPIO_FUNC_CODE(0x5)   /**< \brief SPI1_MOSI */

/** @} */

/**
 * \name PIOB_17功能模式
 * @{
 */

#define PIOB_17_INACTIVE    AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOB_17_PULLDOWN    AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOB_17_PULLUP      AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOB_17 驱动回转速率
 * @{
 */

#define PIOB_17_SLEW_FAST   AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOB_17_SLEW_SLOW   AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */
/******************************************************************************/

/**
 * \name PIOB_18引脚功能
 * @{
 */

#define PIOB_18_TSI0_CH11   AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief TSI0_CH11 */
#define PIOB_18_GPIO        AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOB_18_TPM2_CH0    AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM2_CH0 */
#define PIOB_18_I2S_TX_BCLK AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief I2S_TX_BCLK */

/** @} */

/**
 * \name PIOB_18功能模式
 * @{
 */

#define PIOB_18_INACTIVE    AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOB_18_PULLDOWN    AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOB_18_PULLUP      AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOB_18 驱动回转速率
 * @{
 */

#define PIOB_18_SLEW_FAST   AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOB_18_SLEW_SLOW   AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */
/******************************************************************************/

/**
 * \name PIOB_19引脚功能
 * @{
 */

#define PIOB_19_TSI0_CH12   AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief TSI0_CH12 */
#define PIOB_19_GPIO        AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOB_19_TPM2_CH1    AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM2_CH1 */
#define PIOB_19_I2S_TX_FS   AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief I2S_TX_FS */

/** @} */

/**
 * \name PIOB_19功能模式
 * @{
 */

#define PIOB_19_INACTIVE    AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOB_19_PULLDOWN    AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOB_19_PULLUP      AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOB_19 驱动回转速率
 * @{
 */

#define PIOB_19_SLEW_FAST   AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOB_19_SLEW_SLOW   AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */
/******************************************************************************/

/**
 * \name PIOC_0引脚功能
 * @{
 */

#define PIOC_0_TSI0_CH13    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief TSI0_CH13 */
#define PIOC_0_ADC0_SE14    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_SE14 */
#define PIOC_0_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOC_0_EXTRG_IN     AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief EXTRG_IN */
#define PIOC_0_CMP0_OUT     AMDR_GPIO_FUNC_CODE(0x5)   /**< \brief CMP0_OUT */
#define PIOC_0_I2S0_TXD0    AMDR_GPIO_FUNC_CODE(0x6)   /**< \brief I2S0_TXD0 */

/** @} */

/**
 * \name PIOC_0功能模式
 * @{
 */

#define PIOC_0_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOC_0_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOC_0_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOC_0 驱动回转速率
 * @{
 */

#define PIOC_0_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOC_0_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */
/******************************************************************************/

/**
 * \name PIOC_1引脚功能
 * @{
 */

#define PIOC_1_TSI0_CH14    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief TSI0_CH14 */
#define PIOC_1_ADC0_SE15    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_SE15 */
#define PIOC_1_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOC_1_LLWU_P6      AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief LLWU_P6 */
#define PIOC_1_RTC_CLKIN    AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief RTC_CLKIN */
#define PIOC_1_I2C1_SCL     AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief EXTRG_IN */
#define PIOC_1_TPM0_CH0     AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief EXTRG_IN */
#define PIOC_1_I2S0_TXD0    AMDR_GPIO_FUNC_CODE(0x6)   /**< \brief I2S0_TXD0 */

/** @} */

/**
 * \name PIOC_1功能模式
 * @{
 */

#define PIOC_1_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOC_1_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOC_1_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOC_1 驱动回转速率
 * @{
 */

#define PIOC_1_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOC_1_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */
/******************************************************************************/

/**
 * \name PIOC_2引脚功能
 * @{
 */

#define PIOC_2_TSI0_CH15    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief TSI0_CH15 */
#define PIOC_2_ADC0_SE11    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_SE11 */
#define PIOC_2_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOC_2_I2C1_SDA     AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief I2C1_SDA */
#define PIOC_2_TPM0_CH1     AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief TPM0_CH1 */
#define PIOC_2_I2S0_TX_FS   AMDR_GPIO_FUNC_CODE(0x6)   /**< \brief I2S0_TX_FS */

/** @} */

/**
 * \name PIOC_2功能模式
 * @{
 */

#define PIOC_2_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOC_2_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOC_2_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOC_2 驱动回转速率
 * @{
 */

#define PIOC_2_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOC_2_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */
/******************************************************************************/

/**
 * \name PIOC_3引脚功能
 * @{
 */

#define PIOC_3_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOC_3_LLWU_P7      AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief LLWU_P7 */
#define PIOC_3_UART1_RX     AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief UART1_RX */
#define PIOC_3_TPM0_CH2     AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief TPM0_CH2 */
#define PIOC_3_CLKOUT       AMDR_GPIO_FUNC_CODE(0x5)   /**< \brief CLKOUT */
#define PIOC_3_I2S0_TX_BCLK AMDR_GPIO_FUNC_CODE(0x6)   /**< \brief I2S0_TX_BCLK */

/** @} */

/**
 * \name PIOC_3功能模式
 * @{
 */

#define PIOC_3_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOC_3_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOC_3_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOC_3 驱动回转速率
 * @{
 */

#define PIOC_3_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOC_3_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */
/******************************************************************************/

/**
 * \name PIOC_4引脚功能
 * @{
 */

#define PIOC_4_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOC_4_LLWU_P8      AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief LLWU_P8 */
#define PIOC_4_SPI0_PCS0    AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief SPI0_PCS0 */
#define PIOC_4_UART1_TX     AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief UART1_TX */
#define PIOC_4_TPM0_CH3     AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief TPM0_CH3 */
#define PIOC_4_I2S0_MCLK    AMDR_GPIO_FUNC_CODE(0x5)   /**< \brief I2S0_MCLK */

/** @} */

/**
 * \name PIOC_4功能模式
 * @{
 */

#define PIOC_4_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOC_4_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOC_4_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOC_4 驱动回转速率
 * @{
 */

#define PIOC_4_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOC_4_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */
/******************************************************************************/

/**
 * \name PIOC_5引脚功能
 * @{
 */

#define PIOC_5_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOC_5_LLWU_P9      AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief LLWU_P9 */
#define PIOC_5_SPI0_SCK     AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief SPI0_SCK */
#define PIOC_5_LPTMR0_ALT2  AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief LPTMR0_ALT2 */
#define PIOC_5_I2S0_RXD0    AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief I2S0_RXD0 */
#define PIOC_5_CMP0_OUT     AMDR_GPIO_FUNC_CODE(0x6)   /**< \brief CMP0_OUT */

/** @} */

/**
 * \name PIOC_5功能模式
 * @{
 */

#define PIOC_5_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOC_5_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOC_5_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOC_5 驱动回转速率
 * @{
 */

#define PIOC_5_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOC_5_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */
/******************************************************************************/

/**
 * \name PIOC_6引脚功能
 * @{
 */

#define PIOC_6_CMP0_IN0     AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief CMP0_IN0 */
#define PIOC_6_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOC_6_LLWU_P10     AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief LLWU_P10 */
#define PIOC_6_SPI0_MOSI    AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief SPI0_MOSI */
#define PIOC_6_EXTRG_IN     AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief EXTRG_IN */
#define PIOC_6_I2S0_RX_BCLK AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief I2S0_RX_BCLK */
#define PIOC_6_SPI0_MISO    AMDR_GPIO_FUNC_CODE(0x5)   /**< \brief SPI0_MISO */
#define PIOC_6_I2S0_MCLK    AMDR_GPIO_FUNC_CODE(0x6)   /**< \brief I2S0_MCLK */

/** @} */

/**
 * \name PIOC_6功能模式
 * @{
 */

#define PIOC_6_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOC_6_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOC_6_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOC_6 驱动回转速率
 * @{
 */

#define PIOC_6_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOC_6_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */
/******************************************************************************/

/**
 * \name PIOC_7引脚功能
 * @{
 */

#define PIOC_7_CMP0_IN1     AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief CMP0_IN1 */
#define PIOC_7_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOC_7_SPI0_MISO    AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief SPI0_MISO */
#define PIOC_7_I2S0_RX_FS   AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief I2S0_RX_FS */
#define PIOC_7_SPI0_MOSI    AMDR_GPIO_FUNC_CODE(0x5)   /**< \brief SPI0_MOSI */

/** @} */

/**
 * \name PIOC_7功能模式
 * @{
 */

#define PIOC_7_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOC_7_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOC_7_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOC_7 驱动回转速率
 * @{
 */

#define PIOC_7_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOC_7_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */
/******************************************************************************/

/**
 * \name PIOC_8引脚功能
 * @{
 */

#define PIOC_8_CMP0_IN2     AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief CMP0_IN2 */
#define PIOC_8_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOC_8_I2C0_SCL     AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief I2C0_SCL */
#define PIOC_8_TPM0_CH4     AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM0_CH4 */
#define PIOC_8_I2S0_MCLK    AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief I2S0_MCLK */

/** @} */

/**
 * \name PIOC_8功能模式
 * @{
 */

#define PIOC_8_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOC_8_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOC_8_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOC_8 驱动回转速率
 * @{
 */

#define PIOC_8_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOC_8_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */
/******************************************************************************/

/**
 * \name PIOC_9引脚功能
 * @{
 */

#define PIOC_9_CMP0_IN3     AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief CMP0_IN3 */
#define PIOC_9_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOC_9_I2C0_SDA     AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief I2C0_SDA */
#define PIOC_9_TPM0_CH5     AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM0_CH5 */
#define PIOC_9_I2S0_RX_BCLK AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief I2S0_RX_BCLK */

/** @} */

/**
 * \name PIOC_9功能模式
 * @{
 */

#define PIOC_9_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOC_9_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOC_9_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOC_9 驱动回转速率
 * @{
 */

#define PIOC_9_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOC_9_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */
/******************************************************************************/

/**
 * \name PIOC_10引脚功能
 * @{
 */

#define PIOC_10_GPIO        AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOC_10_I2C1_SCL    AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief I2C1_SCL */
#define PIOC_10_I2S0_RX_FS  AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief I2S0_RX_FS */

/** @} */

/**
 * \name PIOC_10功能模式
 * @{
 */

#define PIOC_10_INACTIVE    AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOC_10_PULLDOWN    AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOC_10_PULLUP      AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOC_10 驱动回转速率
 * @{
 */

#define PIOC_10_SLEW_FAST   AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOC_10_SLEW_SLOW   AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */
/******************************************************************************/

/**
 * \name PIOC_11引脚功能
 * @{
 */

#define PIOC_11_GPIO        AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOC_11_I2C1_SDA    AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief I2C1_SDA */
#define PIOC_11_I2S0_RXD0   AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief I2S0_RXD0 */

/** @} */

/**
 * \name PIOC_11功能模式
 * @{
 */

#define PIOC_11_INACTIVE    AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOC_11_PULLDOWN    AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOC_11_PULLUP      AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOC_11 驱动回转速率
 * @{
 */

#define PIOC_11_SLEW_FAST   AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOC_11_SLEW_SLOW   AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */
/******************************************************************************/

/**
 * \name PIOD_0引脚功能
 * @{
 */

#define PIOD_0_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOD_0_SPI0_PCS0    AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief SPI0_PCS0 */
#define PIOD_0_TPM0_CH0     AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief TPM0_CH0 */

/** @} */

/**
 * \name PIOD_0功能模式
 * @{
 */

#define PIOD_0_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOD_0_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOD_0_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOD_0 驱动回转速率
 * @{
 */

#define PIOD_0_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOD_0_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */
/******************************************************************************/

/**
 * \name PIOD_1引脚功能
 * @{
 */

#define PIOD_1_ADC0_SE5B    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_SE5B */
#define PIOD_1_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOD_1_SPI0_SCK     AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief SPI0_SCK */
#define PIOD_1_TPM0_CH1     AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief TPM0_CH1 */

/** @} */

/**
 * \name PIOD_1功能模式
 * @{
 */

#define PIOD_1_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOD_1_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOD_1_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOD_1 驱动回转速率
 * @{
 */

#define PIOD_1_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOD_1_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */
/******************************************************************************/

/**
 * \name PIOD_2引脚功能
 * @{
 */

#define PIOD_2_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOD_2_SPI0_MOSI    AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief SPI0_MOSI */
#define PIOD_2_UART2_RX     AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief UART2_RX */
#define PIOD_2_TPM0_CH2     AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief TPM0_CH2 */
#define PIOD_2_SPI0_MISO    AMDR_GPIO_FUNC_CODE(0x5)   /**< \brief SPI0_MISO */

/** @} */

/**
 * \name PIOD_2功能模式
 * @{
 */

#define PIOD_2_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOD_2_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOD_2_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOD_2 驱动回转速率
 * @{
 */

#define PIOD_2_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOD_2_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */
/******************************************************************************/

/**
 * \name PIOD_3引脚功能
 * @{
 */

#define PIOD_3_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOD_3_SPI0_MISO    AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief SPI0_MISO */
#define PIOD_3_UART2_TX     AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief UART2_TX */
#define PIOD_3_TPM0_CH3     AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief TPM0_CH3 */
#define PIOD_3_SPI0_MOSI    AMDR_GPIO_FUNC_CODE(0x5)   /**< \brief SPI0_MOSI */

/** @} */

/**
 * \name PIOD_3功能模式
 * @{
 */

#define PIOD_3_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOD_3_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOD_3_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOD_3 驱动回转速率
 * @{
 */

#define PIOD_3_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOD_3_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */
/******************************************************************************/

/**
 * \name PIOD_4引脚功能
 * @{
 */

#define PIOD_4_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOD_4_LLWU_P14     AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief LLWU_P14 */
#define PIOD_4_SPI1_PCS0    AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief SPI1_PCS0 */
#define PIOD_4_UART2_RX     AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief UART2_RX */
#define PIOD_4_TPM0_CH4     AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief TPM0_CH4 */

/** @} */

/**
 * \name PIOD_4功能模式
 * @{
 */

#define PIOD_4_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOD_4_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOD_4_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOD_4 驱动回转速率
 * @{
 */

#define PIOD_4_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOD_4_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */
/******************************************************************************/

/**
 * \name PIOD_5引脚功能
 * @{
 */

#define PIOD_5_ADC0_SE6B    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_SE6B */
#define PIOD_5_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOD_5_SPI1_SCK     AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief SPI1_SCK */
#define PIOD_5_UART2_TX     AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief UART2_TX */
#define PIOD_5_TPM0_CH5     AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief TPM0_CH5 */

/** @} */

/**
 * \name PIOD_5功能模式
 * @{
 */

#define PIOD_5_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOD_5_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOD_5_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOD_5 驱动回转速率
 * @{
 */

#define PIOD_5_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOD_5_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */
/******************************************************************************/

/**
 * \name PIOD_6引脚功能
 * @{
 */

#define PIOD_6_ADC0_SE7B    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_SE7B */
#define PIOD_6_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOD_6_LLWU_P15     AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief LLWU_P15 */
#define PIOD_6_SPI1_MOSI    AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief SPI1_MOSI */
#define PIOD_6_UART0_RX     AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief UART0_RX */
#define PIOD_6_SPI1_MISO    AMDR_GPIO_FUNC_CODE(0x5)   /**< \brief SPI1_MISO */

/** @} */

/**
 * \name PIOD_6功能模式
 * @{
 */

#define PIOD_6_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOD_6_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOD_6_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOD_6 驱动回转速率
 * @{
 */

#define PIOD_6_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOD_6_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/**
 * \name PIOD_6 管脚驱动能力
 * @{
 */

#define PIOD_6_DRIVE_NORMAL  AMDR_GPIO_DES_CODE(0x0)    /**< \brief 普通驱动力 */
#define PIOD_6_DRIVE_HIGH    AMDR_GPIO_DES_CODE(0x1)    /**< \brief 强驱动力*/

/** @} */

/* 该管脚没有配置管脚滤波功能 */

/******************************************************************************/

/**
 * \name PIOD_7引脚功能
 * @{
 */

#define PIOD_7_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOD_7_SPI1_MISO    AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief SPI1_MISO */
#define PIOD_7_UART0_TX     AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief UART0_TX */
#define PIOD_7_SPI1_MOSI    AMDR_GPIO_FUNC_CODE(0x5)   /**< \brief SPI1_MOSI */

/** @} */

/**
 * \name PIOD_7功能模式
 * @{
 */

#define PIOD_7_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOD_7_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOD_7_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOD_7 驱动回转速率
 * @{
 */

#define PIOD_7_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOD_7_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/**
 * \name PIOD_7 管脚驱动能力
 * @{
 */

#define PIOD_7_DRIVE_NORMAL AMDR_GPIO_DES_CODE(0x0)    /**< \brief 普通驱动力 */
#define PIOD_7_DRIVE_HIGH   AMDR_GPIO_DES_CODE(0x1)    /**< \brief 强驱动力*/

/** @} */

/* 该管脚没有配置管脚滤波功能 */

/******************************************************************************/

/**
 * \name PIOE_0引脚功能
 * @{
 */

#define PIOE_0_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOE_0_SPI1_MISO    AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief SPI1_MISO */
#define PIOE_0_UART1_TX     AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief UART1_TX */
#define PIOE_0_RTC_CLKOUT   AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief RTC_CLKOUT */
#define PIOE_0_CMP0_OUT     AMDR_GPIO_FUNC_CODE(0x5)   /**< \brief CMP0_OUT */
#define PIOE_0_I2C1_SDA     AMDR_GPIO_FUNC_CODE(0x6)   /**< \brief I2C1_SDA */

/** @} */

/**
 * \name PIOE_0功能模式
 * @{
 */

#define PIOE_0_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOE_0_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOE_0_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOE_0 驱动回转速率
 * @{
 */

#define PIOE_0_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOE_0_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOE_1引脚功能
 * @{
 */

#define PIOE_1_GPIO         AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOE_1_SPI1_MOSI    AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief SPI1_MOSI */
#define PIOE_1_UART1_RX     AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief UART1_RX */
#define PIOE_1_SPI1_MISO    AMDR_GPIO_FUNC_CODE(0x5)   /**< \brief SPI1_MISO */
#define PIOE_1_I2C1_SCL     AMDR_GPIO_FUNC_CODE(0x6)   /**< \brief I2C1_SCL */

/** @} */

/**
 * \name PIOE_1功能模式
 * @{
 */

#define PIOE_1_INACTIVE     AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOE_1_PULLDOWN     AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOE_1_PULLUP       AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOE_1 驱动回转速率
 * @{
 */

#define PIOE_1_SLEW_FAST    AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOE_1_SLEW_SLOW    AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOE_16引脚功能
 * @{
 */
#define PIOE_16_ADC0_DP1    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_DP1 */
#define PIOE_16_ADC0_SE1    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_SE1 */
#define PIOE_16_GPIO        AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOE_16_SPI0_PCS0   AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief SPI0_PCS0 */
#define PIOE_16_UART2_TX    AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief UART2_TX */
#define PIOE_16_TPM_CLKIN0  AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief TPM_CLKIN0 */

/** @} */

/**
 * \name PIOE_16功能模式
 * @{
 */

#define PIOE_16_INACTIVE    AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOE_16_PULLDOWN    AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOE_16_PULLUP      AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOE_16 驱动回转速率
 * @{
 */

#define PIOE_16_SLEW_FAST   AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOE_16_SLEW_SLOW   AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOE_17引脚功能
 * @{
 */
#define PIOE_17_ADC0_DM1    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_DM1 */
#define PIOE_17_ADC0_SE5A   AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_SE5A */
#define PIOE_17_GPIO        AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOE_17_SPI0_SCK    AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief SPI0_SCK */
#define PIOE_17_UART2_RX    AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief UART2_RX */
#define PIOE_17_TPM_CLKIN1  AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief TPM_CLKIN1 */
#define PIOE_17_LPTMR0_ALT3 AMDR_GPIO_FUNC_CODE(0x6)   /**< \brief LPTMR0_ALT3 */

/** @} */

/**
 * \name PIOE_17功能模式
 * @{
 */

#define PIOE_17_INACTIVE    AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOE_17_PULLDOWN    AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOE_17_PULLUP      AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOE_17 驱动回转速率
 * @{
 */

#define PIOE_17_SLEW_FAST   AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOE_17_SLEW_SLOW   AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOE_18引脚功能
 * @{
 */
#define PIOE_18_ADC0_DP2    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_DP2 */
#define PIOE_18_ADC0_SE2    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_SE2 */
#define PIOE_18_GPIO        AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOE_18_SPI0_MOSI   AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief SPI0_MOSI */
#define PIOE_18_I2C0_SDA    AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief I2C0_SDA */
#define PIOE_18_SPI0_MISO   AMDR_GPIO_FUNC_CODE(0x5)   /**< \brief SPI0_MISO */

/** @} */

/**
 * \name PIOE_18功能模式
 * @{
 */

#define PIOE_18_INACTIVE    AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOE_18_PULLDOWN    AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOE_18_PULLUP      AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOE_18 驱动回转速率
 * @{
 */

#define PIOE_18_SLEW_FAST   AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOE_18_SLEW_SLOW   AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOE_19引脚功能
 * @{
 */
#define PIOE_19_ADC0_DM2    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_DM2 */
#define PIOE_19_ADC0_SE6A   AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_SE6A */
#define PIOE_19_GPIO        AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOE_19_SPI0_MISO   AMDR_GPIO_FUNC_CODE(0x2)   /**< \brief SPI0_MISO */
#define PIOE_19_I2C0_SCL    AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief I2C0_SCL */
#define PIOE_19_SPI0_MOSI   AMDR_GPIO_FUNC_CODE(0x5)   /**< \brief SPI0_MOSI */

/** @} */

/**
 * \name PIOE_19功能模式
 * @{
 */

#define PIOE_19_INACTIVE    AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOE_19_PULLDOWN    AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOE_19_PULLUP      AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOE_19 驱动回转速率
 * @{
 */

#define PIOE_19_SLEW_FAST   AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOE_19_SLEW_SLOW   AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOE_20引脚功能
 * @{
 */
#define PIOE_20_ADC0_DP0    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_DP0 */
#define PIOE_20_ADC0_SE0    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_SE0 */
#define PIOE_20_GPIO        AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOE_20_TPM1_CH0    AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM1_CH0 */
#define PIOE_20_UART0_TX    AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief UART0_TX */

/** @} */

/**
 * \name PIOE_20功能模式
 * @{
 */

#define PIOE_20_INACTIVE    AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOE_20_PULLDOWN    AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOE_20_PULLUP      AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOE_20 驱动回转速率
 * @{
 */

#define PIOE_20_SLEW_FAST   AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOE_20_SLEW_SLOW   AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOE_21引脚功能
 * @{
 */
#define PIOE_21_ADC0_DM0    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_DM0 */
#define PIOE_21_ADC0_SE4A   AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_SE4A */
#define PIOE_21_GPIO        AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOE_21_TPM1_CH1    AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM1_CH1 */
#define PIOE_21_UART0_RX    AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief UART0_RX */

/** @} */

/**
 * \name PIOE_21功能模式
 * @{
 */

#define PIOE_21_INACTIVE    AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOE_21_PULLDOWN    AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOE_21_PULLUP      AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOE_21 驱动回转速率
 * @{
 */

#define PIOE_21_SLEW_FAST   AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOE_21_SLEW_SLOW   AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOE_22引脚功能
 * @{
 */
#define PIOE_22_ADC0_DP3    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_DP3 */
#define PIOE_22_ADC0_SE3    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_SE3 */
#define PIOE_22_GPIO        AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOE_22_TPM2_CH0    AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM2_CH0 */
#define PIOE_22_UART2_TX    AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief UART2_TX */

/** @} */

/**
 * \name PIOE_22功能模式
 * @{
 */

#define PIOE_22_INACTIVE    AMDR_GPIO_PE_CODE(0x0)     /**< \brief 消无上拉/下拉 */
#define PIOE_22_PULLDOWN    AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOE_22_PULLUP      AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOE_22 驱动回转速率
 * @{
 */

#define PIOE_22_SLEW_FAST   AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOE_22_SLEW_SLOW   AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOE_23引脚功能
 * @{
 */
#define PIOE_23_ADC0_DM3    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_DM3 */
#define PIOE_23_ADC0_SE7A   AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_SE7A */
#define PIOE_23_GPIO        AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOE_23_TPM2_CH1    AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM2_CH1 */
#define PIOE_23_UART2_RX    AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief UART2_RX */

/** @} */

/**
 * \name PIOE_23功能模式
 * @{
 */

#define PIOE_23_INACTIVE    AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOE_23_PULLDOWN    AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOE_23_PULLUP      AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOE_23 驱动回转速率
 * @{
 */

#define PIOE_23_SLEW_FAST   AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOE_23_SLEW_SLOW   AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOE_24引脚功能
 * @{
 */

#define PIOE_24_GPIO        AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOE_24_TPM0_CH0    AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM0_CH0 */
#define PIOE_24_I2C0_SCL    AMDR_GPIO_FUNC_CODE(0x5)   /**< \brief I2C0_SCL */

/** @} */

/**
 * \name PIOE_24功能模式
 * @{
 */

#define PIOE_24_INACTIVE    AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOE_24_PULLDOWN    AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOE_24_PULLUP      AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOE_24 驱动回转速率
 * @{
 */

#define PIOE_24_SLEW_FAST   AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOE_24_SLEW_SLOW   AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOE_25引脚功能
 * @{
 */

#define PIOE_25_GPIO        AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOE_25_TPM0_CH1    AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM0_CH1 */
#define PIOE_25_I2C0_SDA    AMDR_GPIO_FUNC_CODE(0x5)   /**< \brief I2C0_SDA */

/** @} */

/**
 * \name PIOE_25功能模式
 * @{
 */

#define PIOE_25_INACTIVE    AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOE_25_PULLDOWN    AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOE_25_PULLUP      AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOE_25 驱动回转速率
 * @{
 */

#define PIOE_25_SLEW_FAST   AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOE_25_SLEW_SLOW   AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOE_29引脚功能
 * @{
 */
#define PIOE_29_CMP0_IN5    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief CMP0_IN5 */
#define PIOE_29_ADC0_SE4B   AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_SE4B */
#define PIOE_29_GPIO        AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOE_29_TPM0_CH2    AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM0_CH2 */
#define PIOE_29_TPM_CLKIN0  AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief TPM_CLKIN0 */

/** @} */

/**
 * \name PIOE_29功能模式
 * @{
 */

#define PIOE_29_INACTIVE    AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOE_29_PULLDOWN    AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOE_29_PULLUP      AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOE_29 驱动回转速率
 * @{
 */

#define PIOE_29_SLEW_FAST   AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOE_29_SLEW_SLOW   AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOE_30引脚功能
 * @{
 */
#define PIOE_30_DAC0_OUT    AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief DAC0_OUT */
#define PIOE_30_ADC0_SE23   AMDR_GPIO_FUNC_CODE(0x0)   /**< \brief ADC0_SE23 */
#define PIOE_30_GPIO        AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOE_30_TPM0_CH3    AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM0_CH3 */
#define PIOE_30_TPM_CLKIN1  AMDR_GPIO_FUNC_CODE(0x4)   /**< \brief TPM_CLKIN1 */

/** @} */

/**
 * \name PIOE_30功能模式
 * @{
 */

#define PIOE_30_INACTIVE    AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOE_30_PULLDOWN    AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOE_30_PULLUP      AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOE_30 驱动回转速率
 * @{
 */

#define PIOE_30_SLEW_FAST   AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOE_30_SLEW_SLOW   AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * \name PIOE_31引脚功能
 * @{
 */

#define PIOE_31_GPIO        AMDR_GPIO_FUNC_CODE(0x1)   /**< \brief GPIO */
#define PIOE_31_TPM0_CH4    AMDR_GPIO_FUNC_CODE(0x3)   /**< \brief TPM0_CH4 */

/** @} */

/**
 * \name PIOE_31功能模式
 * @{
 */

#define PIOE_31_INACTIVE    AMDR_GPIO_PE_CODE(0x0)     /**< \brief 无上拉/下拉 */
#define PIOE_31_PULLDOWN    AMDR_GPIO_PULL_CODE(0x0)   /**< \brief 下拉模式 */
#define PIOE_31_PULLUP      AMDR_GPIO_PULL_CODE(0x1)   /**< \brief 上拉模式 */

/** @} */

/**
 * \name PIOE_31 驱动回转速率
 * @{
 */

#define PIOE_31_SLEW_FAST   AMDR_GPIO_SRE_CODE(0x0)    /**< \brief 快速模式 */
#define PIOE_31_SLEW_SLOW   AMDR_GPIO_SRE_CODE(0x1)    /**< \brief 慢速模式 */

/** @} */

/* 该管脚没有配置管脚滤波功能 */
/* 没有配置管脚驱动力功能 */

/******************************************************************************/

/**
 * @} amhw_if_pin
 */

#ifdef __cplusplus
}
#endif

#endif /* __KS16XX_PIN_H */

/* end of file */
