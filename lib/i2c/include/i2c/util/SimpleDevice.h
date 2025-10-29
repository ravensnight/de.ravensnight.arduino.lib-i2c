#ifndef __SimpleDevice_h__
#define __SimpleDevice_h__

#include <i2c/common.h>
#include <i2c/util/SimpleDeviceModel.h>
#include <i2c/I2CHostHandler.h>
#include <i2c/I2CAddressProvider.h>

namespace ravensnight::i2c::util {

    /**
     * Defines an I2C Device / I2C Slave
     */
    class SimpleDevice : public I2CHostHandler {

        private:

            Command _command = Command::GetState;
            uint8_t _index = 0;

            SimpleDeviceModel& _model;

        public:

            SimpleDevice(SimpleDeviceModel& model);

            /**
             * Parse some data received via I2C and calculate a new state.
             * @return true, if new state is valid / data could be parsed successfully
             */
            bool parse(const uint8_t* params, uint8_t len);
            
            /**
             * Return the number of bytes to send on the bus.
             * Else return -1 for error.
             */
            int16_t prepareResponse(uint8_t* out, uint8_t maxLen);

    };

}

#endif // __SimpleDevice_h__