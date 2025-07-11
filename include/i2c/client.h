#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <i2c/common.h>

namespace ravensnight::i2c {
class I2CClient {

    private:
        
        TwoWire* _i2c = 0;
        uint8_t _address = 0;
        uint8_t _stop = 0;

        void sendData(uint8_t command, uint8_t index, const uint8_t buffer[], uint8_t len);

    public:

        I2CClient();

        void setup(TwoWire* twi, uint8_t address);
        void sendStopWithRequest(bool stop);
        void skipAllAvailable();

        int16_t getState(uint16_t& state);

        int16_t getDetails(uint8_t index, uint8_t& value);
        int16_t getDetails(uint8_t index, uint16_t& value);
        int16_t getDetails(uint8_t index, uint8_t buffer[], uint8_t len);

        void setDetails(uint8_t index, uint8_t value);
        void setDetails(uint8_t index, uint16_t value);
        void setDetails(uint8_t index, const uint8_t buffer[], uint8_t len);

        void resetMaster();

        static void waitFor(TwoWire* conn, const uint8_t addrs[], uint8_t size, uint16_t delayTime);
        static void scanAll(TwoWire* conn);

};

}

#endif // __CLIENT_H__