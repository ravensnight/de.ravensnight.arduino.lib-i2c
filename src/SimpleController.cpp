#include <i2c/SimpleController.h>
#include <i2c/LoggerConfig.h>

using namespace ravensnight::logging;
using namespace ravensnight::i2c;

SimpleController::SimpleController(I2CClient& client) : _i2c(client) {
}

uint8_t SimpleController::getReg(Command cmd, int index) {
    uint8_t reg = (cmd << 6) | (index & 0x3F);
    return reg;
}

int16_t SimpleController::getState(uint16_t& state) {

    uint8_t reqData[] = { getReg(Command::GetState, 0) };
    uint8_t resData[2];

    if (_i2c.request(reqData, 1, resData, 2) == 2) {
        state = (resData[0] << 8);
        state |= (resData[1]);
        return 2;
    } else {
        return -1;
    }
}

int16_t SimpleController::getDetails(uint8_t index, uint8_t& value) {
    uint8_t buffer[1];
    int16_t res = getDetails(index, buffer, 1);

    if (res == 1) {
        value = buffer[0];
    }
    return res;
}

int16_t SimpleController::getDetails(uint8_t index, uint16_t& value) {
    uint8_t buffer[2];
    int16_t res = getDetails(index, buffer, 2);

    if (res == 2) {
        value = (buffer[0] << 8);
        value |= buffer[1];
    }
    return res;
}

int16_t SimpleController::getDetails(uint8_t index, uint8_t buffer[], uint8_t len) {
    uint8_t reqData[1] = { getReg( Command::GetDetails, index ) };
    return _i2c.request(reqData, 1, buffer, len);
}

void SimpleController::setDetails(uint8_t index, uint8_t value) {
    uint8_t buffer[1] = { value };
    setDetails(index, buffer, 1);
}

void SimpleController::setDetails(uint8_t index, uint16_t value) {
    uint8_t buffer[2];
    buffer[0] = (uint8_t)(value >> 8);
    buffer[1] = (uint8_t)(value & 0xFF);
    setDetails(index, buffer, 2);
}

void SimpleController::setDetails(uint8_t index, const uint8_t buffer[], uint8_t size) {
    uint8_t reqData[size + 1];
    reqData[0] = getReg(Command::SetDetails, index);

    memcpy(reqData + 1, buffer, size);
    _i2c.send(reqData, size + 1);
}

void SimpleController::resetDevice() {
    uint8_t reqData[1] = { getReg(Command::Reset, 0) };
    _i2c.send(reqData, 1);
}

ClassLogger SimpleController::_logger(LC_I2C);