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

void I2CChecksum::finalize() {
    _checksum = 255 - _checksum;
    if (_checksum == 0) _checksum = 0xFF;    
}

uint8_t I2CChecksum::get() {
    return _checksum;
}

}