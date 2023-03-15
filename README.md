# EcoOctano 2022-Telemetria E Instrumentação
 The goal of the project was to develop a telemetry system for the science outreach team Eco Octano from Universidade Federal do Paraná (UFPR). The developed project enables all members to access the data acquired by the sensors, allowing for a more accurate measurement of the car's efficiency. Also, it provides a cleaner interface for the driver and more reliable instrumentation.

## About the project
 We built our project based on IoT: we wanted to connect our microcontrollers and integrate their data in a platform that could be accessed anywhere by any team member on any device. That platform will join and analyse all the data to share the information with the application that we built.
 For visualization of the data, a website was developed, in which the entire team could have access to the data in real-time, or close to real-time, with graphics dedicated to each type of data. The protocols and methods used for telemetry allow scalability and easy adaptation for future projects based on IoT since it uses the MQTT protocol. The project can acquire vehicle data, send and display it in real-time.
 
### Web Interface
<p align="center">
<img width="801" alt="Captura de Tela 2023-03-15 às 19 46 45" src="https://user-images.githubusercontent.com/127882225/225461525-9ce0207f-1617-4195-b28d-7a6fab38e3ec.png">
 </p>
 
 Showing, in order, the values of speed and traveled distance. In the middle of the page, we have the graph of speed and power. At the bottom of the page, the button “Salvar dados” allows the saving of all the shown data and organizing it in a spreadsheet. The purpose of having a button to save data after each race is to make it easier to feed a database with the vehicle's performance on each attempt. When dealing with efficiency, this data can help to determine the actions that guarantee the best result on the track. In addition, we can compare it with the files saved on the SD card to verify the reliability of the data displayed on the website.
 The driver has access to the speed information of the vehicle. The vehicle has - in a suitable place inside the prototype - a display attached that shows speed, for quick and easy reading. The information comes directly from the PCB without the need to use the internet. Therefore, the sampled data for the driver is not subject to a possible signal drop.


#### Data flux

 We use the Microsoft cloud platform, Azure, for storage, handling, and displaying the data acquired by the sensors on the PCB. A large amount of documentation, tutorials, and available services in the platform provided by the company made it possible to plot graphs in many styles on a website (local or public) for the performance analysis of each race. The system's main objective is to show the car's speed to the team for easy reading and interpretation. Also, the driver has a non-internet-dependent system that also shows the speed in real- time in order to help them optimize their strategy to guarantee greater vehicle efficiency.
 <p align="center">
<img width="500" alt="Captura de Tela 2023-03-15 às 19 47 33" src="https://user-images.githubusercontent.com/127882225/225463968-53e3f1a0-135c-4d42-bd52-e1fb40454a48.png">
 </p>

### PCB design 
<p align="center">
<img width="364" alt="Captura de Tela 2023-03-15 às 19 47 03" src="https://user-images.githubusercontent.com/127882225/225462535-18c9dcfe-f75a-455f-9033-1288256f244d.png">
 </p>
 
#### TEENSY 4.0:
Teensy is a developing board that contains one microcontroller, whose programming is compatible with the Arduino platforms. The chosen type for the implementation in the Telemetry of the electric car was Teensy 4.0. Some of the reasons that explain this choice are the high speed of the data process, the big number of pinouts, and the reduced size.

#### ESP32:
ESP32 is a microcontroller that is capable of communicating using WiFi, which was a fundamental characteristic for its implementation in the project. The ESP32 will receive the data by serial communication with Teensy 4.0, and send the information to the Azure IoT Hub.
 
 
