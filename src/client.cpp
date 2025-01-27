#include "i2c/client.h"

I2CClient::I2CClient() {
    _i2c = 0;
}

void I2CClient::setup(TwoWire* twi, uint8_t address) {
    if (twi == 0) return;

    _i2c = twi;
    _address = address;    
}

uint16_t I2CClient::getState() {
    if (_i2c == 0) return 0xFFFF;

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

int16_t I2CClient::getDetails(uint8_t index, uint8_t buffer[], uint8_t len) {
    if (_i2c == 0) return -1;

    sendData(Command::GetState, 0, (uint8_t*)0, 0);
    _i2c->requestFrom(_address, (uint8_t)len);

    uint8_t size = _i2c->available(); 
    if (size < len) {
        return -1;
    }

    return _i2c->readBytes(buffer, len);
}

void I2CClient::setDetails(uint8_t index, const uint8_t buffer[], uint8_t size) {
    sendData(Command::SetDetails, index, buffer, size);
}

void I2CClient::resetMaster() {
    sendData(Command::Reset, 0, (uint8_t*)0, 0);
}

void I2CClient::sendData(uint8_t command, uint8_t index, const uint8_t buffer[], uint8_t len) {
    if (_i2c == 0) return;

    uint8_t reg = (command << 6) | (index & 0x3F);

    _i2c->beginTransmission(_address);
    _i2c->write(reg);
    _i2c->write(buffer, len);
    _i2c->endTransmission();

}