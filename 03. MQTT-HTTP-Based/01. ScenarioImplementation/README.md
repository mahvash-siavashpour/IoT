# MQTT-HTTP

## Table of Contents
- Project Decsription
- Files Description
- Deployment Instructions
## Project Decsription

## Files Description

### http-server: <br>
    In this folder, an http server is written using flask.

    a. <b>requirements.txt:</b> The implementation requirements are specified in this file. <br>
    b. <b>JWT.txt:</b> tokens obtained during the execution of this section.<br>
    c. <b>Dbmodel.py:</b> This file contains schemas related to DB.
    d. <b>App.py </b> is the main file of the program that starts working with our server on local host:5000.<br>
    e. <b>Offices. DB </b>is the primary database file of the program, in which an admin and a local server are defined by default. If you don't want to use this database, you can create a db with the two commands flask db_create and flask db_seed and do the initial settings of local server and admin.

### Local-server: <br>
    In this folder, the code and requirements related to the local server are available

    a.<b> Local server.py: </b>All the information needed to connect to the http server and hiveMQ Cloud broker is available in hard-code form, and just running this file is enough.<br>
    b. <b>Requiements.txt</b>

## Deployment Instructions
```
Install the mentioned requirements for http-server and run the app.py file in this folder
```
```
Install the mentioned requirements for local_server and run local_server.py and wait for a while to receive the message "local server login successfully"
```