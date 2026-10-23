// SPDX-License-Identifier: GPL-2.0
// Author: xunicatt
// Copyright (c) 2026 xunicatt <contact.aniket.biswas@gmail.com>

#ifndef LED_LEDS_H
#define LED_LEDS_H

#include <bsp/bsp.h>
#include <stdint.h>

#define LED_1 BIT(0)
#define LED_2 BIT(1)
#define LED_3 BIT(2)

void leds_init(void);
void leds_deinit(void);
void leds_on(const uint8_t led);
void leds_off(const uint8_t led);
void leds_set_brightness(const uint8_t led, uint16_t brightness);

#endif
