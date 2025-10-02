#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <i2c/common.h>
#include <i2c/I2CDeviceHandler.h>
#include <i2c/I2CAddressProvider.h>

namespace ravensnight::i2c {

    /**
     * Defines an I2C Device / I2C Slave
     */
    class I2CDevice {

        private:

            volatile Command _command = Command::GetState;
            volatile uint8_t _index = 0;

            TwoWire* _i2c = 0;
            I2CDeviceHandler* _handler = 0;

            static void i2cReceive(int bytes);
            static void i2cRequest();

            void handleReceive(int bytes);
            void handleRequest();

        public:

            static I2CDevice instance;

            I2CDevice();            
            bool setup(TwoWire* twi, I2CAddressProvider& address, I2CDeviceHandler* handler);

            void skipAllAvailable();
    };

}

#endif // __DEVICE_H__