#pragma once
#include "../Device.hpp"
#include <Arduino.h>
#include <SPI.h>
#include <BMI088.h>

struct BMI088Data
{
    float accelX;
    float accelY;
    float accelZ;
    float gyroX;
    float gyroY;
    float gyroZ;
    float temperature;
};

class BMI088DUT : public Device<BMI088Data>
{
public:
    BMI088DUT(SPIClass &bus, uint32_t acc_cs_pin, uint32_t gyro_cs_pin) : _imu(bus, acc_cs_pin, gyro_cs_pin) {}

private:
    Bmi088 _imu;

    /**
     * @brief Read data from the BMI088 sensor.
     * @return The read data.
     */
    BMI088Data read() override
    {
        _imu.readSensor();

        return BMI088Data{
            _imu.getAccelX_mss(),
            _imu.getAccelY_mss(),
            _imu.getAccelZ_mss(),
            _imu.getGyroX_rads(),
            _imu.getGyroY_rads(),
            _imu.getGyroZ_rads(),
            _imu.getTemperature_C()};
    }

    /**
     * @brief Not used, just return true.
     * @param value Foo
     * @return true
     */
    bool write(BMI088Data value) override
    {
        UNUSED(value);
        return true;
    };

public:
    bool begin()
    {
        _imu.begin();
        return true;
    }

    void testAndPrintResult(std::optional<BMI088Data> dataToWrite) override
    {
        UNUSED(dataToWrite);

        auto data = read();

        auto result =
            String("BMI088 Sensor Data:\n") +
            "AccX=" + String(data.accelX) +
            ", AccY=" + String(data.accelY) +
            ", AccZ=" + String(data.accelZ) +
            "\n" +
            "GyroX=" + String(data.gyroX) +
            ", GyroY=" + String(data.gyroY) +
            ", GyroZ=" + String(data.gyroZ) +
            "\n" +
            "Temperature (C): " + String(data.temperature) +
            "\n";

        ULOG_INFO("BMI088 Sensor Data:\n%s", result.c_str());
    }
};