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
 * \brief ��I2C�ж�ģʽ�µĴӻ���ʾ���̣�ͨ��HW��Ľӿ�ʵ��
 *
 * - ��������:
 *   1. ���ӻ�I2C�ӿں�����I2C�ӿڶ�Ӧ�������ӣ�
 *   2. ���������ص�demo���ϣ��������ϵ��λ��
 *
 * - ʵ������:
 *   1. ������ɣ�LED��200msʱ������˸��
 *
 * \par Դ����
 * \snippet demo_hw_i2c_slave_int.c src_hw_i2c_slave_int
 *
 * \internal
 * \par Modification history
 * - 1.00 15-11-04  LXY , first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_hw_i2c_slave_int
 * \copydoc demo_hw_i2c_slave_int.c
 */

/** [src_hw_i2c_slave_int] */
#include "ametal.h"
#include "am_board.h"
#include "am_prj_config.h"

#define I2C_SLAVE            AMHW_I2C0        /* I2C�ӻ�������ͨ����   */
#define OPERATE_ADDR         0x50             /* ���������ӻ���ַ����*/

volatile uint8_t  g_i2c_slvrxrdyflg  = 0;     /* I2C�ӻ�����׼��������ʶλ */
volatile uint8_t  g_i2c_slvtxrdyflg  = 0;     /* I2C�ӻ�����׼��������ʶλ */


/**
 * \brief I2C�ӻ���ʼ�����ú���
 *
 * \param[in] p_hw_i2c : ָ��I2C�Ĵ������ָ��
 * \param[in] dev_addr : ���������ӻ��ĵ�ַ����
 *
 * \retval  AM_OK  : ��ʼ���������
 */
int i2c_slave_init (amhw_i2c_t *p_hw_i2c, uint8_t dev_addr)
{
    amhw_i2c_disable(p_hw_i2c);

    amhw_i2c_addr1_set(p_hw_i2c, dev_addr, AMHW_I2C_ADDRMODE_BIT7);
    amhw_i2c_addrmode_set(p_hw_i2c , AMHW_I2C_ADDRMODE_BIT7);
    amhw_i2c_enable(p_hw_i2c);
    amhw_i2c_int_enable(I2C_SLAVE, AMHW_I2C_INT_IICIE);

    return AM_OK;
}

/**
 * \brief I2C�ӻ���������
 *
 * \param[in] p_hw_i2c : ָ��I2C�Ĵ������ָ��
 * \param[in] p_txbuf  : ��������/��ַ����
 * \param[in] nbytes   : ���ͻ���ĳ���
 *
 * \retval  AM_OK  : �������
 */
int i2c_slv_send (amhw_i2c_t      *p_hw_i2c,
                  const uint8_t   *p_txbuf,
                  uint32_t         nbytes)
{
    uint8_t i;

    amhw_i2c_int_enable(p_hw_i2c, AMHW_I2C_INT_IICIE);
    for (i = 0; i < nbytes; i++) {
        while (!g_i2c_slvtxrdyflg);
        g_i2c_slvtxrdyflg = 0;

        /* �ӻ�д������ */
        amhw_i2c_data_write(p_hw_i2c, *p_txbuf++);
        amhw_i2c_int_enable(p_hw_i2c, AMHW_I2C_INT_IICIE);
    }

    return AM_OK;
}

/**
 * \brief I2C�ӻ���������
 *
 * \param[in] p_hw_i2c : ָ��I2C�Ĵ������ָ��
 * \param[in] p_rxbuf  : ��������/��ַ����
 * \param[in] nbytes   : ���ջ���ĳ���
 *
 * \retval  AM_OK  : �������
 */
int i2c_slv_recv (amhw_i2c_t *p_hw_i2c,
                  uint8_t    *p_rxbuf,
                  uint32_t    nbytes)
{
    uint8_t i;

    amhw_i2c_int_enable(p_hw_i2c, AMHW_I2C_INT_IICIE);
    for (i = 0; i < nbytes; i++) {

        while (!g_i2c_slvrxrdyflg);
        g_i2c_slvrxrdyflg = 0;
        
        /* ����Ӧ���ź�  */
        amhw_i2c_ack_respond(p_hw_i2c);

        /* �ӻ��������� */
        *p_rxbuf++ = amhw_i2c_data_read(p_hw_i2c);
    }

    return AM_OK;
}

/**
 * \brief I2C�ӻ��жϷ�����
 * \param[in] p_arg : �жϺ�������ڲ���
 * \return   ��
 */
void i2c_slave_isr (void *p_arg)
{
    amhw_i2c_t *p_hw_i2c = (amhw_i2c_t *)p_arg;

    uint32_t i2c_status = amhw_i2c_stat_get(p_hw_i2c);
    amhw_i2c_stat_clr(p_hw_i2c, AMHW_I2C_STAT_IICIF);

    if (i2c_status & AMHW_I2C_STAT_IAAS) {
        if (i2c_status & AMHW_I2C_STAT_SRW) {
            g_i2c_slvtxrdyflg = 1;
            amhw_i2c_transmode_set(p_hw_i2c , AMHW_I2C_TRANSMODE_SEND);
        } else {
            g_i2c_slvrxrdyflg = 1;
            amhw_i2c_transmode_set(p_hw_i2c , AMHW_I2C_TRANSMODE_RECV);
        }
    } else {
        if (i2c_status & AMHW_I2C_STAT_SRW) {

            if (i2c_status & AMHW_I2C_STAT_RXAK) {

                   /* ����ģʽ���ͷ����� */
                   amhw_i2c_transmode_set(p_hw_i2c, AMHW_I2C_TRANSMODE_RECV);
                   amhw_i2c_nak_respond(p_hw_i2c);
                   amhw_i2c_data_read(p_hw_i2c);
            } else {
                g_i2c_slvtxrdyflg = 1;
                amhw_i2c_transmode_set(p_hw_i2c , AMHW_I2C_TRANSMODE_SEND);
            }

        } else {
            g_i2c_slvrxrdyflg = 1;
            amhw_i2c_transmode_set(p_hw_i2c , AMHW_I2C_TRANSMODE_RECV);
        }
    }
}


/**
 * \brief ������
 */
int main (void)
{
    uint8_t           i2c_rxdata_buf[32];
    uint8_t           i2c_rxaddr_buf[32];
    uint32_t          length = 16;

    /* �弶��ʼ�� */
    am_board_init();

    /* I2C0 Ӳ��ƽ̨��ʼ�� */
    amhw_plfm_i2c0_init();

    /* I2C �ӻ���ʼ������ */
    i2c_slave_init(I2C_SLAVE, OPERATE_ADDR);

    am_int_connect(INUM_I2C0,  i2c_slave_isr, (void*)I2C_SLAVE);
    am_int_enable(INUM_I2C0);

    /* �����������͸��ӻ��ĵ�ַ */
    i2c_slv_recv(I2C_SLAVE,
                 (uint8_t *)i2c_rxaddr_buf,
                 1);

    /* �����������͸��ӻ����ӵ�ַ */
    i2c_slv_recv(I2C_SLAVE,
                 (uint8_t *)i2c_rxaddr_buf,
                 1);

    /* �����������͸��ӻ������� */
    i2c_slv_recv(I2C_SLAVE,
                 (uint8_t *)i2c_rxdata_buf,
                 length);

    am_mdelay(100);

    /* �����������͸��ӻ��ĵ�ַ */
    i2c_slv_recv(I2C_SLAVE,
                 (uint8_t *)i2c_rxaddr_buf,
                 1);

    /* �����������͸��ӻ����ӵ�ַ */
    i2c_slv_recv(I2C_SLAVE,
                 (uint8_t *)i2c_rxaddr_buf,
                 1);

    /* ���ʹ��������յ������� */
    i2c_slv_send(I2C_SLAVE,
                 (uint8_t *)i2c_rxdata_buf,
                 length);

    while (1) {
        am_led_on(LED0);
        am_mdelay(200);
        am_led_off(LED0);
        am_mdelay(200);

    }
}
/** [src_hw_i2c_slave_int] */
/* end of file */
