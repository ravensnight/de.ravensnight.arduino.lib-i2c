#ifndef __master_h__
#define __master_h__

#include <Arduino.h>

namespace ravensnight::i2c::avr {

    enum class twi_result : uint8_t {
        success = 0,
        no_ack = 1,
        arbitration_error = 2,    
        no_start = 3        
    };    

    class twi {

        public:

                // ----------------------------------------------------------------
                // Internal AVR twi/register Functions
                // ----------------------------------------------------------------

                static uint8_t      status();
                static void         wait_ready();            
                static uint8_t      send_init_cond();
                static uint8_t      send_data(uint8_t value);

                static void         init();
                static void         init(uint32_t clockSpeed);
                static uint8_t      read_ack(void);
                static uint8_t      read_nack(void);
                static twi_result   start(uint8_t hostAddr, bool read, uint8_t retries);
                static twi_result   write(uint8_t data);
                static void         stop(void);          
    };

}

#endif // __master_h__