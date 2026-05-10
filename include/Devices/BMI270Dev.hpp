#pragma once
#include "Device.hpp"
#include <Arduino.h>
#include <SPI.h>
#include <SparkFun_BMI270_Arduino_Library.h>

class BMI270Dev : public Device
{

private:
    BMI270 _imu;
    SPIClass &_bus;
    uint32_t _csPin;

    bool init() override
    {
        if (_imu.beginSPI(_csPin, 1e6, _bus) != BMI2_OK)
            return false;

        if (_imu.setAccelODR(BMI2_ACC_ODR_400HZ) != BMI2_OK)
            return false;

        if (_imu.setGyroODR(BMI2_GYR_ODR_400HZ) != BMI2_OK)
            return false;

        return true;
    }

    bool test() override
    {
        delay(10);
        if (_imu.getSensorData() != BMI2_OK)
        {
            loge("Fucked up when reading sensor data.");
            return false;
        }

        auto data = _imu.data;
        auto result =
            String("Sensor Data:\r\n") +
            "AccX = " + String(data.accelX) +
            ", AccY = " + String(data.accelY) +
            ", AccZ = " + String(data.accelZ) +
            "\r\n" +
            "GyroX = " + String(data.gyroX) +
            ", GyroY = " + String(data.gyroY) +
            ", GyroZ = " + String(data.gyroZ) +
            "\r\n";

        logi(result);
        return true;
    }

public:
    BMI270Dev(String name, SPIClass &bus, uint32_t csPin)
        : Device(name), _bus(bus), _csPin(csPin) {}
};