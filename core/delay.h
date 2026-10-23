// SPDX-License-Identifier: GPL-2.0
// Author: xunicatt
// Copyright (c) 2026 xunicatt <contact.aniket.biswas@gmail.com>

#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>

void delay_init(void);
void delay_deinit(void);

/* This functions relies on IRQ. So if IRQ is disabled
 * this apis will hang forever */
void usleep(uint16_t us);
void msleep(uint16_t ms);

/* Alternative to usleep/msleep. This function doesn't
 * relies on IRQ */
void delay(uint16_t ms);

uint16_t micros(void);
uint16_t millis(void);

#endif
