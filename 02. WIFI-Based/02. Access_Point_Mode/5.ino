#include <ESP8266WiFi.h>     

const char *ssid = "AP Q5"; // The name of the AP  
const char *password = "12345678";  

IPAddress local_IP(192,168,4,16);
IPAddress gateway(192,168,9,144);
IPAddress subnet(255,255,255,0);

void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.println('\n');

  WiFi.softAP(ssid, password);           // Start the access point
  
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.println("\" started");
  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());         // Send the IP address of the ESP8266 to seral monitor
  Serial.println("do you want change your IP?(y/n) ");
  while(Serial.available() == 0) {}        // wait for user input to change IP
  Serial.read();                            
  Serial.println("\n ************");
  WiFi.softAPConfig(local_IP, gateway, subnet);
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
}

void loop() { }
