#include <i2c/client.h>
#include <i2c/LoggerConfig.h>

using namespace ravensnight::logging;
using namespace ravensnight::i2c;

I2CClient::I2CClient() {
    _i2c = 0;
}

void I2CClient::setup(TwoWire* twi, uint8_t address) {
    if (twi == 0) return;

    _i2c = twi;
    _address = address;    
}

void I2CClient::sendStopWithRequest(bool stop) {
    _stop = stop ? 1 : 0;
}

int16_t I2CClient::getState(uint16_t& state) {
    if (_i2c == 0) return -1;

    sendData(Command::GetState, 0, (uint8_t*)0, 0);
    uint8_t size = _i2c->requestFrom(_address, (uint8_t)2, _stop); 
    if (size < 2) {
        return -1;
    }

    state = (_i2c->read() << 8);
    state |= (_i2c->read());

    return 2;
}

int16_t I2CClient::getDetails(uint8_t index, uint8_t& value) {
    uint8_t buffer[1];
    int16_t res = getDetails(index, buffer, 1);

    if (res == 1) {
        value = buffer[0];
    }
    return res;
}

int16_t I2CClient::getDetails(uint8_t index, uint16_t& value) {
    uint8_t buffer[2];
    int16_t res = getDetails(index, buffer, 2);

    if (res == 2) {
        value = (buffer[0] << 8);
        value |= buffer[1];
    }
    return res;
}

int16_t I2CClient::getDetails(uint8_t index, uint8_t buffer[], uint8_t len) {
    if (_i2c == 0) return -1;

    skipAllAvailable();

    sendData(Command::GetDetails, index, (uint8_t*)0, 0);
    uint8_t size = _i2c->requestFrom(_address, (uint8_t)len, _stop);
    
    if (size < len) {
        return -1;
    }

    return _i2c->readBytes(buffer, len);
}

void I2CClient::setDetails(uint8_t index, uint8_t value) {
    uint8_t buffer[1] = { value };
    setDetails(index, buffer, 1);
}

void I2CClient::setDetails(uint8_t index, uint16_t value) {
    uint8_t buffer[2];
    buffer[0] = (uint8_t)(value >> 8);
    buffer[1] = (uint8_t)(value & 0xFF);

    setDetails(index, buffer, 2);
}

void I2CClient::setDetails(uint8_t index, const uint8_t buffer[], uint8_t size) {
    sendData(Command::SetDetails, index, buffer, size);
}

void I2CClient::resetMaster() {
    sendData(Command::Reset, 0, (uint8_t*)0, 0);
}

void I2CClient::sendData(uint8_t command, uint8_t index, const uint8_t buffer[], uint8_t len) {
    if (_i2c == 0) return;

    skipAllAvailable();
    uint8_t reg = (command << 6) | (index & 0x3F);

    _i2c->beginTransmission(_address);
    _i2c->write(reg);
    if (len > 0) {
        _i2c->write(buffer, len);
    }
    _i2c->endTransmission();

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

void I2CClient::skipAllAvailable() {
    while (this->_i2c->available() > 0) {
        _i2c->read();
    }
}

ClassLogger I2CClient::_logger(LC_I2C);