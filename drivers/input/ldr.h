// SPDX-License-Identifier: GPL-2.0
// Author: xunicatt
// Copyright (c) 2026 xunicatt <contact.aniket.biswas@gmail.com>

#ifndef INPUT_LDR_H
#define INPUT_LDR_H

#include <stdint.h>

void ldr_init(void);
void ldr_deinit(void);
uint16_t ldr_read(void);

#endif
