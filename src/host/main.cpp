#include <Arduino.h>

#include <utils/BufferInputStream.h>
#include <utils/BufferOutputStream.h>

#include <i2c/I2CFixedAddress.h>
#include <i2c/I2CHost.h>

#include <Logger.h>
#include <DefaultSink.h>

#include "Handler.h"

using namespace ravensnight::logging;
using namespace ravensnight::utils;
using namespace ravensnight::i2c;

#if defined(I2C_AVR)
    I2CHost host;
#else 
    I2CHost host(&Wire);

    #define PIN_I2C_SCL GPIO_NUM_5
    #define PIN_I2C_SDA GPIO_NUM_4
#endif

DefaultSink logAdapter(&Serial);

I2CFixedAddress addr(0x01);
Handler h;

void setup() {

    #ifdef ESP32
    Serial.begin(115200);
    Serial.setDebugOutput(true);

        #ifdef I2C_WIRE
        Wire.setPins(PIN_I2C_SDA, PIN_I2C_SCL);
        #endif
    #endif

    Logger::setLevel(LogLevel::trace);
    Logger::attach(&logAdapter);    
    Logger::debug("setup()");

    host.setHandler(&h);
    host.setUseChecksum(true);
    
    Logger::debug("setup() - start i2c client");
    
    host.setup(addr);
}

void loop() {
    delay(1000);
    Logger::debug("In loop()");
}