#pragma once
#include "Device.hpp"
#include <Arduino.h>
#include <aw2023.h>

class AW2023Dev : public Device
{

private:
    AW2023 _ledController;

    bool init() override
    {
        if (!_ledController.begin())
            return false;
        if (!_ledController.switchSyncMode(true))
            return false;
        if (!_ledController.ledConfig(0, true, true, true, 0x0F))
            return false;

        return true;
    }

    bool test() override
    {
        if (!_ledController.setPWM(255, 255, 255))
            return false;

        delay(100);

        if (!_ledController.setPWM(0, 0, 0))
            return false;

        return true;
    }

public:
    AW2023Dev(String name, TwoWire &bus) : Device(name), _ledController(bus) {}
};