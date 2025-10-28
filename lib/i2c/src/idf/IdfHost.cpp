#ifdef I2C_IDF

#include <i2c/idf/IdfHost.h>
#include <i2c/LoggerConfig.h>

namespace ravensnight::i2c::idf {

IdfHost::IdfHost(IdfHostNum hostNum) {
    _sda = GPIO_NUM_8;
    _scl = GPIO_NUM_9;
    _hostNum = hostNum;
}

/**
 * Assign some pins.
 */
void IdfHost::setPins(gpio_num_t sda, gpio_num_t scl) {
    _sda = sda;
    _scl = scl;
}

/**
 * Install the host.
 */
bool IdfHost::install(uint8_t hostAddr) {

    // ------------------------------------------------------------------------
    // check creating a queue
    // ------------------------------------------------------------------------
    _queueHandle = xQueueCreate(IDFHOST_QUEUE_LEN, sizeof(IdfHostEvent));
    if (!_queueHandle) {
        _logger.error("Failed to create queue.");
        return false;
    }
    
    // ------------------------------------------------------------------------
    // setup the device
    // ------------------------------------------------------------------------
    i2c_port_t port = (_hostNum == IdfHostNum::slave_0) ? I2C_NUM_0 : I2C_NUM_1;
    i2c_slave_config_t i2c_slv_config = {
        .i2c_port = port,
        .sda_io_num = _sda,
        .scl_io_num = _scl,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .send_buf_depth = IDFHOST_TX_BUF_LEN,
        .receive_buf_depth = IDFHOST_RX_BUF_LEN,
        .slave_addr = hostAddr,
    };

    ESP_ERROR_CHECK(i2c_new_slave_device(&i2c_slv_config, &_slaveHandle));

    // ------------------------------------------------------------------------
    // install the callbacks
    // ------------------------------------------------------------------------
    i2c_slave_event_callbacks_t cbs = {
        .on_request = i2c_slave_request_cb,
        .on_receive = i2c_slave_receive_cb
    };
    ESP_ERROR_CHECK(i2c_slave_register_event_callbacks(_slaveHandle, &cbs, this));

    // ------------------------------------------------------------------------
    // Create the queue listener task
    // ------------------------------------------------------------------------
    xTaskCreate(IdfHost::i2c_slave_queue_listener, "IdfHost", 1024 * 4, this, 10, &_taskHandle);

    return true;
}

QueueHandle_t& IdfHost::queue() {
    return _queueHandle;
}

int16_t IdfHost::receiveBytes(const uint8_t* inBuffer, uint8_t len) {
    if (len > IDFHOST_RX_BUF_LEN) {
        return -1;
    }

    memcpy(_rxBuffer, inBuffer, len);
    _rxLen = len;

    return len;
}

void IdfHost::processReceive() {
    if (!parseRequest(_rxBuffer, _rxLen)) {
        _logger.error("Failed to parse the data received.");
    }
}

void IdfHost::processRequest() {
    int16_t res = buildResponse(_txBuffer, IDFHOST_TX_BUF_LEN);
    if (res < 0) {
        _logger.error("Failed to build response from given data.");
        return;
    }

    uint32_t sent = 0;    
    i2c_slave_write(_slaveHandle, _txBuffer, res, &sent, IDFHOST_TX_TIMEOUT);

    if (sent != res) {
        _logger.warn("Didnt send the right number of bytes %d != %d", res, sent);
    }
}

bool IdfHost::i2c_slave_request_cb(i2c_slave_dev_handle_t i2c_slave, const i2c_slave_request_event_data_t *evt_data, void *arg) {
    IdfHostEvent evt = IdfHostEvent::request;

    IdfHost* context = (IdfHost*)arg;
    BaseType_t xTaskWoken = 0;

    xQueueSendFromISR(context->queue(), &evt, &xTaskWoken);
    return xTaskWoken;
}

bool IdfHost::i2c_slave_receive_cb(i2c_slave_dev_handle_t i2c_slave, const i2c_slave_rx_done_event_data_t *evt_data, void *arg) {

    IdfHostEvent evt = IdfHostEvent::receive;

    IdfHost* context = (IdfHost*)arg;
    BaseType_t xTaskWoken = 0;

    // You can get data and length via i2c_slave_rx_done_event_data_t
    int16_t res = context->receiveBytes(evt_data->buffer, evt_data->length);
    if (res < 0) {
        _logger.error("Could not cache data.");
        return 0;
    }

    xQueueSendFromISR(context->queue(), &evt, &xTaskWoken);
    return xTaskWoken;
}

void IdfHost::i2c_slave_queue_listener(void *arg) {
    IdfHost* context = (IdfHost*)arg;
    IdfHostEvent evt;

    while (true) {
        if (xQueueReceive(context->queue(), &evt, 10) == pdTRUE) {

            if (evt == IdfHostEvent::receive) {
                context->processReceive();
            }
            else {  // IdfHostEvent::request
                context->processRequest();
            }
        }
    }
}

ravensnight::logging::ClassLogger IdfHost::_logger(LC_I2C);

}

#endif // I2C_IDF