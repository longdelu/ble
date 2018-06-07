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
 * \brief SPI从机通过DMA控制器接收数据，通过HW层的接口实现
 *
 * - 操作步骤:
 *   1. 将SPI从机接口和SPI主机接口连接起来；
 *   2. 将程序下载到demo板，重新上电或者复位。
 *
 * - 实验现象:
 *   1. 主机发送数据给从机，从机通过DMA接收数据；
 *   2. 从机检验接收的数据，如果正确，点亮LED，否则LED闪烁报警。
 *
 * \par 源代码
 * \snippet demo_hw_spi_slave_dma.c src_hw_spi_slave_dma
 *
 * \internal
 * \par History
 * - 1.00 15-10-30  liqng, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_hw_spi_slave_dma
 * \copydoc demo_hw_spi_slave_dma.c
 */
 
/** [src_hw_spi_slave_dma] */
#include "ametal.h"
#include "am_board.h"
#include "am_prj_config.h"

#define BUF_LEN     32           /**< \brief 缓冲区长度      */
uint8_t g_buf_recv[BUF_LEN];     /**< \brief 接收缓存区      */
uint8_t g_buf_send[BUF_LEN];     /**< \brief 发送缓存区      */

bool_t g_dma_err_occur = FALSE;  /**< \brief DMA传输错误标识 */
bool_t g_recv_done     = FALSE;  /**< \brief 接收完成标志    */

void spi0_pin_init (void);
void spi1_pin_init (void);
void spi_slve_init (amhw_spi_t *p_hw_spi);
void spi_master_init (amhw_spi_t *p_hw_spi);

/**
 * \brief DMA中断服务服务
 */
void dma_isr (void *p_arg , uint8_t flag)
{
    if(flag == AMDR_DMA_INT_ERROR) {
        g_dma_err_occur = TRUE;     /* DMA错误     */
    } else if(flag == AMDR_DMA_INT_NORMAL) {
        g_recv_done = TRUE;         /* DMA传输完成 */
    }
}

/**
 * \brief SPI0从机DMA接收配置
 */
int spi0_dma_recv_cfg(amhw_spi_t *p_hw_spi)
{
    uint32_t flags;

    int state;

    amhw_dma_xfer_desc_t xfer_desc; /* DAM通道描述符 */

    amdr_dma_chan_cfg(DMA_CHAN_0,
                      AMHW_DMA_TRIGGER_DISABLE |    /**< \brief DMA正常模式   */
                      DMA_REQUEST_MUX0_SPI0RX);     /**< \brief 请求源        */


    /* 连接DMA中断服务函数 */
    amdr_dma_isr_connect(DMA_CHAN_0, dma_isr, (void *)0);

    /* DMA传输配置 */
    flags = AMHW_DMA_DCR_INTERRUTP_ENABLE        |  /* DMA中断使能            */
            AMHW_DMA_DCR_PER_REQUEST_ENABLE      |  /* 外设请求使能           */
            AMHW_DMA_DCR_SINGLE_TRANSFERS        |  /* 单次传输               */
            AMHW_DMA_DCR_SOURCE_SIZE_8_BIT       |  /* 源地址1字节读取        */
            AMHW_DMA_DCR_DESTINATION_SIZE_8_BIT  |  /* 目的地址1字节写入      */
            AMHW_DMA_DCR_REQUEST_AFFECTED        |  /* 请求有影响             */
            AMHW_DMA_DCR_START_DISABLE    ;         /* DMA软件启动使能        */

    /* 建立通道描述符 */
    amdr_dma_xfer_desc_build(&xfer_desc,                  /* 通道描述符       */
                             (uint32_t)(&(p_hw_spi->dl)), /* 源缓冲区首地址   */
                             (uint32_t)(g_buf_recv),      /* 目的缓冲区首地址 */
                              BUF_LEN,                    /* 传输字节数       */
                              flags);                     /* 传输配置         */

    /* 启动DMA传输，马上开始传输 */
    state = amdr_dma_chan_start(&xfer_desc, AMHW_DMA_PER_TO_MER, DMA_CHAN_0);

    if ( state == AM_ERROR) {
        return AM_ERROR;
    }

    return AM_OK;
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
    
    /* DMA相关初始化 */
    amhw_plfm_dma_init();
    amdr_dma_init(&g_dma_dev, &g_dma_devinfo);
    
    /* SPI引脚初始化 */
    spi0_pin_init();
    spi1_pin_init();
    
    /* 开启SPI时钟 */
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_SPI0);
    amhw_sim_periph_clock_enable(AMHW_SIM_SCGC_SPI1);
    
    /* SPI初始化 */
    spi_slve_init(AMHW_SPI0);
    spi_master_init(AMHW_SPI1);
    
    /* SPI0接收DMA配置 */
    spi0_dma_recv_cfg(AMHW_SPI0);
    
    /* 主机发送数据 */
    for (i = 0; i < BUF_LEN; i++) {
        
        g_buf_send[i] = i;
        while((amhw_spi_stat_get(AMHW_SPI1) & AMHW_SPI_STAT_T_EMPTY) == 0);
        amhw_spi_data8_wirte(AMHW_SPI1,g_buf_send[i]);
    }
    
    am_mdelay(1000);
    
    while (1) {

        /* 传输完成 */
        if (g_recv_done) {
            
            /* 检验收到的特征码 */
            for (i = 0; i < BUF_LEN; i++) {
                if (g_buf_send[i] != g_buf_recv[i]) {
                    error_ocur = TRUE;
                    break;
                }
            }
            
            break;
        }

        /* DMA 错误 */
        if (g_dma_err_occur) {
            error_ocur = FALSE;
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
 * \brief SPI0从机初始化(8bit数据宽度 SPI_MODE1,使能DMA接收)
 */
void spi_slve_init (amhw_spi_t *p_hw_spi)
{
    /* 配置SPI为从机 */
    amhw_spi_workmode_cfg(p_hw_spi, AMHW_SPI_WMODE_SLAVE);

    /* 配置8bit数据宽度，使能DMA接收 */
    amhw_spi_feature_cfg(p_hw_spi, AMHW_SPI_CFG_8BIT_WIDTH | AMHW_SPI_CFG_RDMA_EN);

    /* 配置时钟相位和极性 */
    amhw_spi_mode_cfg(p_hw_spi, AMHW_SPI_MODE_1);

    /* 使能SPI模块 */
    amhw_spi_enable(p_hw_spi);
}

/**
 * \brief SPI1从机初始化(8bit数据宽度 SPI_MODE1)
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

/** [src_hw_spi_slave_dma] */

/* end of file */
