#include <utils/StreamHelper.h>
#include <i2c/AbstractClient.h>
#include <i2c/LoggerConfig.h>

using namespace ravensnight::utils;
using namespace ravensnight::logging;

namespace ravensnight::i2c {

AbstractClient::AbstractClient() {
    _useChecksum = true;
}

/** enable / disable checksum */
void AbstractClient::setUseChecksum(bool enable) {
    _useChecksum = enable;
}

/** @return true, if checksum is enabled */
bool AbstractClient::useChecksum() {
    return _useChecksum;
}

/** Probe if a slave/host exists */
bool AbstractClient::probe() {
    bool res = start(true);
    stop();

    return res;
}

int16_t AbstractClient::bus_read(uint8_t* rxBuffer, uint8_t maxRxSize) {

    if (!start(false)) {
        return -1;
    }

    if (maxRxSize > I2C_CLIENT_RX_BUFFER_SIZE) {
        _logger.warn("Cannot read more than %d bytes. Stop here.", I2C_CLIENT_RX_BUFFER_SIZE);
        return -1;
    }

    // read data
    uint8_t val = 0;
    int16_t res = 0;

    if (useChecksum()) {
        _logger.trace("Read bytes from bus w/ checksum: %d ", maxRxSize + 1);
        res = read(_rxBuffer, maxRxSize + 1);
        if (res < 0) return -1;

        _logger.trace("Received %d bytes.", res);
        _logger.dump("Received data w/ checksum: ", _rxBuffer, res, 0);

        _checksum.reset();
        _checksum.update(_rxBuffer, maxRxSize);
        _checksum.build(val);

        if (val != _rxBuffer[maxRxSize]) {
            _logger.error("Checksum mismatch. Stop.");
            return -1;
        }

        res--; // do not send checksum byte to caller.

    } else {
        _logger.trace("Read bytes from bus w/o checksum: %d ", maxRxSize);
        res = read(_rxBuffer, maxRxSize);
        if (res < 0) {
            _logger.error("No data received.");
            return -1;
        }

        _logger.trace("Received %d bytes.", res);
        _logger.dump("Received data w/o checksum: ", _rxBuffer, res, 0);
    }

    memcpy(rxBuffer, _rxBuffer, res);
    return res;
}

int16_t AbstractClient::bus_write(const uint8_t* txBuffer, uint8_t txSize) {
    if (!start(true)) {
        return -1;
    }

    if (txSize > I2C_CLIENT_TX_BUFFER_SIZE) {
        _logger.warn("Cannot transmit more than %d bytes. Stop here.", I2C_CLIENT_TX_BUFFER_SIZE);
        return -1;
    }

    uint16_t res = 0;
    memcpy(_txBuffer, txBuffer, txSize);
    if (useChecksum()) {
        uint8_t chksum;

        _checksum.reset();
        _checksum.update(txBuffer, txSize);
        _checksum.build(chksum);

        _txBuffer[txSize] = chksum;

        res = write(_txBuffer, txSize + 1);
    } else {
        res = write(_txBuffer, txSize);
    }

    return res;
}

/** Publish some bytes */
int16_t AbstractClient::send(const uint8_t* txBuffer, uint8_t txSize) {    
    int16_t res = bus_write(txBuffer, txSize);
    stop();

    return res;
}

/** Reqest some data without any paramters */
int16_t AbstractClient::request(uint8_t* rxBuffer, uint8_t maxRxSize) {
    int16_t res = bus_read(rxBuffer, maxRxSize);
    stop();

    return res;
}

/** Reqest some data and some some parameters before. */
int16_t AbstractClient::request(const uint8_t* txBuffer, uint8_t txSize, uint8_t* rxBuffer, uint8_t maxRxSize) {
    int16_t res = bus_write(txBuffer, txSize);
    if (res < 0) {
        _logger.error("AbstractClient::request - Failed to write data to bus. Stop.");
        stop();
        return -1;
    }
        
    // restart read - no stop signal here.
    res = bus_read(rxBuffer, maxRxSize);
    if (res < 0) {
        _logger.error("AbstractClient::request - Failed to read data from bus. Stop.");        
    }

    stop();
    return res;
}

ClassLogger AbstractClient::_logger(LC_I2C);

}

