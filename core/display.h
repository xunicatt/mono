// SPDX-License-Identifier: GPL-2.0
// Author: xunicatt
// Copyright (c) 2026 xunicatt <contact.aniket.biswas@gmail.com>

#ifndef DISPLAY_H 
#define DISPLAY_H

#include <stdbool.h>
#include <stdint.h>

#define NUM_TO_C(x) ((x) + '0')
#define C_TO_NUM(x) ((x) - '0')

void display_init(void);
void display_deinit(void);
void display_print(const char *str, bool flush);
void display_printn(const char *str, uint16_t len, bool flush);
void display_printc(const uint8_t pos, const char c, bool flush);
void display_print_num(uint16_t num, bool flush);
void display_shift_left(void);
void display_shift_right(void);
void display_clear(bool flush);
void display_set_colon(bool show, bool flush);
void display_toggle_colon(bool flush);
void display_flush(void);
uint8_t display_get_bitmap(char c);

#endif
