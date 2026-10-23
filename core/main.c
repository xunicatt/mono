// SPDX-License-Identifier: GPL-2.0
// Author: xunicatt
// Copyright (c) 2026 xunicatt <contact.aniket.biswas@gmail.com>

#include <core/delay.h>
#include <core/irq.h>
#include <init/init.h>
#include <stdint.h>

void main(void) {
    init();

    for (;;) {
        msleep(1000);
    }
} 
