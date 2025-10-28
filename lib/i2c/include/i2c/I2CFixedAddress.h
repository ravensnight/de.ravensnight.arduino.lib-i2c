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

            void begin() const;
            void end() const;

            int16_t getAddress() const;
            uint8_t getNumberOfTries() const;
            uint16_t getRetryDelay() const;            
    };
}


#endif // __I2CFixedAddress_h__
