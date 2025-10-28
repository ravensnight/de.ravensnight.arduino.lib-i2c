#if defined(I2C_AVR)
    #undef I2C_IDF
    #undef I2C_WIRE
#elif defined(I2C_IDF)
    #undef I2C_AVR
    #undef I2C_WIRE
#else 
    #undef I2C_AVR
    #undef I2C_IDF
    #ifndef I2C_WIRE
        #warning No explicite I2C library selected. Using wire.h
        #define I2C_WIRE 1
    #endif
#endif