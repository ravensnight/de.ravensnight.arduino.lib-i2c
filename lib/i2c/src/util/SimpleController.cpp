
#include <utils/StreamHelper.h>

#include <i2c/util/SimpleController.h>
#include <i2c/LoggerConfig.h>

using namespace ravensnight::utils;
using namespace ravensnight::logging;
using namespace ravensnight::i2c;
using namespace ravensnight::i2c::util;

SimpleController::SimpleController(I2CClient& client) : _client(client) {
}

uint8_t SimpleController::getReg(Command cmd, int index) {
    uint8_t reg = (cmd << 6) | (index & 0x3F);
    return reg;
}

int16_t SimpleController::getState(uint16_t& state) {

    uint8_t txData[] = { getReg(Command::GetState, 0) };
    uint8_t rxData[2];

    if (_client.request(txData, 1, rxData, 2) == 2) {        
        StreamHelper::read16(rxData, state);
        return 2;
    } else {
        return -1;
    }
}

int16_t SimpleController::getDetails(uint8_t index, uint8_t& value) {
    uint8_t buffer[1];
    int16_t res = getDetails(index, buffer, 1);

    if (res == 1) {
        StreamHelper::read8(buffer, value);
    }
    return res;
}

int16_t SimpleController::getDetails(uint8_t index, uint16_t& value) {
    uint8_t buffer[2];
    int16_t res = getDetails(index, buffer, 2);

    if (res == 2) {        
        StreamHelper::read16(buffer, value);
    }
    return res;
}

int16_t SimpleController::getDetails(uint8_t index, uint8_t buffer[], uint8_t len) {
    uint8_t txData[1] = { getReg( Command::GetDetails, index ) };
    return _client.request(txData, 1, buffer, len);
}

void SimpleController::setDetails(uint8_t index, uint8_t value) {
    uint8_t buffer[1] = { value };
    setDetails(index, buffer, 1);
}

void SimpleController::setDetails(uint8_t index, uint16_t value) {
    uint8_t buffer[2];
    StreamHelper::write16(buffer, value);
    
    setDetails(index, buffer, 2);
}

void SimpleController::setDetails(uint8_t index, const uint8_t buffer[], uint8_t size) {
    uint8_t txData[size + 1];
    txData[0] = getReg(Command::SetDetails, index);
    memcpy(txData + 1, buffer, size);

    _client.send(txData, size + 1);
}

void SimpleController::resetDevice() {
    uint8_t txData[1] = { getReg(Command::Reset, 0) };
    _client.send(txData, 1);
}

ClassLogger SimpleController::_logger(LC_I2C);