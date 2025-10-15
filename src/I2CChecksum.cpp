#include <i2c/I2CChecksum.h>

namespace ravensnight::i2c {

I2CChecksum::I2CChecksum() {
}

void I2CChecksum::reset() {
    _checksum = 0;
}

void I2CChecksum::update(uint8_t val) {
    _checksum += val;
}

void I2CChecksum::update(const uint8_t* buffer, size_t len) {
    for (size_t i = 0; i < len; i++) {
        update(buffer[i]);
    }
}

void I2CChecksum::build(uint8_t& checksum) {
    checksum = 255 - _checksum;
    if (checksum == 0) checksum = 0xFF;    
}

}