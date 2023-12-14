#define BLYNK_PRINT Serial

// Blynk template information and authentication token
#define BLYNK_TEMPLATE_ID "TMPL611kemsnQ"
#define BLYNK_TEMPLATE_NAME "AirQu"
#define BLYNK_AUTH_TOKEN "5jNNupitKZaE88ZcehdYVUQWF_PNHqST"

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LiquidCrystal_I2C.h>
#include <Blynk.h>
#include <BlynkSimpleEsp32.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "pitches.h"

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme;  // I2C

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27, 16 columns and 2 rows

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "SAMSUNGS21"; // Your WiFi credentials.
char pass[] = "12345678";

#if CONFIG_FREERTOS_UNICORE 
static const BaseType_t app_cpu = 0; 
#else 
static const BaseType_t app_cpu = 1;  
#endif 

const int buzzerPin = 5;  // Buzzer pin for ESP32
const int ledPin = 2;  // Replace with your LED pin
float gas_reference = 250000;   // Typical sensor resistance at good air quality
float hum_score, gas_score;
float hum_reference = 40;  // Set your reference humidity here
int   getgasreference_count = 0;

// void beep(int note, int duration) {
//   tone(buzzerPin, note, duration);
//   delay(duration);
// }

// void soundBeep() {
//   beep(NOTE_A4, 500);
//   beep(NOTE_A4, 500);    
//   beep(NOTE_A4, 500);
//   beep(NOTE_F4, 350);
//   beep(NOTE_C5, 150);  
//   beep(NOTE_A4, 500);
//   beep(NOTE_F4, 350);
//   beep(NOTE_C5, 150);
//   beep(NOTE_A4, 650);
 
//   delay(500);
 
//   beep(NOTE_E5, 500);
//   beep(NOTE_E5, 500);
//   beep(NOTE_E5, 500);  
//   beep(NOTE_F5, 350);
//   beep(NOTE_C5, 150);
//   beep(NOTE_GS4, 500);
//   beep(NOTE_F4, 350);
//   beep(NOTE_C5, 150);
//   beep(NOTE_A4, 650);
 
//   delay(500);
 
//   beep(NOTE_A5, 500);
//   beep(NOTE_A4, 300);
//   beep(NOTE_A4, 150);
//   beep(NOTE_A5, 500);
//   beep(NOTE_GS5, 325);
//   beep(NOTE_G5, 175);
//   beep(NOTE_FS5, 125);
//   beep(NOTE_F5, 125);    
//   beep(NOTE_FS5, 250);
 
//   delay(325);
 
//   beep(NOTE_AS4, 250);
//   beep(NOTE_DS5, 500);
//   beep(NOTE_D5, 325);  
//   beep(NOTE_CS5, 175);  
//   beep(NOTE_C5, 125);  
//   beep(NOTE_AS4, 125);  
//   beep(NOTE_C5, 250);  
 
//   delay(350);
// }

void setup() {
  Serial.begin(115200);
  Serial.println(F("BME680 test"));

  Blynk.begin(auth, ssid, pass);

  Wire.begin();
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }

  pinMode(LED_BUILTIN, OUTPUT);  // Initialize the player ON indicator LED

  lcd.init();       //initialize the lcd
  lcd.backlight();  //open the backlight

  pinMode(buzzerPin, OUTPUT);    // Initialize the buzzer pin
  pinMode(ledPin, OUTPUT);  // Initialize the LED pin

  bme.setTemperatureOversampling(BME680_OS_2X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_2X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150);

  xTaskCreatePinnedToCore( 
    vBmeTask,     // Function to be called 
    "BMESensorTask",   // Name of the task 
    10000,              // Stack size (bytes in ESP32, words in FreeRTOS) 
    NULL,               // Parameter to pass 
    1,                  // Task priority 
    NULL,               // Task handle 
    app_cpu             // Run on one core for demo purposes (ESP32 only) 
  );

  // Run the sensor for a burn-in period
  GetGasReference();
}

void vBmeTask(void *parameter){
  while(1){
    unsigned long endTime = bme.beginReading();
    if (endTime == 0) {
      Serial.println(F("Failed to begin reading :("));
      return;
    }

    delay(50);  // This represents parallel work.
    
    if (!bme.endReading()) {
      Serial.println(F("Failed to complete reading :("));
      return;
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    Serial.print(F("\n\n\nTemperature = "));
    Serial.print(bme.temperature);
    Serial.println(F(" *C"));
    lcd.print("Temp: ");
    lcd.print(bme.temperature);
    lcd.print(" *C");
    Blynk.virtualWrite(V0, bme.temperature);

    lcd.setCursor(0, 1);
    Serial.print(F("Pressure = "));
    Serial.print(bme.pressure / 100.0);
    Serial.println(F(" hPa"));
    lcd.print("Pres: ");
    lcd.print(bme.pressure / 100.0);
    lcd.print(" hPa");
    Blynk.virtualWrite(V3, bme.pressure / 100.0);
    delay(5000);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    Serial.print(F("Humidity = "));
    Serial.print(bme.humidity);
    Serial.println(F(" %"));
    lcd.print("Humidity: ");
    lcd.print(bme.humidity);
    lcd.print(" %");
    Blynk.virtualWrite(V1, bme.humidity);

    lcd.setCursor(0, 1);
    Serial.print(F("Gas = "));
    Serial.print(bme.gas_resistance / 1000.0);
    Serial.println(F(" KOhms"));
    lcd.print("Gas: ");
    lcd.print(bme.gas_resistance / 1000.0);
    lcd.print(" KOhms");
    Blynk.virtualWrite(V4, bme.gas_resistance / 1000.0);
    delay(5000);
    

    float hum_score, gas_score;
    float hum_reference = 40;  // Set your reference humidity here
    int   getgasreference_count = 0;
    float air_quality_score;

    // Calculate humidity contribution to IAQ index
    float current_humidity = bme.humidity;
    if (current_humidity >= 38 && current_humidity <= 42)
      hum_score = 0.25 * 100;  // Humidity +/-5% around optimum
    else {
      // sub-optimal
      if (current_humidity < 38)
        hum_score = 0.25 / hum_reference * current_humidity * 100;
      else
        hum_score = ((-0.25 / (100 - hum_reference) * current_humidity) + 0.416666) * 100;
    }

    // Calculate gas contribution to IAQ index
    float gas_lower_limit = 5000;   // Bad air quality limit
    float gas_upper_limit = 50000;  // Good air quality limit
    if (bme.gas_resistance < gas_lower_limit) gas_reference = gas_lower_limit;
    if (bme.gas_resistance > gas_upper_limit) gas_reference = gas_upper_limit;
    gas_score = (0.75 / (gas_upper_limit - gas_lower_limit) * gas_reference -
                 (gas_lower_limit * (0.75 / (gas_upper_limit - gas_lower_limit)))) *
                100;

    // Combine results for the final IAQ index value (0-100% where 100% is good quality air)
    air_quality_score = hum_score + gas_score;

    lcd.clear();
    lcd.setCursor(0, 0);
    Serial.print(F("IAQ Score = "));
    Serial.println(String(air_quality_score, 1));
    lcd.print("IAQ Score : ");
    lcd.print(air_quality_score);

    if (bme.readGas() < 120000) {
      Serial.println("***** Poor Air Quality *****");
      digitalWrite(ledPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
      // soundBeep();
    } else {
      digitalWrite(ledPin, LOW);
      digitalWrite(buzzerPin, LOW);
    }

    delay(2000);

    if((getgasreference_count++)%10==0) GetGasReference();  // Get new gas reference value after 10 minutes

    String iaqText = CalculateIAQ(air_quality_score);
    Serial.print(F("IAQ Text = "));
    Serial.println(iaqText);
    Blynk.virtualWrite(V2, iaqText);

    Serial.print(F("Approx. Altitude = "));
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(F(" m"));
  }
}

void GetGasReference(){
  // Now run the sensor for a burn-in period, then use combination of relative humidity and gas resistance to estimate indoor air quality as a percentage.
  Serial.println("Getting a new gas reference value");
  int readings = 10;
  for (int i = 1; i <= readings; i++){ // read gas for 10 x 0.150mS = 1.5secs
    gas_reference += bme.gas_resistance;
  }
  gas_reference = gas_reference / readings;
} 

void loop() {
  Blynk.run();
}

String CalculateIAQ(float score) {
  String IAQ_text = "Air Quality is ";
  score = (100 - score) * 5;
  if (score >= 301)
    IAQ_text += "Very Bad";
  else if (score >= 201 && score <= 300)
    IAQ_text += "Worse";
  else if (score >= 151 && score <= 200)
    IAQ_text += "Bad";
  else if (score >= 101 && score <= 150)
    IAQ_text += "Little Bad";
  else if (score >= 51 && score <= 100)
    IAQ_text += "Average";
  else if (score >= 0 && score <= 50)
    IAQ_text += "Good";
  return IAQ_text;
}