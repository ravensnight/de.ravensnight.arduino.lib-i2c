#ifdef I2C_WIRE

#include <i2c/wire/WireClient.h>
#include <i2c/LoggerConfig.h>

namespace ravensnight::i2c::wire {

WireClient::WireClient(TwoWire* wire) {
    _i2c = wire;
    _hostAddr = 0x00;
}

void WireClient::setup(uint8_t hostAddr) {
    if (_i2c == 0) return;

    _hostAddr = hostAddr;
    _i2c->begin();
}

bool WireClient::start(bool write) {    
    if (_i2c == 0) return false;
    
    if (write) {
        _i2c->beginTransmission(_hostAddr);
        _state = State::write;
    } else {
        if (_state == State::write) {
            _i2c->endTransmission(false);
        }
        _state = State::read;
    }

    return true;
}

int16_t WireClient::read(uint8_t* buffer, uint8_t len) {
    if (_i2c == 0) return -1;

    if (_state != State::read) {
        _logger.error("Invalid state. Cannot start reading.");
        return -1;
    }

    _logger.trace("Request data from slave (addr:%d).", _hostAddr);
    _i2c->requestFrom(_hostAddr, len, (uint8_t)1);
    
    uint16_t res = 0;
    uint8_t pos = 0;
    while (_i2c->available()) {
        if (pos >= len) {
            break;
        }
        
        res = _i2c->read();
        if (res < 0) {
            break;
        }

        buffer[pos] = (uint8_t)res;
        pos++;
    }

    _logger.trace("Read %d bytes.", pos);
    _logger.dump("Buffer received.", buffer, pos, 0);
    return pos;
}

int16_t WireClient::write(const uint8_t* buffer, uint8_t len) {
    if (_state != State::write) {
        _logger.error("Invalid state. Cannot start writing.");
        return -1;
    }

    uint8_t num = 0;
    while (num < len) {
        if (_i2c->write(buffer[num]) == 0) {
            break;
        }

        num++;
    }

    _logger.trace("Sent %d bytes", num);
    _logger.dump("Buffer sent: ", buffer, num, 0);
    return num;
}

void WireClient::stop() {
    if (_i2c == 0) return;

    if (_state == State::write) {
        _i2c->endTransmission(true);
    }

    _state = State::idle;
}

ClassLogger WireClient::_logger(LC_I2C);

}

#endif // I2C_WIRE