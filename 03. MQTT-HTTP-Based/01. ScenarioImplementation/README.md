# MQTT-HTTP

### Files Description

1. http-server: <br>
    In this folder, an http server is written using flask.

        a. requirements.txt: The implementation requirements are specified in this file.
        b. JWT.txt: tokens obtained during the execution of this section.
        c. Dbmodel.py: This file contains schemas related to DB.
        d. App.py is the main file of the program that starts working with our server on local host:5000.
        e. Offices. DB is the primary database file of the program, in which an admin and a local server are defined by default. If you don't want to use this database, you can create a db with the two commands flask db_create and flask db_seed and do the initial settings of local server and admin.

2.  Local-server: <br>
    In this folder, the code and requirements related to the local server are available
    
        a. Local server.py: All the information needed to connect to the http server and hiveMQ Cloud broker is available in hard-code form, and just running this file is enough.
        b. Requiements.txt