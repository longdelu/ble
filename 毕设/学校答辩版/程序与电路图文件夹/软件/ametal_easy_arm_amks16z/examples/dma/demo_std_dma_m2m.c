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
 * \brief DMA�ڲ㵽�������̣���DMA������ʵ��
 *
 * \par Դ���� 
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

uint8_t g_buf_src[50];          /**< \brief Դ�����ݻ�����   */
uint8_t g_buf_dst[50];          /**< \brief Ŀ������ݻ����� */

bool_t  g_trans_done;            /**< \brief ������ɱ�־     */

/**
 * \brief ��ƹ�Ҵ���ʱ���������ĵ�ַ��Ҫ��16�ֽڶ��롣
 */
amhw_dma_xfer_desc_t g_desc;

/**
 * \brief DMA�жϷ������
 *
 * \param[in] p_arg : �û��Զ��������ͨ�� amdr_dma_isr_connect() �������ݡ�
 * \param[in] flag  : DMA�жϱ�־���ɵײ��������룬�ò����Ŀ���ȡֵ��
 *                    (#AMDR_DMA_INT_ERROR) �� (#AMDR_DMA_INT_NORMAL)��

 * \return    �ޡ�
 */
void dma_isr (void *p_arg , uint8_t flag)
{
    int flag_chan  = (int)p_arg;
    if (flag == AMDR_DMA_INT_NORMAL) {

        /* DMA������� */
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
        // �û��Զ���ִ�д���
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
 * \brief DMA m2mģʽ���Գ���
 *
 * \retval  AM_OK     : ����ɹ�
 * \retval  AM_ERROR  : ����ʧ��
 */
int dma_m2m_test(void)
{
    uint32_t flags;
    uint32_t i;

    amdr_dma_chan_cfg(DMA_CHAN_0,
                      AMHW_DMA_TRIGGER_DISABLE |       /**< \brief DMA����ģʽ  */
                      DMA_REQUEST_MUX0_ALWAYSON0);     /**< \brief ����Դ                */
                      
    for (i = 0; i < 50; i++) {
        g_buf_src[i] = i;
        g_buf_dst[i] = 0;
    }

    /* ����DMA�жϷ����� */
    amdr_dma_isr_connect(DMA_CHAN_0, dma_isr, (void *)0);

    /* DMA�������� */
    flags = AMHW_DMA_DCR_PER_REQUEST_DISABLE    |  /* ��������Դ����   */
            AMHW_DMA_DCR_CYCLE_TRANSFERS        |  /* ѭ������         */
            AMHW_DMA_DCR_AUTO_ALIGN_DISABLE     |  /* �Զ��������     */
            AMHW_DMA_DCR_SOURCE_SIZE_8_BIT      |  /* Դ��ַ1�ֽ�      */
            AMHW_DMA_DCR_DESTINATION_SIZE_8_BIT |  /* Ŀ�ĵ�ַ1�ֽ�д��*/
            AMHW_DMA_DCR_REQUEST_AFFECTED       |  /* ������Ӱ��       */
            AMHW_DMA_DCR_NO_LINKING             |  /* ��ͨ������       */
            AMHW_DMA_DCR_INTERRUTP_ENABLE       |  /* DMA�ж�ʹ��      */
            AMHW_DMA_DCR_START_ENABLE    ;         /* DMA��ʼ����ʹ��  */

    /* ����ͨ�������� */
    amdr_dma_xfer_desc_build(&g_desc,               /* ͨ��������       */
                             (uint32_t)(g_buf_src), /* Դ�����ݻ�����   */
                             (uint32_t)(g_buf_dst), /* Ŀ������ݻ����� */
                              50,                   /* �����ֽ���       */
                              flags);               /* ��������         */

    /* ����DMA���䣬���Ͽ�ʼ���� */
    if (amdr_dma_chan_start(&g_desc,
                            AMHW_DMA_MER_TO_MER,   /* �ڴ浽����    */
                            DMA_CHAN_0) == AM_ERROR) {
        return AM_ERROR;
    }

    while(g_trans_done == FALSE);                   /* �ȴ��������  */

    for (i = 0; i < 50; i++) {
        if (g_buf_src[i] != g_buf_dst[i]) {
            return AM_ERROR;
        }
    }
    return AM_OK;
}

/**
 * \brief ���������
 */
int main (void)
{  
    am_uart_handle_t  uart_handle;
    
    /* �弶��ʼ��  */
    am_board_init();
    
    amhw_plfm_uart1_init();

    uart_handle = amdr_uart_init(&g_uart1_dev, &g_uart1_devinfo);

    /* ƽ̨��ʼ�� */
    amhw_plfm_dma_init();

    /* DMA��ʼ�� */
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

