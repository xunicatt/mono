// SPDX-License-Identifier: GPL-2.0
// Author: xunicatt
// Copyright (c) 2026 xunicatt <contact.aniket.biswas@gmail.com>

#include <core/delay.h>
#include <core/display.h>
#include <drivers/rtc/ds3231.h>
#include <run/run.h>

void run(void) {
    static struct datetime *dt;
    static char buffer[5];
    buffer[4] = '\0';

    for (;;) {
        dt = ds3231_get();
        buffer[0] = NUM_TO_C(dt->hours / 10);
        buffer[1] = NUM_TO_C(dt->hours % 10);
        buffer[2] = NUM_TO_C(dt->minutes / 10);
        buffer[3] = NUM_TO_C(dt->minutes % 10);

        display_print(buffer, false);
        display_toggle_colon(true);

        msleep(1000);
    }
}
