# MQTT-HTTP

## Table of Contents
- Project Decsription
- Files Description
- Deployment Instructions
## Project Decsription
This is a full implementation of an employee authentication and office module that automates the access granting and entrance logging in offices in a building. There is a local and a cantral server implemented using MQTT and HTTP protocols. There are specific endpoints for each of the commands that are implemented using Flask and also a DataBase to storing all the logs and information of the employees and offices.
## Files Description

### http-server: <br>
> In this folder, an http server is written using flask.<br>

#### a. requirements.txt:
 The implementation requirements are specified in this file.
#### b. JWT.txt:
 tokens obtained during the execution of this section.
#### c. Dbmodel.py:
 This file contains schemas related to DB.
#### d. App.py 
 is the main file of the program that starts working with our server on local host:5000.
#### e. Offices. DB 
is the primary database file of the program, in which an admin and a local server are defined by default. If you don't want to use this database, you can create a db with the two commands flask db_create and flask db_seed and do the initial settings of local server and admin.
### Local-server: <br>
> In this folder, the code and requirements related to the local server are available

#### a. Local server.py: 
All the information needed to connect to the http server and hiveMQ Cloud broker is available in hard-code form, and just running this file is enough.
#### b. Requiements.txt

## Deployment Instructions
### Server Side
```
Install the mentioned requirements for http-server and run the app.py file in this folder
```
```
Install the mentioned requirements for local_server and run local_server.py and wait for a while to receive the message "local server login successfully"
```
### Client Side
Follow the instruntions is [here](https://console.hivemq.cloud/clients/arduino-esp8266?uuid=2124b8ecf14e4692b62288592e5c4a12) to install the libraries. We need to install these libraries:
- littlefs
- ntpclient
- pubsubclient<br>

We need to upload the certificate in the NodeMCU board. You can eithrt use the certificate I provided or use your own. <br>
In order to upload the certificate go to ``` Tools>ESP8266 LittleFS Data Upload ```
<br>
Finally you hve to program the code on the board and wait for the MQTT and HTTP connections to be established.