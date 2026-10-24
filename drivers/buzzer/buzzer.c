// SPDX-License-Identifier: GPL-2.0
// Author: xunicatt
// Copyright (c) 2026 xunicatt <contact.aniket.biswas@gmail.com>

#include <bsp/bsp.h>
#include <drivers/buzzer/buzzer.h>

#define PORT (GPIOC)
#define PIN  (GPIO_PIN_7)

void buzzer_init(void) {
    PIN_OUT_PP(PORT, PIN);
}

void buzzer_deinit(void) {
    PIN_DEFAULT(PORT, PIN);
}

void buzzer_on(void) {
    PIN_SET(PORT, PIN);
}

void buzzer_off(void) {
    PIN_RESET(PORT, PIN);
}
