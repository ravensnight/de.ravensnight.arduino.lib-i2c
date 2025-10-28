#ifndef __Handler_h__
#define __Handler_h__

#include <Arduino.h>

#include <i2c/I2CHostHandler.h>

#define BUFFER_LEN 10

class Handler : public ravensnight::i2c::I2CHostHandler {

    private:

        uint8_t _buffer[BUFFER_LEN] = { 0 };

    public:

    Handler();

    /**
     * Parse some data received via I2C and calculate a new state.
     * @return true, if new state is valid / data could be parsed successfully
     */
    bool parse(const uint8_t* in, uint8_t len);
    
    /**
     * Return the number of bytes to write to bus
     * if the data being addressed by previous parse function can be read.
     */
    int16_t preapreResponse(uint8_t* out, uint8_t maxLen);

};


#endif // __Handler_h__