# WIFI
The NodeMCU board used in this project has a wifi chip that can be in two modes: Station and Access Point.
In station mode it is able to connect to other devices and networks and in access point it can create a local network of its own. In these projects we aimed to utilize the wifi ability of this board in different real-word problems.

## Table of Contents
- Station_Mode
- Access_Point_Mode
- Movie_Controller
- Servo_Controller
- Car_Sensor

## Station_Mode
In this simple implementation of the station mode the module is able to scan all the wifi networks nearby and show them on a serial monitor. It can also connect to each of them if the password is provided. 
## Access_Point_Mode
In this project the NodeMCU module works as a network provider and creates a local wifi network with a static IP address.
## Movie_Controller
The goal of this project was to design a network that can connect two devices who communicate with each other. <br>
The scenario is that there is are a couple of videos available on pc. We aim to control playing of these movies from our smart phones. The NodeMCU creates a network that enables the phone to connect and communicate with the board. Then NodeMCU sends the html page to the phone, giving us the ability to control the movies. This is where a python code comes into play. Using this code we are able to read the serial monitor's data (that comes from the commands we give on the smart device). After reading and processing the commands the laptop executes the commands aa the smart device has requested. The id of the movie we request is obtained via a card that is read by the RFID reader.
## Servo_Controller
The cameras in an stadium are supposed to be flexible (being able to move). In this project we created a module that can send an html page, giving ability to anyone requesting the page to be able to move the camera (which is a servo motor itself). The html page is send via a network that is created by our NodeMCU board and then the commands are recieved by the same board and then transmitted to the servo motor.
## Car_Sensor
The reverse gear in many cars requires a distance sensor that can alert the driver when the distance is too close. The live distance between the car and the object can be seen in an html page sent from the NodeMCU module (via wifi). Also there is an LED and a buzzer that alert the driver whenever the distance is too close.