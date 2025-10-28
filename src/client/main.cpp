#include <Arduino.h>
#include <i2c/I2CClient.h>

#include <utils/BufferInputStream.h>
#include <utils/BufferOutputStream.h>

#include <Logger.h>
#include <DefaultSink.h>

using namespace ravensnight::utils;
using namespace ravensnight::logging;
using namespace ravensnight::i2c;

#ifdef I2C_AVR
    I2CClient client;
#else 
    I2CClient client(&Wire);

    #define PIN_I2C_SCL GPIO_NUM_5
    #define PIN_I2C_SDA GPIO_NUM_4
#endif

DefaultSink loggerSink(&Serial);

#define BUFLEN (uint8_t)10
uint8_t txBuffer[BUFLEN] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
uint8_t rxBuffer[BUFLEN] { 0 };

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

void loop() {

    // BufferInputStream is(send, BUFLEN);

    Logger::dump("Send some data.", txBuffer, BUFLEN, 0);
    client.send(txBuffer, BUFLEN);

    Logger::debug("Request some data without parameters.");
    int16_t len = client.request(rxBuffer, BUFLEN);

    if (len >= 0) {
        Logger::dump("Received data.", rxBuffer, len, 0);
        Logger::debug("Send data received.");
        Logger::debug("Request some data with parameters.");
        client.request(rxBuffer, BUFLEN, rxBuffer, BUFLEN);
    } else {
        Logger::error("Receive failed.");
    }

    delay(500);
}