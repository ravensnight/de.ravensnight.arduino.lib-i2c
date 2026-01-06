#ifndef __I2CDevice_h__
#define __I2CDevice_h__

#include <i2c/configure.h>

#if I2C_IMPL == 2

#include <i2c/avr/AvrDevice.h>

using namespace ravensnight::i2c::avr;
namespace ravensnight::i2c {
    typedef AvrDevice I2CDevice;
}

#elif I2C_IMPL == 1

#include <i2c/wire/WireDevice.h>
using namespace ravensnight::i2c::wire;
namespace ravensnight::i2c {
    typedef WireDevice I2CDevice;
}

#else
    #error No I2CHost implementation selected.
#endif // I2C_*

#endif // __I2CHost_h__