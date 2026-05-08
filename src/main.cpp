#include <Arduino.h>
#include <USBSerial.h>

#include <Devices/BMI088DUT.hpp>
#include <Devices/UARTPort.hpp>

#include "config.h"

static SPIClass SPI_IMU(SPI2_MOSI_PIN, SPI2_MISO_PIN, SPI2_SCK_PIN);
static SPIClass SPI_AT7456E(SPI6_MOSI_PIN, SPI6_MISO_PIN, SPI6_SCK_PIN);

static BMI088DUT bmi088(SPI_IMU, BMI270_CS_PIN, BMI270_CS_PIN);

static UARTPort uart1(Serial1, UART1_TX_PIN, UART1_RX_PIN, 115200);
static UARTPort uart2(Serial2, UART2_TX_PIN, UART2_RX_PIN, 115200);
static UARTPort uart3(Serial3, UART3_TX_PIN, UART3_RX_PIN, 115200);
static UARTPort uart4(Serial4, UART4_TX_PIN, UART4_RX_PIN, 115200);
static UARTPort uart6(Serial6, UART6_TX_PIN, UART6_RX_PIN, 115200);
static UARTPort uart7(Serial7, UART7_TX_PIN, UART7_RX_PIN, 115200);
static UARTPort uart8(Serial8, UART8_TX_PIN, UART8_RX_PIN, 115200);

static const std::pair<String, UARTPort *> uartPorts[] = {
    {"UART1", &uart1},
    {"UART2", &uart2},
    {"UART3", &uart3},
    {"UART4", &uart4},
    {"UART6", &uart6},
    {"UART7", &uart7},
    {"UART8", &uart8},
};

void setup()
{
  // Initialize SerialUSB for logging
  SerialUSB.begin();
  auto logger = [](ulog_level_t severity, char *msg)
  { SerialUSB.printf("%d [%s]: %s\n", millis(), ulog_level_name(severity), msg); };

  ulog_init();
  ulog_subscribe(logger, ULOG_INFO_LEVEL);
}

void loop() { /*Never fuck here*/ }