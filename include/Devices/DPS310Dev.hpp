#pragma once
#include "Device.hpp"
#include <Arduino.h>
#include <Adafruit_DPS310.h>

class DPS310Dev : public Device
{

private:
    Adafruit_DPS310 _baro;
    TwoWire &_bus;

    static constexpr auto TIMEOUT = 500;

    bool init() override
    {
        return _baro.begin_I2C(0x76, &_bus);
    }

    bool test() override
    {
        auto startTime = millis();
        while (!((_baro.temperatureAvailable()) && (_baro.pressureAvailable())))
        {
            if (millis() - startTime > TIMEOUT)
            {
                loge("Fucked up when reading sensor data.");
                return false;
            }
            delay(10);
        }
        sensors_event_t baroEvent, tempEvent;
        _baro.getEvents(&baroEvent, &tempEvent);

        // Formatting of float will fuck up, so just add them one by one.
        auto result = String("Sensor Data:\n") +
                      "Pressure = " + String(baroEvent.pressure) + "\n" +
                      "Temperature = " + String(tempEvent.temperature) + "\n";

        logi(result);
        return true;
    }

public:
    DPS310Dev(String name, TwoWire &bus) : Device(name), _bus(bus) {}
};