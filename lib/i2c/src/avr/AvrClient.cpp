#ifdef __AVR__ 
#ifdef I2C_AVR

#include <avr/io.h>
#include <util/twi.h>

#include <i2c/avr/twi_master.h>
#include <i2c/avr/AvrClient.h>

#ifndef F_CPU
#define F_CPU 8000000UL
#endif // F_CPI

namespace ravensnight::i2c::avr {

AvrClient::AvrClient() : AvrClient(F_SCL) {
}

AvrClient::AvrClient(uint32_t clockSpeed) : AbstractClient() {   
    _hostAddr = 0x00;
    _clockSpeed = clockSpeed;
}

void AvrClient::setup(uint8_t hostAddr) {
    _hostAddr = hostAddr;
    twi::init(_clockSpeed);
}

bool AvrClient::start(bool write) {
    twi_result res = twi::start(_hostAddr, write, AVR_TWICLIENT_RETRIES);

    if (res != twi_result::success) {
        twi::stop();
        return false;
    }

    return true;
}

int16_t AvrClient::read(uint8_t* buffer, uint8_t len) {
    for (uint8_t i = 0; i < len; i++) {
        if (i < (len - 1)) {
            buffer[i] = twi::read_ack();
        }
        else {
            buffer[i] = twi::read_nack();
        }
    }
    return len;
}

int16_t AvrClient::write(const uint8_t* buffer, uint8_t len) {

    for (uint8_t i = 0; i < len; i++) {
        twi_result res = twi::write(buffer[i]);
        if (res != twi_result::success) {
            return -1;
        }
    }

    return len;
}

void AvrClient::stop() {
    twi::stop();
}

}

#endif // I2C_AVR
#endif // __AVR__