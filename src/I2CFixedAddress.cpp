#include <i2c/I2CFixedAddress.h>

using namespace ravensnight::i2c;

I2CFixedAddress::I2CFixedAddress(uint8_t addr) {
    _address = addr;
}

int16_t I2CFixedAddress::getAddress() {
    return _address;
}

uint8_t I2CFixedAddress::getNumberOfTries() {
    return 1;   // shall be enough :)
}

uint16_t I2CFixedAddress::getRetryDelay() {
    return 1000; // just for safety
}

void I2CFixedAddress::begin() {
    // do nothing here
}

void I2CFixedAddress::end() {
    // do nothing here
}

