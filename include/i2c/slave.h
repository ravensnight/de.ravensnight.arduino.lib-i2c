#ifndef __SLAVE_H__
#define __SLAVE_H__

#include <i2c/common.h>

class I2CSlave {

    private:
        
        TwoWire* _i2c = 0;
        uint8_t _address = 0;

        void I2CSlave::sendData(uint8_t command, uint8_t index, const uint8_t buffer[], uint8_t len);

    public:

        I2CSlave();

        void setup(TwoWire* twi, uint8_t address);

        uint16_t getState();

        int16_t getDetails(uint8_t index, uint8_t buffer[], uint8_t len);

        void setDetails(uint8_t index, const uint8_t buffer[], uint8_t len);

        void resetMaster();
};

#endif // __SLAVE_H__