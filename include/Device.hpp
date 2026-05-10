#pragma once
#include <optional>
#include <ulog.h>

class Device
{
protected:
    String _name;

    inline void logi(const String &msg)
    {
        ULOG_INFO("*%s*: %s", _name.c_str(), msg.c_str());
    }

    inline void loge(const String &msg)
    {
        ULOG_ERROR("*%s*: %s", _name.c_str(), msg.c_str());
    }

    virtual bool init() = 0;
    virtual bool test() = 0;

public:
    Device(const String &deviceName) : _name(deviceName) {}

    bool begin()
    {
        logi("Initializing...");
        auto result = init();
        if (result)
            logi("Initialization successful.\r\n");
        else
            loge("Initialization failed.\r\n");
        return result;
    }

    uint8_t runTest(uint8_t times)
    {
        logi("Running test...");
        uint8_t successCount = 0;

        for (uint8_t i = 0; i < times; i++)
        {
            successCount += test() ? 1 : 0;
        }

        char resultBuffer[50];
        snprintf(resultBuffer, sizeof(resultBuffer), "Test completed: %u/%u passed.\r\n", successCount, times);
        logi(resultBuffer);
        return successCount;
    }
};