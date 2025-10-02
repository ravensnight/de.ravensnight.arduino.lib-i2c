#include <i2c/I2CDevice.h>

using namespace ravensnight::i2c;

I2CDevice::I2CDevice() {
    _i2c = 0;
    _handler = 0;
    _command = Command::GetState;
    _index = 0;
}

bool I2CDevice::setup(TwoWire* twi, I2CAddressProvider& address, I2CDeviceHandler* handler) {
    if (twi == 0) {
        return;
    }

    int16_t res = -1;
    address.begin();
    for (uint8_t i = 0; i < address.getNumberOfTries(); i++) {

        res = address.getAddress();
        if (res >= 0) {
            break;
        }

        _delay_ms(address.getRetryDelay());
    }
    address.end();

    if ((res < 0) || (res > 255)) {
        return false;
    }

    _i2c = twi;
    _handler = handler;

    _i2c->onReceive(I2CDevice::i2cReceive);
    _i2c->onRequest(I2CDevice::i2cRequest);
    _i2c->begin(res);

    return true;
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
    if ((_handler == 0) || (bytes < 0)) return;
    size_t b = (size_t)bytes;

    uint8_t buffer[b];
    size_t r = _i2c->readBytes(buffer, b);

    if (r < b) {
        return;
    }

    if (r > 0) {    
        _command = (Command)(buffer[0] >> 6);
        _index = 0;

        if (_command == Command::GetDetails) {
            _index = buffer[0] & 0x3F;
        }
        else if (_command == Command::SetDetails) {
            _index = buffer[0] & 0x3F;

            int8_t len = _handler->getDetailsSize(_index);
            if ((len <= 0) || (len > (bytes + 1))) {
                return;
            }

            if (r == (uint8_t)len) {
                _handler->setDetails(_index, buffer + 1);
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
