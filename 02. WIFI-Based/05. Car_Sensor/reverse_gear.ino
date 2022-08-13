#include <ESP8266WiFi.h>
 
// change these values to match your network
const char* ssid     = "***";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "*****"; 
 
WiFiServer server(80);

 
String tmpString = "";
unsigned int count = 0; 

const int trigPin = 12;
const int echoPin = 14;

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034

#define LED D1
#define BUZZER D0
long duration;
float distance;


String html_page = R"=====(
<!DOCTYPE html>
<html>
 <head>
 <meta name='viewport' content='width=device-width, initial-scale=1.0'/>
 <meta charset='utf-8'>
 <meta http-equiv='refresh' content='0.5'>
 <style>
   body {font-size:100%;} 
   #main {display: table; margin: auto;  padding: 0 10px 0 10px; } 
   h2 {text-align:center; } 
   p { text-align:center; }
   datalist {
  display: flex;
  justify-content: space-between;
  color: red;
  width: 50%;
  }
  
  input {
  width: 50%;
  }
 </style>
   <title>Auto Update Example Using HTML</title>
 </head>
 
 <body>
    <h1>
      Reverse Gear Distance Measuring
    </h1>
    <div>
            <input type="range" list="tickmarks" id="rangeInput" min="0" max="100" value = %dis% >
    </div>
    <div>
        <datalist id="tickmarks">
            <option value="0" label="0" >0</option>
            <option value="10">10</option>
            <option value="20">20</option>
            <option value="30">30</option>
            <option value="40">40</option>
            <option value="50" label="50">50</option>
            <option value="60">60</option>
            <option value="70">70</option>
            <option value="80">80</option>
            <option value="90">90</option>
            <option value="100" label="100">100</option>
        </datalist>
    </div>
     <div > 
       <p id='count'>Distance = %dis%</p>
     </div>
   </div> 
 </body>
</html>
)====="; 
 

void setup() 
{
    Serial.begin(115200);
    Serial.println();
    Serial.println("Serial started at 115200");
    Serial.println();
 
    // Connect to a WiFi network
    Serial.print(F("Connecting to "));  Serial.println(ssid);
    WiFi.begin(ssid, password);
 
    while (WiFi.status() != WL_CONNECTED) 
    {
        Serial.print(".");
        delay(500);
    }
 
    Serial.println("");
    Serial.println(F("CONNECTED"));
    Serial.print("[IP ");              
    Serial.print(WiFi.localIP()); 
    Serial.println("]");
 
    // start a server
    server.begin();
    Serial.println("Server started");


    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    pinMode(LED, OUTPUT); // set the digital pin as output.
    pinMode(BUZZER, OUTPUT);
} 




void loop() 
{


    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client)  {  return;  }
  
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    
    // Calculate the distance
    distance = duration * SOUND_VELOCITY/2;
    
       
    // Prints the distance on the Serial Monitor
    Serial.print("Distance (cm): ");
    Serial.println(distance);
    if(distance<=10){
       digitalWrite(LED, HIGH);
       digitalWrite (BUZZER, HIGH);
    }
    else{
       digitalWrite(LED, LOW); 
       digitalWrite (BUZZER, LOW);
    }
 
    tmpString = html_page;
    tmpString.replace("%dis%", String(distance) );
 
    client.flush();
    client.print( tmpString );  
 
 
 
    delay(5);
 
 
} 
