#include "i2c.h"
#include "ds1307.h"

unsigned char dec2bcd(unsigned char value) {
    return ((value / 10 * 16) + (value % 10));
}

unsigned char bcd2dec(unsigned char value) {
    return ((value / 16 * 10) + (value % 16));
}

void dsInit(void) {
    i2cInit();
}

void dsStartClock(void) {
    int seconds;
    seconds = dsReadData(SEC);
    dsWriteData(0x7f & seconds, SEC);
    return;
}

void dsWriteData(unsigned char value, unsigned char address) {
    i2cWriteByte(1, 0, DS1307_CTRL_ID | I2C_WRITE);
    i2cWriteByte(0, 0, address);
    i2cWriteByte(0, 1, value);
}

int dsReadData(unsigned char address) {
    int result;
    i2cWriteByte(1, 0, DS1307_CTRL_ID | I2C_WRITE);
    i2cWriteByte(0, 0, address);
    i2cWriteByte(1, 0, DS1307_CTRL_ID | I2C_READ);
    result = i2cReadByte(1, 1);
    return result;
}
