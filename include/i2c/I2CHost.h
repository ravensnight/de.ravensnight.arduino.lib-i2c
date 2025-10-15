#ifndef __I2CHost_h__
#define __I2CHost_h__

#include <Arduino.h>
#include <Wire.h>
#include <ClassLogger.h>
#include <i2c/I2CAddressProvider.h>
#include <i2c/I2CChecksum.h>
#include <i2c/I2CHostHandler.h>

using namespace ravensnight::logging;
namespace ravensnight::i2c {

    #ifndef I2C_RECEIVE_BUFFER_SIZE
    #define I2C_RECEIVE_BUFFER_SIZE 64
    #endif

    #ifndef I2C_RESPONSE_BUFFER_SIZE
    #define I2C_RESPONSE_BUFFER_SIZE 64
    #endif 

    class I2CHost {

        private:

            static ClassLogger _logger;

            TwoWire*        _i2c = 0;            
            I2CHostHandler* _handler = 0;
            I2CChecksum     _checksum;
            bool            _useChecksum = true;
            uint8_t         _receiveBuffer[I2C_RECEIVE_BUFFER_SIZE] = { 0 };
            uint8_t         _responseBuffer[I2C_RESPONSE_BUFFER_SIZE] = { 0 };

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

            void setUseChecksum(bool use);
            bool getUseChecksum();

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