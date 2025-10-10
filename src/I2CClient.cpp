#include <i2c/I2CClient.h>
#include <i2c/LoggerConfig.h>

using namespace ravensnight::i2c;

I2CClient::I2CClient(TwoWire* twi, uint8_t hostAddr) {
    _i2c = twi;
    _hostAddr = hostAddr;
}

void I2CClient::skipAllAvailable() {
    while (this->_i2c->available() > 0) {
        _i2c->read();
    }
}

int16_t I2CClient::send(const uint8_t* buffer, uint8_t len) {
    if (_i2c == 0) return -1;

    size_t result = 0;
    skipAllAvailable();

    _i2c->beginTransmission(_hostAddr);
    result = _i2c->write(buffer, len);
    _i2c->endTransmission(1); // send stop

    _logger.trace("Sent %d bytes to host.", result);
    return result;
}

int16_t I2CClient::request(const uint8_t* params, uint8_t paramsLen, uint8_t* response, uint8_t responseLen) {
    if (_i2c == 0) return -1;

    size_t result = 0;
    skipAllAvailable();

    if ((params != 0) && (paramsLen > 0)) {

        _i2c->beginTransmission(_hostAddr);
        result = _i2c->write(params, paramsLen);
        _i2c->endTransmission(0); // send stop

        _logger.trace("Parameters send len = %d", result);
    }

    _logger.trace("Send request.", result);

    _i2c->requestFrom(_hostAddr, responseLen, (uint8_t)1);

    _logger.trace("Ready %d bytes", responseLen);
    return _i2c->readBytes(response, responseLen);
}


void I2CClient::scanAll(TwoWire* conn) {

    uint8_t err;
    for (uint8_t i = 0; i < 127; i++) {
        conn->beginTransmission(i);
        err = conn->endTransmission();

        switch (err) {
            case 0:
                _logger.info("Found device at: %x", i);
                break;

            case 4:
                _logger.warn("Unknown error at %x", i);
                break;

            default:                
                break;
        }
    }
}

void I2CClient::waitFor(TwoWire* conn, const uint8_t addrs[], uint8_t len, uint16_t d) {
    bool available[len];
    uint8_t avl = 0;

    _logger.info("Wait for I2C addresses.");

    for (uint8_t i = 0; i < len; i++) {
        available[i] = false;
    }

    do {
        avl = 0;
        for (uint8_t i = 0; i < len; i++) {
            if (available[i]) {
                avl++;
            } else {
                conn->beginTransmission(addrs[i]);
                if (conn->endTransmission() == 0) {
                    available[i] = true;
                    avl++;
                }
            }
        }
        delay(d);
        
    } while (avl < len);
}

ClassLogger I2CClient::_logger(LC_I2C);