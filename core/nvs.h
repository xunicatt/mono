// SPDX-License-Identifier: GPL-2.0
// Author: xunicatt
// Copyright (c) 2026 xunicatt <contact.aniket.biswas@gmail.com>

#ifndef NVS_H
#define NVS_H

#include <stdint.h>

void nvs_lock(void);
void nvs_unlock(void);
uint8_t nvs_read(uint16_t index);
void nvs_write(uint16_t index, uint8_t value);

#endif
