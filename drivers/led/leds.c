// SPDX-License-Identifier: GPL-2.0
// Author: xunicatt
// Copyright (c) 2026 xunicatt <contact.aniket.biswas@gmail.com>

#include <bsp/bsp.h>
#include <drivers/led/leds.h>

#define LED1_PORT (GPIOD)
#define LED2_PORT (GPIOD)
#define LED3_PORT (GPIOA)

#define LED1_PIN (GPIO_PIN_4)
#define LED2_PIN (GPIO_PIN_3)
#define LED3_PIN (GPIO_PIN_3)

void leds_init(void) {
    PIN_OUT_PP_FAST(LED1_PORT, LED1_PIN);
    PIN_OUT_PP_FAST(LED2_PORT, LED2_PIN);
    PIN_OUT_PP_FAST(LED3_PORT, LED3_PIN);

    /* Enable TIM2 peripheral */
    CLK_PCKENR1 |= CLK_PCKENR1_TIM2;

    /* Disable TIM2 */
    TIM2_CR1 &= ~TIM2_CR1_CEN;

    /* 16Mhz / 16 _ 1 Mhz */
    TIM2_PSCR = 0x04;

    /* Auto reload at: 999 (0 - 999 => 1000 steps) */
    TIM2_ARRH = 0x03;
    TIM2_ARRL = 0xE7;

    /* PWM: 0% */
    TIM2_CCR1H = 0x00;
    TIM2_CCR1L = 0x00;

    /* PWM: 0% */
    TIM2_CCR2H = 0x00;
    TIM2_CCR2L = 0x00;

    /* PWM: 0% */
    TIM2_CCR3H = 0x00;
    TIM2_CCR3L = 0x00;

    /*
     * PWM Mode[6:4] (6 (0b110) << 4)
     * Output Comare Preload Enable[3] (1 << 3)
     * Output mode[1:0] (0)
     */
    TIM2_CCMR1 |= 0x68;

    TIM2_CCER1 |= TIM2_CCER1_CC1E;
    TIM2_CCER1 &= ~TIM2_CCER1_CC1P;

    /*
     * PWM Mode[6:4] (6 (0b110) << 4)
     * Output Comare Preload Enable[3] (1 << 3)
     * Output mode[1:0] (0)
     */
    TIM2_CCMR2 |= 0x68;

    TIM2_CCER1 |= TIM2_CCER1_CC2E;
    TIM2_CCER1 &= ~TIM2_CCER1_CC2P;

    /*
     * PWM Mode[6:4] (6 (0b110) << 4)
     * Output Comare Preload Enable[3] (1 << 3)
     * Output mode[1:0] (0)
     */
    TIM2_CCMR3 |= 0x68;

    TIM2_CCER2 |= TIM2_CCER2_CC3E;
    TIM2_CCER2 &= ~TIM2_CCER2_CC3P;

    TIM2_CR1 |= TIM2_CR1_ARPE;

    /* enabel TIM2 */
    TIM2_CR1 |= TIM2_CR1_CEN;
}

void leds_deinit(void) {
    TIM2_CR1 &= ~TIM2_CR1_CEN;
    PIN_DEFAULT(LED3_PORT, LED3_PIN);
    PIN_DEFAULT(LED2_PORT, LED2_PIN);
    PIN_DEFAULT(LED1_PORT, LED1_PIN);
    CLK_PCKENR1 &= ~CLK_PCKENR1_TIM2;
}

void leds_on(const uint8_t led) {
    leds_set_brightness(led, 1024);
}

void leds_off(const uint8_t led) {
    leds_set_brightness(led, 0);
}

void leds_set_brightness(const uint8_t led, uint16_t brightness) {
    if (led & LED_1) {
        /* Set PWM Duty Cycle */
        TIM2_CCR3H = brightness >> 8;
        TIM2_CCR3L = (uint8_t)brightness;
    }

    if (led & LED_2) {
        /* Set PWM Duty Cycle */
        TIM2_CCR2H = brightness >> 8;
        TIM2_CCR2L = (uint8_t)brightness;
    }

    if (led & LED_3) {
        /* Set PWM Duty Cycle */
        TIM2_CCR1H = brightness >> 8;
        TIM2_CCR1L = (uint8_t)brightness;
    }
}
