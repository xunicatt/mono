// SPDX-License-Identifier: GPL-2.0
// Author: xunicatt
// Copyright (c) 2026 xunicatt <contact.aniket.biswas@gmail.com>

#include <bsp/bsp.h>
#include <core/delay.h>
#include <drivers/display/tm1637.h>

#define PORT     (GPIOA)
#define CLK_PIN  (GPIO_PIN_1)
#define DATA_PIN (GPIO_PIN_2)

#define CMD_ADDRS (0xC0)
#define CMD_ADDRS_AUTO_INC (0x00)
#define CMD_ADDRS_FIXED (0x04)

#define CMD_DATA  (0x40)
#define CMD_WRITE (0x00)
#define CMD_READ  (0x02)

#define CMD_DISPCTRL          (0x80)
#define CMD_DISPCTRL_DISP_OFF (0x00)
#define CMD_DISPCTRL_DISP_ON  (0x08)

static uint8_t state_;
static uint8_t brightness_;

static void tm1637_start(void) {
    PIN_SET(PORT, CLK_PIN);
    PIN_SET(PORT, DATA_PIN);
    usleep(2);
    PIN_RESET(PORT, DATA_PIN);
}

static void tm1637_stop(void) {
    PIN_RESET(PORT, CLK_PIN);
    usleep(2);
    PIN_RESET(PORT, DATA_PIN);
    usleep(2);
    PIN_SET(PORT, CLK_PIN);
    usleep(2);
    PIN_SET(PORT, DATA_PIN);
}

static void tm1637_wait_for_ack(void) {
    PIN_RESET(PORT, CLK_PIN);
    usleep(5);

    /* TODO: check for data pin low */

    PIN_SET(PORT, CLK_PIN);
    usleep(2);
    PIN_RESET(PORT, CLK_PIN);
}

static void tm1637_write_byte(uint8_t byte) {
    uint8_t i = 0;

    while (i < 8) {
        PIN_RESET(PORT, CLK_PIN);

        if (byte & 0x1) {
            PIN_SET(PORT, DATA_PIN);
        } else {
            PIN_RESET(PORT, DATA_PIN);
        }

        usleep(3);
        byte >>= 1;

        PIN_SET(PORT, CLK_PIN);
        usleep(3);
        i++;
    }
}

void tm1637_init(void) {
    state_ = CMD_DISPCTRL_DISP_ON;

    PIN_OUT_PP_FAST(PORT, CLK_PIN);
    PIN_OUT_PP_FAST(PORT, DATA_PIN);

    tm1637_start();
    tm1637_write_byte(CMD_DATA | CMD_ADDRS_AUTO_INC);
    tm1637_wait_for_ack();
    tm1637_stop();

    tm1637_set_brightness(MAX_BRIGHTNESS);
}

void tm1637_deinit(void) {
    PIN_DEFAULT(PORT, DATA_PIN);
    PIN_DEFAULT(PORT, CLK_PIN);
}

void tm1637_set_brightness(uint8_t brightness) {
    brightness_ = brightness;

    tm1637_start();
    tm1637_write_byte(CMD_DISPCTRL | state_ | brightness_);
    tm1637_wait_for_ack();
    tm1637_stop();
}

void tm1637_sleep(bool sleep) {
    state_ = sleep ? CMD_DISPCTRL_DISP_OFF : CMD_DISPCTRL_DISP_ON;

    tm1637_start();
    tm1637_write_byte(CMD_DISPCTRL | state_ | brightness_);
    tm1637_wait_for_ack();
    tm1637_stop();
}

void tm1637_write(uint8_t pos, uint8_t data) {
    if (pos >= DIGITS) {
        return;
    }

    tm1637_start();
    tm1637_write_byte(CMD_DATA | CMD_ADDRS_FIXED | CMD_WRITE);
    tm1637_wait_for_ack();
    tm1637_stop();

    tm1637_start();
    tm1637_write_byte(CMD_ADDRS | pos);
    tm1637_wait_for_ack();

    tm1637_write_byte(data);
    tm1637_wait_for_ack();
    tm1637_stop();
}

void tm1637_write_all(const uint8_t *data, uint8_t len) {
    uint8_t i = 0;

    if (len > DIGITS) {
        return;
    }

    tm1637_start();
    tm1637_write_byte(CMD_DATA | CMD_ADDRS_AUTO_INC | CMD_WRITE);
    tm1637_wait_for_ack();
    tm1637_stop();

    tm1637_start();
    tm1637_write_byte(CMD_ADDRS | i);
    tm1637_wait_for_ack();

    while (i < len) {
        tm1637_write_byte(data[i]);
        tm1637_wait_for_ack();
        i++;
    }

    tm1637_stop();
}
