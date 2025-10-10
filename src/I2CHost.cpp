#include <i2c/I2CHost.h>
#include <i2c/I2CHostHandler.h>
#include <i2c/LoggerConfig.h>

using namespace ravensnight::i2c;

I2CHost::I2CHost() {
    _i2c = 0;
}

bool I2CHost::start(TwoWire* twi, I2CAddressProvider& address, I2CHostHandler* handler) {
    if (twi == 0) {
        return false;
    }

    int16_t addr = -1;
    address.begin();
    for (uint8_t i = 0; i < address.getNumberOfTries(); i++) {

        addr = address.getAddress();
        if (addr >= 0) {
            break;
        }

        delay(address.getRetryDelay());
    }
    address.end();

    if ((addr < 0) || (addr > 255)) {
        return false;
    }

    _i2c = twi;
    _handler = handler;

    _i2c->onReceive(I2CHost::__i2cReceive);
    _i2c->onRequest(I2CHost::__i2cRequest);

    _logger.trace("Start I2C host (addr: %d)", addr);
    _i2c->begin(addr);

    return true;    
}

void I2CHost::i2cReceive(int bytes) {
    if ((_handler == 0) || (bytes < 0)) return;
    size_t b = (size_t)bytes;

    _logger.trace("Send %d bytes to handler.", bytes);
    _handler->parse(*_i2c, b);
}

void I2CHost::i2cRequest() {
    if (_handler == 0) return;
    skipAll();

    if (_handler->canRead()) {
        _logger.trace("Read model data from handler.");
        _handler->read(*_i2c);
    }
}

void I2CHost::skipAll() {
    if (_i2c != 0) {
        while (_i2c->available()) _i2c->read();
    }
}

void I2CHost::__i2cReceive(int bytes) {
    I2CHost::instance.i2cReceive(bytes);
}

void I2CHost::__i2cRequest() {
    I2CHost::instance.i2cRequest();
}

I2CHost I2CHost::instance;
ClassLogger I2CHost::_logger(LC_I2C);