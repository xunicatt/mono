// SPDX-License-Identifier: GPL-2.0
// Author: xunicatt
// Copyright (c) 2026 xunicatt <contact.aniket.biswas@gmail.com>

#ifndef DISPLAY_TM1637_H
#define DISPLAY_TM1637_H

#include <stdbool.h>
#include <stdint.h>

#define DIGITS (4)
#define MIN_BRIGHTNESS (0)
#define MAX_BRIGHTNESS (7)

#define SEG_A     (0x01)
#define SEG_B     (0x02)
#define SEG_C     (0x04)
#define SEG_D     (0x08)
#define SEG_E     (0x10)
#define SEG_F     (0x20)
#define SEG_G     (0x40)
#define SEG_COLON (0x80)
#define SEG_ALL   (0x7F)

void tm1637_init(void);
void tm1637_deinit(void);
void tm1637_set_brightness(uint8_t brightness);
void tm1637_sleep(bool sleep);
void tm1637_write(uint8_t pos, uint8_t data);
void tm1637_write_all(const uint8_t *data, uint8_t len);

#endif
