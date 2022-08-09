#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>
#include <TZ.h>
#include <FS.h>
#include <LittleFS.h>
#include <CertStoreBearSSL.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

// Update these with values suitable for your network.
const char* ssid = "Bsmk";
const char* password = "****";
const char* mqtt_server = "2124b8ecf14e4692b62288592e5c4a12.s2.eu.hivemq.cloud";

// A single, global CertStore which can be used by all connections.
// Needs to stay live the entire time any of the WiFiClientBearSSLs
// are present.
BearSSL::CertStore certStore;

WiFiClientSecure espClient;
PubSubClient * client;
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (500)
char msg[MSG_BUFFER_SIZE];
int light_value = 0;
bool inside = false;
bool permitted_hours = false;
bool new_worker = false;
bool entry_granted = false;
bool chilling = false;
bool ver = false;



const int trigPin = D0;
const int echoPin = D1;
//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
long duration;
float distance;

#define FRONT_LED D6            
#define INSIDE_LED D5 

constexpr uint8_t RST_PIN = D3;     
constexpr uint8_t SS_PIN = D4;       

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

Servo servo;
String tag;

String verified = "441437673";


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void setDateTime() {
  // You can use your own timezone, but the exact time is not used at all.
  // Only the date is needed for validating the certificates.
  configTime(12600, 0 , "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(100);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println();

  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.printf("%s %s", tzname[0], asctime(&timeinfo));
}



void callback(char* topic, byte* payload, unsigned int length) {
  String my_topic = topic;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  int i;
  for (i = 0; i < length; i++) {
    msg[i]=(char)payload[i];
  }
  msg[i] = '\0';
  
  Serial.println(msg);
  if(my_topic == "/time"){
      Serial.println("/time");
      if(msg[0] == 'y' && msg[1] == 'e' && msg[2] == 's'){
            Serial.println("Lighs on");
            digitalWrite(FRONT_LED, HIGH);
            permitted_hours = true;
      }
      else if (msg[0] == 'n' && msg[1] == 'o'){
            Serial.println("Lighs off");
            digitalWrite(FRONT_LED, LOW);
            permitted_hours = false;
      }
  }
  else if(my_topic == "/LDR"){
      Serial.println("/LDR");
      int sensorValue = analogRead(A0);   // read the input on analog pin 0
      light_value = atoi(msg);
      if (light_value >= 0 && ver){
        servo.write(90);
        delay(50);

        inside = !inside;
        chilling = true;
        Serial.println("you can enter");
        int desired = light_value*1023/100;
        Serial.print("Your Desired Light: ");
        Serial.println(desired);
        Serial.print("Current Light: ");
        Serial.println(sensorValue);
        if(desired > sensorValue){
            analogWrite(INSIDE_LED, desired - sensorValue);
        }
        tag = "";
      }
      else{
        tag = "";
        chilling = false;
        Serial.println("you cannot enter");
      }
      
      
  }

}


void reconnect() {
  // Loop until we’re reconnected
  while (!client->connected()) {
    Serial.print("Attempting MQTT connection…");
    String clientId = "ESP8266Client - MyClient";
    // Attempt to connect
    // Insert your password
    if (client->connect(clientId.c_str(), "mahvash", "Mahvash123")) {
      Serial.println("connected");

      
      // … and resubscribe
      client->subscribe("/time");
      client->subscribe("/LDR");

      
    } else {
      Serial.print("failed, rc = ");
      Serial.print(client->state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

float getDistance(){
  
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
    return distance;
}


void setup() {
  delay(500);
  // When opening the Serial Monitor, select 9600 Baud
  Serial.begin(9600);
  delay(500);

  LittleFS.begin();
  setup_wifi();
  setDateTime();


  // you can use the insecure mode, when you want to avoid the certificates
  //espclient->setInsecure();

  int numCerts = certStore.initCertStore(LittleFS, PSTR("/certs.idx"), PSTR("/certs.ar"));
  Serial.printf("Number of CA certs read: %d\n", numCerts);
  if (numCerts == 0) {
    Serial.printf("No certs found. Did you run certs-from-mozilla.py and upload the LittleFS directory before running?\n");
    return; // Can't connect to anything w/o certs!
  }
  BearSSL::WiFiClientSecure *bear = new BearSSL::WiFiClientSecure();
  // Integrate the cert store with this connection
  bear->setCertStore(&certStore);

  client = new PubSubClient(*bear);

  client->setServer(mqtt_server, 8883);
  client->setCallback(callback);



  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
  SPI.pins(6, 7, 8, 0) ;
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 - Initializes the MFRC522 chip.
  servo.attach(D2);
  servo.write(0);
  delay(1000);  

  pinMode(FRONT_LED, OUTPUT);
  pinMode(INSIDE_LED, OUTPUT);

  
  Serial.println("Set up completed");
  
}



void loop() {
  if (!client->connected()) {
    reconnect();
  }
  client->loop();


    distance = getDistance();
    if(distance < 10){
      if (new_worker){
        client->publish("/ultrasonic", "1");
        Serial.println("ultrasonic pub");
        new_worker = !new_worker;
      }
    }
    else{
      new_worker = !new_worker;
      digitalWrite(FRONT_LED, LOW);
    }

    if (rfid.PICC_IsNewCardPresent() && permitted_hours){
      Serial.println("new card");
          if (rfid.PICC_ReadCardSerial()) {
                for (byte i = 0; i < 4; i++) {
                  tag += rfid.uid.uidByte[i];
                }
                
                Serial.println("**************");
                Serial.print(F("UID: "));
                Serial.println(tag);
                if (tag == verified){
                  
                  ver = true;
                  if(inside){
                    inside = !inside;
                    client->publish("/rfid", "441437673_1_exit");
                    Serial.println("441437673_1_exit");
                    servo.write(0);
                    analogWrite(INSIDE_LED, 0);
                    chilling = false;
                    permitted_hours = false;
                  }
                  else{
                    client->publish("/rfid", "441437673_1_enter");
                    Serial.println("441437673_1_enter");
                  }
                  
                }
                else{
                  ver = false;
                }
                rfid.PICC_HaltA();
                rfid.PCD_StopCrypto1();
                tag = "";
        }
   }

   if(inside && chilling){
        int sensorValue = analogRead(A0);   // read the input on analog pin 0
        if (light_value >= 0){
            int desired = light_value*1023/100;
            if(desired > sensorValue){
                analogWrite(INSIDE_LED, desired - sensorValue);
            }
        }
   }
    
    delay(5);


}
