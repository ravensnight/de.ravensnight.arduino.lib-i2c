#include "Model.h"

#include <Logger.h>

using namespace ravensnight::logging;

Model::Model() {
    reset();
}

/**
 * Provide some state information.
 */
uint16_t Model::getState() {
    uint16_t res = 0;
    for (uint8_t i = 0; i < BUFFER_LEN; i++) {
        res += _buffer[i];
    }

    return res;
}

/**
 * Provide the data size you expect for element at given index
 */
int8_t Model::getDetailsSize(uint8_t index) {
    return 1;
}

/**
 * Update element from given buffer as sent by controller
 */
void Model::setDetails(uint8_t index, const uint8_t buffer[]) {
    if (index < BUFFER_LEN) {
        _buffer[index] = buffer[0];
    }
}

/**
 * Fill the buffer so that I2C Device can sent data to its controller.
 */
void Model::getDetails(uint8_t index, uint8_t data[]) {
    if (index < BUFFER_LEN) {
        data[0] = _buffer[index];
    }
}

/**
 * Reset the internal states.
 */
void Model::reset() {
    memset(_buffer, 0, BUFFER_LEN);
}


