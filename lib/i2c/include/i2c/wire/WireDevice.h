#ifndef __TwiHost_h__
#define __TwiHost_h__

#include <Arduino.h>
#include <Wire.h>
#include <Logger.h>
#include <i2c/I2CDeviceAdapter.h>
#include <i2c/AbstractDevice.h>

using namespace ravensnight::logging;

namespace ravensnight::i2c::wire {

    #define TWIHOST_RX_BUFFER_SIZE 32
    #define TWIHOST_TX_BUFFER_SIZE 32
    #define INVALID_ADDR 0xFF

    class WireDevice : public AbstractDevice {

        private:

            static Logger _logger;

            TwoWire* _wire = 0;
            uint8_t  _addr = INVALID_ADDR;

            uint8_t _rxBuffer[TWIHOST_RX_BUFFER_SIZE] = {0};
            uint8_t _txBuffer[TWIHOST_TX_BUFFER_SIZE] = {0};

        protected:

            void skipAll();

            void receive(int bytes);
            void transmit();

        public:

            WireDevice(TwoWire* twi);

            /** Initialize the instance. */
            bool install(uint8_t add);

            static void __onReceive(int bytes);
            static void __onRequest();
            
            static WireDevice* instance;
    };

}

#endif // __TwiHost_h__