#ifdef I2C_WIRE

#include <i2c/wire/WireHost.h>
#include <i2c/LoggerConfig.h>

namespace ravensnight::i2c::wire {

WireHost* WireHost::instance = 0;
ClassLogger WireHost::_logger(LC_I2C);

WireHost::WireHost(TwoWire* wire) {
    _wire = wire;
}

/**
 * Initialize the instance.
 */
bool WireHost::install(uint8_t addr) {
    _logger.debug("WireHost::install");

    if (_wire != 0) {
        _wire->onReceive(WireHost::__onReceive);
        _wire->onRequest(WireHost::__onRequest);
        _wire->begin(addr);

        WireHost::instance = this;
        return true;
    }

    return false;
}

void WireHost::skipAll() {
    while (_wire->available()) _wire->read();
}

void WireHost::receive(int bytes) {
    _logger.trace("WireHost::doReceive");

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

void WireHost::transmit() {
    _logger.trace("WireHost::doReply");

    int16_t res = this->buildResponse(_txBuffer, TWIHOST_TX_BUFFER_SIZE);
    if ((res < 0) || (res > TWIHOST_TX_BUFFER_SIZE)) {
        _logger.error("Receiving response data failed.");
        return;
    }  

    res = _wire->write(_txBuffer, res);
    _logger.trace("Sent response to master: bytes=%d", res);    
}

void WireHost::__onReceive(int bytes) {
    WireHost::instance->receive(bytes);
}

void WireHost::__onRequest() {
    WireHost::instance->transmit();
}

}

#endif // I2C_WIRE