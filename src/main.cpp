#include <Arduino.h>
#include <USBSerial.h>
#include <vector>

#include <Devices/BMI088.hpp>
#include <Devices/UARTPort.hpp>

#include "config.h"

static SPIClass SPI_IMU(SPI2_MOSI_PIN, SPI2_MISO_PIN, SPI2_SCK_PIN);
static SPIClass SPI_AT7456E(SPI6_MOSI_PIN, SPI6_MISO_PIN, SPI6_SCK_PIN);

static BMI088 bmi088("BMI088", SPI_IMU, BMI088A_CS_PIN, BMI088G_CS_PIN);

static const String serialTestData = "Boy Next Door\n"
                                     "I'm fucking coming!\n"
                                     "Do you like what you see?\n";

static UARTPort uart1("UART1", Serial1, UART1_TX_PIN, UART1_RX_PIN, serialTestData, 115200);
static UARTPort uart2("UART2", Serial2, UART2_TX_PIN, UART2_RX_PIN, serialTestData, 115200);
static UARTPort uart3("UART3", Serial3, UART3_TX_PIN, UART3_RX_PIN, serialTestData, 115200);
static UARTPort uart4("UART4", Serial4, UART4_TX_PIN, UART4_RX_PIN, serialTestData, 115200);
static UARTPort uart6("UART6", Serial6, UART6_TX_PIN, UART6_RX_PIN, serialTestData, 115200);
static UARTPort uart7("UART7", Serial7, UART7_TX_PIN, UART7_RX_PIN, serialTestData, 115200);
static UARTPort uart8("UART8", Serial8, UART8_TX_PIN, UART8_RX_PIN, serialTestData, 115200);

static std::vector<Device *> deviceList = {&bmi088, &uart1, &uart2, &uart3, &uart4, &uart6, &uart7, &uart8};

constexpr uint8_t testTimes = 5;

void blink()
{
  static uint8_t n = 1;
  // Active LOW
  digitalWrite(LED_BLUE_PIN, !(n & 0b001));
  digitalWrite(LED_RED_PIN, !(n & 0b010));
  digitalWrite(LED_GREEN_PIN, !(n & 0b100));

  n <<= 1;
  if (n > 0b111)
    n = 1;
}

void setup()
{
  // Initialize SerialUSB for logging
  SerialUSB.begin();
  auto logger = [](ulog_level_t severity, char *msg)
  { SerialUSB.printf("%d [%s]: %s\n", millis(), ulog_level_name(severity), msg); };

  ulog_init();
  ulog_subscribe(logger, ULOG_INFO_LEVEL);

  pinMode(LED_BLUE_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);

  static HardwareTimer blinkTimer(TIM12);
  blinkTimer.setOverflow(5, HERTZ_FORMAT);
  blinkTimer.attachInterrupt(blink);
  blinkTimer.resume();

  SPI_IMU.begin();
  SPI_AT7456E.begin();

  while (!SerialUSB)
    ;
  delay(1000); // Wait for SerialUSB to be ready
  ULOG_INFO("I'm fucking coming!");

  for (auto device : deviceList)
  {
    if (device->begin())
    {
      device->runTest(testTimes);
    }
  }
}

void loop() { /*Never fuck here*/ }