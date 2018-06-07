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
 * \brief 在I2C轮询模式下操作EEPROM，通过HW层的接口实现
 *
 * - 操作步骤:
 *   1. 将I2C接口和EEPROM接口对应引脚连接；
 *   2. 将程序下载到demo板上，并重新上电或复位。
 *
 * - 实验现象:
 *   1. 1.串口打印“transfer success”表明传输成功；
 *
 * \par 源代码
 * \snippet demo_hw_i2c_master.c src_hw_i2c_master
 *
 * \internal
 * \par History
 * - 1.00 15-11-02  LXY , first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_hw_i2c_master
 * \copydoc demo_hw_i2c_master.c
 */

/** [src_hw_i2c_master] */
#include "ametal.h"
#include "am_board.h"
#include "am_prj_config.h"

#define I2C_MASTER      AMHW_I2C0  /**< \brief I2C主机控制器通道号定义   */
#define I2C_SPEED       100000     /**< \brief I2C控制器速度参数定义     */
#define EEPROM_ADDR     0x50       /**< \brief EEPROM设备地址定义        */
#define EEPROM_SUB_ADDR 0x00       /**< \brief EEPROM设备地址定义        */
#define TEST_LEN        0x6        /**< \brief 操作EEPROM的页大小        */

/**
 * \brief I2C主机发送地址/数据
 */
int i2c_mst_send (amhw_i2c_t *p_hw_i2c,
                    uint8_t dev_addr,
                    uint8_t sub_addr,
                    uint8_t *p_buf,
                    uint8_t length)
{
    uint8_t i;

    uint16_t dev_addr_real = dev_addr << 1;

    /* 启动信号 */
    amhw_i2c_start_signal_send(p_hw_i2c);

    /* 设置传输模式 */
    amhw_i2c_transmode_set(p_hw_i2c , AMHW_I2C_TRANSMODE_SEND);

    /* 写入从设备地址   */
    amhw_i2c_data_write(p_hw_i2c , dev_addr_real);

    if (amhw_i2c_stat_get(p_hw_i2c) & AMHW_I2C_STAT_ARBL) {
        amhw_i2c_stat_clr(p_hw_i2c , AMHW_I2C_STAT_ARBL);
        return AM_ERROR;
    }

    /* 等待设备地址传输完成 */
    while (!(amhw_i2c_stat_get(p_hw_i2c) & AMHW_I2C_STAT_IICIF));
    amhw_i2c_stat_clr(p_hw_i2c , AMHW_I2C_STAT_IICIF);

    /* 发送寄存器子地址 */
    amhw_i2c_transmode_set(p_hw_i2c , AMHW_I2C_TRANSMODE_SEND);
    amhw_i2c_data_write(p_hw_i2c ,sub_addr);

    if (amhw_i2c_stat_get(p_hw_i2c) & AMHW_I2C_STAT_ARBL) {
        amhw_i2c_stat_clr(p_hw_i2c , AMHW_I2C_STAT_ARBL);
        return AM_ERROR;
    }

    /* 发送数据 */
    for (i = 0;i < length; i++) {

        while (!(amhw_i2c_stat_get(p_hw_i2c) & AMHW_I2C_STAT_IICIF));
        amhw_i2c_stat_clr(p_hw_i2c , AMHW_I2C_STAT_IICIF);

        amhw_i2c_transmode_set(p_hw_i2c , AMHW_I2C_TRANSMODE_SEND);
        amhw_i2c_data_write(p_hw_i2c , ((uint8_t *)(p_buf))[i]);
    }

    /* 发送完成后，发送停止信号 */
    while (!(amhw_i2c_stat_get(p_hw_i2c) & AMHW_I2C_STAT_IICIF));
    amhw_i2c_stat_clr(p_hw_i2c , AMHW_I2C_STAT_IICIF);
    amhw_i2c_stop_signal_send(p_hw_i2c);
    amhw_i2c_transmode_set(p_hw_i2c, AMHW_I2C_TRANSMODE_RECV);

    return AM_OK;
}

/**
 * \brief  I2C主机接收地址/数据
 */
int i2c_mst_recv (amhw_i2c_t *p_hw_i2c,
        uint8_t dev_addr,
        uint8_t sub_addr,
        uint8_t *p_buf,
        uint8_t length)
{
    uint8_t i;

    uint16_t dev_addr_real = dev_addr << 1;

    amhw_i2c_start_signal_send(p_hw_i2c);

    amhw_i2c_transmode_set(p_hw_i2c , AMHW_I2C_TRANSMODE_SEND);

    /* 写入从设备地址   */
    amhw_i2c_data_write(p_hw_i2c , dev_addr_real);

    if (amhw_i2c_stat_get(p_hw_i2c) & AMHW_I2C_STAT_ARBL) {
        amhw_i2c_stat_clr(p_hw_i2c , AMHW_I2C_STAT_ARBL);
        return AM_ERROR;
    }

    while (!(amhw_i2c_stat_get(p_hw_i2c) & AMHW_I2C_STAT_IICIF));
    amhw_i2c_stat_clr(p_hw_i2c , AMHW_I2C_STAT_IICIF);

    /* 写入子地址 */
    amhw_i2c_transmode_set(p_hw_i2c , AMHW_I2C_TRANSMODE_SEND);
    amhw_i2c_data_write(p_hw_i2c , sub_addr);

    /* 子地址发送完了才不需要检查仲裁丢失，到这里仲裁完毕 */
    if (amhw_i2c_stat_get(p_hw_i2c) & AMHW_I2C_STAT_ARBL) {
        amhw_i2c_stat_clr(p_hw_i2c , AMHW_I2C_STAT_ARBL);
        return AM_ERROR;
    }

    while (!(amhw_i2c_stat_get(p_hw_i2c) & AMHW_I2C_STAT_IICIF));
    amhw_i2c_stat_clr(p_hw_i2c , AMHW_I2C_STAT_IICIF);

    /* 重启信号,写入从设备地址 */
    dev_addr_real |= 0x01;
    amhw_i2c_restart_signal_send(p_hw_i2c);
    amhw_i2c_transmode_set(p_hw_i2c , AMHW_I2C_TRANSMODE_SEND);
    amhw_i2c_data_write(p_hw_i2c , dev_addr_real);

    /* 发送完成后，读一次数据告诉I2C主机去读取数据，此时读取的数据是无效的 */
    while (!(amhw_i2c_stat_get(p_hw_i2c) & AMHW_I2C_STAT_IICIF));
    amhw_i2c_stat_clr(p_hw_i2c , AMHW_I2C_STAT_IICIF);
    amhw_i2c_transmode_set(p_hw_i2c , AMHW_I2C_TRANSMODE_RECV);
    amhw_i2c_data_read(p_hw_i2c);

    for (i = 0; i < length; i++) {

        while (!(amhw_i2c_stat_get(p_hw_i2c) & AMHW_I2C_STAT_IICIF));
        amhw_i2c_stat_clr(p_hw_i2c , AMHW_I2C_STAT_IICIF);

        amhw_i2c_transmode_set(p_hw_i2c , AMHW_I2C_TRANSMODE_RECV);

        if (i == (length - 2)) {
            amhw_i2c_nak_respond(p_hw_i2c);
        } else {
            amhw_i2c_ack_respond(p_hw_i2c);
        }

        /* 发送停止信号 */
        if (i == (length - 1)) {
            amhw_i2c_stop_signal_send(p_hw_i2c);
            amhw_i2c_ack_respond(p_hw_i2c);
        }

        p_buf[i] = amhw_i2c_data_read(p_hw_i2c);
    }

    return AM_OK;
}

/**
 * \brief I2C时钟配置结构体
 */
typedef struct i2c_div_table {
    uint8_t   icr;         /**< \brief I2C 乘数因子        */
    uint16_t  scl_div;     /**< \brief I2C 分频因子        */
} i2c_div_table_t;

/* I2C 时钟分频因子列表  */
const i2c_div_table_t g_div_table[50] = {
    { 0x00, 20 },   { 0x01, 22 },   { 0x02, 24 },   { 0x03, 26 },
    { 0x04, 28 },   { 0x05, 30 },   { 0x09, 32 },   { 0x06, 34 },
    { 0x0a, 36 },   { 0x07, 40 },   { 0x0c, 44 },   { 0x10, 48 },
    { 0x11, 56 },   { 0x12, 64 },   { 0x0f, 68 },   { 0x13, 72 },
    { 0x14, 80 },   { 0x15, 88 },   { 0x19, 96 },   { 0x16, 104 },
    { 0x1a, 112 },  { 0x17, 128 },  { 0x1c, 144 },  { 0x1d, 160 },
    { 0x1e, 192 },  { 0x22, 224 },  { 0x1f, 240 },  { 0x23, 256 },
    { 0x24, 288 },  { 0x25, 320 },  { 0x26, 384 },  { 0x2a, 448 },
    { 0x27, 480 },  { 0x2b, 512 },  { 0x2c, 576 },  { 0x2d, 640 },
    { 0x2e, 768 },  { 0x32, 896 },  { 0x2f, 960 },  { 0x33, 1024 },
    { 0x34, 1152 }, { 0x35, 1280 }, { 0x36, 1536 }, { 0x3a, 1792 },
    { 0x37, 1920 }, { 0x3b, 2048 }, { 0x3c, 2304 }, { 0x3d, 2560 },
    { 0x3e, 3072 }, { 0x3f, 3840 }
};

/**
 * \brief I2C主机速率设置
 *
 * \param[in] p_hw_i2c : 指向I2C寄存器块的指针
 * \param[in] speed    : 主机速度参数
 *
 * \retval  AM_OK  : 设置完成
 */
int i2c_speed_set (amhw_i2c_t *p_hw_i2c, uint32_t speed)
{

    uint8_t  mult , i , multiplier;
    uint32_t source_clk , rate , abs_error;
    uint32_t min_error = 0xffffffff;
    uint32_t best_mult = 0;
    uint32_t best_icr;


    /*  获取当前I2C运行时钟  */
    source_clk = amdr_clk_periph_rate_get(p_hw_i2c);

    if(speed > 1000000) {
        return  -AM_EINVAL;
    }
    for (mult = 0 ; mult <= 2 ; mult++) {
        multiplier = 1 << mult;
        for (i = 0 ; i < 50 ; i++) {
           rate = source_clk / (multiplier * g_div_table[i].scl_div);
           abs_error = speed > rate ? speed - rate : rate - speed;
           if (abs_error < min_error) {
               best_mult = mult;
               best_icr  =  g_div_table[i].icr;
               min_error = abs_error;
           }
           if (min_error == 0) {
               break;
           }
        }
    }

    /* 写入分频因子，设置波特率  */
    amhw_i2c_clk_div_set(p_hw_i2c , (best_mult << 6)|best_icr);
    return AM_OK;
}

/**
 * \brief I2C主机初始化配置
 *
 * \param[in] p_hw_i2c : 指向I2C寄存器块的指针
 * \param[in] speed    : 主机速度参数
 *
 * \retval  AM_OK      : 配置完成
 * \retval  -AM_EINVAL : 参数无效
 */
int i2c_mst_init (amhw_i2c_t *p_hw_i2c, uint32_t speed)
{

    if (p_hw_i2c == NULL) {
        return -AM_EINVAL;
    }

    /* 关闭I2C控制器，配置参数 */
    amhw_i2c_disable(p_hw_i2c);

    /* 设置速率 */
    i2c_speed_set(p_hw_i2c, speed);

    /* 参数配置完成，启动I2C控制器 */
    amhw_i2c_enable(p_hw_i2c);

    amhw_i2c_int_enable(p_hw_i2c, AMHW_I2C_INT_IICIE);

    return AM_OK;
}

/**
 * \brief 主函数
 */
int main (void)
{
    uint8_t eeprom_buf[TEST_LEN] = {0};
    uint8_t i;

    /* 板级初始化 */
    am_board_init();

    /* I2C0 平台相关初始化 */
    amhw_plfm_i2c0_init();

    /* I2C主机初始化配置 */
    i2c_mst_init(I2C_MASTER, I2C_SPEED);

    for (i = 0; i < TEST_LEN; i++) {
        eeprom_buf[i] = i ;
    }

    i2c_mst_send(I2C_MASTER, EEPROM_ADDR, EEPROM_SUB_ADDR, eeprom_buf, TEST_LEN);

    am_mdelay(100);

    for (i = 0; i < TEST_LEN; i++) {
        eeprom_buf[i] = 0;
    }

    /* I2C读数据 */

    i2c_mst_recv(I2C_MASTER, EEPROM_ADDR, EEPROM_SUB_ADDR, eeprom_buf, TEST_LEN);

    am_mdelay(100);

    /* 校验写入和读取的数据是否一致 */
    for (i = 0; i < TEST_LEN; i++) {

        /* 校验失败 */
        if (eeprom_buf[i] != (i)) {
            am_kprintf("transfer failure\n");
            while(1);
        }
    }

    am_kprintf("transfer success\n");

    while(1);
}

/** [src_hw_i2c_master] */
/* end of file */
