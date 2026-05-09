#pragma once
#include "Device.hpp"
#include <Arduino.h>

/**
 * @brief UART tester device class
 *
 * While testing a UART port, U're gonna connect the RX and TX pins together,
 * then send data to the port and check if the same data is received back.
 *
 */
class UARTPort : public Device
{
private:
    uint32_t _txPin;
    uint32_t _rxPin;
    uint32_t _baudRate;
    HardwareSerial &_serial;
    const String &_testData;

    bool init() override
    {
        _serial.setTx(_txPin);
        _serial.setRx(_rxPin);
        _serial.setTimeout(100); 
        _serial.begin(_baudRate);
        return true;
    }

    bool test() override
    {
        if (_testData.length() > SERIAL_RX_BUFFER_SIZE)
        {
            loge("Data length exceeds RX buffer size. Test failed.");
            return false;
        }

        // Clear any existing data in the buffer
        while (_serial.available())
        {
            _serial.read();
        }

        _serial.print(_testData);

        auto receivedData = _serial.readString();
        if (receivedData != _testData)
        {
            loge("Data mismatch. Test failed.");
            return false;
        }
        return true;
    }

public:
    UARTPort(String name, HardwareSerial &serial, uint32_t txPin, uint32_t rxPin, const String &testData, uint32_t baudRate = 115200)
        : Device(name), _serial(serial), _txPin(txPin), _rxPin(rxPin), _baudRate(baudRate), _testData(testData) {}
};