#include <i2c/I2CAutoAddress.h>

using namespace ravensnight::i2c;

I2CAutoAddress::I2CAutoAddress(TwoWire* twi, uint8_t addr, uint16_t reg) {
    _twi = twi;
    _address = addr;
    _register = reg;
}

void I2CAutoAddress::begin() {
    // do nothing
}

void I2CAutoAddress::end() {
    // do nothing
}

int16_t I2CAutoAddress::getAddress() {
    if (_twi == 0) return -1;

    // skip existing bytes
    while (this->_twi->available() > 0) {
        _twi->read();
    }

    // send the register
    _twi->beginTransmission(_address);
    _twi->write(_register);
    _twi->endTransmission();

    // send the address request
    int16_t result = -1;
    _twi->requestFrom((int)_address, (int)1);
    while (_twi->available()) {
        if (result < 0) {
            result = _twi->read();
        } else {
            _twi->read(); // skip
        }
    } 

    return result;
}

void I2CAutoAddress::setNumberOfTries(uint8_t tries) {
    _tries = tries;
}

void I2CAutoAddress::setRetryDelay(uint16_t delay) {
    _delay = delay;
}

uint8_t I2CAutoAddress::getNumberOfTries() {
    return _tries;
}

uint16_t I2CAutoAddress::getRetryDelay() {
    return _delay;
}