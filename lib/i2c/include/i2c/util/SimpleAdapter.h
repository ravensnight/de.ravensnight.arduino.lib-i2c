#ifndef __SimpleDevice_h__
#define __SimpleDevice_h__

#include <i2c/common.h>
#include <i2c/util/SimpleAdapterModel.h>
#include <i2c/I2CDeviceAdapter.h>

namespace ravensnight::i2c::util {

    /**
     * Defines an I2C Device / I2C Slave
     */
    class SimpleAdapter : public I2CDeviceAdapter {

        private:

            Command _command = Command::GetState;
            uint8_t _index = 0;

            SimpleAdapterModel& _model;

        public:

            SimpleAdapter(SimpleAdapterModel& model);

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