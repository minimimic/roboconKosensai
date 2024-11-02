/*
 * PlayStation Controller library
 * Copyright (c) 2013 Hiroshi Suga
 */

#include "PS_PAD.hpp"

PS_PAD::PS_PAD (PinName mosi, PinName miso, PinName sck, PinName cs) : _spi(mosi, miso, sck), _cs(cs) {
    _spi.format(8, 3);
    _spi.frequency(250000);
    _cs = 1;
    _vib1 = 0;
    _vib2 = 0;
    _connected = false;
    
}

// PS_PAD::PS_PAD (SPI &spi, PinName cs) : _spi(spi), _cs(cs) {
//     _spi.format(8, 3);
//     _spi.frequency(250000);
//     _cs = 1;
//     _vib1 = 0;
//     _vib2 = 0;
//     _connected = false;
// }

int PS_PAD::init () {
    const char enter_config_mode[5]  = {0x01, 0x43, 0x00, 0x01, 0x00};
    const char enable_analog_mode[9] = {0x01, 0x44, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00};
    const char enable_vibration[9]   = {0x01, 0x4d, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff};
    const char exit_config_mode[9]   = {0x01, 0x43, 0x00, 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A};
    char buf[10];
 
    send(enter_config_mode, 5, buf);
    if (buf[2] == 0xff) {
        return -1;
    }
    wait_us(16000);
    send(enable_analog_mode, 9, buf);
    wait_us(16000);
    send(enable_vibration, 9, buf);
    wait_us(16000);
    send(exit_config_mode, 9, buf);
    wait_us(16000);
    return 0;
}

int PS_PAD::poll () {
    const char poll_command[9] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    int i;
    char cmd[10], buf[10];

    memcpy(cmd, poll_command, 9);
    cmd[3] = _vib1;
    cmd[4] = _vib2;
    send(cmd, 9, buf);
    if (buf[2] != 0x5a) {
        return -1;
    }

    for (i = 0; i < 6; i ++) {
        _pad[i] = buf[3 + i];
    }
    _connected = true;
    return 0;
}

int PS_PAD::read (TYPE t) {
    bool isConnected = !(_pad[2]==255&&_pad[3]==255&&_pad[4]==255&&_pad[5]==255);
    if(t==IS_CONNECTED) return isConnected;
    if (!_connected) {
        return 0;
    }

    switch (t) {
    case PAD_LEFT:
        return _pad[0] & 0x80 ? 0 : 1;
    case PAD_BOTTOM:
        return _pad[0] & 0x40 ? 0 : 1;
    case PAD_RIGHT:
        return _pad[0] & 0x20 ? 0 : 1;
    case PAD_TOP:
        return _pad[0] & 0x10 ? 0 : 1;
    case PAD_START:
        return _pad[0] & 0x08 ? 0 : 1;
    case ANALOG_LEFT:
        return _pad[0] & 0x04 ? 0 : 1;
    case ANALOG_RIGHT:
        return _pad[0] & 0x02 ? 0 : 1;
    case PAD_SELECT:
        return _pad[0] & 0x01 ? 0 : 1;
    case PAD_SQUARE:
        return _pad[1] & 0x80 ? 0 : 1;
    case PAD_X:
        return _pad[1] & 0x40 ? 0 : 1;
    case PAD_CIRCLE:
        return _pad[1] & 0x20 ? 0 : 1;
    case PAD_TRIANGLE:
        return _pad[1] & 0x10 ? 0 : 1;
    case PAD_R1:
        return _pad[1] & 0x08 ? 0 : 1;
    case PAD_L1:
        return _pad[1] & 0x04 ? 0 : 1;
    case PAD_R2:
        return _pad[1] & 0x02 ? 0 : 1;
    case PAD_L2:
        return _pad[1] & 0x01 ? 0 : 1;
    case BUTTONS:
        return ~((_pad[1] << 8) | _pad[0]) & 0xffff;
    case ANALOG_RX:
        return _pad[2] - 0x80;
    case ANALOG_RY:
        return (_pad[3] - 0x80) * -1;
    case ANALOG_LX:
        return _pad[4] - 0x80;
    case ANALOG_LY:
        return (_pad[5] - 0x80) * -1;
    case IS_CONNECTED:
        break;
    }
    return 0;
}

int PS_PAD::vibration (int v1, int v2) {
    _vib1 = v1 ? 1 : 0;
    if (v2 < 0) v2 = 0;
    if (v2 > 0xff) v2 = 0;
    _vib2 = v2;
    poll();
    return 0;
}

int PS_PAD::send (const char *cmd, int len, char *dat) {
    int i;

    _cs = 0;
    wait_us(10);
    for (i = 0; i < len; i ++) {
        dat[i] = __rbit(_spi.write(__rbit(cmd[i] << 24)) << 24);
        wait_us(10);
    }
    _cs = 1;
    return i;
}

//自分で実装
uint32_t PS_PAD::__rbit(uint32_t x) {
    uint32_t r = 0;
    int b = 32;
    while (b--) {
        r <<= 1;
        r |= (x & 1);
        x >>= 1;
    }
    return r;
}

