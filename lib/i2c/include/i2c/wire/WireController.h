
#ifndef __WireController_h__
#define __WireController_h__

#include <Arduino.h>
#include <Wire.h>
#include <Logger.h>
#include <i2c/AbstractController.h>

using namespace ravensnight::logging;

namespace ravensnight::i2c::wire {

    enum class State {
        idle = 0,        
        read = 1,
        write = 2
    };

    class WireController : public AbstractController {

        private:

            static Logger _logger;

            State   _state = State::idle;
            uint8_t _hostAddr = 0;

            TwoWire* _i2c = 0;

        public:

            WireController(TwoWire* wire); 

            void connect(uint8_t hostAddr);
            bool probe(uint8_t addr);

        protected:

            bool start(bool write);
            int16_t read(uint8_t* buffer, uint8_t len);
            int16_t write(const uint8_t* buffer, uint8_t len);
            void stop();            
    };
}


#endif // __WireController_h__