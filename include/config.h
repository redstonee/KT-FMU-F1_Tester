#pragma once
#include <pins_arduino.h>
#include <variant_generic.h>

struct TimerCfg
{
    uint16_t pin;
    TIM_TypeDef *timer;
    uint8_t channel;
};

constexpr TimerCfg PWMPorts[] = {
    {PE14, TIM1, 4},
    {PE13, TIM1, 3},
    {PE11, TIM1, 2},
    {PE9, TIM1, 1},
    {PB1_ALT1, TIM3, 4},
    {PB0_ALT1, TIM3, 3},
    {PD12, TIM4, 1},
    {PD13, TIM4, 2},
    {PD14, TIM4, 3},
    {PD15, TIM4, 4},
};

constexpr auto UART1_TX_PIN = PA9;
constexpr auto UART1_RX_PIN = PA10;
constexpr auto UART2_TX_PIN = PA2;
constexpr auto UART2_RX_PIN = PA3;
constexpr auto UART3_TX_PIN = PD8;
constexpr auto UART3_RX_PIN = PD9;
constexpr auto UART4_TX_PIN = PA0;
constexpr auto UART4_RX_PIN = PA1;
constexpr auto UART6_TX_PIN = PC6;
constexpr auto UART6_RX_PIN = PC7;
constexpr auto UART7_TX_PIN = PE8; // Not used
constexpr auto UART7_RX_PIN = PE7;
constexpr auto UART8_TX_PIN = PE1;
constexpr auto UART8_RX_PIN = PE0;

constexpr auto SPI2_SCK_PIN = PD3;
constexpr auto SPI2_MISO_PIN = PC2_C;
constexpr auto SPI2_MOSI_PIN = PC3_C;
constexpr auto BMI088A_CS_PIN = PD4;
constexpr auto BMI088G_CS_PIN = PD5;
constexpr auto BMI270_CS_PIN = PA15;

constexpr auto SPI6_SCK_PIN = PA5;
constexpr auto SPI6_MISO_PIN = PA6;
constexpr auto SPI6_MOSI_PIN = PA7;
constexpr auto AT7456E_CS_PIN = PB12;

constexpr auto I2C1_SCL_PIN = PB6;
constexpr auto I2C1_SDA_PIN = PB7;
constexpr auto I2C2_SCL_PIN = PB10;
constexpr auto I2C2_SDA_PIN = PB11;

constexpr auto CAN_TX_PIN = PB8;
constexpr auto CAN_RX_PIN = PB9;

constexpr auto SAFETY_SW_PIN = PB13;
constexpr auto SAFETY_LED_PIN = PB14;
constexpr auto BUZZER_PIN = PB15;

constexpr auto VSENSE_PIN = PC0;
constexpr auto ISENSE_PIN = PC1;

constexpr auto SD_D0_PIN = PC8;
constexpr auto SD_D1_PIN = PC9;
constexpr auto SD_D2_PIN = PC10;
constexpr auto SD_D3_PIN = PC11;
constexpr auto SD_CLK_PIN = PC12;
constexpr auto SD_CMD_PIN = PD2;

constexpr auto LED_BLUE_PIN = PE4;
constexpr auto LED_RED_PIN = PE5;
constexpr auto LED_GREEN_PIN = PE6;
constexpr auto UART6_RX_INV_PIN = PD0;