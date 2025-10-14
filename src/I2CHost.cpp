#include <i2c/I2CHost.h>
#include <i2c/I2CChecksum.h>
#include <i2c/I2CHostHandler.h>
#include <i2c/LoggerConfig.h>

using namespace ravensnight::i2c;

I2CHost::I2CHost() {
    _i2c = 0;
    _useChecksum = true;
}

void I2CHost::setUseChecksum(bool use) {
    _useChecksum = use;
}

bool I2CHost::getUseChecksum() {
    return _useChecksum;
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
    if ((_handler == 0) || (bytes < 0) || (bytes > I2C_RECEIVE_BUFFER_SIZE)) return;
    
    size_t b = (size_t)bytes;
    I2CChecksum checksum;

    if (_useChecksum) {
        b--;
    }

    _logger.trace("Read %d bytes from i2c.", bytes);

    int16_t res = 0;
    for (size_t i = 0; i < b; i++) {
        res = _i2c->read();
        if (res < 0) {
            _logger.debug("i2cReceive - end of stream.");
            break;
        }
        
        _receiveBuffer[i] = (uint8_t)res;
        if (_useChecksum) checksum.update(res);        
    }
    
    if (_useChecksum) {
        checksum.finalize();

        res = _i2c->read();
        if (res < 0) {
            _logger.debug("i2cReceive - checksum receive failed.");
        }

        if (res != checksum.get()) {
            _logger.info("i2cReceive - Checksum does not match %d <> %d", res, checksum.get());
            return;
        }
    }

    _handler->parse(_receiveBuffer, b);
}

void I2CHost::i2cRequest() {
    if (_handler == 0) return;
    skipAll();

    I2CChecksum checksum;
    if (_handler->canRead()) {
        _logger.trace("Read model data from handler.");
        
        int16_t res = _handler->read(_responseBuffer, I2C_RESPONSE_BUFFER_SIZE);
        for (int16_t i = 0; i < res; i++) {
            if (_i2c->write(_responseBuffer[i]) == 0) {
                _logger.warn("Failed to send I2C response. Break.");
                return;
            }

            if (_useChecksum) checksum.update(_responseBuffer[i]);
        }

        if (_useChecksum) {
            checksum.finalize();
            _i2c->write(checksum.get());
            _logger.trace("Sent response checksum: %d", checksum.get());
        }

        _logger.trace("Sent response to master: bytes=%d", res);
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