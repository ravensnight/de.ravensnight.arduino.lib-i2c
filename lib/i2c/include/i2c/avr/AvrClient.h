
#ifndef __AvrClient_h__
#define __AvrClient_h__

#include <Arduino.h>
#include <i2c/AbstractClient.h>

namespace ravensnight::i2c::avr {

    #ifndef F_SCL
    #define F_SCL 100000UL
    #endif

    #ifndef AVR_TWICLIENT_RETRIES
    #define AVR_TWICLIENT_RETRIES 5
    #endif 

    class AvrClient : public AbstractClient {

        private:
            uint32_t _clockSpeed;
            uint8_t _hostAddr;  

        public:

            AvrClient();
            AvrClient(uint32_t clockSpeed);

            /** Initialize */
            void setup(uint8_t hostAddr);

        protected:

            bool start(bool write);
            int16_t read(uint8_t* buffer, uint8_t len);
            int16_t write(const uint8_t* buffer, uint8_t len);
            void stop();

    };
}

#endif // __AvrClient_h__