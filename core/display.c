// SPDX-License-Identifier: GPL-2.0
// Author: xunicatt
// Copyright (c) 2026 xunicatt <contact.aniket.biswas@gmail.com>

#include <core/display.h>
#include <drivers/display/tm1637.h>

#define ALPHAS (26)
#define NUMS   (10)

#define IS_ALPHA(c) ((c) >= 'A' && (c) <= 'Z')
#define IS_NUM(c)   ((c) >= '0' && (c) <= '9')

#define MEMSET(buff, c, n) \
do {                       \
    uint16_t i = 0;        \
    while (i < (n)) {      \
        buff[i] = (c);     \
        i++;               \
    }                      \
} while (0)

static const uint8_t alphabets[ALPHAS] = {
    SEG_ALL & ~(SEG_D),                 /* A */
    SEG_ALL & ~(SEG_A | SEG_B),         /* B */
    SEG_ALL & ~(SEG_B | SEG_C | SEG_G), /* C */
    SEG_ALL & ~(SEG_A | SEG_F),         /* D */
    SEG_ALL & ~(SEG_B | SEG_C),         /* E */
    SEG_ALL & ~(SEG_B | SEG_C | SEG_D), /* F */
    SEG_ALL & ~(SEG_B | SEG_G),         /* G */
    SEG_ALL & ~(SEG_A | SEG_D),         /* H */
    SEG_E | SEG_F,                      /* I */
    SEG_ALL & ~(SEG_A | SEG_F | SEG_G), /* J */
    SEG_G,                              /* K */
    SEG_D | SEG_E | SEG_F,              /* L */
    SEG_G,                              /* M */
    SEG_ALL & ~(SEG_D | SEG_G),         /* N */
    SEG_ALL & ~(SEG_G),                 /* O */
    SEG_ALL & ~(SEG_C | SEG_D),         /* P */
    SEG_ALL & ~(SEG_D | SEG_E),         /* Q */
    SEG_E | SEG_F | SEG_G,              /* R */
    SEG_ALL & ~(SEG_B | SEG_E),         /* S */
    SEG_ALL & ~(SEG_A | SEG_B | SEG_C), /* T */
    SEG_ALL & ~(SEG_A | SEG_G),         /* U */
    SEG_G,                              /* M */
    SEG_G,                              /* M */
    SEG_G,                              /* M */
    SEG_ALL & ~(SEG_A | SEG_E),         /* Y */
    SEG_ALL & ~(SEG_C | SEG_F),         /* Z */
};

static const uint8_t numbers[NUMS] = {
    SEG_ALL & ~(SEG_G),                 /* 0 */
    SEG_B | SEG_C,                      /* 1 */
    SEG_ALL & ~(SEG_C | SEG_F),         /* 2 */
    SEG_ALL & ~(SEG_E | SEG_F),         /* 3 */
    SEG_ALL & ~(SEG_A | SEG_D | SEG_E), /* 4 */
    SEG_ALL & ~(SEG_B | SEG_E),         /* 5 */
    SEG_ALL & ~(SEG_B),                 /* 6 */
    SEG_A | SEG_B | SEG_C,              /* 7 */
    SEG_ALL,                            /* 8 */
    SEG_ALL & ~(SEG_E),                 /* 9 */
};

static uint8_t buffer[DIGITS];

void display_init(void) {
    MEMSET(buffer, 0, DIGITS);
}

void display_deinit(void) {
    MEMSET(buffer, 0, DIGITS);
}

void display_print(const char *str, bool flush) {
    uint8_t i = 0;
    uint8_t colon;

    while (str[i] != '\0' && i < DIGITS) {
        colon = buffer[i] & SEG_COLON;
        buffer[i] = display_get_bitmap(str[i]);
        buffer[i] |= colon;

        i++;
    }

    if (flush) {
        display_flush();
    }
}

void display_printn(const char *str, uint16_t len, bool flush) {
    uint8_t i = 0;
    uint8_t colon;

    while (i < len && i < DIGITS) {
        colon = buffer[i] & SEG_COLON;
        buffer[i] = display_get_bitmap(str[i]);
        buffer[i] |= colon;

        i++;
    }

    if (flush) {
        display_flush();
    }
}

void display_printc(const uint8_t pos, const char c, bool flush) {
    uint8_t colon;

    if (pos >= DIGITS) {
        return;
    }

    colon = buffer[pos] & SEG_COLON;
    buffer[pos] = display_get_bitmap(c);
    buffer[pos] |= colon;

    if (flush) {
        display_flush();
    }
}

void display_print_num(uint16_t num, bool flush) {
    uint8_t i = DIGITS;
    uint8_t rem = 0;
    uint8_t colon;

    if (num == 0) {
        display_clear(false);
        display_printc(DIGITS - 1, '0', flush);
        return;
    }

    while (num != 0 && i > 1) {
        rem = num % 10;
        num /= 10;

        colon = buffer[i - 1] & SEG_COLON;
        buffer[i - 1] = display_get_bitmap(rem + '0');
        buffer[i - 1] |= colon;

        i--;
    }

    if (flush) {
        display_flush();
    }
}

void display_shift_left(void) {
    uint8_t i  = 0;
    uint8_t colon;

    while (i < DIGITS - 1) {
        colon = buffer[i] & SEG_COLON;
        buffer[i] = buffer[i + 1];
        buffer[i] |= colon;

        i++;
    }
}

void display_shift_right(void) {
    uint8_t i  = DIGITS - 1;
    uint8_t colon;

    while (i > 1) {
        colon = buffer[i] & SEG_COLON;
        buffer[i] = buffer[i - 1];
        buffer[i] |= colon;

        i--;
    }
}

void display_clear(bool flush) {
    MEMSET(buffer, 0, DIGITS);

    if (flush) {
        display_flush();
    }
}

void display_set_colon(bool show, bool flush) {
    if (show) {
        buffer[1] |= SEG_COLON;
    } else {
        buffer[1] &= ~SEG_COLON;
    }

    if (flush) {
        display_flush();
    }
}

void display_flush(void) {
    tm1637_write_all(buffer, DIGITS);
}

uint8_t display_get_bitmap(char c) {
    if (IS_ALPHA(c)) {
        return alphabets[c - 'A'];
    }

    if (IS_NUM(c)) {
        return numbers[c - '0'];
    }

    if (c == '-') {
        return SEG_G;
    }

    return 0x00;
}
