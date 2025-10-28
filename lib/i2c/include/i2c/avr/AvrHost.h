#ifndef __AvrHost_h__
#define __AvrHost_h__

#include <Arduino.h>
#include <i2c/AbstractHost.h>

namespace ravensnight::i2c::avr {

    #define AVRHOST_RX_BUFFER_SIZE 32
    #define AVRHOST_TX_BUFFER_SIZE 32

    enum class AvrHostStatus {
        idle = 0,
        receiving,
        sending
    };

    typedef struct {
        AvrHostStatus status = AvrHostStatus::idle;

        uint8_t reqBufferPos;
        uint8_t reqBuffer[AVRHOST_RX_BUFFER_SIZE];
        uint8_t respBufferPos;
        uint8_t respBuffer[AVRHOST_TX_BUFFER_SIZE];

    } AvrHostData;

    class AvrHost : public AbstractHost {
        
        protected:

            bool install(uint8_t hostAddr);

        public:

            static AvrHost* instance;

            AvrHost();
            AvrHostData state;

            // ----------------------------------------------------------------
            // Internal AVR twi/register Functions
            // ----------------------------------------------------------------
            static uint8_t twi_status();
            static void twi_receive_start(AvrHostData& data);
            static void twi_receive_next(AvrHostData& data);
            static void twi_response_start(AvrHostData& data);
            static void twi_response_next(AvrHostData& data);
            static void twi_stop(AvrHostData& data);
            static void twi_ack();
    };

}

#endif // __AvrHost_h__