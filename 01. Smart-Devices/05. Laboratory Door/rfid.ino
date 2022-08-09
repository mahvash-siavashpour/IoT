/* Read RFID Tag with RC522 RFID Reader
 *  Made by miliohm.com
 */
 
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

constexpr uint8_t RST_PIN = D3;     
constexpr uint8_t SS_PIN = D4;     

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class


Servo servo;
String tag;

String verified = "441437673";
String unVerified = "2111823226";

#define LED D1
#define BUZZER D0

const char* ssid       = "Bsmk";
const char* password   =  "*********";
const long utcOffsetInSeconds = 12600; // 3.5 * 60 * 60
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP; //Creates a named instance of the WiFi UDP class that can send and receive UDP messages
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);


void setup() {
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 - Initializes the MFRC522 chip.
  servo.attach(D2);
  servo.write(0);
  delay(1000);
  pinMode(LED, OUTPUT); // set the digital pin as output.
  pinMode(BUZZER, OUTPUT);

  //wifi
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();

}

void loop() {
  timeClient.update();
  if ( ! rfid.PICC_IsNewCardPresent()) // check for new card
    return;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    
    Serial.println("**************");
    Serial.print(F("UID: "));
    Serial.println(tag);
    Serial.print(daysOfTheWeek[timeClient.getDay()]);
    Serial.print(", ");
    Serial.print(timeClient.getHours());
    Serial.print(":");
    Serial.print(timeClient.getMinutes());
    Serial.print(":");
    Serial.println(timeClient.getSeconds());
    if (tag == verified && timeClient.getHours() >= 9 && timeClient.getHours() < 15){
      Serial.println("Accecc Granted!");
      servo.write(90);
      digitalWrite(LED, HIGH);
      delay(3000);
      
      digitalWrite(LED, LOW); 
      servo.write(0);
      delay(1000);
    }
    else{
      Serial.println("Accecc Denied!");
      digitalWrite (BUZZER, HIGH);
      delay(3000);
      digitalWrite (BUZZER, LOW);
    }
    tag = "";

    
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}
