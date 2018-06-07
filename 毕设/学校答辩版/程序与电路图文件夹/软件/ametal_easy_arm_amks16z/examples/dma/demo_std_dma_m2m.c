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
 * \brief DMA内层到外设例程，由DMA驱动层实现
 *
 * \par 源代码 
 * \snippet demo_std_dma_m2m.c src_std_dma_m2m
 *
 * \internal
 * \par Modification History
 * - 1.00 15-10-16 LXY , first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_std_dma_m2m
 * \copydoc demo_std_dma_m2m.c
 */
 
/** [src_std_dma_m2m] */
#include "ametal.h"
#include "am_board.h"
#include "am_debug.h"
#include "am_delay.h"
#include "am_prj_config.h"

uint8_t g_buf_src[50];          /**< \brief 源端数据缓冲区   */
uint8_t g_buf_dst[50];          /**< \brief 目标端数据缓冲区 */

bool_t  g_trans_done;            /**< \brief 传输完成标志     */

/**
 * \brief 非乒乓传输时，描述符的地址不要求16字节对齐。
 */
amhw_dma_xfer_desc_t g_desc;

/**
 * \brief DMA中断服务程序。
 *
 * \param[in] p_arg : 用户自定义参数，通过 amdr_dma_isr_connect() 函数传递。
 * \param[in] flag  : DMA中断标志，由底层驱动传入，该参数的可能取值：
 *                    (#AMDR_DMA_INT_ERROR) 或 (#AMDR_DMA_INT_NORMAL)。

 * \return    无。
 */
void dma_isr (void *p_arg , uint8_t flag)
{
    int flag_chan  = (int)p_arg;
    if (flag == AMDR_DMA_INT_NORMAL) {

        /* DMA传输完成 */
        switch(flag_chan) {
        case DMA_CHAN_0:g_trans_done = TRUE;
            break;
        case DMA_CHAN_1:
            break;
        case DMA_CHAN_2:
            break;
        case DMA_CHAN_3:
            break;
        default:
            break;
        }
    } else  {
        // 用户自定义执行代码
        switch(flag_chan) {
        case DMA_CHAN_0:
            break;
        case DMA_CHAN_1:
            break;
        case DMA_CHAN_2:
            break;
        case DMA_CHAN_3:
            break;
        default:
            break;
        }
    }
}

/**
 * \brief DMA m2m模式测试程序。
 *
 * \retval  AM_OK     : 传输成功
 * \retval  AM_ERROR  : 传输失败
 */
int dma_m2m_test(void)
{
    uint32_t flags;
    uint32_t i;

    amdr_dma_chan_cfg(DMA_CHAN_0,
                      AMHW_DMA_TRIGGER_DISABLE |       /**< \brief DMA正常模式  */
                      DMA_REQUEST_MUX0_ALWAYSON0);     /**< \brief 请求源                */
                      
    for (i = 0; i < 50; i++) {
        g_buf_src[i] = i;
        g_buf_dst[i] = 0;
    }

    /* 连接DMA中断服务函数 */
    amdr_dma_isr_connect(DMA_CHAN_0, dma_isr, (void *)0);

    /* DMA传输配置 */
    flags = AMHW_DMA_DCR_PER_REQUEST_DISABLE    |  /* 外设请求源禁能   */
            AMHW_DMA_DCR_CYCLE_TRANSFERS        |  /* 循环传输         */
            AMHW_DMA_DCR_AUTO_ALIGN_DISABLE     |  /* 自动对齐禁能     */
            AMHW_DMA_DCR_SOURCE_SIZE_8_BIT      |  /* 源地址1字节      */
            AMHW_DMA_DCR_DESTINATION_SIZE_8_BIT |  /* 目的地址1字节写入*/
            AMHW_DMA_DCR_REQUEST_AFFECTED       |  /* 请求有影响       */
            AMHW_DMA_DCR_NO_LINKING             |  /* 无通道连接       */
            AMHW_DMA_DCR_INTERRUTP_ENABLE       |  /* DMA中断使能      */
            AMHW_DMA_DCR_START_ENABLE    ;         /* DMA开始传输使能  */

    /* 建立通道描述符 */
    amdr_dma_xfer_desc_build(&g_desc,               /* 通道描述符       */
                             (uint32_t)(g_buf_src), /* 源端数据缓冲区   */
                             (uint32_t)(g_buf_dst), /* 目标端数据缓冲区 */
                              50,                   /* 传输字节数       */
                              flags);               /* 传输配置         */

    /* 启动DMA传输，马上开始传输 */
    if (amdr_dma_chan_start(&g_desc,
                            AMHW_DMA_MER_TO_MER,   /* 内存到外设    */
                            DMA_CHAN_0) == AM_ERROR) {
        return AM_ERROR;
    }

    while(g_trans_done == FALSE);                   /* 等待传输完成  */

    for (i = 0; i < 50; i++) {
        if (g_buf_src[i] != g_buf_dst[i]) {
            return AM_ERROR;
        }
    }
    return AM_OK;
}

/**
 * \brief 主函数入口
 */
int main (void)
{  
    am_uart_handle_t  uart_handle;
    
    /* 板级初始化  */
    am_board_init();
    
    amhw_plfm_uart1_init();

    uart_handle = amdr_uart_init(&g_uart1_dev, &g_uart1_devinfo);

    /* 平台初始化 */
    amhw_plfm_dma_init();

    /* DMA初始化 */
    amdr_dma_init(&g_dma_dev, &g_dma_devinfo);

    if (dma_m2m_test() == AM_OK){
        am_uart_poll_send(uart_handle, "transfer success\n", sizeof("transfer success\n"));
    } else {
        am_uart_poll_send(uart_handle, "transfer failure\n", sizeof("transfer failure\n"));
    }
    while (1){

    }
}

/** [src_std_dma_m2m] */
 
/* end of file */

