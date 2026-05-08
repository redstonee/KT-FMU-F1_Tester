#pragma once
#include <optional>
#include <ulog.h>

template <typename T>
class Device
{
protected:
    virtual T read() = 0;
    virtual bool write(T value) = 0;
    

public:
    bool begin()
    {
        return true;
    };

    virtual void testAndPrintResult(std::optional<T> dataToWrite) = 0;
};