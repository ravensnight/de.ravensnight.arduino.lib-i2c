#include <i2c/I2CClient.h>
#include <i2c/I2CChecksum.h>
#include <i2c/LoggerConfig.h>

using namespace ravensnight::i2c;

I2CClient::I2CClient(TwoWire* twi, uint8_t hostAddr) {
    _i2c = twi;
    _hostAddr = hostAddr;
    _useChecksum = true;
}

void I2CClient::setUseChecksum(bool checksum) {
    _useChecksum = checksum;
}

bool I2CClient::getUseChecksum() {
    return _useChecksum;
}

void I2CClient::skipAllAvailable() {
    while (this->_i2c->available() > 0) {
        _i2c->read();
    }
}

int16_t I2CClient::transmit(const uint8_t* buffer, uint8_t size, bool stopBit) {
    if (_i2c == 0) return -1;

    size_t sent = 0;
    uint8_t res = 0;
    I2CChecksum checksum;

    skipAllAvailable();

    _i2c->beginTransmission(_hostAddr);    

    for (uint8_t i = 0; i < size; i++) {
        res = _i2c->write(buffer[i]); 
        if (res == 0) break;

        if (_useChecksum) checksum.update(buffer[i]);
        sent++;
    }

    if ((sent == size) && _useChecksum) {
        _i2c->write(checksum.get());
    }

    res = _i2c->endTransmission((uint8_t)stopBit);

    _logger.trace("Sent %d bytes to host. result: %d", sent, res);

    if (res != 0) {
        _i2c->endTransmission(true);
        return -1;
    }

    return sent;
}

int16_t I2CClient::send(const uint8_t* buffer, uint8_t len) {
    return transmit(buffer, len, true);
}

int16_t I2CClient::request(const uint8_t* params, uint8_t paramsLen, uint8_t* response, uint8_t responseLen) {
    if (_i2c == 0) return -1;

    I2CChecksum _checksum;

    int16_t res = 0;
    size_t  len = responseLen;
    if (_useChecksum) len++;

    skipAllAvailable();

    // send the parameters, if there are some
    if ((params != 0) && (paramsLen > 0)) {
        res = transmit(params, paramsLen, false);
        
        if ((res < 0) || (res < paramsLen)) {            
            _logger.warn("Failed sending parameters. result=%d", res);
            return -1;
        }
    }

    // request and read the data.
    _logger.trace("Request response data.");
    _i2c->requestFrom(_hostAddr, len, (uint8_t)true);

    int b;
    for (uint8_t i = 0; i < len; i++) {
        b = _i2c->read();
        if (b < 0) {
            return -1;
        }

        response[i] = (uint8_t)b;
        if (_useChecksum && (i < responseLen)) {     // anything except the checksum byte
            _checksum.update(b);
        }
    }

    if (_useChecksum) {
        _checksum.finalize();

        if (_checksum.get() != (uint8_t)b) {
            _logger.error("Checksum invalid. %d != %d", _checksum.get(), b);
            return -1;
        }
    }

    _logger.trace("Received %d bytes", responseLen);
    return responseLen;
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