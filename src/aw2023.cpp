#include "aw2023.h"

#define BIT(x) (1U << (x))

#define AW2023_REG_RSTR 0x00
#define AW2023_REG_GCR1 0x01
#define AW2023_REG_ISR 0x02
#define AW2023_REG_PATST 0x03
#define AW2023_REG_GCR2 0x04
#define AW2023_REG_LCTR 0x30
#define AW2023_REG_LCFG0 0x31
#define AW2023_REG_LCFG1 0x32
#define AW2023_REG_LCFG2 0x33
#define AW2023_REG_PWM0 0x34
#define AW2023_REG_PWM1 0x35
#define AW2023_REG_PWM2 0x36
#define AW2023_REG_LED0T0 0x37
#define AW2023_REG_LED0T1 0x38
#define AW2023_REG_LED0T2 0x39
#define AW2023_REG_LED1T0 0x3A
#define AW2023_REG_LED1T1 0x3B
#define AW2023_REG_LED1T2 0x3C
#define AW2023_REG_LED2T0 0x3D
#define AW2023_REG_LED2T1 0x3E
#define AW2023_REG_LED2T2 0x3F

#define AW2023_CHIP_ID 0x09
#define AW2023_CHIP_RESET 0x55
#define AW2023_CHIP_EN BIT(0)
#define AW2023_FREQ_250Hz 0
#define AW2023_FREQ_125Hz BIT(5)
#define AW2023_PWM_LIN BIT(3)
#define AW2023_LED2_EN BIT(2)
#define AW2023_LED1_EN BIT(1)
#define AW2023_LED0_EN BIT(0)
#define AW2023_LED_SYNC_MODE BIT(7)
#define AW2023_LED_FADE_OUT BIT(6)
#define AW2023_LED_FADE_IN BIT(5)
#define AW2023_LED_PATERN_MODE BIT(4)
#define AW2023_T1_0_0s 0
#define AW2023_T1_0_13s BIT(4)
#define AW2023_T1_0_26s BIT(5)
#define AW2023_T1_0_38s BIT(4) | BIT(5)
#define AW2023_T1_0_51s BIT(6)
#define AW2023_T1_0_77s BIT(4) | BIT(6)
#define AW2023_T1_1_04s BIT(5) | BIT(6)
#define AW2023_T1_1_6s BIT(4) | BIT(5) | BIT(6)
#define AW2023_T1_2_1s BIT(7)
#define AW2023_T1_2_6s BIT(4) | BIT(7)
#define AW2023_T1_3_1s BIT(5) | BIT(7)
#define AW2023_T1_4_2s BIT(4) | BIT(5) | BIT(7)
#define AW2023_T1_5_2s BIT(6) | BIT(7)
#define AW2023_T1_6_2s BIT(4) | BIT(6) | BIT(7)
#define AW2023_T1_7_3s BIT(5) | BIT(6) | BIT(7)
#define AW2023_T1_8_3s BIT(4) | BIT(5) | BIT(6) | BIT(7)
#define AW2023_T2_0_04s 0
#define AW2023_T2_0_13s BIT(0)
#define AW2023_T2_0_26s BIT(1)
#define AW2023_T2_0_38s BIT(0) | BIT(1)
#define AW2023_T2_0_51s BIT(2)
#define AW2023_T2_0_77s BIT(0) | BIT(2)
#define AW2023_T2_1_04s BIT(1) | BIT(2)
#define AW2023_T2_1_6s BIT(0) | BIT(1) | BIT(2)
#define AW2023_T2_2_1s BIT(3)
#define AW2023_T2_2_6s BIT(0) | BIT(3)
#define AW2023_T2_3_1s BIT(1) | BIT(3)
#define AW2023_T2_4_2s BIT(0) | BIT(1) | BIT(3)
#define AW2023_T2_5_2s BIT(2) | BIT(3)
#define AW2023_T2_6_2s BIT(0) | BIT(2) | BIT(3)
#define AW2023_T2_7_3s BIT(1) | BIT(2) | BIT(3)
#define AW2023_T2_8_3s BIT(0) | BIT(1) | BIT(2) | BIT(3)
#define AW2023_T3_0_0s 0
#define AW2023_T3_0_13s BIT(4)
#define AW2023_T3_0_26s BIT(5)
#define AW2023_T3_0_38s BIT(4) | BIT(5)
#define AW2023_T3_0_51s BIT(6)
#define AW2023_T3_0_77s BIT(4) | BIT(6)
#define AW2023_T3_1_04s BIT(5) | BIT(6)
#define AW2023_T3_1_6s BIT(4) | BIT(5) | BIT(6)
#define AW2023_T3_2_1s BIT(7)
#define AW2023_T3_2_6s BIT(4) | BIT(7)
#define AW2023_T3_3_1s BIT(5) | BIT(7)
#define AW2023_T3_4_2s BIT(4) | BIT(5) | BIT(7)
#define AW2023_T3_5_2s BIT(6) | BIT(7)
#define AW2023_T3_6_2s BIT(4) | BIT(6) | BIT(7)
#define AW2023_T3_7_3s BIT(5) | BIT(6) | BIT(7)
#define AW2023_T3_8_3s BIT(4) | BIT(5) | BIT(6) | BIT(7)
#define AW2023_T4_0_04s 0
#define AW2023_T4_0_13s BIT(0)
#define AW2023_T4_0_26s BIT(1)
#define AW2023_T4_0_38s BIT(0) | BIT(1)
#define AW2023_T4_0_51s BIT(2)
#define AW2023_T4_0_77s BIT(0) | BIT(2)
#define AW2023_T4_1_04s BIT(1) | BIT(2)
#define AW2023_T4_1_6s BIT(0) | BIT(1) | BIT(2)
#define AW2023_T4_2_1s BIT(3)
#define AW2023_T4_2_6s BIT(0) | BIT(3)
#define AW2023_T4_3_1s BIT(1) | BIT(3)
#define AW2023_T4_4_2s BIT(0) | BIT(1) | BIT(3)
#define AW2023_T4_5_2s BIT(2) | BIT(3)
#define AW2023_T4_6_2s BIT(0) | BIT(2) | BIT(3)
#define AW2023_T4_7_3s BIT(1) | BIT(2) | BIT(3)
#define AW2023_T4_8_3s BIT(0) | BIT(1) | BIT(2) | BIT(3)
#define AW2023_T0_0_4s 0
#define AW2023_T0_0_13s BIT(4)
#define AW2023_T0_0_26s BIT(5)
#define AW2023_T0_0_38s BIT(4) | BIT(5)
#define AW2023_T0_0_51s BIT(6)
#define AW2023_T0_0_77s BIT(4) | BIT(6)
#define AW2023_T0_1_04s BIT(5) | BIT(6)
#define AW2023_T0_1_6s BIT(4) | BIT(5) | BIT(6)
#define AW2023_T0_2_1s BIT(7)
#define AW2023_T0_2_6s BIT(4) | BIT(7)
#define AW2023_T0_3_1s BIT(5) | BIT(7)
#define AW2023_T0_4_2s BIT(4) | BIT(5) | BIT(7)
#define AW2023_T0_5_2s BIT(6) | BIT(7)
#define AW2023_T0_6_2s BIT(4) | BIT(6) | BIT(7)
#define AW2023_T0_7_3s BIT(5) | BIT(6) | BIT(7)
#define AW2023_T0_8_3s BIT(4) | BIT(5) | BIT(6) | BIT(7)
#define AW2023_REPEAT_TIMES(time) (time & 0x0F)

bool AW2023::begin()
{
    uint8_t chipId;
    if (!readRegister(AW2023_REG_RSTR, &chipId))
        return false;

    if (chipId != AW2023_CHIP_ID)
        return false;

    if (!writeRegister(AW2023_REG_RSTR, AW2023_CHIP_RESET))
        return false;
    delay(5);

    if (!writeRegister(AW2023_REG_GCR1, AW2023_CHIP_EN))
        return false;

    if (!writeRegister(AW2023_REG_GCR2, AW2023_FREQ_250Hz))
        return false;

    if (!writeRegister(AW2023_REG_LCTR, AW2023_FREQ_250Hz | AW2023_LED0_EN | AW2023_LED1_EN | AW2023_LED2_EN))
        return false;

    return true;
}

bool AW2023::switchSyncMode(bool enable)
{
    uint8_t temp;
    if (!readRegister(AW2023_REG_LCFG0, &temp))
        return false;

    if (enable)
        temp |= AW2023_LED_SYNC_MODE;
    else
        temp &= ~AW2023_LED_SYNC_MODE;

    if (!writeRegister(AW2023_REG_LCFG0, temp))
        return false;

    return true;
}

bool AW2023::setMaxCurrent(MaxCurrent current)
{
    uint8_t gcr2;
    if (!readRegister(AW2023_REG_GCR2, &gcr2))
        return false;

    gcr2 = (gcr2 & 0xFC) | current;

    if (!writeRegister(AW2023_REG_GCR2, gcr2))
        return false;

    return true;
}

bool AW2023::setPWM(uint8_t led0, uint8_t led1, uint8_t led2)
{
    if (!writeRegister(AW2023_REG_PWM0, led0))
        return false;
    if (!writeRegister(AW2023_REG_PWM1, led1))
        return false;
    if (!writeRegister(AW2023_REG_PWM2, led2))
        return false;

    return true;
}

bool AW2023::ledConfig(uint8_t id, bool fadeIn, bool fadeOut, bool patternMode, uint8_t current)
{
    if (id > 2 || current > 15)
        return false;

    uint8_t config = 0;
    if (fadeIn)
        config |= AW2023_LED_FADE_IN;
    if (fadeOut)
        config |= AW2023_LED_FADE_OUT;
    if (patternMode)
        config |= AW2023_LED_PATERN_MODE;

    config |= current;

    uint8_t reg = AW2023_REG_LCFG0 + id;
    if (!writeRegister(reg, config))
        return false;

    return true;
}

bool AW2023::readRegister(uint8_t reg, uint8_t *data)
{
    _wire.beginTransmission(slaveAddress);
    _wire.write(reg);
    if (_wire.endTransmission(false) != 0)
    {
        return false;
    }

    if (_wire.requestFrom(slaveAddress, static_cast<uint8_t>(1)) != 1)
    {
        return false;
    }
    *data = _wire.read();
    return true;
}

bool AW2023::writeRegister(uint8_t reg, uint8_t data)
{
    _wire.beginTransmission(slaveAddress);
    _wire.write(reg);
    _wire.write(data);
    if (_wire.endTransmission() != 0)
    {
        return false;
    }
    return true;
}