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
            logi("Initialization successful.\n");
        else
            loge("Initialization failed.\n");
        return result;
    }

    bool runTest(uint8_t times)
    {
        logi("Running test...");
        bool result=true;

        for (uint8_t i = 0; i < times; i++)
        {
            if (!test())
            {
                result = false;
                break;
            }
        }

        if (result)
            logi("Test passed.\n");
        else
            loge("Test fucked up.\n");
        return result;
    }

};