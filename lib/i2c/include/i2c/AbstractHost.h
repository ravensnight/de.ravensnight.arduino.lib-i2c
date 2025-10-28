#ifndef __AbstractHost_h__
#define __AbstractHost_h__

#include <Arduino.h>
#include <Wire.h>
#include <ClassLogger.h>
#include <i2c/I2CAddressProvider.h>
#include <i2c/util/Checksum.h>
#include <i2c/I2CHostHandler.h>

using namespace ravensnight::logging;
namespace ravensnight::i2c {

    #ifndef I2C_RECEIVE_BUFFER_SIZE
    #define I2C_RECEIVE_BUFFER_SIZE 64
    #endif

    #ifndef I2C_RESPONSE_BUFFER_SIZE
    #define I2C_RESPONSE_BUFFER_SIZE 64
    #endif 

    class AbstractHost {

        private:

            static ClassLogger _logger;

            I2CHostHandler* _handler = 0;
            bool            _useChecksum = true;

            ravensnight::i2c::util::Checksum  _checksum;

        protected:

            AbstractHost();            

            virtual bool install(uint8_t hostAddr) = 0;

        public:

            /**
             * Enable/Disable checksum calculation.
             */
            void setUseChecksum(bool enable);

            /**
             * @return true, if checksum is enabledm else false
             */
            bool useChecksum();

            /**
             * Set the in/out handler.
             */
            void setHandler(I2CHostHandler* handler);

            /**
             * Handle some data received from wire
             * @return false on failure, else true
             */
            bool parseRequest(const uint8_t* inBuffer, uint8_t len);

            /**
             * Get some data to send to wire
             * @return number of bytes to send or -1 for error
             */
            int16_t buildResponse(uint8_t* outBuffer, uint8_t maxLen);            

            /**
             * Install and start the I2C host.
             */
            bool setup(I2CAddressProvider& addr);
    };

}

#endif