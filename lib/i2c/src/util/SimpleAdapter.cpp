#include <i2c/util/SimpleAdapter.h>
#include <utils/StreamHelper.h>

using namespace ravensnight::utils;
using namespace ravensnight::i2c;
using namespace ravensnight::i2c::util;

SimpleAdapter::SimpleAdapter(SimpleAdapterModel& model) : _model(model) {
    _command = Command::GetState;
    _index = 0;
}

bool SimpleAdapter::parse(const uint8_t* buffer, uint8_t bytes) {
    if (bytes > 0) {    
        _command = (Command)(buffer[0] >> 6);
        _index = 0;

        switch (_command) {

            case Command::GetState: {
                return true;
            }

            case Command::GetValue: {
                _index = buffer[0] & 0x3F;
                return true;
            }

            case Command::SetValue: {
                _index = buffer[0] & 0x3F;

                int8_t len = _model.getDetailsSize(_index);            // get size expected
                if ((len <= 0) || (len < (uint8_t)(bytes - 1))) {       // check size expected being less then available.
                    return false;
                }

                _model.setDetails(_index, buffer + 1);
                return true;                
            }

            case Command::ResetValues: {
                _model.reset();
                return true;
            }

            case Command::ResetDevice: {
                _model.reboot();
                return true;
            }

            default:
                break;
        }
    }  

    return false;
}

int16_t SimpleAdapter::prepareResponse(uint8_t* buffer, uint8_t maxLen) {
    switch (_command) {
        case Command::GetState: {
            if (maxLen < 2) {
                return -1;                
            }

            uint16_t state = _model.getState();
            StreamHelper::write16(buffer, state);
            
            return 2;
        }

        case Command::GetValue: {
            int8_t len = _model.getDetailsSize(_index);
            if ((len <= 0) || (len > (int8_t)maxLen)) {
                return -1;
            }

            _model.getDetails(_index, buffer);
            return len;
        }

        default:
            return -1;
    }

}
