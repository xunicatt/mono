// SPDX-License-Identifier: GPL-2.0
// Author: xunicatt
// Copyright (c) 2026 xunicatt <contact.aniket.biswas@gmail.com>

#include <bsp/bsp.h>
#include <core/delay.h>
#include <core/display.h>
#include <core/error.h>
#include <drivers/led/leds.h>

void error(uint8_t code) {
    if (code == ERR_OK) {
        return;
    }

    display_print_num(code, false);
    display_printc(0, 'E', true);

    TRAP();
}

void trap(void) __trap {
    DISIRQ();

    bool state = false;
    for (;;) {
        state = !state;
        if (state) {
            leds_on(LED_2);
        } else {
            leds_off(LED_2);
        }

        delay(250);
    }
}
