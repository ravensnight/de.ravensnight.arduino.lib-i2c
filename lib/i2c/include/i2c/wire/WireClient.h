
#ifndef __WireClient_h__
#define __WireClient_h__

#include <Arduino.h>
#include <Wire.h>
#include <ClassLogger.h>
#include <i2c/AbstractClient.h>

using namespace ravensnight::logging;

namespace ravensnight::i2c::wire {

    enum class State {
        idle = 0,        
        read = 1,
        write = 2
    };

    class WireClient : public AbstractClient {

        private:

            static ClassLogger _logger;

            State   _state = State::idle;
            uint8_t _hostAddr = 0;

            TwoWire* _i2c = 0;

        public:

            WireClient(TwoWire* wire); 
            void setup(uint8_t hostAddr);

        protected:

            bool start(bool write);
            int16_t read(uint8_t* buffer, uint8_t len);
            int16_t write(const uint8_t* buffer, uint8_t len);
            void stop();            
    };
}


#endif // __WireClient_h__