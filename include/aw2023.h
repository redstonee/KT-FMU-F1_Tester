#pragma once

#include <Arduino.h>
#include <Wire.h>

class AW2023
{
public:
    enum MaxCurrent
    {
        MAX_15mA = 0,
        MAX_30mA,
        MAX_5mA,
        MAX_10mA,
    };

private:
    static constexpr uint8_t slaveAddress = 0x45;

    TwoWire &_wire;

    bool readRegister(uint8_t reg, uint8_t *data);
    bool writeRegister(uint8_t reg, uint8_t data);

public:
    AW2023(TwoWire &wire) : _wire(wire) {}

    bool begin();
    bool switchSyncMode(bool enable);
    bool setMaxCurrent(MaxCurrent current);
    bool setPWM(uint8_t led0, uint8_t led1, uint8_t led2);
    bool ledConfig(uint8_t id, bool fadeIn, bool fadeOut, bool opMode,uint8_t current);
};