#ifndef __Checksum_h__
#define __Checksum_h__

#include <Arduino.h>

namespace ravensnight::i2c::util {

    class Checksum {

        private:
            uint8_t _checksum = 0;

        public:

            Checksum();

            void reset();
            void update(uint8_t val);
            void update(const uint8_t* buffe, size_t len);

            void build(uint8_t& checksum);
    };
}

#endif // __Checksum_h__