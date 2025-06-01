#ifndef __I2C_H__
#define __I2C_H__

#include <Wire.h>

namespace ravensnight::i2c {

/**
 * This I2C Library expects a single byte to be sent as request.
 * the following requests are supported:
 * 1) get state:    command = 0x00
 * 2) get details:  command = 0x40 + (elenent index & 0x3F)
 * 3) set details:  command = 0x80 + (element index & 0x3F)
 * 4) do a reset:   command = 0xFF 
 */

typedef enum {

    GetState   = 0x0,     // Read the dirty mask
    GetDetails = 0x1,     // get some details for indexed element
    SetDetails = 0x2,     // set some details on indexed element
    Reset      = 0x3      // Reset all values    

} Command;

}

#endif // __I2C_H__
