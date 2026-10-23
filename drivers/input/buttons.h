// SPDX-License-Identifier: GPL-2.0
// Author: xunicatt
// Copyright (c) 2026 xunicatt <contact.aniket.biswas@gmail.com>

#ifndef INPUT_BUTTONS_H
#define INPUT_BUTTONS_H

#include <bsp/bsp.h>
#include <stdbool.h>
#include <stdint.h>

#define BUTTON_T (0) /* Touch Button */
#define BUTTON_1 (1)
#define BUTTON_2 (2)
#define BUTTON_3 (3)
#define BUTTON_COUNT (4)

#define EVENT_NONE         (0)
#define EVENT_PRESSED      BIT(0)
#define EVENT_LONG_PRESSED BIT(1)

void buttons_init(void);
void buttons_deinit(void);
const uint8_t *buttons_read(void);

#endif
