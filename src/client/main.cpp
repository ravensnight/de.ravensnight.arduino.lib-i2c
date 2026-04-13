#include <Arduino.h>
#include <i2c/I2CController.h>
#include <i2c/util/SimpleController.h>

#include <utils/BufferInputStream.h>
#include <utils/BufferOutputStream.h>

#include <Logger.h>
#include <SerialLogAdapter.h>

using namespace ravensnight::utils;
using namespace ravensnight::logging;
using namespace ravensnight::i2c;
using namespace ravensnight::i2c::util;

namespace ravensnight::logging {
    LogLevel getLogLevel(const char* category) {
        return LogLevel::trace;
    }
}

#if I2C_IMPL == 2
    I2CController client;
#else 
    I2CController client(&Wire);

    #ifdef ESP32
        #define PIN_I2C_SCL GPIO_NUM_9
        #define PIN_I2C_SDA GPIO_NUM_10
    #endif
#endif

SimpleController controller(client);
SerialLogAdapter logAdapter;

uint8_t counter = 0;

void setup() {

    #ifdef ESP32
        #if I2C_IMPL == 1
            Wire.setPins(PIN_I2C_SDA, PIN_I2C_SCL);
        #endif
    #endif

    Logger::setup(&logAdapter);    

    client.setUseChecksum(true);
    client.connect(0x01);    
}

#define DELAY 400

void loop() {

    // BufferInputStream is(send, BUFLEN);
    uint16_t state;
    
    if (controller.getState(state) > 0) {
        Logger::root.debug("Received initial state: %d", state);
    } else {
        Logger::root.error("Retrieving initial state failed.", state);
    }

    delay(DELAY);

    uint8_t val = 10 - counter;
    Logger::root.debug("Set value %d at position %d", val, counter);
    controller.setDetails(counter, val);

    delay(DELAY);

    Logger::root.debug("Request value at position %d", counter);
    if (controller.getDetails(counter, val) > 0) {
        Logger::root.debug("Received value %d", val);
    }

    delay(DELAY);

    if (controller.getState(state) > 0) {
        Logger::root.debug("Received state post update: %d", state);
    } else {
        Logger::root.error("Retrieving updated state failed.", state);
    }
    
    counter++;
    if (counter >= 10) {
        controller.resetDevice();
        counter = 0;
    }

    delay(DELAY);
}