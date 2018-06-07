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
 * \brief SPI从机演示例程，通过HW层的接口实现
 *
 * - 操作步骤:
 *   1. 将SPI0从机接口和SPI1主机接口对应连接起来；
 *   2. 将程序下载到demo板，重新上电或者复位。
 *
 * - 实验现象:
 *   1. 主机SPI1和从机SPI0互发数据(SPI0使用中断方式收发数据)；
 *   2. 比较SPI1和SPI0数据，如果一致点亮LED0，否则LED0以200ms时间间隔闪烁。
 *
 * \par 源代码
 * \snippet demo_hw_spi_slave_irq.c src_hw_spi_slave_irq
 *
 * \internal
 * \par History
 * - 1.00 15-10-30  liqing, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_hw_spi_slave_irq
 * \copydoc demo_hw_spi_slave_irq.c
 */
 
/** [src_hw_spi_slave_irq] */
#include "ametal.h"
#include "am_board.h"
#include "am_prj_config.h"


#define NUM_COUNT   32                            /**< \brief 缓冲区大小     */
uint8_t g_spi0_slve_txbuf[NUM_COUNT]   = {0};     /**< \brief 从机发送缓冲区 */
uint8_t g_spi0_slve_rxbuf[NUM_COUNT]   = {0};     /**< \brief 从机接收缓冲区 */
uint8_t g_spi1_master_txbuf[NUM_COUNT] = {0};     /**< \brief 主机发送缓冲区 */
uint8_t g_spi1_master_rxbuf[NUM_COUNT] = {0};     /**< \brief 主机接收缓冲区 */

void spi0_pin_init (void);
void spi1_pin_init (void);
void spi_slve_init (amhw_spi_t *p_hw_spi);
void spi_master_init (amhw_spi_t *p_hw_spi);

/**
 * \brief SPI0从机中断处理函数
 */
void spi0_irq(void *p_arg)
{
    amhw_spi_t *p_hw_spi = (amhw_spi_t*)p_arg;
    
    static uint8_t send_pos = 0;
    static uint8_t recv_pos = 0;
    
    /* 接收数据 */
    if (amhw_spi_stat_get(p_hw_spi) & AMHW_SPI_STAT_R_FULL) {
        g_spi0_slve_rxbuf[recv_pos++] = amhw_spi_data8_read(p_hw_spi);
    }
    
    /* 发送数据 */
    if (amhw_spi_stat_get(p_hw_spi) & AMHW_SPI_STAT_T_EMPTY) {
        amhw_spi_data8_wirte(p_hw_spi,g_spi0_slve_txbuf[send_pos++]);
    }
}


/**
 * \brief 主函数
 */
int main (void)
{
    uint8_t i;
    
    bool_t error_ocur = FALSE;
    
    /* 板级初始化 */
    am_board_init();
    
    /* 引脚配置 */
    spi0_pin_init();
    spi1_pin_init();
    
    /* 时钟使能 */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_SPI0);
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_SPI1);
    
    /* SPI初始化，SPI0为从机，SPI1为主机 */
    spi_slve_init(AMHW_SPI0);
    spi_master_init(AMHW_SPI1);


    /* 构造发送的数据 */
    for (i = 0; i < NUM_COUNT; i++) {
        g_spi0_slve_txbuf[i]   = i * 3;
        g_spi1_master_txbuf[i] = i * 2;
    }
    
    /* SPI0从机使用中断接收和发送数据 */
    amhw_spi_int_enable(AMHW_SPI0, AMHW_SPI_IRQ_R_MODDEF);
    amhw_spi_int_enable(AMHW_SPI0, AMHW_SPI_IRQ_T);

    am_int_connect(INUM_SPI0, spi0_irq, (void*)AMHW_SPI0);
    am_int_enable(INUM_SPI0);
    
    /* SPI主机收发数据 */
    for (i = 0; i < NUM_COUNT; i++) {
        while((amhw_spi_stat_get(AMHW_SPI1) & AMHW_SPI_STAT_T_EMPTY) == 0);
        amhw_spi_data8_wirte(AMHW_SPI1,g_spi1_master_txbuf[i]);
        
        while((amhw_spi_stat_get(AMHW_SPI1) & AMHW_SPI_STAT_R_FULL) == 0);
        g_spi1_master_rxbuf[i] = amhw_spi_data8_read(AMHW_SPI1);
    }
   
    /* 等待最后的数据发送完成 */
    am_mdelay(1000);
    
    /* 数据检验 */
   for (i = 0; i < NUM_COUNT; i++) {
        if (g_spi0_slve_txbuf[i]   != g_spi1_master_rxbuf[i] ||
            g_spi1_master_txbuf[i] != g_spi0_slve_rxbuf[i]) {
                
            error_ocur  =TRUE;
            break;   
        }
    } 

    while (1) {
        
        /* 数据校验错误，LED闪烁提醒 */
        if(error_ocur) {
            am_led_on(LED0);
            am_mdelay(200);
            am_led_off(LED0);
            am_mdelay(200);
        
        /* LED灯常亮 */
        } else {
            am_led_on(LED0);
        }

    }
}

void spi_speed_cfg (amhw_spi_t *p_hw_spi, uint32_t target_speed)
{

    uint32_t real_speed;              /* 计算出的速度    */
    uint32_t pdiv, best_pdiv;         /* 预分频值        */
    uint32_t div, best_div;           /* 分配值          */
    uint32_t diff, min_diff;          /* 速度差值        */
    uint32_t spi_freq;                /* SPI输入时钟频率 */

    spi_freq = amhw_clock_periph_freq_get(p_hw_spi);

   min_diff = 0xFFFFFFFFU;

   /* 设置为最大的分频值,速度为最小 */
   best_pdiv  = 7;
   best_div   = 8;

   /* 查找最合适的速度 */
   for (pdiv = 0; (pdiv <= 7) && min_diff; pdiv++)
   {
       for (div = 0; (div <= 8) && min_diff; div++)
       {
           /* all_div = (pdiv+1) * 2^(div+1) */
           real_speed = (spi_freq / ((pdiv + 1) << (div+1)));

           /* 实际速度应小于目标速度 */
           if (target_speed >= real_speed)
           {
               diff = target_speed-real_speed;

               if (min_diff > diff)
               {
                   /* 最合适的速度 */
                   min_diff   = diff;
                   best_pdiv  = pdiv;
                   best_div   = div;
               }
           }
       }
   }

   amhw_spi_div_cfg(p_hw_spi, best_pdiv, best_div);
}

/**
 * \brief SPI0从机的引脚初始化
 */
void spi0_pin_init (void)
{
    /* 必须需要片选 */
    am_gpio_pin_cfg(PIOE_16, PIOE_16_SPI0_PCS0); 
    am_gpio_pin_cfg(PIOE_17, PIOE_17_SPI0_SCK );
    am_gpio_pin_cfg(PIOE_18, PIOE_18_SPI0_MOSI);
    am_gpio_pin_cfg(PIOE_19, PIOE_19_SPI0_MISO);
}

/**
 * \brief SPI1主机的引脚初始化
 */
void spi1_pin_init (void)
{
    am_gpio_pin_cfg(PIOD_4, PIOD_4_SPI1_PCS0); 
    am_gpio_pin_cfg(PIOD_5, PIOD_5_SPI1_SCK  | AM_GPIO_PULLUP );
    am_gpio_pin_cfg(PIOD_6, PIOD_6_SPI1_MOSI | AM_GPIO_PULLUP);
    am_gpio_pin_cfg(PIOD_7, PIOD_7_SPI1_MISO | AM_GPIO_PULLUP);
}

/**
 * \brief SPI0从机初始化（8bit数据宽度 SPI_MODE1）
 */
void spi_slve_init (amhw_spi_t *p_hw_spi)
{
    /* 配置SPI为从机 */
    amhw_spi_workmode_cfg(p_hw_spi, AMHW_SPI_WMODE_SLAVE);

    /* 配置8bit数据宽度 */
    amhw_spi_feature_cfg(p_hw_spi, AMHW_SPI_CFG_8BIT_WIDTH);

    /* 配置时钟相位和极性 */
    amhw_spi_mode_cfg(p_hw_spi, AMHW_SPI_MODE_1);

    /* 使能SPI模块 */
    amhw_spi_enable(p_hw_spi);
}

/**
 * \brief SPI1从机初始化（8bit数据宽度 SPI_MODE1）
 */
void spi_master_init (amhw_spi_t *p_hw_spi)
{
    /* 配置SPI为主机 */
    amhw_spi_workmode_cfg(p_hw_spi, AMHW_SPI_WMODE_MASTER);

    /* 配置8bit数据宽度 */
    amhw_spi_feature_cfg(p_hw_spi, AMHW_SPI_CFG_8BIT_WIDTH);

    /* 配置时钟相位和极性 */
    amhw_spi_mode_cfg(p_hw_spi, AMHW_SPI_MODE_1);
    
    /* 配置CS引脚为片选输出 */
    amhw_spi_cs_cfg(p_hw_spi, AMHW_SPI_CS_SPI_OUT);

    /* 配置传输速度 */
    spi_speed_cfg(p_hw_spi, 3000000);

    /* 使能SPI模块 */
    amhw_spi_enable(p_hw_spi);
}

/** [src_hw_spi_slave_irq] */

/* end of file */
