#ifndef __I2CDeviceModel_h__
#define __I2CDeviceModel_h__

#include <Arduino.h>

namespace ravensnight::i2c {

    class SimpleDeviceModel {

        public:

            /**
             * Provide some state information.
             */
            virtual uint16_t getState() = 0;

            /**
             * Provide the data size you expect for element at given index
             */
            virtual int8_t getDetailsSize(uint8_t index) = 0;

            /**
             * Update element from given buffer as sent by controller
             */
            virtual void setDetails(uint8_t index, const uint8_t buffer[]) = 0;

            /**
             * Fill the buffer so that I2C Device can sent data to its controller.
             */
            virtual void getDetails(uint8_t index, uint8_t data[]) = 0;

            /**
             * Reset the internal states.
             */
            virtual void reset() = 0;
    };

}


#endif // __I2CDeviceModel_h__