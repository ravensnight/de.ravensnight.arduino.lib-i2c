#ifndef __I2CHost_h__
#define __I2CHost_h__

#include <i2c/configure.h>

#if defined(I2C_WIRE)

#include <i2c/wire/WireHost.h>
using namespace ravensnight::i2c::wire;
namespace ravensnight::i2c {
    typedef WireHost I2CHost;
}

#elif defined(I2C_IDF)

#include <i2c/idf/IdfHost.h>

using namespace ravensnight::i2c::idf;
namespace ravensnight::i2c {
    typedef IdfHost I2CHost;
}

#elif defined(I2C_AVR)

#include <i2c/avr/AvrHost.h>

using namespace ravensnight::i2c::avr;
namespace ravensnight::i2c {
    typedef AvrHost I2CHost;
}

#else
    #error No I2CHost implementation selected.
#endif // I2C_*

#endif // __I2CHost_h__