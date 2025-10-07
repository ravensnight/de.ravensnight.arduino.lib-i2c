#ifndef __I2CAddressProvider_h__
#define __I2CAddressProvider_h__

#include <Arduino.h>

namespace ravensnight::i2c {

    class I2CAddressProvider {

        public:

            /** Opens the acquiry session. This function is called by the device in order to initiate the retrieval process. */
            virtual void begin() = 0;

            /**
             * Provides the address for a device / I2C slave.
             * If the result is -1 an address could not be acquired. 
             * If acquiry was successful a value between 0 and 255 will be returned.
             */
            virtual int16_t getAddress() = 0;

            /**
             * Provide the number of tries to call #getAddress()
             */
            virtual uint8_t getNumberOfTries() = 0;

            /**
             * Provide the delay in millis between each try.
             */
            virtual uint16_t getRetryDelay() = 0;

            /** Closes the acquiry session. This function is called by the device in order stop all resources used for retrieval. */
            virtual void end() = 0;

    };

}


#endif // __I2CAddressProvider_h__