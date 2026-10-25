// SPDX-License-Identifier: GPL-2.0
// Author: xunicatt
// Copyright (c) 2026 xunicatt <contact.aniket.biswas@gmail.com>

#include <core/display.h>
#include <drivers/display/tm1637.h>

/* Counts */
#define ALPHAS (26)
#define NUMS   (10)

/* Special Characters */
#define SC_SPACE  (~(SEG_ALL)) /* SPACE */
#define SC_HYPHEN (SEG_G)      /* - */

/* Alphabets */
#define ALPHA_A (SEG_ALL & ~(SEG_D))                 /* A */
#define ALPHA_B (SEG_ALL & ~(SEG_A | SEG_B))         /* B */
#define ALPHA_C (SEG_ALL & ~(SEG_B | SEG_C | SEG_G)) /* C */
#define ALPHA_D (SEG_ALL & ~(SEG_A | SEG_F))         /* D */
#define ALPHA_E (SEG_ALL & ~(SEG_B | SEG_C))         /* E */
#define ALPHA_F (SEG_ALL & ~(SEG_B | SEG_C | SEG_D)) /* F */
#define ALPHA_G (SEG_ALL & ~(SEG_B | SEG_G))         /* G */
#define ALPHA_H (SEG_ALL & ~(SEG_A | SEG_D))         /* H */
#define ALPHA_I (SEG_E | SEG_F)                      /* I */
#define ALPHA_J (SEG_ALL & ~(SEG_A | SEG_F | SEG_G)) /* J */
#define ALPHA_K (SC_HYPHEN)                          /* K */
#define ALPHA_L (SEG_D | SEG_E | SEG_F)              /* L */
#define ALPHA_M (SC_HYPHEN)                          /* M */
#define ALPHA_N (SEG_ALL & ~(SEG_D | SEG_G))         /* N */
#define ALPHA_O (SEG_ALL & ~(SEG_G))                 /* O */
#define ALPHA_P (SEG_ALL & ~(SEG_C | SEG_D))         /* P */
#define ALPHA_Q (SEG_ALL & ~(SEG_D | SEG_E))         /* Q */
#define ALPHA_R (SEG_E | SEG_F | SEG_G)              /* R */
#define ALPHA_S (SEG_ALL & ~(SEG_B | SEG_E))         /* S */
#define ALPHA_T (SEG_ALL & ~(SEG_A | SEG_B | SEG_C)) /* T */
#define ALPHA_U (SEG_ALL & ~(SEG_A | SEG_G))         /* U */
#define ALPHA_V (SC_HYPHEN)                          /* V */
#define ALPHA_W (SC_HYPHEN)                          /* W */
#define ALPHA_X (SC_HYPHEN)                          /* X */
#define ALPHA_Y (SEG_ALL & ~(SEG_A | SEG_E))         /* Y */
#define ALPHA_Z (SEG_ALL & ~(SEG_C | SEG_F))         /* Z */

/* Numbers */
#define NUM_0 (SEG_ALL & ~(SEG_G))                 /* 0 */
#define NUM_1 (SEG_B | SEG_C)                      /* 1 */
#define NUM_2 (SEG_ALL & ~(SEG_C | SEG_F))         /* 2 */
#define NUM_3 (SEG_ALL & ~(SEG_E | SEG_F))         /* 3 */
#define NUM_4 (SEG_ALL & ~(SEG_A | SEG_D | SEG_E)) /* 4 */
#define NUM_5 (SEG_ALL & ~(SEG_B | SEG_E))         /* 5 */
#define NUM_6 (SEG_ALL & ~(SEG_B))                 /* 6 */
#define NUM_7 (SEG_A | SEG_B | SEG_C)              /* 7 */
#define NUM_8 (SEG_ALL)                            /* 8 */
#define NUM_9 (SEG_ALL & ~(SEG_E))                 /* 9 */

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
    ALPHA_A,
    ALPHA_B,
    ALPHA_C,
    ALPHA_D,
    ALPHA_E,
    ALPHA_F,
    ALPHA_G,
    ALPHA_H,
    ALPHA_I,
    ALPHA_J,
    ALPHA_K,
    ALPHA_L,
    ALPHA_M,
    ALPHA_N,
    ALPHA_O,
    ALPHA_P,
    ALPHA_Q,
    ALPHA_R,
    ALPHA_S,
    ALPHA_T,
    ALPHA_U,
    ALPHA_V,
    ALPHA_W,
    ALPHA_X,
    ALPHA_Y,
    ALPHA_Z,
};

static const uint8_t numbers[NUMS] = {
    NUM_0,
    NUM_1,
    NUM_2,
    NUM_3,
    NUM_4,
    NUM_5,
    NUM_6,
    NUM_7,
    NUM_8,
    NUM_9,
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

    switch (c) {
    case '-':
        return SC_HYPHEN;

    default: 
        return SC_SPACE;
    }
}
