#ifndef __Model_h__
#define __Model_h__

#include <Arduino.h>

#include <i2c/util/SimpleDeviceModel.h>

#define BUFFER_LEN 10

class Model : public ravensnight::i2c::util::SimpleDeviceModel {

    private:

        uint8_t _buffer[BUFFER_LEN];

    public:

        Model();

        /**
         * Provide some state information.
         */
        uint16_t getState();

        /**
         * Provide the data size you expect for element at given index
         */
        int8_t getDetailsSize(uint8_t index);

        /**
         * Update element from given buffer as sent by controller
         */
        void setDetails(uint8_t index, const uint8_t buffer[]);

        /**
         * Fill the buffer so that I2C Device can sent data to its controller.
         */
        void getDetails(uint8_t index, uint8_t data[]);

        /**
         * Reset the internal states.
         */
        void reset();

};


#endif // __Model_h__