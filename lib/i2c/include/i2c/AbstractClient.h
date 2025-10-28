#ifndef __AbstractClient_h__
#define __AbstractClient_h__

#include <Arduino.h>

#include <Logger.h>
#include <ClassLogger.h>
#include <i2c/util/Checksum.h>

using namespace ravensnight::logging;

namespace ravensnight::i2c {

    #ifndef I2C_CLIENT_TX_BUFFER_SIZE
    #define I2C_CLIENT_TX_BUFFER_SIZE 32
    #endif

    #ifndef I2C_CLIENT_RX_BUFFER_SIZE
    #define I2C_CLIENT_RX_BUFFER_SIZE 32
    #endif

    class AbstractClient {

        private:

            static ClassLogger _logger;

            bool    _useChecksum;
            ravensnight::i2c::util::Checksum  _checksum;

            uint8_t _txBuffer[I2C_CLIENT_TX_BUFFER_SIZE + 1]; // transmit buffer add one byte for checksum
            uint8_t _rxBuffer[I2C_CLIENT_RX_BUFFER_SIZE + 1]; // receive buffer add one byte for checksum

            int16_t bus_write(const uint8_t* txBuffer, uint8_t txSize);
            int16_t bus_read(uint8_t* rxBuffer, uint8_t maxRxSize);

        public:

            AbstractClient();

            /** enable / disable checksum */
            void setUseChecksum(bool enable);

            /** @return true, if checksum is enabled */
            bool useChecksum();
            
            /** Probe if a slave/host exists */
            bool probe();

            /** Publish some bytes */
            int16_t send(const uint8_t* txBuffer, uint8_t size);

            /** Request some data without any paramters */
            int16_t request(uint8_t* rxBuffer, uint8_t maxRxSize);

            /** Request some data and some some parameters before. */
            int16_t request(const uint8_t* txBuffer, uint8_t txSize, uint8_t* rxBuffer, uint8_t maxRxSize);

            /** Initialize */
            virtual void setup(uint8_t hostAddr) = 0;

        protected:

            virtual bool start(bool write) = 0;
            virtual int16_t read(uint8_t* buffer, uint8_t len) = 0;
            virtual int16_t write(const uint8_t* buffer, uint8_t len) = 0;
            virtual void stop() = 0;

    };
}


#endif // __AbstractClient_h__