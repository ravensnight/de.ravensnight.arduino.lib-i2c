#include "i2c/slave.h"

I2CSlave::I2CSlave() {
    _i2c = 0;
}

void I2CSlave::setup(TwoWire* twi, uint8_t address) {
    if (twi == 0) return;

    _i2c = twi;
    _address = address;    
}

uint16_t I2CSlave::getState() {
    if (_i2c == 0) return;

    sendData(Command::GetState, 0, (uint8_t*)0, 0);
    _i2c->requestFrom(_address, (uint8_t)2);

    uint8_t size = _i2c->available(); 
    if (size < 2) {
        return 0xFFFF;
    }

    uint16_t result = (_i2c->read() << 8);
    result |= (_i2c->read());

    return result;
}

int16_t I2CSlave::getDetails(uint8_t index, uint8_t buffer[], uint8_t len) {
    if (_i2c == 0) return;

    sendData(Command::GetState, 0, (uint8_t*)0, 0);
    _i2c->requestFrom(_address, (uint8_t)len);

    uint8_t size = _i2c->available(); 
    if (size < len) {
        return -1;
    }

    return _i2c->readBytes(buffer, len);
}

void I2CSlave::setDetails(uint8_t index, const uint8_t buffer[], uint8_t size) {
    sendData(Command::SetDetails, index, buffer, size);
}

void I2CSlave::resetMaster() {
    sendData(Command::Reset, 0, (uint8_t*)0, 0);
}

void I2CSlave::sendData(uint8_t command, uint8_t index, const uint8_t buffer[], uint8_t len) {
    if (_i2c == 0) return;

    uint8_t reg = (command << 6) | (index & 0x3F);

    _i2c->beginTransmission(len + 1);
    _i2c->write(reg);
    _i2c->write(buffer, len);
    _i2c->endTransmission();

}