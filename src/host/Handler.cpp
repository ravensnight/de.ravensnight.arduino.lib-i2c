#include "Handler.h"

#include <Logger.h>

using namespace ravensnight::logging;

Handler::Handler() {
    memset(_buffer, 0, BUFFER_LEN);
}

/**
 * Parse some data received via I2C and calculate a new state.
 * @return true, if new state is valid / data could be parsed successfully
 */
 bool Handler::parse(const uint8_t* in, uint8_t len) {
    Logger::debug("parse - begin (%d)", len);

    if (len > BUFFER_LEN) {
        return -1;
    } 

    for (uint8_t i = 0; i < len; i++) {
        _buffer[i] = in[i];
        // _buffer[i] = i;
    }

    Logger::dump("Received", _buffer, len, 0);
    return true;
}
    
/**
 * Return -1 if the data being addressed by previous parse function cannot be read.
 * Else return the real size of the buffer to write.
 */
int16_t Handler::preapreResponse(uint8_t* out, uint8_t maxLen) {
    Logger::debug("prepareResponse - begin");

    if (maxLen < BUFFER_LEN) {
        return -1;
    }

    for (uint8_t i = 0; i < BUFFER_LEN; i++) {
        out[i] = BUFFER_LEN - _buffer[i];
    }

    return 10;
}

