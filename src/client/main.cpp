#include <Arduino.h>
#include <i2c/I2CClient.h>
#include <i2c/util/SimpleController.h>

#include <utils/BufferInputStream.h>
#include <utils/BufferOutputStream.h>

#include <Logger.h>
#include <DefaultSink.h>

using namespace ravensnight::utils;
using namespace ravensnight::logging;
using namespace ravensnight::i2c;
using namespace ravensnight::i2c::util;

#ifdef I2C_AVR
    I2CClient client;
#else 
    I2CClient client(&Wire);

    #define PIN_I2C_SCL GPIO_NUM_5
    #define PIN_I2C_SDA GPIO_NUM_4
#endif

SimpleController controller(client);
DefaultSink loggerSink(&Serial);

uint8_t counter = 0;

void setup() {

    #ifdef ESP32
    Serial.begin(115200);
    Serial.setDebugOutput(true);
        #ifdef I2C_WIRE
        Wire.setPins(PIN_I2C_SDA, PIN_I2C_SCL);
        #endif
    #endif

    Logger::setLevel(LogLevel::trace);
    Logger::attach(&loggerSink);    

    client.setUseChecksum(true);
    client.setup(0x01);    
}

#define DELAY 400

void loop() {

    // BufferInputStream is(send, BUFLEN);
    uint16_t state;
    
    if (controller.getState(state) > 0) {
        Logger::debug("Received initial state: %d", state);
    } else {
        Logger::error("Retrieving initial state failed.", state);
    }

    delay(DELAY);

    uint8_t val = 10 - counter;
    Logger::debug("Set value %d at position %d", val, counter);
    controller.setDetails(counter, val);

    delay(DELAY);

    Logger::debug("Request value at position %d", counter);
    if (controller.getDetails(counter, val) > 0) {
        Logger::debug("Received value %d", val);
    }

    delay(DELAY);

    if (controller.getState(state) > 0) {
        Logger::debug("Received state post update: %d", state);
    } else {
        Logger::error("Retrieving updated state failed.", state);
    }
    
    counter++;
    if (counter >= 10) {
        controller.resetDevice();
        counter = 0;
    }

    delay(DELAY);
}