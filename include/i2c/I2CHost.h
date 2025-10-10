#ifndef __I2CHost_h__
#define __I2CHost_h__

#include <Arduino.h>
#include <Wire.h>
#include <ClassLogger.h>
#include <i2c/I2CAddressProvider.h>
#include <i2c/I2CHostHandler.h>

using namespace ravensnight::logging;
namespace ravensnight::i2c {

    class I2CHost {

        private:

            static ClassLogger _logger;

            TwoWire* _i2c = 0;
            I2CHostHandler* _handler = 0;

            /**
             * listener methods.
             */
            static void __i2cReceive(int bytes);
            static void __i2cRequest();            

            /**
             * handler methods
             */
            void i2cReceive(int bytes);
            void i2cRequest();            

            /**
             * Skip data from buffer.
             */
            void skipAll();


        protected:

            I2CHost();

        public:

            static I2CHost instance;

            bool start(TwoWire* twi, I2CAddressProvider& addr, I2CHostHandler* handler);
    };

}

#endif