// SPDX-License-Identifier: GPL-2.0
// Author: xunicatt
// Copyright (c) 2026 xunicatt <contact.aniket.biswas@gmail.com>

#ifndef RTC_DS3231_H
#define RTC_DS3231_H

#include <stdint.h>

struct datetime {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;

    uint8_t day;
    uint8_t month;
    uint8_t year;
    uint8_t weekday;
};

void ds3231_init(void);
void ds3231_deinit(void);
struct datetime *ds3231_get(void);
void ds3231_set(struct datetime *time);
int8_t ds3231_get_temp(void);

#endif
