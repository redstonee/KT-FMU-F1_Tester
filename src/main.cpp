#include <Arduino.h>
#include <USBSerial.h>
#include <vector>

#include <Devices/BMI088Dev.hpp>
#include <Devices/BMI270Dev.hpp>
#include <Devices/IST8310Dev.hpp>
#include <Devices/DPS310Dev.hpp>
#include <Devices/UARTPort.hpp>
#include <Devices/ADCPort.hpp>

#include "config.h"

static SPIClass SPI_IMU(SPI2_MOSI_PIN, SPI2_MISO_PIN, SPI2_SCK_PIN);
static SPIClass SPI_AT7456E(SPI6_MOSI_PIN, SPI6_MISO_PIN, SPI6_SCK_PIN);

static TwoWire I2CExternal(I2C1_SDA_PIN, I2C1_SCL_PIN);
static TwoWire I2CInternal(I2C2_SDA_PIN, I2C2_SCL_PIN);

static bool buttonPressed = false;

void handleTimer()
{
  static uint8_t n = 0;
  // Active LOW
  digitalWrite(LED_BLUE_PIN, !(n == 0));
  digitalWrite(LED_RED_PIN, !(n == 1));
  digitalWrite(LED_GREEN_PIN, !(n == 2));

  digitalWrite(SAFETY_LED_PIN, n % 2);

  if (buttonPressed)
  {
    buttonPressed = false;
    digitalWrite(BUZZER_PIN, HIGH);
  }
  else
  {
    digitalWrite(BUZZER_PIN, LOW);
  }

  n++;
  n %= 3;
}

void setup()
{
  // Initialize SerialUSB for logging
  SerialUSB.begin();
  auto logger = [](ulog_level_t severity, char *msg)
  {
    switch (severity)
    {
    case ULOG_WARNING_LEVEL:
      SerialUSB.print("\033[33m");
      break;
    case ULOG_ERROR_LEVEL:
    case ULOG_CRITICAL_LEVEL:
      SerialUSB.print("\033[31m");
      break;
    case ULOG_INFO_LEVEL:
      SerialUSB.print("\033[32m");
    default:
      break;
    }
    SerialUSB.printf("%d [%s]: %s\033[0m\r\n", millis(), ulog_level_name(severity), msg);
  };

  ulog_init();
  ulog_subscribe(logger, ULOG_INFO_LEVEL);

  pinMode(LED_BLUE_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(SAFETY_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(UART6_RX_INV_PIN, OUTPUT);
  digitalWrite(UART6_RX_INV_PIN, LOW); // Ensure normal operation of UART6;

  attachInterrupt(SAFETY_SW_PIN, []()
                  { buttonPressed = true; }, FALLING);

  static HardwareTimer blinkTimer(TIM12);
  blinkTimer.setOverflow(5, HERTZ_FORMAT);
  blinkTimer.attachInterrupt(handleTimer);
  blinkTimer.resume();

  SPI_IMU.begin();
  SPI_AT7456E.begin();
  I2CInternal.begin();
  I2CExternal.begin();

  for (auto pwmPort : PWMPorts)
  {
    auto timer = new HardwareTimer(pwmPort.timer);
    timer->setPWM(pwmPort.channel, pwmPort.pin, 10, 50);
  }
}

static const String serialTestData = "Boy Next Door\n"
                                     "I'm fucking coming!\n"
                                     "Do you like what you see?\n";

static UARTPort uart1("UART1", Serial1, UART1_TX_PIN, UART1_RX_PIN, serialTestData, 115200);
static UARTPort uart2("UART2", Serial2, UART2_TX_PIN, UART2_RX_PIN, serialTestData, 115200);
static UARTPort uart3("UART3", Serial3, UART3_TX_PIN, UART3_RX_PIN, serialTestData, 115200);
static UARTPort uart4("UART4", Serial4, UART4_TX_PIN, UART4_RX_PIN, serialTestData, 115200);
static UARTPort uart6("UART6", Serial6, UART6_TX_PIN, UART6_RX_PIN, serialTestData, 115200);
// UARTPort uart7("UART7", Serial7, UART7_TX_PIN, UART7_RX_PIN, serialTestData, 115200); // RX only, skip testing
static UARTPort uart8("UART8", Serial8, UART8_TX_PIN, UART8_RX_PIN, serialTestData, 115200);

static BMI088Dev bmi088("BMI088", SPI_IMU, BMI088A_CS_PIN, BMI088G_CS_PIN);
static BMI270Dev bmi270("BMI270", SPI_IMU, BMI270_CS_PIN);

static IST8310Dev ist8310("IST8310", I2CInternal);
static DPS310Dev dps310("DPS310", I2CInternal);

static ADCPort vSensePort("Voltage Sensing", VSENSE_PIN, VSENS_DIVIDER_RATIO, 3.3, 10);
static ADCPort iSensePort("Current Sensing", ISENSE_PIN, 1, 3.3, 10);

constexpr uint8_t testTimes = 5;

void loop()
{
  static std::vector<Device *> deviceList = {
      &uart1,
      &uart2,
      &uart3,
      &uart4,
      &uart6,
      &uart8,
      &bmi088,
      &bmi270,
      &ist8310,
      &dps310,
      &vSensePort,
      &iSensePort,
  };

  while (SerialUSB.available())
  {
    SerialUSB.read();
  }

  while (!SerialUSB.available())
  {
    SerialUSB.println("Press any key to start testing...");
    delay(1000);
  }

  ULOG_INFO("I'm fucking coming!");
  for (auto device : deviceList)
  {
    if (device->begin())
    {
      device->runTest(testTimes);
    }
  }
}