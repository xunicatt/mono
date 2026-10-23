// SPDX-License-Identifier: GPL-2.0
// Author: xunicatt
// Copyright (c) 2026 xunicatt <contact.aniket.biswas@gmail.com>

#include <bsp/bsp.h>
#include <core/delay.h>

static uint16_t ms_;

void delay_init(void) {
    /* Enable TIM1 CLK */
    CLK_PCKENR1 |= CLK_PCKENR1_TIM1;

    /* Disbale TIM1 */
    TIM1_CR1 &= ~TIM1_CR1_CEN;
    /* 1 Mhz */
    TIM1_PSCRL = 0x00;
    TIM1_PSCRL = 0x0F;
    /* 999 */
    TIM1_ARRH = 0x03;
    TIM1_ARRL = 0xE7;

    TIM1_CNTRH = 0;
    TIM1_CNTRL = 0;

    /* Enable Auto Reload */
    TIM1_CR1 |= TIM1_CR1_ARPE;
    /* Enable IRQ */
    TIM1_IER |= TIM1_IER_UIE;

    /* Enable TIM1 */
    TIM1_CR1 |= TIM1_CR1_CEN;

    /* Enable TIM4 */
    CLK_PCKENR1 |= CLK_PCKENR1_TIM4;

    TIM4_CR1 &= ~TIM4_CR1_CEN;
    /* Timer clock = 16 MHz / 128 = 125 kHz. */
    TIM4_PSCR = TIM4_PSCR_DIV128;
    /* 125 */
    TIM4_ARR = 124;
    /* Enable TIM4 counter */
    TIM4_CR1 |= TIM4_CR1_CEN;

    ms_ = 0;
}

void delay_deinit(void) {
    TIM1_CR1 &= ~TIM1_CR1_CEN;
    CLK_PCKENR1 &= ~CLK_PCKENR1_TIM1;

    TIM4_CR1 &= ~TIM4_CR1_CEN;
    CLK_PCKENR1 &= ~CLK_PCKENR1_TIM4;
}

void usleep(uint16_t us) {
    const uint16_t start = micros();
    while ((micros() - start) < us) {
        NOP();
    }
}

void msleep(uint16_t ms) {
    const uint16_t start = millis();
    while ((millis() - start) < ms) {
        NOP();
    }
}

void delay(uint16_t ms) {
    while (ms > 0) {
        WAIT(TIM4_SR1, TIM4_SR1_UIF);
        TIM4_SR1 &= ~TIM4_SR1_UIF;
        ms--;
    }
}

uint16_t micros(void) {
    uint16_t cnt;
    CRITICAL(cnt = (TIM1_CNTRH << 8) | TIM1_CNTRL);
    return cnt;
}

uint16_t millis(void) {
    uint16_t ms;
    CRITICAL(ms = ms_);
    return ms;
}

void TIM1_UPD_OVF_IRQHandler(void) __interrupt(11) {
  ms_++;
  TIM1_SR1 &= ~TIM1_SR1_UIF;
}
