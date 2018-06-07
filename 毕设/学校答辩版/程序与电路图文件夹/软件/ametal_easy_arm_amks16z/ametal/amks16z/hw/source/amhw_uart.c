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
 * \brief ���ڣ�UART�������ӿ�
 *
 * 1. ȫ˫����������׼������(NRZ)��ʽ
 * 2. ���ж���ʹ�ܵ�˫����ɷ���ķ��ͽ������ͽ�����
 * 3. �����ʿɱ�̣� 13 λģ����Ƶ����
 * 4. �ж���������ѯ����
 * 1. �������ݼĴ����ղ��ҷ������
 * 2. �������ݼĴ�����
 * 3. ���չ��ء���ż�������֡�������������
 * 4. ���н��������
 * 5. �����������л��
 * 6. ֧�� LIN ���жϼ��
 * 5. Ӳ����żУ�����ɺͼ��
 * 6. �ɱ�̵� 8 λ��ר 9 λ�ַ�����
 * 7. �ɱ�̵� 1 λ�� 2 λֹͣλ
 * 8. �����߻��ַ��־���ѽ�������
 * 9. ��ѡ�� 13 λ��ֹ������/11 λ��ֹ�����
 * 10. ��ѡ�ķ������������
 * 11. 5 ͨ�� DMA �ӿ�
 *
 * \internal
 * \par Modification History
 * - 1.00 15-07-03  xiongyanming, first implementation.
 * \endinternal
 */
#include "amhw_uart.h"
#include "amhw_amks16z.h"
#include "amdr_clk.h"
/**
 * \brief �Ƚ�����С
 *
 * \param[in] u_clk       : ����ʱ��
 * \param[in] p_baudrate  : ���ڲ�����
 * \param[in] p_osr_val   : osr������
 *
 *
 *
 * \return AM_OK    : �������ʼ������
 *         AM_ERROR : �������ʲ�����
 */
static int __err_cal (uint32_t  uart0clk,
                      uint32_t  baud,
                      uint32_t *p_osr_val)
{
    uint8_t  i               = 0;
    uint32_t calculated_baud = 0;
    uint32_t baud_diff       = 0;
    uint32_t osr_val         = 0;
    uint32_t sbr_val         = 0;

    uint32_t temp            = 0;

    /* ����С�Ĺ�������OSR_VAL�������һ��baud_diff*/
    osr_val      = 4;
    /* �õ����ۼ���Ĳ����� */
    sbr_val = (uint32_t)(uart0clk/(baud * osr_val));
    calculated_baud = (uart0clk / (osr_val * sbr_val));

    /* ����Ĳ�����ƫ�� */
    if (calculated_baud > baud) {
        baud_diff = calculated_baud - baud;
    }  else {
        baud_diff = baud - calculated_baud;
    }

    /* ��������ʵĹ�������osr_val */
    for (i = 5; i <= 32; i++) {
        sbr_val = (uint32_t)(uart0clk/(baud * i));
        calculated_baud = (uart0clk / (i * sbr_val));

        if (calculated_baud > baud){
             temp = calculated_baud - baud;
        }else{
            temp = baud - calculated_baud;
        }
        if (temp <= baud_diff) {
            baud_diff = temp;
            osr_val = i;
        }
    }
    *p_osr_val = osr_val;
    if (baud_diff < ((baud / 100) * 3)) {
           return AM_OK;
    }else {
        return AM_ERROR;
    }

}

/**
 * \brief USART���ݷ���(��ѯģʽ)
 *
 * \param[in] p_hw_usart : ָ�򴮿ڼĴ������ָ��
 * \param[in] p_txbuf    : �������ݻ�����
 * \param[in] nbytes     : �������ݸ���
 *
 * \return �ɹ����͵����ݸ���
 */
uint32_t amhw_uart_poll_send (amhw_uart_t  *p_hw_uart,
                              const uint8_t *p_txbuf,
                              uint32_t       nbytes)
{
    uint32_t len = nbytes;

    while (len--) {
         while(!amhw_uart_stat1_tdre_get(p_hw_uart));
        amhw_uart_txdata_write(p_hw_uart, (uint8_t)(*p_txbuf++));
    }

    return nbytes;
}


/**
 * \brief USART���ݽ���(��ѯģʽ)
 *
 * \param[in] p_hw_usart : ָ�򴮿ڼĴ������ָ��
 * \param[in] p_rxbuf    : �������ݻ�����
 * \param[in] nbytes     : �������ݸ���
 *
 * \return �ɹ����յ����ݸ���
 */
uint32_t amhw_uart_poll_receive (amhw_uart_t *p_hw_uart,
                                 uint8_t      *p_rxbuf,
                                 uint32_t      nbytes)
{
    uint32_t len = nbytes;

    while (len--) {
        while(!amhw_uart_stat1_rdre_get(p_hw_uart));
            *p_rxbuf++ = amhw_uart_rxdata_read(p_hw_uart);
    }

    return nbytes;
}

/**
 * \brief ���ڲ���������
 *
 * \param[in] p_hw_usart : ָ�򴮿ڼĴ������ָ��
 * \param[in] sysclk     : ���ڵ�����ʱ��Ƶ��
 * \param[in] baud       : �������õĴ��ڲ�����
 *
 * \return ����0��ʾʵ�ʵĲ����ʣ�С��0��������ʧ�ܡ�
 */
int amhw_uart_baudrate_set (amhw_uart_t *p_hw_uart, uint32_t sysclk, int baud)
{
    uint32_t osr_val  = 0;
    uint32_t sbr_val  = 0;
    uint32_t uart0clk = 0;
    /* ȷ����Ч�� clock value*/
    if ((sysclk > 50000000) || (sysclk < 32000))
       {
           sysclk = 0;
        /* ��ʼ���쳣������������ϵͳʱ��ֵ��Ч*/
           return AM_ERROR;
       }

    /* uart0 ʱ�ӵ�λΪ HZ */
    uart0clk  = sysclk;
    /* �ڸı�UART1/2�Ĵ���ֵǰ ���շ��ͽ��� */
    amhw_uart_disable(p_hw_uart);

    if(p_hw_uart == AMHW_UART0){
         if(__err_cal(uart0clk,baud,&osr_val) == AM_OK) {
            /* ���OSR��ֵ��4x - 8x֮�䣬ʹ��˫���ز���*/
             if ((osr_val >3) && (osr_val < 9)){
                 p_hw_uart->uart0_c5 |= AMHW_UART0_C5_BOTHEDGE_EN;
                }
             sbr_val = (uint32_t)((uart0clk)/(baud * (osr_val)));
             amhw_uart_c4_osr_set(p_hw_uart,(osr_val-1));
          }else {
              return AM_ERROR;
          }
    }else{
        sbr_val = (uint16_t)((sysclk)/(baud * 16));
    }

    amhw_uart_bdh_sbr_set(p_hw_uart,((sbr_val&0x1F00)>>8));
    amhw_uart_bdl_sbr_set(p_hw_uart,sbr_val & AMHW_UART_BDL_SBR_MASK);

    /* ʹ�ܽ��պʹ��� */
    amhw_uart_enable(p_hw_uart);
    return baud;
}

/**
 * \brief UART���⹦��ģʽ����(�ػ�ģʽ�� ����ģʽ)
 */
void amhw_uart_special_mode_set (amhw_uart_t *p_hw_uart, uint8_t mode)
{
    switch (mode) {

    case AHHW_UART_SPEC_MODE_LOOP:
        p_hw_uart->c1 |= AMHW_UART_C1_LOOPS;
        p_hw_uart->c1 &= ~AMHW_UART_C1_RSRC;
        break;

    case AMHW_UART_SPEC_MODE_SINGLE_WIRE:
        p_hw_uart->c1 |= AMHW_UART_C1_LOOPS;
        p_hw_uart->c1 |= AMHW_UART_C1_RSRC;
        break;

    default:
        break;

    }
}


/* end of file */

