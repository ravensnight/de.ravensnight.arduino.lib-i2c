#include <i2c/DefaultAddressServer.h>

using namespace ravensnight::i2c;

DefaultAddressServer::DefaultAddressServer(uint8_t min, uint8_t max) {    
    if (min > max) {
        _min = max;
        _max = min;
    } else {
        _min = min;
        _max = max;
    }

    _next = _min;
}

/**
 * Provide some state information.
 */
uint16_t DefaultAddressServer::getState() {
    if (hasMoreAdresses()) {
        return (uint16_t)ServerState::ready;
    } else {
        return (uint16_t)ServerState::noaddress;
    }
}

/**
 * Provide the data size you expect for element at given index
 */
int8_t DefaultAddressServer::getDetailsSize(uint8_t index) {
    if (hasMoreAdresses()) {
        return (int8_t)1;
    } 
    else {
        return (int8_t)-1;
    }
}

/**
 * Update element from given buffer as sent by controller
 */
void DefaultAddressServer::setDetails(uint8_t index, const uint8_t buffer[]) {
    // nothing to write. ignore function.
}

/**
 * Fill the buffer so that I2C Device can sent data to its controller.
 */
void DefaultAddressServer::getDetails(uint8_t index, uint8_t data[]) {
    int16_t res = getNextAddress();
    if (res > 0) {
        data[0] = res;
    }
}

/**
 * Reset the internal states.
 */
void DefaultAddressServer::reset() {
    _next = _min;
}

bool DefaultAddressServer::hasMoreAdresses() {
    return ((_next >= _min) && (_next <= _max));
}

int16_t DefaultAddressServer::getNextAddress() {
    int16_t res = -1;
    if (hasMoreAdresses()) {
        res = _next;
        _next++;
    }

    return res;
}
