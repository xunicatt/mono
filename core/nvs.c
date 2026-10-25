// SPDX-License-Identifier: GPL-2.0
// Author: xunicatt
// Copyright (c) 2026 xunicatt <contact.aniket.biswas@gmail.com>

#include <bsp/bsp.h>
#include <core/nvs.h>

#define MAX         (640)
#define EEPROM_BASE (0x004000)

void nvs_lock(void) {
    FLASH_IAPSR &= ~FLASH_IAPSR_DUL;
}

void nvs_unlock(void) {
    if ((FLASH_IAPSR & FLASH_IAPSR_DUL) != 0) {
        return;
    }

    FLASH_DUKR = FLASH_DUKR_KEY1;
    FLASH_DUKR = FLASH_DUKR_KEY2;

    WAIT(FLASH_IAPSR, FLASH_IAPSR_DUL);
}

uint8_t nvs_read(uint16_t index) {
    if (index >= MAX) {
        return -1;
    }

    return *((volatile uint8_t*)(EEPROM_BASE + index));
}

void nvs_write(uint16_t index, uint8_t value) {
    volatile uint8_t *ptr = ((volatile uint8_t *)(EEPROM_BASE + index));
    *ptr = value;

    WAIT(FLASH_IAPSR, FLASH_IAPSR_EOP);
}
