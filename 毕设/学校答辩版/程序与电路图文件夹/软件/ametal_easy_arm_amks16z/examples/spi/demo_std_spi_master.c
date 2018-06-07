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
 * \brief 以SPI为例，演示了SPI中Transfer的使用方法，通过标准接口实现
 *
 * - 操作步骤:
 *   1. 将SPI MOSI和MISO短接,虚拟从机设备连接（从机设备DEV0，和从机设备DEV1）
 *   2. 按下按键传输一次数据
 *
 * - 实验现象:
 * \note
 *   1. 注意传输标识：AM_SPI_TRANS_EOT 的设置，只有当本次传输结束后，
 *      需要将SSEL置为无效电平时，才需要设置该标识，其它情况均无需设置该标识。
 *
 * \par 源代码
 * \snippet demo_std_spi_master.c src_std_spi_master
 *
 * \internal
 * \par History
 * - 1.00 15-10-24  liqing, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_std_spi_master
 * \copydoc demo_std_spi_master.c
 */
 
/** [src_std_spi_master] */
#include "ametal.h"
#include "am_board.h"
#include "am_prj_config.h"

/* 从机设备DEV0 */

am_spi_device_t   g_dev0;     /**< \brief 从机设备0      */
am_spi_device_t   g_dev1;     /**< \brief 从机设备1      */
am_spi_transfer_t g_trans;    /**< \brief 传输内容       */
                                                         
#define KEY_PIN     PIOC_0    /**< \brief 按键引脚       */
#define BUZZER_PIN  PIOC_3    /**< \brief 蜂鸣器引脚     */

#define BUF_LEN   10          /**< \brief 测试缓冲区大小 */
uint8_t g_tx_buf[BUF_LEN];    /**< \brief 写数据缓存     */
uint8_t g_rx_buf[BUF_LEN];    /**< \brief 读数据缓存     */

void dev1_cs_ctr (struct am_spi_device *p_dev, int state);
void buzzer_dirve (bool_t error_ocur);
void trans_once (am_spi_handle_t handle);
void trans_complete (void *p_arg,int status);

/**
 * \brief 主函数
 */
int main (void)
{
    am_spi_handle_t spi_handle;
    
    /* 板级初始化 */
    am_board_init();
    
    /* SPI0 初始化 */
    amhw_plfm_spi0_init();
    spi_handle = amdr_spi_init(&g_spi0_dev, &g_spi0_devinfo);
    am_spi_connect(spi_handle);
    
    /* 配置key引脚 */
    am_gpio_pin_cfg(KEY_PIN, AM_GPIO_INPUT | AM_GPIO_PULLUP);

    while (1) {
        
        /* 按下key 发送一次数据 */
        if (am_gpio_get(KEY_PIN) == 0) {
            trans_once(spi_handle);
        }
    }
}

/* 蜂鸣器发声 */
void buzzer_dirve (bool_t error_ocur)
{
    uint8_t count = 100;
    am_gpio_pin_cfg(BUZZER_PIN, AM_GPIO_OUTPUT | AM_GPIO_PULLUP);

    /* 错误 */
    if (error_ocur) {
        
        /* 一直发声 */
        while (1) {
            am_gpio_toggle(BUZZER_PIN);
            am_mdelay(5);
        }

    } else {

        /* 发声大约 0.5s */
        for(count = 100; count > 0; count--) {
            am_gpio_toggle(BUZZER_PIN);
            am_mdelay(5);
        }
    }
}

/* 传输完成回调函数 */
void trans_complete (void *p_arg,int status)
{
    uint8_t i = 0;

    am_wait_t *p_wait = (am_wait_t *)p_arg;

    bool_t error_ocur = FALSE;

    if (status != AM_OK) {
        error_ocur = TRUE;
    } else {
        
        /* 检查数据完整性 */
        for(i = 0; i < BUF_LEN; i++) {
            if (g_tx_buf[i] != g_rx_buf[i]) {
                error_ocur = TRUE;
                break;
            }
        }
    }

    /* 驱动蜂鸣器发声 */
    buzzer_dirve(error_ocur);

    am_wait_done(p_wait);
}

/* Dev1片选引脚控制回调函数 */
void dev1_cs_ctr (struct am_spi_device *p_dev, int state)
{
    am_gpio_pin_cfg(PIOC_8, AM_GPIO_OUTPUT);
    am_gpio_set(PIOC_8, !state);
}
 
/* 传输函数 */
void trans_once (am_spi_handle_t handle)
{
    uint8_t i;
    
    am_wait_t trans_wait;
    
    /* 初始化Dev0 */
    am_spi_mkdev (&g_dev0,
                   handle,
                   8,
                   AM_SPI_MODE_1,
                   3000000,
                   PIOC_4,  /* 使用引脚控制片选 */
                   NULL);
                  
    /* 数据传输之前应使用am_spi_setup (am_spi_device_t *p_dev)检验设备是否支持 */
    if (am_spi_setup(&g_dev0) != AM_OK) {
        return;
    }
    
    for(i = 0;i < BUF_LEN; i++) {
        g_tx_buf[i] = i;
    }
    
    am_spi_mktrans(&g_trans,
                    g_tx_buf,
                    g_rx_buf,
                    BUF_LEN,
                    0,           /* 片选不影响          */
                    0,           /* 使用p_dev中的字宽度 */
                    0,           /* 无延时              */
                    0,           /* 使用默认速率        */
                    0,           /* 无特殊标志          */
                    trans_complete,
                   (void *)&trans_wait);
    am_spi_transfer(&g_dev0, &g_trans);
    
    am_wait_on(&trans_wait);
    
    am_mdelay(1000);
    
    for(i = 0;i < BUF_LEN; i++) {
        g_tx_buf[i] = i + 1;
    }
    
    am_spi_mktrans(&g_trans,
                    g_tx_buf,
                    g_rx_buf,
                    BUF_LEN,
                    0,                /* 片选不影响          */
                    0,                /* 使用p_dev中的字宽度 */
                    0,                /* 无延时              */
                    0,                /* 使用默认速率        */
                    AM_SPI_TRANS_EOT, /* 最后一个传输        */
                    trans_complete,
                   (void *)&trans_wait);
    am_spi_transfer(&g_dev0, &g_trans);
    
    am_wait_on(&trans_wait);
    
    am_mdelay(2000);

    /* 初始化Dev1 */
    am_spi_mkdev (&g_dev1,
                   handle,
                   8,
                   AM_SPI_MODE_1,
                   3000000,
                   0,
                   dev1_cs_ctr);  /* 使用回调函数控制片选 */
    
    if (am_spi_setup(&g_dev1) != AM_OK) {
        return;
    }
    
    for(i = 0;i < BUF_LEN; i++) {
        g_tx_buf[i] = i + 2;
    }
    
    am_spi_mktrans(&g_trans,
                    g_tx_buf,
                    g_rx_buf,
                    BUF_LEN,
                    0,               /* 片选不影响          */
                    0,               /* 使用p_dev中的字宽度 */
                    0,               /* 无延时              */
                    0,               /* 使用默认速率        */
                    0,               /* 最后一个传输        */
                    trans_complete,
                   (void *)&trans_wait);
    am_spi_transfer(&g_dev1, &g_trans);
    
    am_wait_on(&trans_wait);
    
    am_mdelay(1000);

    for(i = 0;i < BUF_LEN; i++) {
       g_tx_buf[i] = i + 3;
    }

    am_spi_mktrans(&g_trans,
                    g_tx_buf,
                    g_rx_buf,
                    BUF_LEN,
                    0,               /* 片选不影响          */
                    0,               /* 使用p_dev中的字宽度 */
                    0,               /* 无延时             */
                    0,               /* 使用默认速率       */
                    AM_SPI_TRANS_EOT,/* 最后一个传输       */
                    trans_complete,
                   (void *)&trans_wait);
    am_spi_transfer(&g_dev1, &g_trans);
    
    am_wait_on(&trans_wait);
    
    am_mdelay(2000);
}

/** [src_std_spi_master] */

/* end of file */
