<p align="center">
  <p1 style="font-size: 50px"> 🌀 AirQu: Air Quality Monitoring System</p1>
</p>

<p align ="center">
  <a href="#contributors">Contributors</a> •
  <a href="#introduction">Introduction to the problem and the solution</a> •
  <a href="#hardware-design-and-implementation-details">Hardware design and implementation details</a> • 
  <a href="#network-infrastucture">Network infrastructure</a> •
  <a href="#software-implementation-details">Software Implementation Details</a> •
  <a href="#test-results-and-performance-evaluation">Test Results and Performance Evaluation</a> •
  <a href="#conclusion-and-future-work">Conclusion and future work</a>
</p>

## Contributors
This is a Real Time System & Internet of Things practicum final project made by group A7 that consists of :

- [Muhammad Cavan Naufal Azizi](https://github.com/CavanNaufal) - 2106702730
- [Albertus Timothy Gunawan](https://github.com/albertustimothyy) - 2106639472
- [Muhammad Suhaili](https://github.com/aleeein) - 2106731535
- [Raditya Dito](https://github.com/RadityaDito) - 2106733912

as undergraduates of Computer Engineering Major, Department of Electrical Engineering, Faculty of Engineering, Universitas Indonesia.
<!-- This project implements ESP32 with BME680 sensor to monitor indoor air quality using BSEC Library and Blynk platform. Here, BME680 is used to measure temperature, humidity, Indoor Air Quality (IAQ), and CO2 percentage in the room and display them on the Blynk platform. If there are parameters that exceed the threshold where it can be identified that the air in the room is bad, the program will turn on the alarm (buzzer) and a red light bulb then send an email notification to the user indicating that the air quality around the vicinity of the AirQu is poor. -->

## Introduction to the problem and the solution
In this day and age, air pollution poses a significant threat to the public health, enviromental sustainibility, and overall quality of life. The rapid industrialization, urbanization, and increased vehicular traffic have led to the release of various pollutants into the atmosphere. These pollutants, including particulate matter, nitrogen dioxide, sulfur dioxide, and volatile organic compounds, have detrimental effects on respiratory health, cardiovascular systems, and the environment. 

Recognizing these challenges, our air quality monitoring system project emerges as a solution-driven initiative. This project aims to bridge the existing gaps in air quality monitoring infrastructure by developing a comprehensive, cost-effective, and scalable system using sensors and software integration so that it could be used remotely by using the internet  and as simple as a mobile phone to monitor the air quality around the sensor.

## Hardware design and implementation details
In this project, we're making an air quality monitoring device system using ESP32 and BME680 sensor to read the surrounding air information and displays it to user, as well as necessary alert devices when a certain threshold is reaches that indicates the surrounding air is dangerous. The equipments and tools we use are as follows:

- ESP32 Wi-Fi
- BME680 sensor module
- LED lamp
- Buzzer
- Protoboard
- Jumper cables

The reason why we choose BME680 sensor module is because of its integration with temperature, pressure, humidity, and gas detection sensor. BME680 could also detect wide range of gases like Volatile Organic Compounds(VOCs) which helps to measure Indoor Air Quality. 

Volatile organic compounds (VOCs) are substances from organic origin, i.e. they contain carbon and are found in gaseous form in the air. OCs include hydrocarbons, alcohols, aldehydes and organic acids, solvents, liquid fuels, synthetic substances and numerous organic compounds formed in the course of biological processes.

BME680 gas sensor has metal oxide(MOx) surface to measure the VOC gases in the surrounding environment. Generally Metal oxide based sensors have metal oxide sensitive surface, a chip to measure the electrical conductivity and a heater to heat the surface of the sensitive metal oxide.

When the sensor is turned on the heater heats the metal oxide surface and oxygen is absorbed on the surface. In the clean air the electrons in the Metal Oxide are attracted to Oxygen molecules in the air and forms a potential barrier which resists the flow of electric current.

So when VOC gases comes in contact with the metal oxide sensitive surface the gases react with the oxygen molecules on the surface thus the electrons are free from oxygen molecules which reduces the potential barrier and increases the flow of electric current.

## Network infrastructure
For this project, we use blynk platform for its IoT integration and portability. It uses Wi-Fi connection to establish link with ESP32 and its functionality with the sensor. For better understanding, consider looking at the schematic below.

<img src="https://github.com/CavanNaufal/IoT_Project_A7/assets/88538229/8006b424-9318-4905-a2bc-b4911c6c46ff" width=100% height=50%>

## Software Implementation Details
We use one .ino file to include all the programming necessary for this project. On this section, we'll try to explain the important parts in the program.

```
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
``` 
Here are the libraries used for this program. Adafruit is used by BME680 sensor to do its job, LiquidCrystal is used by the LCD, and Blynk as well as WiFi are used for Blynk platform integration. Additionally, pitches is used for the buzzer to produce sound melodies.

```
void beep(int note, int duration) {
  tone(buzzerPin, note, duration);
  delay(duration);
}

void soundBeep(){
  beep(note, duration);
}
```
These two functions are used to produce sound melodies for the buzzer when it's triggered to turn on.

```
bme.setTemperatureOversampling(BME680_OS_2X);
bme.setHumidityOversampling(BME680_OS_2X);
bme.setPressureOversampling(BME680_OS_2X);
bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
bme.setGasHeater(320, 150);
```
This part of the code initialize some parameters, including oversampling, gas heater, and filters. It uses setTemperatureOversampling(), setHumidityOversampling(), and setPressureOversampling() to initialize the oversampling for temperature, humidity, and pressure, respectively. This is being done so that a better resolution of the data can be obtained. We use BME680_OS_2X for double oversampling to get better results.

As for setIIRFilterSize(), it takes in a single parameter which is the size of the filter. This internal infinite impulse response (IIR) filter is used to reduce the bandwidth of the sensor readings so that unnecessary changes caused by external disturbances could be significantly diminished. This is mainly used on temperature and pressure readings which fluctuate a lot and need low pass filtering. BME680_FILTER_SIZE_3 is the default value, hence why we use it.

The setGasHeater() configures a heater for gas measurement. It takes two parameters to initialize it. The first parameter is the target temperature for the heater which is set up as 32 degrees Celsius and the second parameter is the time duration set as 150ms.

```
String CalculateIAQ(float score)
```
This function returns a string message to notify the user the current Indoor Air Quality condition. This is based on the BME680 sensor module datasheet as shown below.


<img src="https://github.com/CavanNaufal/IoT_Project_A7/assets/88538229/6828d5aa-9948-4f37-88a0-0fa9741eb1a1" width=50% height=100%>


## Test Results and Performance Evaluation
In the testing phase of our IoT project, we conducted several tests to ensure that the program and circuit operate according to the established criteria and expectations. In the Blynk 2.0 application, the ESP32 attempts to connect to the WiFi network using the provided SSID and password. Subsequently, the LCD screen will light up and display the values of Temperature, Humidity, Gas, Pressure, and IAQ (Indoor Air Quality). 

The LED and buzzer will activate under certain conditions. Finally, on the Blynk mobile app, the values of Temperature, Humidity, Gas, and Pressure will be displayed, along with a sentence indicating the current air quality condition based on the IAQ value obtained.

The results from the three tests conducted on the original circuit using the Blynk platform all aligned with the established expectations and predefined criteria. Throughout these tests, the entire system's functions and responses were directly evaluated to ensure the circuit operated optimally and seamlessly integrated with Blynk in real-time. eyond this, the system also includes LED indicators that illuminate according to the IAQ conditions and a buzzer designed to sound an alert when the indoor air quality deteriorates

<img src="https://github.com/CavanNaufal/IoT_Project_A7/assets/88538229/468001ea-ffb8-4f88-bbeb-255a8e2ebd1d" width=50% height=50%>

<img src="https://github.com/CavanNaufal/IoT_Project_A7/assets/88538229/d6528b07-00d4-4ac9-a5c3-441233f148bb" width=50% height=50%>


## Conclusion and future work
The development of the air quality monitoring system represents a significant step towards addressing the pressing challenges posed by air pollution. The integration of the BME680 sensor, ESP32 microcontroller, and Blynk platform has resulted in a comprehensive solution that not only monitors key environmental parameters but also ensures real-time accessibility for users through a user-friendly interface.

For our future work, we would like to come up with a way so that the ESP32 would have its own power resource to function. In this way, ESP32 doesn't have to connect to our PC's or laptops to do its job and could be brought up anywhere with the user if needed.
