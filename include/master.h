#ifndef __MASTER_H__
#define __MASTER_H__

#include "i2c.h"

class I2CMasterHandler {

    public:

        /**
         * Provide some state information.
         */
        virtual uint16_t getState() = 0;

        /**
         * Provide the data size you expect for element at given index
         */
        virtual int8_t getDetailsSize(uint8_t index) = 0;

        /**
         * Update element from given buffer as sent by slave
         */
        virtual void setDetails(uint8_t index, const byte buffer[]) = 0;

        /**
         * Fill the buffer so that I2C Master can sent them to slave.
         */
        virtual void getDetails(uint8_t index, byte data[]) = 0;

        /**
         * Reset the internal states.
         */
        virtual void reset();
};


class I2CMaster {

    private:

        volatile Command _command = Command::GetState;
        volatile uint8_t _index = 0;

        TwoWire* _i2c = 0;
        I2CMasterHandler* _handler = 0;

        static void i2cReceive(int bytes);
        static void i2cRequest();

        void handleReceive(int bytes);
        void handleRequest();

    public:

        static I2CMaster instance;

        I2CMaster();
        void setup(TwoWire* twi, uint8_t address, I2CMasterHandler* handler);

        void skipAllAvailable();
};


#endif // __MASTER_H__