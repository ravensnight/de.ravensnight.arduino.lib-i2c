#include <i2c/LoggerConfig.h>
#include <i2c/AbstractHost.h>
#include <utils/BufferInputStream.h>
#include <utils/BufferOutputStream.h>

using namespace ravensnight::logging;
using namespace ravensnight::utils;

namespace ravensnight::i2c {

AbstractHost::AbstractHost() {
    _handler = 0;
    _useChecksum = true;
}

void AbstractHost::setHandler(I2CHostHandler* handler) {
    _handler = handler;
}

void AbstractHost::setUseChecksum(bool enable) {
    _useChecksum = enable;
}

bool AbstractHost::useChecksum() {
    return _useChecksum;
}

bool AbstractHost::parseRequest(const uint8_t* in, uint8_t len) {
    if (_handler == 0) return false;

    uint8_t dataSize = len;

    if (useChecksum()) {
        dataSize = len - 1;

        _checksum.reset();
        _checksum.update(in, dataSize);

        uint8_t chksum= 0;
        _checksum.build(chksum);

        if (in[dataSize] != chksum) {
            _logger.error("AbstractHost::onReceive - Checksum mismatch %d != %d", in[dataSize], chksum);
            return false;
        }
    }

    return _handler->parse(in, dataSize);    
}

int16_t AbstractHost::buildResponse(uint8_t* out, uint8_t maxBufferSize) {
    if (_handler == 0) return -1;

    uint8_t responseSize = maxBufferSize;
    if (useChecksum()) responseSize--;

    int16_t dataSize = _handler->prepareResponse(out, responseSize);
    if ((dataSize > responseSize) || (dataSize < 0)) {
        _logger.error("Error while receiving response data. Result: %d", dataSize);
        return -1;
    }

    if (useChecksum()) {
        uint8_t chksum = 0;
        _checksum.reset();
        _checksum.update(out, dataSize);
        _checksum.build(chksum);

        out[dataSize] = chksum;
        dataSize++;
    }

    return dataSize;
}

bool AbstractHost::setup(I2CAddressProvider& addr) {
    int16_t address = -1;
    uint8_t tries = 0;

    addr.begin();
    while (tries < addr.getNumberOfTries())  {
        address = addr.getAddress();
        if (address >= 0) {
            break;
        }

        delay(addr.getRetryDelay());
    }
    addr.end();

    if (address < 0) {
        _logger.error("AddressProvider did not serv a host address. Stop.");
        return false;
    }
    
    return install(address);
}

ClassLogger AbstractHost::_logger(LC_I2C);

}