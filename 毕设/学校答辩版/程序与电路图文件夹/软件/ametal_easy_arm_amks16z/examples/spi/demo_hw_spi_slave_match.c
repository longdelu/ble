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
 * \brief SPI从机匹配中断演示例程，通过HW层的接口实现
 *
 * - 操作步骤:
 *   1. 将SPI从机接口和主机接口对应连接起来；
 *
 * - 实验现象:
 *   1. SPI0从机接收SPI1主机数据，如果数据被匹配，则发送从机特征码给主机；
 *   2. SPI1主机检验收到的特征码是否正确，如果正确点亮LED，否则LED闪烁报警。
 *
 * \par 源代码
 * \snippet demo_hw_spi_slave_match.c src_hw_spi_slave_match
 *
 * \internal
 * \par History
 * - 1.00 15-10-30  liqing, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_hw_spi_slave_match
 * \copydoc demo_hw_spi_slave_match.c
 */
 
/** [src_hw_spi_slave_match] */
#include "ametal.h"
#include "am_board.h"
#include "am_prj_config.h"

#define SLAVE_MATCH_DATA    0x55            /**< \brief 匹配数据      */
#define DATA_LENGTH         0x5             /**< \brief 特征码长度    */

/** \brief 从机特征码    */
const uint8_t g_slv_data[DATA_LENGTH] = {  
   1, 2, 1, 3, 5
};

/** \brief 主机发送缓冲区 */
uint8_t g_master_tx_buf[6] = {SLAVE_MATCH_DATA, 0, 0, 0, 0, 0};

/** \brief 主机接收缓冲区 */
uint8_t g_master_rx_buf[6] = {0};

void spi0_pin_init (void);
void spi1_pin_init (void);
void spi_slve_init (amhw_spi_t *p_hw_spi);
void spi_master_init (amhw_spi_t *p_hw_spi);

/**
 * \brief SPI中断处理函数
 */
void spi_irq_handle (void *p_arg)
{
    static volatile uint8_t pos = 0;               /* 数据位置          */
    static volatile bool_t  match_irq  = TRUE;     /* 匹配中断打开标志   */
    static volatile bool_t  send_irq   = FALSE;    /* 发送中断打开标志   */

    amhw_spi_t *p_hw_spi = (amhw_spi_t*)p_arg;

    /* 匹配中断 */
    if (match_irq && ((amhw_spi_stat_get(p_hw_spi) & AMHW_SPI_STAT_MATCH) != 0 )) {
        
        /* 开始发送特征码配置 */
        p_hw_spi->s = 0x40;
        pos = 0;
        
        amhw_spi_int_disable(p_hw_spi, AMHW_SPI_IRQ_MATCH); /* 关闭匹配中断 */
        match_irq = FALSE;
        
        amhw_spi_int_enable(p_hw_spi, AMHW_SPI_IRQ_T);      /* 开启发送中断 */
        send_irq  = TRUE;
    }

    /* 发送中断 */
    if (send_irq && (amhw_spi_stat_get(p_hw_spi) & AMHW_SPI_STAT_T_EMPTY) != 0 ) {
        
        /* 发送数据 */
        if (pos < DATA_LENGTH) {
            amhw_spi_data8_read(p_hw_spi);
            amhw_spi_data8_wirte(p_hw_spi, g_slv_data[pos++]);
            
        /* 回到最初状态 */
        } else {
            p_hw_spi->s = 0x40;
            amhw_spi_matchdata16_wirte(p_hw_spi, 0x55);
            
            amhw_spi_int_enable(p_hw_spi, AMHW_SPI_IRQ_MATCH);
            match_irq = TRUE;
            
            amhw_spi_int_disable(p_hw_spi, AMHW_SPI_IRQ_T);
            send_irq  = FALSE;
        }
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
    
    /* SPI引脚初始化 */
    spi0_pin_init();
    spi1_pin_init();
    
    /* 开启SPI时钟 */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_SPI0);
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_SPI1);
    
    /* SPI 从机初始化 */
    spi_slve_init(AMHW_SPI0);
    spi_master_init(AMHW_SPI1);

    /* 配置匹配数据(如果配置了8bit数据则，只有低8位有效 ) */
    amhw_spi_matchdata16_wirte(AMHW_SPI0, SLAVE_MATCH_DATA);

    /* 清除匹配标志 */
    AMHW_SPI0->s = 0x40;
    
    /* 开启匹配中断 */
    amhw_spi_int_enable(AMHW_SPI0, AMHW_SPI_IRQ_MATCH);
    am_int_connect(INUM_SPI0, spi_irq_handle, (void*)AMHW_SPI0);
    am_int_enable(INUM_SPI0);
    
    /* 主机发送请求码，并接收特征码 */
    for (i = 0; i < (DATA_LENGTH + 1); i++) {
        while((amhw_spi_stat_get(AMHW_SPI1) & AMHW_SPI_STAT_T_EMPTY) == 0);
        amhw_spi_data8_wirte(AMHW_SPI1,g_master_tx_buf[i]);
        
        while((amhw_spi_stat_get(AMHW_SPI1) & AMHW_SPI_STAT_R_FULL) == 0);
        g_master_rx_buf[i] = amhw_spi_data8_read(AMHW_SPI1);
    }
    
    am_mdelay(1000);
    
    /* 检验收到的特征码 */
    for (i = 0; i < DATA_LENGTH ; i++) {
        if (g_master_rx_buf[i+1] != g_slv_data[i]) {
            error_ocur = TRUE;
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
 * \brief SPI0从机引脚初始化
 */
void spi0_pin_init (void)
{
    /* 必须使用片选,低电平有效 */
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
    /* 配置SPI为从机 */
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

/** [src_hw_spi_slave_match] */

/* end of file */
