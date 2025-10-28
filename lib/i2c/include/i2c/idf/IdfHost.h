#ifndef __IdfHost_h__
#define __IdfHost_h__

#include <Arduino.h>
#include <i2c/AbstractHost.h>
#include <ClassLogger.h>

// enable i2c slave version 2
#ifndef CONFIG_I2C_ENABLE_SLAVE_DRIVER_VERSION_2
#define CONFIG_I2C_ENABLE_SLAVE_DRIVER_VERSION_2 1
#endif

#include <driver/i2c_slave.h>

namespace ravensnight::i2c::idf {

    #define IDFHOST_RX_BUF_LEN  32
    #define IDFHOST_TX_BUF_LEN  32
    #define IDFHOST_QUEUE_LEN   16
    #define IDFHOST_TX_TIMEOUT  500

    enum class IdfHostNum : uint8_t {
        slave_0 = 0,
        slave_1 = 1
    };

    enum class IdfHostEvent : uint8_t {
        receive = 0,
        request = 1
    };

    class IdfHost : public AbstractHost {

        private:

            static ravensnight::logging::ClassLogger _logger;

            gpio_num_t  _sda, _scl;
            IdfHostNum  _hostNum = IdfHostNum::slave_0;
            i2c_slave_dev_handle_t _slaveHandle = 0;

            QueueHandle_t _queueHandle = 0;
            TaskHandle_t _taskHandle = 0;

            uint8_t _rxLen = 0; // the number of bytes received
            uint8_t _rxBuffer[IDFHOST_RX_BUF_LEN] = { 0 };  // the bytes received
            uint8_t _txBuffer[IDFHOST_TX_BUF_LEN] = { 0 };  // the bytes to send.

        public:

            IdfHost(IdfHostNum hostNum);

            /**
             * Assign some pins.
             */
            void setPins(gpio_num_t sda, gpio_num_t scl);

            /**
             * Install the host.
             */
            bool install(uint8_t hostAddr);

            /** Provide the q hande for static callback functions */
            QueueHandle_t& queue();

            int16_t receiveBytes(const uint8_t* inBuffer, uint8_t len);

            void processReceive();
            void processRequest();

            /**
             * Callback methods
             */
            static bool i2c_slave_request_cb(i2c_slave_dev_handle_t i2c_slave, const i2c_slave_request_event_data_t *evt_data, void *arg);
            static bool i2c_slave_receive_cb(i2c_slave_dev_handle_t i2c_slave, const i2c_slave_rx_done_event_data_t *evt_data, void *arg);
            static void i2c_slave_queue_listener(void *arg);
    };

}

#endif // __IdfHost_h__