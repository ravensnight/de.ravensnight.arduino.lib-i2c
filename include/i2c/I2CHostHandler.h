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
            virtual bool parse(const uint8_t* buffer, size_t length) = 0;
            
            /**
             * Return true, if the data being addressed by previous parse function can be read.
             */
            virtual bool canRead() = 0;

            /**
             * Send some output data matching the internal current state.
             * @return the number of bytes filled in buffer.
             */
            virtual int16_t read(uint8_t* buffer, size_t maxLen) = 0;
    };

}

#endif // __I2CHostHandler_h__
