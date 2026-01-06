
#ifndef __AvrController_h__
#define __AvrController_h__

#include <Arduino.h>
#include <i2c/AbstractController.h>

namespace ravensnight::i2c::avr {

    #ifndef F_SCL
    #define F_SCL 100000UL
    #endif

    #ifndef AVR_TWICLIENT_RETRIES
    #define AVR_TWICLIENT_RETRIES 5
    #endif 

    class AvrController : public AbstractController {

        private:
            uint32_t _clockSpeed;
            uint8_t _hostAddr;  

        protected:

            bool start(bool write);
            int16_t read(uint8_t* buffer, uint8_t len);
            int16_t write(const uint8_t* buffer, uint8_t len);
            void stop();

        public:

            AvrController();
            AvrController(uint32_t clockSpeed);

            /** Connect a dedicated device */
            void connect(uint8_t hostAddr);

            bool probe(uint8_t addr);
    };
}

#endif // __AvrController_h__