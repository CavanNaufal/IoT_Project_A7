#define BLYNK_PRINT Serial

// Blynk template information and authentication token
#define BLYNK_TEMPLATE_ID "TMPL611kemsnQ"
#define BLYNK_TEMPLATE_NAME "AirQu"
#define BLYNK_AUTH_TOKEN "5jNNupitKZaE88ZcehdYVUQWF_PNHqST"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Blynk.h>
#include <BlynkSimpleEsp32.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "bsec.h"

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = ""; // Your WiFi credentials.
char pass[] = "";

Bsec iaqSensor;
String output;

// Set the I2C address LCD module
#define I2C_ADDR 0x27

// Define LCD column and row size
#define LCD_COLS 20
#define LCD_ROWS 4

// Define your custom I2C pins
#define SDA_PIN 21  // Replace with your chosen SDA pin
#define SCL_PIN 22  // Replace with your chosen SCL pin

// Create LCD instance
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLS, LCD_ROWS);

void checkIaqSensorStatus(void);
void errLeds(void);

void setup(void)
{
  Serial.begin(115200);

  // Initialize LCD with custom I2C pins
  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.init();
  lcd.backlight(); // Enable the backlight

  Blynk.begin(auth, ssid, pass);

  iaqSensor.begin(BME680_I2C_ADDR_PRIMARY, Wire);
  output = "\nBSEC library version " + String(iaqSensor.version.major) + "." + String(iaqSensor.version.minor) + "." + String(iaqSensor.version.major_bugfix) + "." + String(iaqSensor.version.minor_bugfix);
  Serial.println(output);
  checkIaqSensorStatus();

  bsec_virtual_sensor_t sensorList[10] = 
  {
    BSEC_OUTPUT_RAW_TEMPERATURE,
    BSEC_OUTPUT_RAW_PRESSURE,
    BSEC_OUTPUT_RAW_HUMIDITY,
    BSEC_OUTPUT_RAW_GAS,
    BSEC_OUTPUT_IAQ,
    BSEC_OUTPUT_STATIC_IAQ,
    BSEC_OUTPUT_CO2_EQUIVALENT,
    BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY
  };

  iaqSensor.updateSubscription(sensorList, 10, BSEC_SAMPLE_RATE_LP);
  checkIaqSensorStatus();

  // Print the header on LCD
  lcd.setCursor(0, 0);
  lcd.print("Timestamp  Temp   Hum");

  lcd.setCursor(0, 1);
  lcd.print("IAQ   VOC");

  lcd.setCursor(0, 2);
  lcd.print("CO2   Pressure");

  lcd.setCursor(0, 3);
  lcd.print("Blynk Status");
}

void loop(void)
{
  unsigned long time_trigger = millis();
  if (iaqSensor.run())
  {
    output = String(time_trigger);
    output += ", " + String(iaqSensor.rawTemperature);
    output += ", " + String(iaqSensor.pressure);
    output += ", " + String(iaqSensor.rawHumidity);
    output += ", " + String(iaqSensor.gasResistance);
    output += ", " + String(iaqSensor.iaq);
    output += ", " + String(iaqSensor.iaqAccuracy);
    output += ", " + String(iaqSensor.temperature);
    output += ", " + String(iaqSensor.humidity);
    output += ", " + String(iaqSensor.staticIaq);
    output += ", " + String(iaqSensor.co2Equivalent);
    output += ", " + String(iaqSensor.breathVocEquivalent);
    Serial.println(output);

    Serial.print("Pressure: ");
    Serial.print((iaqSensor.pressure) / 1000);
    Serial.println(" hPa");

    Serial.print("Temperature: ");
    Serial.print(iaqSensor.temperature);
    Serial.println(" *C");

    Serial.print("Humidity: ");
    Serial.print(iaqSensor.humidity);
    Serial.println(" %");

    Serial.print("IAQ: ");
    Serial.print(iaqSensor.iaq);
    Serial.println(" PPM");

    Serial.print("CO2 Equivalent: ");
    Serial.print(iaqSensor.co2Equivalent);
    Serial.println(" PPM");

    Serial.print("Breath VOC Equivalent: ");
    Serial.print(iaqSensor.breathVocEquivalent);
    Serial.println(" PPM");
    Serial.println();

    // Display information on the LCD
    lcd.setCursor(7, 0);
    lcd.print(iaqSensor.temperature);
    lcd.print("C ");
    lcd.print(iaqSensor.humidity);
    lcd.print("%");

    lcd.setCursor(4, 1);
    lcd.print(iaqSensor.iaq);
    lcd.setCursor(11, 1);
    lcd.print(iaqSensor.breathVocEquivalent);

    lcd.setCursor(4, 2);
    lcd.print(iaqSensor.co2Equivalent);
    lcd.setCursor(12, 2);
    lcd.print((iaqSensor.pressure) / 1000);

    lcd.setCursor(13, 3);
    lcd.print(Blynk.connected() ? "Online" : "Offline");

    Blynk.run();
    Blynk.virtualWrite(V1, (iaqSensor.pressure) / 1000);
    Blynk.virtualWrite(V2, iaqSensor.temperature);
    Blynk.virtualWrite(V3, iaqSensor.humidity);
    Blynk.virtualWrite(V4, iaqSensor.iaq);
    Blynk.virtualWrite(V5, iaqSensor.co2Equivalent);
    Blynk.virtualWrite(V6, iaqSensor.breathVocEquivalent);
  }
  else
  {
    checkIaqSensorStatus();
  }
}

void checkIaqSensorStatus(void)
{
  if (iaqSensor.status != BSEC_OK)
  {
    if (iaqSensor.status < BSEC_OK)
    {
      output = "BSEC error code : " + String(iaqSensor.status);
      Serial.println(output);
      for (;;)
        errLeds(); /* Halt in case of failure */
    }
    else
    {
      output = "BSEC warning code : " + String(iaqSensor.status);
      Serial.println(output);
    }
  }

  if (iaqSensor.bme680Status != BME680_OK)
  {
    if (iaqSensor.bme680Status < BME680_OK)
    {
      output = "BME680 error code : " + String(iaqSensor.bme680Status);
      Serial.println(output);
      for (;;)
        errLeds(); /* Halt in case of failure */
    }
    else
    {
      output = "BME680 warning code : " + String(iaqSensor.bme680Status);
      Serial.println(output);
    }
  }
}

void errLeds(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}
