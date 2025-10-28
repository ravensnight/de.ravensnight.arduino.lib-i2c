#ifdef __AVR__
#ifdef I2C_AVR

#include <Arduino.h>
#include <avr/io.h>
#include <util/twi.h>
#include <i2c/avr/AvrHost.h>

namespace ravensnight::i2c::avr {

AvrHost* AvrHost::instance = 0;

/**
 * Interrupt service routine.
 */

ISR(TWI_vect) {
    uint8_t status = TWSR & 0xF8;
    if (AvrHost::instance == 0) return;

    switch (status) {

    // --- Master schreibt an Slave (SLA+W) ---
    case TW_SR_SLA_ACK:
        AvrHost::twi_receive_start(AvrHost::instance->state);
        break;

    // --- Datenbyte vom Master empfangen ---
    case TW_SR_DATA_ACK:
        AvrHost::twi_receive_next(AvrHost::instance->state);
        break;

    // --- STOP erkannt (Master fertig mit Write) ---
    case TW_SR_STOP:
        AvrHost::twi_stop(AvrHost::instance->state);
        break;

    // --- Master liest vom Slave (SLA+R) ---
    case TW_ST_SLA_ACK:
        AvrHost::twi_response_start(AvrHost::instance->state);
        break;

    // --- Master liest weiteres Byte ---
    case TW_ST_DATA_ACK:
        AvrHost::twi_response_next(AvrHost::instance->state);
        break;

    // --- Master beendet Lesen (NACK) ---
    case TW_ST_DATA_NACK:
        AvrHost::twi_stop(AvrHost::instance->state);
        break;

    // --- Fehler oder nicht unterstützt ---
    default:
        TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);
        break;
    }
}

uint8_t AvrHost::twi_status() {
    return TWSR & 0xF8;
}

void AvrHost::twi_ack() {
    TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);
}

void AvrHost::twi_receive_start(AvrHostData& data) {
    data.reqBufferPos = 0;
    data.status = AvrHostStatus::receiving;

    twi_ack();
}

void AvrHost::twi_receive_next(AvrHostData& data) {
    if (data.reqBufferPos < AVRHOST_RX_BUFFER_SIZE) {
        data.reqBuffer[data.reqBufferPos] = TWDR;
        data.reqBufferPos++;
    }
    
    twi_ack();
}

void AvrHost::twi_stop(AvrHostData& data) {
    if (data.status == AvrHostStatus::receiving) {
        AvrHost::instance->parseRequest(data.reqBuffer, data.reqBufferPos);                
    }

    data.status = AvrHostStatus::idle;
    data.respBufferPos = 0;
    data.reqBufferPos = 0;

    twi_ack();
}

void AvrHost::twi_response_start(AvrHostData& data) {
    if (data.status == AvrHostStatus::receiving) {
        // Das ist der Fall: Write → Repeated Start → Read
        AvrHost::instance->parseRequest(data.reqBuffer, data.reqBufferPos);
        AvrHost::instance->buildResponse(data.respBuffer, AVRHOST_TX_BUFFER_SIZE);
    } 
    else if (data.status == AvrHostStatus::idle) {
        // Direkter Read ohne Write davor
        AvrHost::instance->buildResponse(data.respBuffer, AVRHOST_TX_BUFFER_SIZE);
    }

    data.status = AvrHostStatus::sending;
    data.respBufferPos = 0;
    TWDR = data.respBuffer[data.respBufferPos++];

    twi_ack();
}

void AvrHost::twi_response_next(AvrHostData& data) {
    if (data.respBufferPos < AVRHOST_TX_BUFFER_SIZE) {
        TWDR = data.respBuffer[data.respBufferPos];
        data.respBufferPos++;
    }
    else {
        TWDR = 0x00; // Dummy falls keine weiteren Daten
    }

    twi_ack();
}

/**
 * Constructor
 */
AvrHost::AvrHost() {    
}

bool AvrHost::install(uint8_t addr) {
    TWAR = (addr << 1);  // 7-bit Adresse

    twi_ack();
    sei();

    AvrHost::instance = this;
    return true;
}

}

#endif // I2C_AVR
#endif // __AVR__