#pragma once
#include "../Device.hpp"
#include <Arduino.h>

/**
 * @brief UART tester device class
 *
 * While testing a UART port, U're gonna connect the RX and TX pins together,
 * then send data to the port and check if the same data is received back.
 *
 */
class UARTPort : public Device<String>
{
private:
    uint32_t _tx_pin;
    uint32_t _rx_pin;
    uint32_t _baudRate;
    HardwareSerial &_serial;

    String read() override
    {
        return _serial.readString();
    }

    /**
     * @brief Send a string to the UART port.
     * @param value The string to send.
     * @return true if the string is sent successfully, false otherwise.
     *
     * @note The length of the string mustn't exceed the RX buffer size,
     * otherwise nothing will be sent and the function will return false.
     */
    bool write(String value) override
    {
        if (value.length() > SERIAL_RX_BUFFER_SIZE)
        {
            return false;
        }
        _serial.print(value);
        return true;
    }

public:
    UARTPort(HardwareSerial &serial, uint32_t tx_pin, uint32_t rx_pin, uint32_t baudRate) : _serial(serial), _tx_pin(tx_pin), _rx_pin(rx_pin), _baudRate(baudRate) {}
    bool begin()
    {
        _serial.setTx(_tx_pin);
        _serial.setRx(_rx_pin);
        _serial.begin(_baudRate);
        return true;
    }

    void testAndPrintResult(std::optional<String> dataToWrite) override
    {
        if (!dataToWrite.has_value())
        {
            ULOG_ERROR("No data to write.");
            return;
        }

        String data = dataToWrite.value();
        if (!write(data))
        {
            ULOG_ERROR("Failed to write data to UART.");
            return;
        }

        String receivedData = read();
        if (receivedData != data)
        {
            ULOG_ERROR("UART data mismatch.");
            ULOG_ERROR("Sent: %s", data.c_str());
            ULOG_ERROR("Received: %s", receivedData.c_str());

            return;
        }
        ULOG_INFO("UART test passed.");
    }
};