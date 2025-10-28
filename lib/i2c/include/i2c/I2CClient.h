#ifndef __I2CClient_h__
#define __I2CClient_h__

#include <i2c/configure.h>

#if defined(I2C_WIRE)
    #include <i2c/wire/WireClient.h>
    using namespace ravensnight::i2c::wire;
    namespace ravensnight::i2c {
        typedef class WireClient I2CClient;
    }

#elif defined(I2C_IDF)
    #error IDF I2C Client not implemented yet.
    /*    
    #include <i2c/idf/IdfClient.h>
    using namespace ravensnight::i2c::idf;
    namespace ravensnight::i2c {
        typedef class IdfClient I2CClient;
    }    
    */
#elif defined(I2C_AVR)

    #include <i2c/avr/AvrClient.h>
    using namespace ravensnight::i2c::avr;
    namespace ravensnight::i2c {
        typedef class AvrClient I2CClient;
    }

#else
    #error No I2CClient implementation selected.
#endif // I2C_CLIENT_*

#endif // __I2CClient_h__