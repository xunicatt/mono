// SPDX-License-Identifier: GPL-2.0
// Author: xunicatt
// Copyright (c) 2026 xunicatt <contact.aniket.biswas@gmail.com>

#include <bsp/bsp.h>
#include <drivers/input/ldr.h>

#define PORT (GPIOD)
#define PIN  (GPIO_PIN_2)

void ldr_init(void) {
    /* Enable ADC Peripheral */
    CLK_PCKENR2 |= CLK_PCKENR2_ADC;

    PIN_IN_FLT(PORT, PIN);

    /* Disable ADC */
    ADC1_CR1 &= ~ADC1_CR1_ADON;
    /* Select Channel 3  */
    ADC1_CSR |= 0x03;
    /* Select Prescaler 8 _ 2Mhz */
    ADC1_CR1 |= (0x04 << 4);
    /* Single Mode Conversion */
    ADC1_CR1 &= ~ADC1_CR1_CONT;
    /* Right alignment */
    ADC1_CR2 |= ADC1_CR2_ALIGN;
    /* Enable ADC */
    ADC1_CR1 |= ADC1_CR1_ADON;
}

void ldr_deinit(void) {
    /* Disable ADC */
    ADC1_CR1 &= ~ADC1_CR1_ADON;
    PIN_DEFAULT(PORT, PIN);
    /* Disable ADC Peripheral */
    CLK_PCKENR2 &= ~CLK_PCKENR2_ADC;
}

uint16_t ldr_read(void) {
    volatile uint8_t lsb, msb;

    ADC1_CR1 |= ADC1_CR1_ADON;
    /* wait for conversion to finish */
    WAIT(ADC1_CSR, ADC1_CSR_EOC);

    CRITICAL(
        lsb = ADC1_DRL;
        msb = ADC1_DRH;
        ADC1_CSR &= ~ADC1_CSR_EOC;
    );

    return ((msb & 0x03) << 8) | lsb;
}
