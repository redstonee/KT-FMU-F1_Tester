#pragma once
#include "Device.hpp"
#include <Arduino.h>

/**
 * @brief ADC tester device class
 *
 * While testing an ADC port, U're gonna connect the input pin to a known voltage,
 * then read the analog value and check if it matches the expected value.
 *
 */
class ADCPort : public Device
{
private:
    uint32_t _inputPin;
    float _divider;
    float _expectedVoltage;
    uint8_t _tolerance; // in percentage, e.g., 5 means ±5%

    bool init() override
    {
        analogReadResolution(12);
        return true;
    }

    bool test() override
    {
        // Read the analog value from the input pin
        float measuredVoltage = analogRead(_inputPin) * 3.3 * _divider / 4095.0;

        // Calculate the acceptable range
        float lowerBound = _expectedVoltage * (1 - _tolerance / 100.0);
        float upperBound = _expectedVoltage * (1 + _tolerance / 100.0);

        String result = "Measured Voltage: " + String(measuredVoltage, 3) + " V ";

        // Check if the measured voltage is within the acceptable range
        if (measuredVoltage < lowerBound || measuredVoltage > upperBound)
        {
            loge(result + "Test failed.");
            return false;
        }
        
        logi(result);
        return true;
    }

public:
    ADCPort(String name, uint32_t inputPin, float divider, float expectedVoltage, uint8_t tolerance)
        : Device(name), _inputPin(inputPin), _divider(divider), _expectedVoltage(expectedVoltage), _tolerance(tolerance) {}
};