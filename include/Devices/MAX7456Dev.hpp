#pragma once
#include "Device.hpp"
#include <Arduino.h>
#include <SPI.h>
#include <max7456.h>

// TODO: Modify the OSD driver library to get the results of driver functions.
class MAX7456Dev : public Device
{

private:
    SPIClass &_bus;
    Max7456 _osd;
    uint32_t _csPin;

    bool init() override
    {
        _osd.init(_csPin);
        _osd.setDisplayOffsets(60, 18);
        _osd.setBlinkParams(_8fields, _BT_BT);

        _osd.activateOSD();

        return true;
    }

    bool test() override
    {
        _osd.print(millis(), 10, 6, 2, 0, false, true);
        delay(100);

        return true;
    }

public:
    MAX7456Dev(String name, SPIClass &bus, uint32_t csPin)
        : Device(name), _bus(bus), _csPin(csPin) {}
};