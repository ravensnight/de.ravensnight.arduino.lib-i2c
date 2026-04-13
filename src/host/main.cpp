#include <Arduino.h>

#include <utils/BufferInputStream.h>
#include <utils/BufferOutputStream.h>

#include <i2c/I2CDevice.h>
#include <i2c/util/SimpleAdapter.h>

#include <Logger.h>
#include <SerialLogAdapter.h>

#include "Model.h"

using namespace ravensnight::logging;
using namespace ravensnight::utils;
using namespace ravensnight::i2c;
using namespace ravensnight::i2c::util;

namespace ravensnight::logging {
    LogLevel getLogLevel(const char* category) {
        return LogLevel::trace;
    }
}

#if I2C_IMPL == 2
    I2CDevice device;
#else 
    I2CDevice device(&Wire);

    #ifdef ESP32
    #define PIN_I2C_SCL GPIO_NUM_9
    #define PIN_I2C_SDA GPIO_NUM_10
    #endif
#endif

SerialLogAdapter logAdapter;

#ifndef AVR_ADDR
#define AVR_ADDR 0x01
#endif

uint8_t addr = AVR_ADDR;

Model model;
SimpleAdapter deviceAdapter(model);


void setup() {

    Logger::setup(&logAdapter);    
    Logger::root.debug("setup()");

    device.setAdapter(&deviceAdapter);
    device.setUseChecksum(true);
    
    Logger::root.debug("setup() - start i2c client");
    
    device.install(addr);
}

void loop() {
    delay(1000);
    Logger::root.debug("In loop()");
}