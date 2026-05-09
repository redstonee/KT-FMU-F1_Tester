#pragma once
#include "Device.hpp"
#include <Arduino.h>
#include <SPI.h>
#include <BMI088.h>

class BMI088Dev : public Device
{

private:
    Bmi088 _imu;

    struct Data
    {
        float accelX;
        float accelY;
        float accelZ;
        float gyroX;
        float gyroY;
        float gyroZ;
        float temperature;
    };

    /**
     * @brief Read data from the BMI088 sensor.
     * @return The read data.
     */
    inline Data read()
    {
        delay(10); // delay to ensure data is ready
        _imu.readSensor();
        return Data{
            _imu.getAccelX_mss(),
            _imu.getAccelY_mss(),
            _imu.getAccelZ_mss(),
            _imu.getGyroX_rads(),
            _imu.getGyroY_rads(),
            _imu.getGyroZ_rads(),
            _imu.getTemperature_C()};
    }

    bool init() override
    {
        if (_imu.begin() < 0)
            return false;
            
        if (!_imu.setOdr(Bmi088::ODR_400HZ))
            return false;

        return true;
    }

    bool test() override
    {
        auto data = read();
        auto result =
            String("Sensor Data:\n") +
            "AccX = " + String(data.accelX) +
            ", AccY = " + String(data.accelY) +
            ", AccZ = " + String(data.accelZ) +
            "\n" +
            "GyroX = " + String(data.gyroX) +
            ", GyroY = " + String(data.gyroY) +
            ", GyroZ = " + String(data.gyroZ) +
            "\n" +
            "Temperature = " + String(data.temperature) +
            "\n";

        logi(result);
        return true;
    }

public:
    BMI088Dev(String name, SPIClass &bus, uint32_t acc_cs_pin, uint32_t gyro_cs_pin)
        : Device(name), _imu(bus, acc_cs_pin, gyro_cs_pin) {}
};