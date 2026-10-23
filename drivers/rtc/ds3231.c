// SPDX-License-Identifier: GPL-2.0
// Author: xunicatt
// Copyright (c) 2026 xunicatt <contact.aniket.biswas@gmail.com>

#include <bsp/bsp.h>
#include <drivers/rtc/ds3231.h>

#define PORT    (GPIOB)
#define SCL_PIN (GPIO_PIN_4)
#define SDA_PIN (GPIO_PIN_5)

#define ADDRS ((uint8_t)0x68)

#define REG_SECONDS  (0x00)
#define REG_MINUTES  (0x01)
#define REG_HOURS    (0x02)
#define REG_DAY      (0x03)
#define REG_DATE     (0x04)
#define REG_MONTH    (0x05)
#define REG_YEAR     (0x06)
#define REG_TEMP_MSB (0x11)
#define REG_TEMP_LSB (0x12)

static struct datetime dt;

static void i2c_start(uint8_t addr, bool read) {
    volatile uint8_t reg8 = 0;
    UNUSED(reg8);

    /* send start */
    I2C_CR2 |= I2C_CR2_START;
    /* wait for master mode select i.e, start bit set */
    WAIT(I2C_SR1, I2C_SR1_SB);

    /* store the addrs with read or write */
    I2C_DR = (addr << 1) | read;
    /* mode selected */
    WAIT(I2C_SR1, I2C_SR1_ADDR);

    /* clear registers by reading them */
    reg8 = I2C_SR1;
    reg8 = I2C_SR3;
}

static void i2c_write(uint8_t data) {
    /* write the data */
    I2C_DR = data;
    /* wait to DR to be empty */
    WAIT(I2C_SR1, I2C_SR1_TXE);
}

static uint8_t i2c_read(bool ack)  {
    if (ack) {
        I2C_CR2 |= I2C_CR2_ACK;
    } else {
        I2C_CR2 &= ~I2C_CR2_ACK;
    }

    /* wait to recieve to finish */
    WAIT(I2C_SR1, I2C_SR1_RXNE);
    /* read the data register back */
    return I2C_DR;
}

static inline void i2c_stop(void) {
    /* send stop */
    I2C_CR2 |= I2C_CR2_STOP;
}

static inline uint8_t bcd_to_dec(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

static inline uint8_t dec_to_bcd(uint8_t dec) {
    return ((dec / 10) << 4) | (dec % 10);
}

void ds3231_init(void) {
    /* Enable I2C Peripheral */
    CLK_PCKENR1 |= CLK_PCKENR1_I2C;

    /* Disable I2C */
    I2C_CR1 &= ~I2C_CR1_PE;
    /* I2C freq 16mhz */
    I2C_FREQR = 16;

    /* From datasheet (fMaster = 16):
     * - Actual Hs  % Error  I2C_CCR  Duty Cycle Bit
     * - 10256.41   2.56     0x0D      0
     */
    I2C_CCRL = 0x0D;
    I2C_CCRH = I2C_CCRH_FS;

    /*
     * MAX 400khz tRise is 300ns
     *  - tMaster = 1/16 ~ 62.5ns
     * (300ns / 62.5ns + 1) = 5.8 ~ 5
     */
    I2C_TRISER = 0x05;
    /* Enable I2C back */
    I2C_CR1 |= I2C_CR1_PE;
}

void ds3231_deinit(void) {
    /* Disbale I2C */
    I2C_CR1 &= ~I2C_CR1_PE;
    /* Disable I2C Peripheral */
    CLK_PCKENR1 &= ~CLK_PCKENR1_I2C;
}

struct datetime *ds3231_get(void) {
    i2c_start(ADDRS, false);
    i2c_write(REG_SECONDS);
    i2c_stop();

    i2c_start(ADDRS, true);
    dt.seconds = bcd_to_dec(i2c_read(true) & 0x7F);
    dt.minutes = bcd_to_dec(i2c_read(true));
    dt.hours   = bcd_to_dec(i2c_read(true) & 0x3F);
    dt.weekday = bcd_to_dec(i2c_read(true));
    dt.day     = bcd_to_dec(i2c_read(true));
    dt.month   = bcd_to_dec(i2c_read(true) & 0x1F);
    dt.year    = bcd_to_dec(i2c_read(false));
    i2c_stop();

    return &dt;
}

void ds3231_set(struct datetime* dt) {
    i2c_start(ADDRS, false);

    i2c_write(REG_SECONDS);
    i2c_write(dec_to_bcd(dt->seconds));
    i2c_write(dec_to_bcd(dt->minutes));
    i2c_write(dec_to_bcd(dt->hours) & 0x3F);
    i2c_write(dec_to_bcd(dt->weekday));
    i2c_write(dec_to_bcd(dt->day));
    i2c_write(dec_to_bcd(dt->month) & 0x1F);
    i2c_write(dec_to_bcd(dt->year));

    i2c_stop();
}

int8_t ds3231_get_temp(void) {
    volatile int8_t temp;

    i2c_start(ADDRS, false);
    i2c_write(REG_TEMP_MSB);
    i2c_stop();

    i2c_start(ADDRS, true);
    temp = i2c_read(false);
    i2c_stop();

    return temp;
}
