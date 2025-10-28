#ifdef __AVR__
#ifdef I2C_AVR

#include <i2c/avr/twi_master.h>

#include <avr/io.h>
#include <util/twi.h>

namespace ravensnight::i2c::avr {

void twi::init() {
    twi::init(100000UL); // default clock speed
}

void twi::init(uint32_t clock) {
    // set Bitrate SCL = F_CPU / (16 + 2*TWBR*Prescaler)
    TWSR = 0x00;                   // Prescaler = 1c) - 
    TWBR = ((F_CPU / clock) - 16) / 2;
}

uint8_t twi::status() {
    return (TWSR & 0xF8);
}

uint8_t twi::send_init_cond() {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    wait_ready(); 

    return status();
}

uint8_t twi::send_data(uint8_t value) {
    TWDR = value;
    TWCR = (1 << TWINT) | (1 << TWEN);
    wait_ready();

    return status();
}


void twi::wait_ready() { 
    // wait until command is ready
    while (!(TWCR & (1 << TWINT)));
}

twi_result twi::start(uint8_t hostAddr, bool write, uint8_t retries) {
    uint8_t stat;
    uint8_t count = 0;

    while (count < retries) {

        count++;

        stat = twi::send_init_cond(); // start condition
        if (stat == TW_MT_ARB_LOST) {
            _delay_ms(100);
            continue; // retry
        }

        if ((stat != TW_START) && (stat != TW_REP_START)) {
            return twi_result::no_start; // error - immediately return
        }

        stat = twi::send_data((hostAddr << 1) | (write ? TW_WRITE : TW_READ));
        if (stat == TW_MT_ARB_LOST) {
            delay(rand() % 10);
            continue; // retry
        }

        /*
        if ((stat != TW_MT_SLA_ACK) && (stat != TW_MT_SLA_NACK)) {
            return twi_result::no_ack; // error - immediately return
        }
        */

        break;
    }

    if (count > retries) {
        return twi_result::arbitration_error;
    }

    return twi_result::success;
}

twi_result twi::write(uint8_t data) {
    uint8_t stat = send_data(data);
    
    if (stat == TW_MT_ARB_LOST) return twi_result::arbitration_error;     // Arbitration lost
    if (stat != TW_MT_DATA_ACK) return twi_result::no_ack;               // no ack

    return twi_result::success;     // OK
}

uint8_t twi::read_ack(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    wait_ready();

    return TWDR;
}

uint8_t twi::read_nack(void) {
    TWCR = (1 << TWINT) | (1 << TWEN);
    wait_ready();

    return TWDR;
}

void twi::stop(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); // No wait — just stop
    _delay_us(10);
}

}

#endif // I2C_AVR
#endif // __AVR__