#ifndef __SimpleController_h__
#define __SimpleController_h__

#include <Logger.h>
#include <i2c/common.h>
#include <i2c/I2CController.h>

using namespace ravensnight::logging;
namespace ravensnight::i2c::util {
    
    class SimpleController {

        private:
        
            static Logger _logger;

            I2CController& _client;
            uint8_t getReg(Command cmd, int index);

        public:

            SimpleController(I2CController& client);

            int16_t getState(uint16_t& state);

            int16_t getDetails(uint8_t index, uint8_t& value);
            int16_t getDetails(uint8_t index, uint16_t& value);
            int16_t getDetails(uint8_t index, uint8_t buffer[], uint8_t len);

            void setDetails(uint8_t index, uint8_t value);
            void setDetails(uint8_t index, uint16_t value);
            void setDetails(uint8_t index, const uint8_t buffer[], uint8_t len);

            void resetValues();
            void resetDevice();

    };

}

#endif // __SimpleController_h__