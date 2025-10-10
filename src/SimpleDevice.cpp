#include <i2c/SimpleDevice.h>

using namespace ravensnight::i2c;

SimpleDevice::SimpleDevice(SimpleDeviceModel* model) {
    _model = model;
    _command = Command::GetState;
    _index = 0;
}

bool SimpleDevice::parse(Stream& is, size_t bytes) {
    if ((_model == 0) || (bytes < 0)) return false;

    uint8_t buffer[bytes];
    size_t avail = is.readBytes(buffer, bytes);

    if (avail < bytes) {
        return false;
    }

    if (avail > 0) {    
        _command = (Command)(buffer[0] >> 6);
        _index = 0;

        switch (_command) {

            case Command::GetState: {
                return true;
            }

            case Command::GetDetails: {
                _index = buffer[0] & 0x3F;
                return true;
            }

            case Command::SetDetails: {
                _index = buffer[0] & 0x3F;

                int8_t len = _model->getDetailsSize(_index);      // get size expected
                if ((len <= 0) || (len < (uint8_t)(avail - 1))) {           // check size expected being less then available.
                    return false;
                }

                _model->setDetails(_index, buffer + 1);
                return true;                
            }

            case Command::Reset: {
                _model->reset();
                return true;
            }

            default:
                break;
        }
    }  

    return false;
}

bool SimpleDevice::canRead() {
    if (_model == 0) return false;

    switch (_command) {
        case Command::GetDetails:
        case Command::GetState:
            return true;

        default:
            return false;
    };
}

void SimpleDevice::read(Print& out) {
    if (_model == 0) return;

    switch (_command) {
        case Command::GetState: {
            uint16_t state = _model->getState();
            out.write(0x0FF & (state >> 8));
            out.write(0x0FF & state);
            break;
        }

        case Command::GetDetails: {
            int8_t len = _model->getDetailsSize(_index);
            if (len <= 0) return;

            uint8_t buffer[len];
            _model->getDetails(_index, buffer);
            out.write(buffer, len);

            break;
        }

        default:
            break;
    }
}
