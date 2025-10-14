#ifndef __I2Device_h__
#define __I2Device_h__

#include <i2c/common.h>
#include <i2c/SimpleDeviceModel.h>
#include <i2c/I2CHostHandler.h>
#include <i2c/I2CAddressProvider.h>

namespace ravensnight::i2c {

    /**
     * Defines an I2C Device / I2C Slave
     */
    class SimpleDevice : public I2CHostHandler {

        private:

            volatile Command _command = Command::GetState;
            volatile uint8_t _index = 0;

            SimpleDeviceModel* _model = 0;

        public:

            SimpleDevice(SimpleDeviceModel* model);

            /**
             * Parse some data received via I2C and calculate a new state.
             * @return true, if new state is valid / data could be parsed successfully
             */
            bool parse(const uint8_t* buffer, size_t length);
            
            /**
             * Return true, if the data being addressed by previous parse function can be read.
             */
            bool canRead();

            /**
             * Send some output data matching the internal current state.
             */
            int16_t read(uint8_t* buffer, size_t maxLen);
    };

}

#endif // __I2Device_h__