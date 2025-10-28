#ifndef __TwiHost_h__
#define __TwiHost_h__

#include <Arduino.h>
#include <Wire.h>
#include <ClassLogger.h>
#include <i2c/I2CAddressProvider.h>
#include <i2c/I2CHostHandler.h>
#include <i2c/AbstractHost.h>

using namespace ravensnight::logging;

namespace ravensnight::i2c::wire {

    #define TWIHOST_RX_BUFFER_SIZE 32
    #define TWIHOST_TX_BUFFER_SIZE 32

    class WireHost : public AbstractHost {

        private:

            static ClassLogger _logger;

            TwoWire* _wire = 0;

            uint8_t _rxBuffer[TWIHOST_RX_BUFFER_SIZE] = {0};
            uint8_t _txBuffer[TWIHOST_TX_BUFFER_SIZE] = {0};

        protected:

            /** Initialize the instance. */
            bool install(uint8_t add);

            void skipAll();

            void receive(int bytes);
            void transmit();

        public:

            WireHost(TwoWire* twi);

            static void __onReceive(int bytes);
            static void __onRequest();
            
            static WireHost* instance;
    };

}

#endif // __TwiHost_h__