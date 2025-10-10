#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <ClassLogger.h>
#include <i2c/common.h>
#include <i2c/I2CClient.h>

using namespace ravensnight::logging;
namespace ravensnight::i2c {
    
    class SimpleController {

        private:
        
            static ClassLogger _logger;

            I2CClient& _i2c;
            uint8_t getReg(Command cmd, int index);

        public:

            SimpleController(I2CClient& client);

            int16_t getState(uint16_t& state);

            int16_t getDetails(uint8_t index, uint8_t& value);
            int16_t getDetails(uint8_t index, uint16_t& value);
            int16_t getDetails(uint8_t index, uint8_t buffer[], uint8_t len);

            void setDetails(uint8_t index, uint8_t value);
            void setDetails(uint8_t index, uint16_t value);
            void setDetails(uint8_t index, const uint8_t buffer[], uint8_t len);

            void resetDevice();

    };

}

#endif // __CLIENT_H__