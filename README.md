<p align="center">
  <p1 style="font-size: 30px"> 🌀 AirQu: Air Quality Monitoring System</p1>
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

## Network infrastructure
For this project, we use blynk platform for its IoT integration and portability. It uses Wi-Fi connection to establish link with ESP32 and its functionality with the sensor. 

![schematic](https://github.com/CavanNaufal/IoT_Project_A7/assets/88538229/8006b424-9318-4905-a2bc-b4911c6c46ff)


## Software Implementation Details


## Test Results and Performance Evaluation


## Conclusion and future work
