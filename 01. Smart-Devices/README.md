# Smart Devices
The purpose of this exercise is to examine the functions and capabilities of the Internet of Things nodes. In this exercise we use NodeMcu as one of the IoT nodes. A NodeMCU board is an open source hardware platform suitable for IoT projects that require wireless connectivity. <br>
In the following sections there is a brief description about each of the projects implemented.

## Table of Contents
- Hello World
- Line Detection
- Smart Highway Lighting
- Smart Light
- Labolatory Door

## Hello World
This is a simplest project in IoT world that just shows a predefined text on the terminal.

## Line Detection
This project aims to implement a navigator module for a robot that moves on specific lines on a path. To implement this module an LDR sensor was used to detect the light reflected from the surface and based on the amount of reflection the path can be detected.

## Smart Highway Lighting
One of the main sources of energy waste are the street lights. They are on during the night time even when there are no cars or people on the streets. In this project we aimed to implement a smart lighting mechanism that can detect objects and send the signal to the nearest light, meaning only turning on the lights that are needed.

## Smart Light
One other main issue in the electricity management is how unnecessary bright lights may be. That is what is focused on in this project. The smart lighting project aims to detect the amount of light in the envoronment, and then based on that measurement it adjusts the brightness of the LEDs.

## Labolatory Door
Automation of the commuting of the people to the office is the goal of this project. Using an RFID tag reader this module is able to detect the information of every individual equesting to enter the office. By using the NTP protocol the module  is able to detect the time and then decide whether or not grant the access to the door.

