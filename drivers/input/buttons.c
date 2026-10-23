// SPDX-License-Identifier: GPL-2.0
// Author: xunicatt
// Copyright (c) 2026 xunicatt <contact.aniket.biswas@gmail.com>

#include <bsp/bsp.h>
#include <core/delay.h>
#include <drivers/input/buttons.h>

#define SHORT_PRESS_MS (50)
#define LONG_PRESS_MS  (500)

#define PORT  (GPIOC)
#define PIN_T (GPIO_PIN_3)
#define PIN_1 (GPIO_PIN_4)
#define PIN_2 (GPIO_PIN_5)
#define PIN_3 (GPIO_PIN_6)

#define STATE_IDLE         (0)
#define STATE_PRESSED      (1)
#define STATE_WAIT_RELEASE (2)

struct context {
    uint8_t state;
    uint16_t start;
};

static struct context context[BUTTON_COUNT] = {
    { STATE_IDLE, 0 },
    { STATE_IDLE, 0 },
    { STATE_IDLE, 0 },
    { STATE_IDLE, 0 },
};

static uint8_t events[BUTTON_COUNT] = {
    EVENT_NONE,
    EVENT_NONE,
    EVENT_NONE,
    EVENT_NONE,
};

static uint8_t button_read(uint8_t pin, uint8_t button, uint8_t level)
{
    struct context *ctx = &context[button];
    uint16_t now = millis();
    uint8_t pressed = (PIN_GET(PORT, pin) == level);

    switch (ctx->state) {
    case STATE_IDLE:
        if (pressed) {
            ctx->state = STATE_PRESSED;
            ctx->start = now;
        }
        break;

    case STATE_PRESSED: 
        if (!pressed) {
            ctx->state = STATE_IDLE;

            if ((now - ctx->start) >= SHORT_PRESS_MS) {
                return EVENT_PRESSED;
            }
        } else {
            if ((now - ctx->start) >= LONG_PRESS_MS) {
                ctx->state = STATE_WAIT_RELEASE;
                return EVENT_LONG_PRESSED;
            }
        }
        break;

    case STATE_WAIT_RELEASE:
        if (!pressed) {
            ctx->state = STATE_IDLE;
        }
        break;
    }

    return EVENT_NONE;
}

void buttons_init(void)
{
    PIN_IN_PU(PORT, PIN_T);
    PIN_IN_PU(PORT, PIN_1);
    PIN_IN_PU(PORT, PIN_2);
    PIN_IN_PU(PORT, PIN_3);
}

void buttons_deinit(void)
{
    PIN_DEFAULT(PORT, PIN_T);
    PIN_DEFAULT(PORT, PIN_1);
    PIN_DEFAULT(PORT, PIN_2);
    PIN_DEFAULT(PORT, PIN_3);
}

const uint8_t *buttons_read(void)
{
    events[BUTTON_T] = button_read(PIN_T, BUTTON_T, 1);
    events[BUTTON_1] = button_read(PIN_1, BUTTON_1, 0);
    events[BUTTON_2] = button_read(PIN_2, BUTTON_2, 0);
    events[BUTTON_3] = button_read(PIN_3, BUTTON_3, 0);

    return events;
}
