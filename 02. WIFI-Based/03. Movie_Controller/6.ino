

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPI.h>
#include <MFRC522.h>
#include <WiFiUdp.h>


#define SS_PIN 4  
#define RST_PIN 5 


MFRC522 rfid(SS_PIN, RST_PIN);
String tag;
String the_ring = "1511714296"; //card
String friends = "233195180193"; //tag


// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "********";
const char* password = "*********";
const char* PARAM_INPUT = "value";
const int output = 2;
String movie_name = "nameeeeeeeeee";
String tmpString = "";

// HTML web page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
  <head>
    
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title> Movie controller</title>
    <style>
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 30px;}
      .button {
        padding: 10px 20px;
        font-size: 24px;
        text-align: center;
        outline: none;
        color: #fff;
        background-color: #2f4468;
        border: none;
        border-radius: 5px;
        box-shadow: 0 6px #999;
        cursor: pointer;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        -webkit-tap-highlight-color: rgba(0,0,0,0);
      }  
      .button:hover {background-color: #1f2e45}
      .button:active {
        background-color: #1f2e45;
        box-shadow: 0 4px #666;
        transform: translateY(2px);
      }
    </style>
  </head>
  <body>
    <h1> Video Controller Web Server</h1>
    <button class="button" onclick="name_handler('name')", style = "background-color: gray">Name</button>
    <br>
    <br>
    <button class="button" onclick="button_handler('Play')", style = "background-color: green">Play</button>
    <button class="button" onclick="button_handler('Stop')", style = "background-color: Red">Stop</button>
    <br>
    <br>
    <button class="button" onclick="button_handler('Up')", style = "background-color: rgb(12, 138, 143)">+</button>
    <button class="button" onclick="button_handler('Down')", style = "background-color: rgb(62, 7, 190)">-</button>
   <script>
   function button_handler(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/button?value=" + x, true);
     xhr.send();
   }
   function name_handler(x){
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/name?value=" + x, true);
     xhr.send();
     var newWin = open('/name','windowName','height=300,width=300');
     newWin.document.write("Movie Name: "+ xhr.response);
     newWin.document.write(xhr.response);
   }
  </script>
  </body>
</html>)rawliteral";


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

AsyncWebServer server(80);

void setup() {
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 - Initializes the MFRC522 chip
  Serial.begin(9600);
  

  WiFi.begin(ssid, password);   //  WiFi.mode(WIFI_STA);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println();
  Serial.print("ESP IP Address: http://");
  Serial.println(WiFi.localIP());
  
  pinMode(output, OUTPUT);
  digitalWrite(output, LOW);
  
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){  // Send web page to client
    request->send_P(200, "text/html", index_html);
  });
  
  server.on("/name", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", movie_name );
  });
  
  server.on("/button", HTTP_GET, [] (AsyncWebServerRequest *request) { // Receive an HTTP GET request from button
      String inputMessage;
      inputMessage = request->getParam(PARAM_INPUT)->value();
      Serial.println(inputMessage);
    request->send(200, "text/plain", "ok");
  });

  server.onNotFound(notFound);
  server.begin();
}


void loop() {
   if ( ! rfid.PICC_IsNewCardPresent()) // check for new card
    return;
   if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
   }
    Serial.println("**************");
    Serial.print(F("UID: "));
    Serial.println(tag);
    if (tag == the_ring){
      movie_name = "The_ring";
    }
    if(tag == friends){
      movie_name = "Friends";
    }
    tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
}
