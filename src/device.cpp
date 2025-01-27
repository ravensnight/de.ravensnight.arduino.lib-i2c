#include <i2c/device.h>

I2CDevice::I2CDevice() {
    _i2c = 0;
    _handler = 0;
    _command = Command::GetState;
    _index = 0;
}

void I2CDevice::setup(TwoWire* twi, uint8_t address, I2CDeviceHandler* handler) {
    if (twi == 0) {
        return;
    }

    _i2c = twi;
    _handler = handler;

    _i2c->onReceive(I2CDevice::i2cReceive);
    _i2c->onRequest(I2CDevice::i2cRequest);
    _i2c->begin(address);
}

void I2CDevice::handleRequest() {
    if (_handler == 0) return;
    skipAllAvailable();

    if (_command == Command::GetState) {
        uint16_t state = _handler->getState();
        _i2c->write(0x0FF & (state >> 8));
        _i2c->write(0x0FF & state);
    }
    else if (_command == Command::GetDetails) {
        int8_t len = _handler->getDetailsSize(_index);
        if (len <= 0) return;

        uint8_t buffer[len];
        _handler->getDetails(_index, buffer);
        _i2c->write(buffer, len);
    }
}

void I2CDevice::handleReceive(int bytes) {
    if (_handler == 0) return;

  uint8_t cmd;

  if (bytes > 0) {    
    _command = (Command)(cmd >> 6);
    _index = 0;

    if (_command == Command::GetDetails) {
        _index = cmd & 0x3F;
    }
    else if (_command == Command::SetDetails) {
        _index = cmd & 0x3F;

        int8_t len = _handler->getDetailsSize(_index);
        if (len <= 0) return;

        uint8_t buffer[len];
        size_t r = _i2c->readBytes(buffer, len);

        if (r == (uint8_t)len) {
            _handler->setDetails(_index, buffer);
        }
    }
    else if (_command == Command::Reset) {
        _handler->reset();
    }
  }  

}

void I2CDevice::skipAllAvailable() {
    if (_i2c != 0) {
        while (_i2c->available()) _i2c->read();
    }
}

void I2CDevice::i2cRequest() {
    I2CDevice::instance.handleRequest();
}

void I2CDevice::i2cReceive(int bytes) {
    I2CDevice::instance.handleReceive(bytes);
}

I2CDevice I2CDevice::instance = I2CDevice();