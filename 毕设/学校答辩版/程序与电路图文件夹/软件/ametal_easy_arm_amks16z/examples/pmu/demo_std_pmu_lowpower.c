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
 * \brief KS16系统模式配置演示例程，通过PMU驱动层实现
 *
 * - 操作步骤:
 *   1. 根据串口输出内容提示进行操作；
 *   2. 波特率：31250。
 *
 * \par 源代码
 * \snippet demo_std_pmu_lowpower.c src_std_pmu_lowpower
 *
 * \internal
 * \par History
 * - 1.00 15-10-30  liqing, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_if_std_pmu_lowpower
 * \copydoc demo_std_pmu_lowpower.c
 */

/** [src_std_pmu_lowpower] */
#include "ametal.h"
#include "am_board.h"
#include "am_prj_config.h"

/* 获取一个字符 */
#define GET_CHAR(c) amhw_uart_poll_receive(AMHW_UART0, &(c), 1)

void current_mode_print (void);
void system_mode_sel_print (void);
void uart0_baud_config (void);

/**
 * \brief 按键中断回调函数
 */
void gpio_irq_handle (void *p_arg)
{
    printf("\r\n GPIO Interrupt Occur!\r\n");
}

/**
 * \brief 主函数
 */
int main (void)
{
    uint8_t recv;

    /*LLWU唤醒源信息 */
    amdr_pmu_llwu_src_info_t llwu_src_info; 

    /* 板级初始化 */
    am_board_init();

    /* 配置UART1 波特率 */
    uart0_baud_config();
    
    printf("\r\nChip rest...\r\n");
    
    am_led_on(0);
    am_mdelay(1000);
    am_led_off(0);
    am_mdelay(1000);
    am_led_on(0);
    am_mdelay(1000);
    am_led_off(0);

    /* 配置唤醒引脚 PTB0 下降沿LLWU 唤醒 */
    amdr_pmu_llwu_src_pin_mkinfo(&llwu_src_info,
                                  AMHW_LLWU_PIN_5_PTB0,
                                  AMHW_LLWU_PIN_ARG_FALLING);
    amdr_pmu_llwu_src_cfg(&llwu_src_info);
    am_gpio_pin_cfg(PIOB_0, AM_GPIO_INPUT);
    
    /* 配置一个普通按键中断，等待和停止模式唤醒 */
    am_gpio_trigger_connect(PIOC_0, gpio_irq_handle, (void*)0);
    am_gpio_trigger_cfg(PIOC_0, AM_GPIO_TRIGGER_FALL);
    am_gpio_pin_cfg(PIOC_0, AM_GPIO_INPUT | AM_GPIO_PULLUP);
    am_gpio_trigger_on(PIOC_0);


    while (1) {

        system_mode_sel_print();
        GET_CHAR(recv);

        switch (recv) {

        case 'a':   /* RUN */
            printf("Enter RUN_MODE...\r\n");
            am_mdelay(100);
            if(amdr_pmu_mode_into(AMDR_PMU_MODE_RUN) != AM_OK) {
                printf("Enter RUN_MODE Failed !\r\n");
            } else {
                uart0_baud_config();
            }
            current_mode_print();
            break;

        case 'b':   /* VLPR */
            printf("Enter VLPR_MODE...\r\n");
            am_mdelay(100);
            if(amdr_pmu_mode_into(AMDR_PMU_MODE_VLPR) != AM_OK) {
                printf("Enter VLPR_MODE Failed !\r\n");
            } else {
                uart0_baud_config();
            }
            current_mode_print();
            break;

        case 'c':   /* WAIT */
            printf("Enter WAIT_MODE...\r\n");
            am_mdelay(100); 
            if(amdr_pmu_mode_into(AMDR_PMU_MODE_WAIT) != AM_OK) {
                printf("Enter WAIT_MODE Failed !\r\n");
            } else {
                printf("Wake Up from WAIT_MODE !\r\n");
            }
            current_mode_print();
            break;

        case 'd':   /* VLPW */
            printf("Enter VLPW_MODE...\r\n");
            am_mdelay(100);
            if(amdr_pmu_mode_into(AMDR_PMU_MODE_VLPW) != AM_OK) {
                printf("Enter VLPW_MODE Failed !\r\n");
            } else {
                printf("Wake Up from VLPW_MODE !\r\n");
            }
            current_mode_print();
            break;

        case 'e':   /* STOP2 */
            printf("Enter STOP2_MODE...\r\n");
            am_mdelay(100);
            if(amdr_pmu_mode_into(AMDR_PMU_MODE_STOP2) != AM_OK) {
                printf("Enter STOP2_MODE Failed !\r\n");
            } else {
                printf("Wake Up from STOP2_MODE !\r\n");
            }
            current_mode_print();
            break;

        case 'f':   /* STOP1 */
            printf("Enter STOP1_MODE...\r\n");
            am_mdelay(100);
            if(amdr_pmu_mode_into(AMDR_PMU_MODE_STOP1) != AM_OK) {
                printf("Enter STOP1_MODE Failed !\r\n");
            } else {
                printf("Wake Up from STOP1_MODE !\r\n");
            }
            current_mode_print();
            break;

        case 'g':   /* STOP */
            printf("Enter STOP_MODE...\r\n");
            am_mdelay(100);
            if(amdr_pmu_mode_into(AMDR_PMU_MODE_STOP) != AM_OK) {
                printf("Enter STOP_MODE Failed !\r\n");
            } else {
                //amhw_clock_init();
                //amhw_clock_freq_update();
                printf("Wake Up from STOP_MODE !\r\n");
            }
            current_mode_print();
            break;

        case 'h':   /* VLPS */
            printf("Enter VLPS_MODE...\r\n");
            am_mdelay(100);
            if(amdr_pmu_mode_into(AMDR_PMU_MODE_VLPS) != AM_OK) {
                printf("Enter VLPS_MODE Failed !\r\n");
            } else {
             //   amhw_clock_init();
             //   amhw_clock_freq_update();
                printf("Wake Up from VLPS_MODE !\r\n");
            }
            current_mode_print();
            am_mdelay(1);
            break;

        case 'i':   /* LLS */
            printf("Enter LLS_MODE...\r\n");
            am_mdelay(100);
            if(amdr_pmu_mode_into(AMDR_PMU_MODE_LLS) != AM_OK) {
                printf("Enter LLS_MODE Failed !\r\n");
            } else {
               // amhw_clock_init();
               // amhw_clock_freq_update();
                printf("Wake Up from LLS_MODE !\r\n");
            }
            current_mode_print();
            break;

        case 'j':   /* VLSS3 */
            printf("Enter VLSS3_MODE...\r\n");
            am_mdelay(100);
            if(amdr_pmu_mode_into(AMDR_PMU_MODE_VLLS3) != AM_OK) {
                printf("Enter VLSS3_MODE Failed !\r\n");
            }
            current_mode_print();
            break;

        case 'k':   /* VLLS1 */
            printf("Enter VLLS1_MODE...\r\n");
            am_mdelay(100);
            if(amdr_pmu_mode_into(AMDR_PMU_MODE_VLLS1) != AM_OK) {
                printf("Enter VLLS1_MODE Failed !\r\n");
            }
            current_mode_print();
            break;

        case 'l':   /* VLLS0 */
            printf("Enter VLLS0_MODE...\r\n");
            am_mdelay(100);
            if(amdr_pmu_mode_into(AMDR_PMU_MODE_VLLS0) != AM_OK) {
                printf("Enter VLLS0_MODE Failed !\r\n");
            }
            am_board_init();
            break;

        default:
            printf("\r\n Enter a wrong word!\r\n");
            break;
        }
    }
}
/*
 * 打印当前模式
 */
void current_mode_print  ()
{
    uint8_t rev;

    printf("\r\n\r\n");
    if(amdr_pmu_mode_get() == AMDR_PMU_MODE_RUN) {
        printf("Current mode is RUN_MODE!\r\n");
    } else {
        printf("Current mode is VLPR_MODE!\r\n");
    }
    
    printf("Enter any key to select system mode!");

    GET_CHAR(rev);
}

/*
 * 打印系统模式选择信息
 */
void system_mode_sel_print ()
{
    printf("\r\n\r\n");
    printf("------------------------------------------------------------------------------------\r\n");
    printf("PTC0`s falling edge is used as wake up source from WAIT、VLPW、VLPS and STOPx mode！\r\n");
    printf("PTB0`s falling edge is used as wake up source from LLS、VLLS3、VLLS1和VLSS0 mode！\r\n");
    printf("\r\n---------System-Mode-Select------------\r\n");
    printf("Enter a  into RUN_MODE   \r\n");
    printf("      b  into VLPR_MODE  \r\n");
    printf("      c  into WAIT_MODE  \r\n");
    printf("      d  into VLPW_MODE  \r\n");
    printf("      e  into STOP2_MODE \r\n");
    printf("      f  into STOP1_MODE \r\n");
    printf("      g  into STOP_MODE  \r\n");
    printf("      h  into VLPS_MODE  \r\n");
    printf("      i  into LLS_MODE   \r\n");
    printf("      j  into VLLS3_MODE \r\n");
    printf("      k  into VLLS1_MODE \r\n");
    printf("      l   into VLLS0_MODE \r\n");
    printf("->");
}

void uart0_baud_config (void)
{
    /* 选择UART0内部快速时钟4M  */
    amhw_sim_uart0_src_set(AMHW_SIM_UART0SRC_MCGIRCLK);

    /* 设置串口波特率 */
    amhw_uart_baudrate_set(AMHW_UART0,
                           amdr_clk_periph_rate_get((void *)(AMHW_UART0)),
                           9600);
}

/** [src_std_pmu_lowpower] */

/* end of file */
