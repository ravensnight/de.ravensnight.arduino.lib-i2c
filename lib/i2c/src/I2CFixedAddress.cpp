#include <i2c/I2CFixedAddress.h>

using namespace ravensnight::i2c;

I2CFixedAddress::I2CFixedAddress(uint8_t addr) {
    _address = addr;
}

int16_t I2CFixedAddress::getAddress() const {
    return _address;
}

uint8_t I2CFixedAddress::getNumberOfTries() const {
    return 1;   // shall be enough :)
}

uint16_t I2CFixedAddress::getRetryDelay() const {
    return 1000; // just for safety
}

void I2CFixedAddress::begin() const {
    // do nothing here
}

void I2CFixedAddress::end() const {
    // do nothing here
}

