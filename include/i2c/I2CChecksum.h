#ifndef __I2CChecksum_h__
#define __I2CChecksum_h__

#include <Arduino.h>

namespace ravensnight::i2c {

    class I2CChecksum {

        private:
            uint8_t _checksum = 0;

        public:

            I2CChecksum();

            void reset();
            void update(uint8_t val);
            void update(const uint8_t* buffe, size_t len);

            void finalize();

            uint8_t get();
    };

}

#endif // __I2CChecksum_h__