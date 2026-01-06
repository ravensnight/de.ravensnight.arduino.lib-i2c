/**
 * Setup I2C Implementation to be used
 * 
 * Options:
 *  [ 1 ] .. WIRE
 *  [ 2 ] .. AVR
 * 
 */
#ifndef I2C_IMPL
    #pragma warning "No explicite I2C library selected. Using wire.h"
    #define I2C_IMPL 1
#else
    #if I2C_IMPL == 2 
        #pragma message "AVR I2C library selected."
    #elif I2C_IMPL == 1
        #pragma message "Wire I2C library selected."
    #else
        #pragma warning "Invalid wire library selected. Using wire.h"
        #undef I2C_IMPL
        #define I2C_IMPL 1
    #endif
#endif