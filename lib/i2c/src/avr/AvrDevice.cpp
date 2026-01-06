#ifdef __AVR__
#include <i2c/configure.h>
#if I2C_IMPL == 2

#include <Arduino.h>
#include <avr/io.h>
#include <util/twi.h>
#include <i2c/avr/AvrDevice.h>

namespace ravensnight::i2c::avr {

AvrDevice* AvrDevice::instance = 0;

/**
 * Interrupt service routine.
 */

ISR(TWI_vect) {
    uint8_t status = TWSR & 0xF8;
    if (AvrDevice::instance == 0) return;

    switch (status) {

    // --- Master schreibt an Slave (SLA+W) ---
    case TW_SR_SLA_ACK:
        AvrDevice::twi_receive_start(AvrDevice::instance->state);
        break;

    // --- Datenbyte vom Master empfangen ---
    case TW_SR_DATA_ACK:
        AvrDevice::twi_receive_next(AvrDevice::instance->state);
        break;

    // --- STOP erkannt (Master fertig mit Write) ---
    case TW_SR_STOP:
        AvrDevice::twi_stop(AvrDevice::instance->state);
        break;

    // --- Master liest vom Slave (SLA+R) ---
    case TW_ST_SLA_ACK:
        AvrDevice::twi_response_start(AvrDevice::instance->state);
        break;

    // --- Master liest weiteres Byte ---
    case TW_ST_DATA_ACK:
        AvrDevice::twi_response_next(AvrDevice::instance->state);
        break;

    // --- Master beendet Lesen (NACK) ---
    case TW_ST_DATA_NACK:
        AvrDevice::twi_stop(AvrDevice::instance->state);
        break;

    // --- Fehler oder nicht unterstützt ---
    default:
        TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);
        break;
    }
}

uint8_t AvrDevice::twi_status() {
    return TWSR & 0xF8;
}

void AvrDevice::twi_ack() {
    TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);
}

void AvrDevice::twi_receive_start(AvrDeviceData& data) {
    data.reqBufferPos = 0;
    data.status = AvrDeviceStatus::receiving;

    twi_ack();
}

void AvrDevice::twi_receive_next(AvrDeviceData& data) {
    if (data.reqBufferPos < AVRHOST_RX_BUFFER_SIZE) {
        data.reqBuffer[data.reqBufferPos] = TWDR;
        data.reqBufferPos++;
    }
    
    twi_ack();
}

void AvrDevice::twi_stop(AvrDeviceData& data) {
    if (data.status == AvrDeviceStatus::receiving) {
        AvrDevice::instance->parseRequest(data.reqBuffer, data.reqBufferPos);                
    }

    data.status = AvrDeviceStatus::idle;
    data.respBufferPos = 0;
    data.reqBufferPos = 0;

    twi_ack();
}

void AvrDevice::twi_response_start(AvrDeviceData& data) {
    if (data.status == AvrDeviceStatus::receiving) {
        // Das ist der Fall: Write → Repeated Start → Read
        AvrDevice::instance->parseRequest(data.reqBuffer, data.reqBufferPos);
        AvrDevice::instance->buildResponse(data.respBuffer, AVRHOST_TX_BUFFER_SIZE);
    } 
    else if (data.status == AvrDeviceStatus::idle) {
        // Direkter Read ohne Write davor
        AvrDevice::instance->buildResponse(data.respBuffer, AVRHOST_TX_BUFFER_SIZE);
    }

    data.status = AvrDeviceStatus::sending;
    data.respBufferPos = 0;
    TWDR = data.respBuffer[data.respBufferPos++];

    twi_ack();
}

void AvrDevice::twi_response_next(AvrDeviceData& data) {
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
AvrDevice::AvrDevice() {    
}

bool AvrDevice::install(uint8_t addr) {
    TWAR = (addr << 1);  // 7-bit Adresse

    twi_ack();
    sei();

    AvrDevice::instance = this;
    return true;
}

}

#endif // I2C_AVR
#endif // __AVR__