/****************************************************************************************************************************
  PWM_DynamicDutyCycle.ino

  For ESP32, ESP32_S2, ESP32_S3, ESP32_C3 boards with ESP32 core v2.0.0+
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/ESP32_FastPWM
  Licensed under MIT license

  This is pure hardware-based PWM
*****************************************************************************************************************************/
/******************************************************************************************************************************
  // All GPIO pins (but GPIO34-39) can be used to generate PWM
  // For ESP32, number of channels is 16, max 20-bit resolution
  // For ESP32_S2, ESP32_S3, number of channels is 8
  // For ESP32_C3, number of channels is 6
******************************************************************************************************************************/

#define _PWM_LOGLEVEL_       4

#include "ESP32_FastPWM.h"

#if ARDUINO_ESP32C3_DEV
  #define pinToUse       9
#else
  #define pinToUse       16
#endif

// Max resolution is 20-bit
// Resolution 65536 (16-bit) for lower frequencies, OK @ 1K
// Resolution  4096 (12-bit) for lower frequencies, OK @ 10K
// Resolution  1024 (10-bit) for higher frequencies, OK @ 50K
// Resolution  256  ( 8-bit)for higher frequencies, OK @ 100K, 200K
// Resolution  128  ( 7-bit) for higher frequencies, OK @ 500K
int PWM_resolution       = 12;

//creates pwm instance
ESP32_FAST_PWM* PWM_Instance;

float frequency;
float dutyCycle;

char dashLine[] = "=====================================================================================";

void printPWMInfo(ESP32_FAST_PWM* PWM_Instance)
{
  Serial.println(dashLine);
  Serial.print("Actual data: pin = ");
  Serial.print(PWM_Instance->getPin());
  Serial.print(", PWM DC = ");
  Serial.print(PWM_Instance->getActualDutyCycle());
  Serial.print(", PWMPeriod = ");
  Serial.print(PWM_Instance->getPWMPeriod());
  Serial.print(", PWM Freq (Hz) = ");
  Serial.println(PWM_Instance->getActualFreq(), 4);
  Serial.println(dashLine);
}

void setup()
{
  Serial.begin(115200);

  while (!Serial && millis() < 5000);

  delay(500);

  Serial.print(F("\nStarting PWM_DynamicDutyCycle on "));
  Serial.println(ARDUINO_BOARD);
  Serial.println(ESP32_FAST_PWM_VERSION);

  frequency = 5000.0f;

  PWM_Instance = new ESP32_FAST_PWM(pinToUse, frequency, 50.0f);

  if (PWM_Instance)
  {
    if (!PWM_Instance->setPWM())
    {
      Serial.println(F("Stop here"));

      // stop here
      while (true)
        delay(1000);
    }
  }

  Serial.println(dashLine);
}

void loop()
{
  dutyCycle = 90.0f;

  Serial.print(F("Change PWM DutyCycle to "));
  Serial.println(dutyCycle);
  PWM_Instance->setPWM(pinToUse, frequency, dutyCycle);

  printPWMInfo(PWM_Instance);

  delay(5000);
  dutyCycle = 20.0f;

  Serial.print(F("Change PWM DutyCycle to "));
  Serial.println(dutyCycle);
  PWM_Instance->setPWM(pinToUse, frequency, dutyCycle);
  printPWMInfo(PWM_Instance);

  delay(5000);
}
