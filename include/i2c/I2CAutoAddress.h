#ifndef __I2CAutoAddress_h__
#define __I2CAutoAddress_h__

#include <Arduino.h>
#include <i2c/common.h>
#include <i2c/I2CAddressProvider.h>

namespace ravensnight::i2c {

    #define DEFAULT_NUMBER_OF_TRIES 10
    #define DEFAULT_RETRY_DELAY 500

    class I2CAutoAddress : public I2CAddressProvider {

        private:
            TwoWire* _twi = 0;
            uint8_t _address = 0; 
            uint16_t _register = 0;
            uint8_t _tries = DEFAULT_NUMBER_OF_TRIES;
            uint16_t _delay = DEFAULT_RETRY_DELAY;

        public:

            /**
             * Create an Auto Address instance, which will try to acquire a devices address by calling a slave. 
             * @param twi the pointer to the 2-wire interface
             * @param addr the target address of the slave providing the address
             */
            I2CAutoAddress(TwoWire* twi, uint8_t addr, uint16_t reg);

            /**
             * Set the number of tries.
             */
            void setNumberOfTries(uint8_t numberOfTries);

            /**
             * Set the delay between two retires in millis
             */
            void setRetryDelay(uint16_t retryDelay);

            void begin();
            void end();

            int16_t getAddress();
            uint8_t getNumberOfTries();
            uint16_t getRetryDelay();            
    };
}


#endif // __I2CAutoAddress_h__
