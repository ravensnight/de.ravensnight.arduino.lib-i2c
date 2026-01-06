#ifndef __AvrHost_h__
#define __AvrHost_h__

#include <Arduino.h>
#include <i2c/AbstractDevice.h>

namespace ravensnight::i2c::avr {

    #define AVRHOST_RX_BUFFER_SIZE 32
    #define AVRHOST_TX_BUFFER_SIZE 32

    enum class AvrDeviceStatus {
        idle = 0,
        receiving,
        sending
    };

    typedef struct {
        AvrDeviceStatus status = AvrDeviceStatus::idle;

        uint8_t reqBufferPos;
        uint8_t reqBuffer[AVRHOST_RX_BUFFER_SIZE];
        uint8_t respBufferPos;
        uint8_t respBuffer[AVRHOST_TX_BUFFER_SIZE];

    } AvrDeviceData;

    class AvrDevice : public AbstractDevice {

        public:

            static AvrDevice* instance;
            AvrDeviceData state;

            AvrDevice();

            // ----------------------------------------------------------------
            // Internal AVR twi/register Functions
            // ----------------------------------------------------------------
            static uint8_t twi_status();
            static void twi_receive_start(AvrDeviceData& data);
            static void twi_receive_next(AvrDeviceData& data);
            static void twi_response_start(AvrDeviceData& data);
            static void twi_response_next(AvrDeviceData& data);
            static void twi_stop(AvrDeviceData& data);
            static void twi_ack();

            bool install(uint8_t hostAddr);

    };

}

#endif // __AvrHost_h__