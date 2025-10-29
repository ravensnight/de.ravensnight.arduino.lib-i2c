#ifndef __I2CHostHandler_h__
#define __I2CHostHandler_h__

#include <Arduino.h>

namespace ravensnight::i2c {

    class I2CHostHandler {

        public:

            /**
             * Parse some data received via I2C and calculate a new state.
             * @return true, if new state is valid / data could be parsed successfully
             */
            virtual bool parse(const uint8_t* params, uint8_t len) = 0;
            
            /**
             * Return the number of bytes to send on the bus.
             * Else return -1 for error.
             */
            virtual int16_t prepareResponse(uint8_t* out, uint8_t maxLen) = 0;

    };

}

#endif // __I2CHostHandler_h__
