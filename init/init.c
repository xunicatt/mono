// SPDX-License-Identifier: GPL-2.0
// Author: xunicatt
// Copyright (c) 2026 xunicatt <contact.aniket.biswas@gmail.com>

#include <bsp/bsp.h>
#include <core/delay.h>
#include <drivers/buzzer/buzzer.h>
#include <drivers/display/tm1637.h>
#include <drivers/input/buttons.h>
#include <drivers/input/ldr.h>
#include <drivers/led/leds.h>
#include <drivers/rtc/ds3231.h>
#include <init/init.h>

static void sysclock_init(void) {
    CLK_ICKR |= CLK_ICKR_HSIEN;
    CLK_ICKR |= CLK_ICKR_LSIEN;
    CLK_ECKR &= ~CLK_ECKR_HSEEN;

    /* Wait for HSI to be stable */
    WAIT(CLK_ICKR, CLK_ICKR_HSIRDY);

    /* Set CLK to 16mhz */
    CLK_CKDIVR = 0x00;

    /* Wait for LSI to be stable */
    WAIT(CLK_ICKR, CLK_ICKR_LSIRDY);

    /* Disable all Peripherals */
    CLK_PCKENR1 = 0x00;
    CLK_PCKENR2 = 0x00;
}

void init(void) {
    sysclock_init();
    delay_init();

    leds_init();
    tm1637_init();
    buzzer_init();
    buttons_init();
    ds3231_init();
    ldr_init();
}
