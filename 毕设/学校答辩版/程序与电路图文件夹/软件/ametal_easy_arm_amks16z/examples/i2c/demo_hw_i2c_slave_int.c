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
 * \brief 在I2C中断模式下的从机演示例程，通过HW层的接口实现
 *
 * - 操作步骤:
 *   1. 将从机I2C接口和主机I2C接口对应引脚连接；
 *   2. 将程序下载到demo板上，并重新上电或复位。
 *
 * - 实验现象:
 *   1. 传输完成，LED以200ms时间间隔闪烁。
 *
 * \par 源代码
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

#define I2C_SLAVE            AMHW_I2C0        /* I2C从机控制器通道号   */
#define OPERATE_ADDR         0x50             /* 主机操作从机地址定义*/

volatile uint8_t  g_i2c_slvrxrdyflg  = 0;     /* I2C从机接收准备就绪标识位 */
volatile uint8_t  g_i2c_slvtxrdyflg  = 0;     /* I2C从机发送准备就绪标识位 */


/**
 * \brief I2C从机初始化配置函数
 *
 * \param[in] p_hw_i2c : 指向I2C寄存器块的指针
 * \param[in] dev_addr : 主机操作从机的地址参数
 *
 * \retval  AM_OK  : 初始化配置完成
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
 * \brief I2C从机发送数据
 *
 * \param[in] p_hw_i2c : 指向I2C寄存器块的指针
 * \param[in] p_txbuf  : 发送数据/地址缓存
 * \param[in] nbytes   : 发送缓存的长度
 *
 * \retval  AM_OK  : 发送完成
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

        /* 从机写入数据 */
        amhw_i2c_data_write(p_hw_i2c, *p_txbuf++);
        amhw_i2c_int_enable(p_hw_i2c, AMHW_I2C_INT_IICIE);
    }

    return AM_OK;
}

/**
 * \brief I2C从机接收数据
 *
 * \param[in] p_hw_i2c : 指向I2C寄存器块的指针
 * \param[in] p_rxbuf  : 接收数据/地址缓存
 * \param[in] nbytes   : 接收缓存的长度
 *
 * \retval  AM_OK  : 接收完成
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
        
        /* 发送应答信号  */
        amhw_i2c_ack_respond(p_hw_i2c);

        /* 从机接收数据 */
        *p_rxbuf++ = amhw_i2c_data_read(p_hw_i2c);
    }

    return AM_OK;
}

/**
 * \brief I2C从机中断服务函数
 * \param[in] p_arg : 中断函数的入口参数
 * \return   无
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

                   /* 接收模式，释放总线 */
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
 * \brief 主函数
 */
int main (void)
{
    uint8_t           i2c_rxdata_buf[32];
    uint8_t           i2c_rxaddr_buf[32];
    uint32_t          length = 16;

    /* 板级初始化 */
    am_board_init();

    /* I2C0 硬件平台初始化 */
    amhw_plfm_i2c0_init();

    /* I2C 从机初始化配置 */
    i2c_slave_init(I2C_SLAVE, OPERATE_ADDR);

    am_int_connect(INUM_I2C0,  i2c_slave_isr, (void*)I2C_SLAVE);
    am_int_enable(INUM_I2C0);

    /* 接收主机发送给从机的地址 */
    i2c_slv_recv(I2C_SLAVE,
                 (uint8_t *)i2c_rxaddr_buf,
                 1);

    /* 接收主机发送给从机的子地址 */
    i2c_slv_recv(I2C_SLAVE,
                 (uint8_t *)i2c_rxaddr_buf,
                 1);

    /* 接收主机发送给从机的数据 */
    i2c_slv_recv(I2C_SLAVE,
                 (uint8_t *)i2c_rxdata_buf,
                 length);

    am_mdelay(100);

    /* 接收主机发送给从机的地址 */
    i2c_slv_recv(I2C_SLAVE,
                 (uint8_t *)i2c_rxaddr_buf,
                 1);

    /* 接收主机发送给从机的子地址 */
    i2c_slv_recv(I2C_SLAVE,
                 (uint8_t *)i2c_rxaddr_buf,
                 1);

    /* 发送从主机接收到的数据 */
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
