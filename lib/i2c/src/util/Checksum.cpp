#include <i2c/util/Checksum.h>

namespace ravensnight::i2c::util {

Checksum::Checksum() {
}

void Checksum::reset() {
    _checksum = 0;
}

void Checksum::update(uint8_t val) {
    _checksum += val;
}

void Checksum::update(const uint8_t* buffer, size_t len) {
    for (size_t i = 0; i < len; i++) {
        update(buffer[i]);
    }
}

void Checksum::build(uint8_t& checksum) {
    checksum = 255 - _checksum;
    if (checksum == 0) checksum = 0xFF;    
}

}