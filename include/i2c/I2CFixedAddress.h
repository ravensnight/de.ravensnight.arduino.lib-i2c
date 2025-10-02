#ifndef __I2CFixedAddress_h__
#define __I2CFixedAddress_h__

#include <Arduino.h>
#include <i2c/I2CAddressProvider.h>

namespace ravensnight::i2c {

    class I2CFixedAddress : public I2CAddressProvider {

        private:
            uint8_t _address;

        public:

            I2CFixedAddress(uint8_t address);

            void begin();
            void end();

            int16_t getAddress();
            uint8_t getNumberOfTries();
            uint16_t getRetryDelay();            
    };
}


#endif // __I2CFixedAddress_h__
