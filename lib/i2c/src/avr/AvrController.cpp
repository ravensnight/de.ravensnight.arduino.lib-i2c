#ifdef __AVR__ 
#include <i2c/configure.h>
#if I2C_IMPL == 2

#include <avr/io.h>
#include <util/twi.h>

#include <i2c/avr/twi_master.h>
#include <i2c/avr/AvrController.h>

#ifndef F_CPU
#define F_CPU 8000000UL
#endif // F_CPI

namespace ravensnight::i2c::avr {

AvrController::AvrController() : AvrController(F_SCL) {
}

AvrController::AvrController(uint32_t clockSpeed) : AbstractController() {   
    _hostAddr = 0x00;
    _clockSpeed = clockSpeed;
}

void AvrController::connect(uint8_t hostAddr) {
    _hostAddr = hostAddr;
    twi::init(_clockSpeed);
}

bool AvrController::probe(uint8_t addr) {
    twi_result res = twi::start(addr, true, AVR_TWICLIENT_RETRIES);
    twi::stop();

    return (res == twi_result::success);
}

bool AvrController::start(bool write) {
    twi_result res = twi::start(_hostAddr, write, AVR_TWICLIENT_RETRIES);

    if (res != twi_result::success) {
        twi::stop();
        return false;
    }

    return true;
}

int16_t AvrController::read(uint8_t* buffer, uint8_t len) {
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

int16_t AvrController::write(const uint8_t* buffer, uint8_t len) {

    for (uint8_t i = 0; i < len; i++) {
        twi_result res = twi::write(buffer[i]);
        if (res != twi_result::success) {
            return -1;
        }
    }

    return len;
}

void AvrController::stop() {
    twi::stop();
}

}

#endif // I2C_AVR
#endif // __AVR__