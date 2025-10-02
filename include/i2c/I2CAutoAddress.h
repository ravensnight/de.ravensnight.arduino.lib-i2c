#ifndef __I2CAutoAddress_h__
#define __I2CAutoAddress_h__

#include <Arduino.h>
#include <i2c/common.h>
#include <i2c/I2CAddressProvider.h>

namespace ravensnight::i2c {

    class I2CAutoAddress : public I2CAddressProvider {

        private:
            TwoWire* _twi = 0;
            uint8_t _address = 0; 
            uint8_t _tries = 1;
            uint16_t _delay = 1000;

        public:

            /**
             * Create an Auto Address instance, which will try to acquire a devices address by calling a slave. 
             * @param twi the pointer to the 2-wire interface
             * @param addr the target address of the slave providing the address
             */
            I2CAutoAddress(TwoWire* twi, uint8_t addr, uint8_t numberOfTries, uint16_t retryDelay);

            void begin();
            void end();

            int16_t getAddress();
            uint8_t getNumberOfTries();
            uint16_t getRetryDelay();            
    };
}


#endif // __I2CAutoAddress_h__
