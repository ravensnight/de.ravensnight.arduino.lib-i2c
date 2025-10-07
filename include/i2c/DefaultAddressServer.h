#ifndef __DefaultAddressServer_h__
#define __DefaultAddressServer_h__

#include <Arduino.h>
#include <i2c/I2CDeviceHandler.h>

namespace ravensnight::i2c {

    enum class ServerState : uint16_t {
        ready = 0,
        noaddress = 1
    };

    class DefaultAddressServer : public I2CDeviceHandler {

        private: 

            uint16_t _next = 0x0;
            uint8_t _min = 0x0;
            uint8_t _max = 0x0;

        protected:

            virtual bool hasMoreAdresses();
            virtual int16_t getNextAddress();

        public:

            DefaultAddressServer(uint8_t min, uint8_t max);

            /**
             * Provide some state information.
             */
            uint16_t getState();

            /**
             * Provide the data size you expect for element at given index
             */
            int8_t getDetailsSize(uint8_t index);

            /**
             * Update element from given buffer as sent by controller
             */
            void setDetails(uint8_t index, const uint8_t buffer[]);

            /**
             * Fill the buffer so that I2C Device can sent data to its controller.
             */
            void getDetails(uint8_t index, uint8_t data[]);

            /**
             * Reset the internal states.
             */
            void reset();            
    };
}


#endif // __DefaultAddressServer_h__