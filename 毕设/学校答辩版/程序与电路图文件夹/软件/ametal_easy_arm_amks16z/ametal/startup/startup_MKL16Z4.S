/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
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
 * \brief MKL16Z4 setup code
 *
 * \internal
 * \par Modification history
 * - 1.00 15-11-01  link, first implementation.
 * \endinternal
 */
 
/*****************************************************************************/
/* Version: GCC for ARM Embedded Processors                                  */
/*****************************************************************************/
    .syntax unified
    .arch armv6-m

    .section .isr_vector, "a"
    .align 2
    .globl __isr_vector
__isr_vector:
    .long   __StackTop                                      /* Top of Stack */
    .long   Reset_Handler                                   /* Reset Handler */
    .long   NMI_Handler                                     /* NMI Handler*/
    .long   HardFault_Handler                               /* Hard Fault Handler*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   SVC_Handler                                     /* SVCall Handler*/
    .long   0                                               /* Reserved*/
    .long   0                                               /* Reserved*/
    .long   PendSV_Handler                                  /* PendSV Handler*/
    .long   SysTick_Handler                                 /* SysTick Handler*/

                                                            /* External Interrupts*/
    .long   am_exc_eint_handler                             /* DMA channel 0 transfer complete and error interrupt*/
    .long   am_exc_eint_handler                             /* DMA channel 1 transfer complete and error interrupt*/
    .long   am_exc_eint_handler                             /* DMA channel 2 transfer complete and error interrupt*/
    .long   am_exc_eint_handler                             /* DMA channel 3 transfer complete and error interrupt*/
    .long   0                                               /* Reserved interrupt*/
    .long   am_exc_eint_handler                             /* FTFA command complete and read collision*/
    .long   am_exc_eint_handler                             /* Low-voltage detect, low-voltage warning*/
    .long   amdr_pmu_llwu_irq_handle                        /* Low Leakage Wakeup*/
    .long   am_exc_eint_handler                             /* I2C0 interrupt*/
    .long   am_exc_eint_handler                             /* I2C1 interrupt*/
    .long   am_exc_eint_handler                             /* SPI0 single interrupt vector for all sources*/
    .long   am_exc_eint_handler                             /* SPI1 single interrupt vector for all sources*/
    .long   am_exc_eint_handler                             /* UART0 status and error*/
    .long   am_exc_eint_handler                             /* UART1 status and error*/
    .long   am_exc_eint_handler                             /* UART2 status and error*/
    .long   am_exc_eint_handler                             /* ADC0 interrupt*/
    .long   am_exc_eint_handler                             /* CMP0 interrupt*/
    .long   am_exc_eint_handler                             /* TPM0 single interrupt vector for all sources*/
    .long   am_exc_eint_handler                             /* TPM1 single interrupt vector for all sources*/
    .long   am_exc_eint_handler                             /* TPM2 single interrupt vector for all sources*/
    .long   am_exc_eint_handler                             /* RTC alarm interrupt*/
    .long   am_exc_eint_handler                             /* RTC seconds interrupt*/
    .long   am_exc_eint_handler                             /* PIT single interrupt vector for all channels*/
    .long   am_exc_eint_handler                             /* I2S0 Single interrupt vector for all sources*/
    .long   0                                               /* Reserved interrupt*/
    .long   am_exc_eint_handler                             /* DAC0 interrupt*/
    .long   am_exc_eint_handler                             /* TSI0 interrupt*/
    .long   am_exc_eint_handler                             /* MCG interrupt*/
    .long   am_exc_eint_handler                             /* LPTMR0 interrupt*/
    .long   0                                               /* Reserved interrupt*/
    .long   am_exc_eint_handler                             /* PORTA pin detect*/
    .long   am_exc_eint_handler                             /* Single interrupt vector for PORTC and PORTD pin detect*/

    .size    __isr_vector, . - __isr_vector

/* Flash Configuration */
    .section .FlashConfig, "a"
    .long 0xFFFFFFFF
    .long 0xFFFFFFFF
    .long 0xFFFFFFFF
    .long 0xFFFFFFFE

    .text
    .thumb

/* Reset Handler */

    .thumb_func
    .align 2
    .globl   Reset_Handler
    .weak    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
    cpsid   i               /* Mask interrupts */
#if 0
    bl SystemInit
#endif
    cpsie   i               /* Unmask interrupts */
/*     Loop to copy data from read only memory to RAM. */

    ldr    r1, =__etext
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

    subs    r3, r2
    ble     .LC0

.LC1:
    subs    r3, 4
    ldr    r0, [r1,r3]
    str    r0, [r2,r3]
    bgt    .LC1
.LC0:

#ifdef __STARTUP_CLEAR_BSS
/*     Loop to zero out BSS section. */
    ldr r1, =__bss_start__
    ldr r2, =__bss_end__

    subs    r2, r1
    ble .LC3

    movs    r0, 0
.LC2:
    str r0, [r1, r2]
    subs    r2, 4
    bge .LC2
.LC3:
#endif
#ifndef __START
#define __START _start
#endif
    bl    __START
    .pool
    .size Reset_Handler, . - Reset_Handler

    .align	1
    .thumb_func
    .weak DefaultISR
    .type DefaultISR, %function
DefaultISR:
    ldr	r0, =DefaultISR
    bx r0
    .size DefaultISR, . - DefaultISR

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro def_irq_handler	handler_name
    .weak \handler_name
    .set  \handler_name, DefaultISR
    .endm

/* Exception Handlers */
    def_irq_handler    NMI_Handler
    def_irq_handler    HardFault_Handler
    def_irq_handler    SVC_Handler
    def_irq_handler    PendSV_Handler
    def_irq_handler    SysTick_Handler
    def_irq_handler    am_exc_eint_handler

    .end
