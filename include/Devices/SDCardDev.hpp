#pragma once
#include "Device.hpp"
#include <Arduino.h>
#include <STM32SD.h>

/**
 * @brief SDCard tester device class
 *
 * Before testing the SDCard, U're gonna format the SD Card to FAT32 or FAT16
 * and insert it into the slot. The tester will try to initialize the card and
 * create a file named "test.shit" (if not exists) and write some random string into it,
 * then read the content back and compare with the original string.
 *
 */
class SDCardDev : public Device
{
private:
    Sd2Card _card;
    SdFatFs _fatFs;

    uint32_t _clkPin;
    uint32_t _cmdPin;
    uint32_t _d0Pin;
    uint32_t _d1Pin;
    uint32_t _d2Pin;
    uint32_t _d3Pin;

    bool init() override
    {
        _card.setCK(_clkPin);
        _card.setCMD(_cmdPin);
        _card.setDx(_d0Pin, _d1Pin, _d2Pin, _d3Pin);
        if (!_card.init())
            return false;

        String info = "Card type: ";
        switch (_card.type())
        {
        case SD_CARD_TYPE_SD1:
            info += "SD1";
            break;
        case SD_CARD_TYPE_SD2:
            info += "SD2";
            break;
        case SD_CARD_TYPE_SDHC:
            info += "SDHC";
            break;
        default:
            info += "Unknown";
            break;
        }
        logi(info);

        if (!_fatFs.init())
        {
            loge("No FAT partition is found.");
            return false;
        }

        uint32_t volumesize = _fatFs.blocksPerCluster();
        volumesize *= _fatFs.clusterCount();
        volumesize /= 2048; // block size: 512B, convert to MiB
        logi("Volume size (MiB): " + String(volumesize));

        return true;
    }

    bool test() override
    {
        if (SD.exists("test.shit"))
        {
            SD.remove("test.shit");
        }

        auto testFile = SD.open("test.shit", FILE_WRITE);
        if (!testFile)
        {
            loge("Failed to open test file for writing.");
            return false;
        }

        String testString;
        for (uint8_t i = 0; i < random(10, 100); i++)
        {
            testString += static_cast<char>(random('0', 'z' + 1));
        }
        testString += '!'; // Terminator
        testFile.print(testString);
        testFile.close();

        testFile = SD.open("test.shit");
        if (!testFile)
        {
            loge("Failed to open test file for reading.");
            return false;
        }
        String readString = testFile.readStringUntil('!');
        readString += '!'; // Append the terminator back for comparison
        testFile.close();

        if (testString != readString)
        {
            loge("Data mismatch! Written: " + testString + ", Read: " + readString);
            return false;
        }

        return true;
    }

public:
    SDCardDev(String name, uint32_t clkPin, uint32_t cmdPin, uint32_t d0Pin, uint32_t d1Pin, uint32_t d2Pin, uint32_t d3Pin)
        : Device(name), _clkPin(clkPin), _cmdPin(cmdPin), _d0Pin(d0Pin), _d1Pin(d1Pin), _d2Pin(d2Pin), _d3Pin(d3Pin) {}
};