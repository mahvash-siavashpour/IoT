
const unsigned int LED_PIN  = 4;  
const short int LED_BUILTIN2 = 16;
int light_threshold = 79;
void setup() {
  Serial.begin(9600);   // initialize serial communication at 9600 BPS
  pinMode(LED_PIN,OUTPUT); // Set LED pin as output
  pinMode(LED_BUILTIN, OUTPUT); // set internal LED pins as output
  pinMode(LED_BUILTIN2, OUTPUT);
  pinMode(D5,OUTPUT) ;  // set D5 for buzzer-pin as output
}
void loop() {
    
    digitalWrite(LED_PIN, HIGH); //Tuen On the LED.
    int sensorValue = analogRead(A0); // read the input on analog pin 0
    float voltage_degree = sensorValue * (100.0 / 1023.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage_degree (0 - 100)
    Serial.println(voltage_degree); // print out the value you read
    if ( voltage_degree > light_threshold){
       digitalWrite(D5, HIGH); //buzzer sound on.
       digitalWrite(LED_BUILTIN, HIGH); // turn blue LED on
       digitalWrite(LED_BUILTIN2, LOW); // turn red LED off
    }
    else{
      digitalWrite(D5, LOW); // buzzer off
      digitalWrite(LED_BUILTIN, LOW); // turn BLue LED off
      digitalWrite(LED_BUILTIN2, HIGH); //turn Red LED on
    }
    
}
