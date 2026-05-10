#pragma once
#include "Device.hpp"
#include <Arduino.h>
#include <IST8310.h>

class IST8310Dev : public Device
{

private:
    IST8310 _mag;
    TwoWire &_bus;

    static constexpr auto TIMEOUT = 100;

    Print *_print = nullptr; // IST8310 requires a Print object, but we don't actually need it for anything.

    bool init() override
    {
        return _mag.setup(_bus);
    }

    bool test() override
    {
        auto startTime = millis();
        while (!_mag.update())
        {
            if (millis() - startTime > TIMEOUT)
            {
                loge("Fucked up when reading sensor data.");
                return false;
            }
            delay(10);
        }

        auto data = _mag.get_magnetometer();
        // Formatting of float will fuck up, so just add them one by one.
        auto result =
            String("Sensor Data:\r\n") +
            "MagX = " + String(data->x) +
            ", MagY = " + String(data->y) +
            ", MagZ = " + String(data->z) +
            "\r\n";
        logi(result);
        return true;
    }

public:
    IST8310Dev(String name, TwoWire &bus) : Device(name), _bus(bus) {}
};