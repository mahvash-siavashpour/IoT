
// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>

// Replace with your network credentials
const char* ssid     = "****";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "*******"; 


String sliderValue = "0";

const char* PARAM_INPUT = "value";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
Servo servo;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
    <head>
        <title>
            IoT
        </title>
    </head>
    <body>
        <h1>Stadium</h1>
        0<input type="range" id="rangeInput" min="0" max="90" onchange="readSlider()"> 90
        <br>
        <p>Value: <span id="demo"></span></p>
        <script>
            function readSlider(){
                var sliderValue = document.getElementById("rangeInput").value
                console.log(sliderValue)
                var output = document.getElementById("demo");
                output.innerHTML = sliderValue;
                var xhr = new XMLHttpRequest();
                xhr.open("GET", "/slider?value="+sliderValue, true);
                xhr.send();
            }
        </script>
    </body>
</html>
)rawliteral";


void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  servo.attach(D2);
  servo.write(0);
  delay(1000);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });


  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue = inputMessage;
      servo.write(sliderValue.toInt());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
  
  // Start server
  server.begin();
}
  
void loop() {
  
}
