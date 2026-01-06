#include <i2c/configure.h>
#if I2C_IMPL == 1

#include <assert.h>
#include <i2c/wire/WireDevice.h>
#include <i2c/LoggerConfig.h>

namespace ravensnight::i2c::wire {

WireDevice* WireDevice::instance = 0;
Logger WireDevice::_logger(LC_I2C);

WireDevice::WireDevice(TwoWire* wire) {
    assert(wire != 0);

    _wire = wire; 
    _addr = INVALID_ADDR;
}

/**
 * Initialize the instance.
 */
bool WireDevice::install(uint8_t addr) {
    _logger.debug("WireDevice::install");

    if (_addr != INVALID_ADDR) {
        _wire->end();
    } 
    else {
        _wire->onReceive(WireDevice::__onReceive);
        _wire->onRequest(WireDevice::__onRequest);
    }

    bool res = true;
    #ifdef __AVR__
        _wire->begin(addr);
    #else
        res = _wire->begin(addr);
    #endif
    
    if (res) {
        _addr = addr;
        WireDevice::instance = this;
    }

    return res;
}

void WireDevice::skipAll() {
    while (_wire->available()) _wire->read();
}

void WireDevice::receive(int bytes) {
    _logger.trace("WireDevice::doReceive");

    if ((bytes < 0) || (bytes > TWIHOST_RX_BUFFER_SIZE)) return;
    
    _logger.trace("doReceive - got %d bytes from i2c.", bytes);

    int16_t res = 0;
    uint8_t len = 0;

    while (_wire->available() && (len < bytes)) {
        res = _wire->read();
        if (res < 0) {
            _logger.error("doReceive - end of stream. stop reading.");
            break;
        }

        _rxBuffer[len] = res;
        len++;
    }

    skipAll(); // read potentially pending bytes.

    if (res < 0) {
        _logger.error("Failed to receive all bytes.");
        return;
    }

    _logger.trace("doReceive - call parseRequest.");
    this->parseRequest(_rxBuffer, len);
}

void WireDevice::transmit() {
    _logger.trace("WireDevice::doReply");

    int16_t res = this->buildResponse(_txBuffer, TWIHOST_TX_BUFFER_SIZE);
    if ((res < 0) || (res > TWIHOST_TX_BUFFER_SIZE)) {
        _logger.error("Receiving response data failed.");
        return;
    }  

    res = _wire->write(_txBuffer, res);
    _logger.trace("Sent response to master: bytes=%d", res);    
}

void WireDevice::__onReceive(int bytes) {
    WireDevice::instance->receive(bytes);
}

void WireDevice::__onRequest() {
    WireDevice::instance->transmit();
}

}

#endif // I2C_WIRE