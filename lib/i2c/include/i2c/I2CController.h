#ifndef __I2CController_h__
#define __I2CController_h__

#include <i2c/configure.h>

#if I2C_IMPL == 2

    #include <i2c/avr/AvrController.h>
    using namespace ravensnight::i2c::avr;
    namespace ravensnight::i2c {
        typedef class AvrController I2CController;
    }

#elif I2C_IMPL == 1

    #include <i2c/wire/WireController.h>
    using namespace ravensnight::i2c::wire;
    namespace ravensnight::i2c {
        typedef class WireController I2CController;
    }

#else
    #error No I2CController implementation selected.
#endif // I2C_CLIENT_*

#endif // __I2CController_h__
