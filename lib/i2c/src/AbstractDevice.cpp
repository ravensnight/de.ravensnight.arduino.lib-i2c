#include <i2c/LoggerConfig.h>
#include <i2c/AbstractDevice.h>
#include <utils/BufferInputStream.h>
#include <utils/BufferOutputStream.h>

using namespace ravensnight::logging;
using namespace ravensnight::utils;

namespace ravensnight::i2c {

    Logger AbstractDevice::_logger(LC_I2C);

    AbstractDevice::AbstractDevice() {
        _adapter = 0;
        _useChecksum = true;
    }

    void AbstractDevice::setAdapter(I2CDeviceAdapter* handler) {
        _adapter = handler;
    }

    void AbstractDevice::setUseChecksum(bool enable) {
        _useChecksum = enable;
    }

    bool AbstractDevice::useChecksum() {
        return _useChecksum;
    }

    bool AbstractDevice::parseRequest(const uint8_t* in, uint8_t len) {
        assert(_adapter != 0);

        uint8_t dataSize = len;

        if (useChecksum()) {
            dataSize = len - 1;

            _checksum.reset();
            _checksum.update(in, dataSize);

            uint8_t chksum= 0;
            _checksum.build(chksum);

            if (in[dataSize] != chksum) {
                _logger.error("AbstractDevice::onReceive - Checksum mismatch %d != %d", in[dataSize], chksum);
                return false;
            }
        }

        return _adapter->parse(in, dataSize);    
    }

    int16_t AbstractDevice::buildResponse(uint8_t* out, uint8_t maxBufferSize) {
        assert(_adapter != 0);

        uint8_t responseSize = maxBufferSize;
        if (useChecksum()) responseSize--;

        int16_t dataSize = _adapter->prepareResponse(out, responseSize);
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
}