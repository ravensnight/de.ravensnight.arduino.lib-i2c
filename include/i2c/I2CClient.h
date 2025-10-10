#ifndef __I2CClient_h__
#define __I2CClient_h__

#include <Arduino.h>
#include <Wire.h>
#include <ClassLogger.h>

using namespace ravensnight::logging;
namespace ravensnight::i2c {

    class I2CClient {

        private:

            static ClassLogger _logger;

            uint8_t _hostAddr = 0;
            TwoWire* _i2c = 0;

            void skipAllAvailable();

        public:

            I2CClient(TwoWire* twi, uint8_t hostAddr);

            /**
             * Send some data buffer to the client.
             * Data being send needs to fulfill die requirements of the host device / slave
             */
            int16_t send(const uint8_t* buffer, uint8_t len);

            /**
             * Request some data from host. Send some parameters for register addressing or 
             * function selection.
             */
            int16_t request(const uint8_t* params, uint8_t paramsLen, uint8_t* response, uint8_t responseLen);

            /**
             * Some global fubnctions to listen on I2C
             */
            static void waitFor(TwoWire* conn, const uint8_t addrs[], uint8_t size, uint16_t delayTime);
            static void scanAll(TwoWire* conn);
    };
}


#endif // __I2CClient_h__