const int trigerPin = D4;  
const int echoPin = D3; 
const int first_row = D5;
const int second_row = D0;
const int third_row = D1;
const int fourth_row = D7;
const int fifth_row = D2;

long  duration; // micro-secend
float distance; // cm

void setup() {
  pinMode(trigerPin,OUTPUT ); // Sets the trigerPin as an Output
  pinMode(echoPin, INPUT ); // Sets the echoPin as an Input
  // led_row pins
  pinMode(first_row, OUTPUT);
  pinMode(second_row, OUTPUT);
  pinMode(third_row, OUTPUT);
  pinMode(fourth_row, OUTPUT);
  pinMode(fifth_row, OUTPUT);
  Serial.begin(9600); // Starts the serial communication
}
void loop() {

  // turn of all LED. this method reduce code-lines
  digitalWrite(first_row, LOW);
  digitalWrite(second_row, LOW);
  digitalWrite(third_row, LOW);
  digitalWrite(fourth_row, LOW);
  digitalWrite(fifth_row, LOW);
  
  // Clears the trigerPin
  digitalWrite(trigerPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigerPin on HIGH state for 10 micro seconds
  digitalWrite(trigerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigerPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);

  // there are overlaps for 2 Consecutive LED-row
  if ( 8.75   <= distance && distance < 14){
    digitalWrite(first_row, HIGH);
  }
  if ( 6.5 <= distance && distance < 12){
    digitalWrite(second_row, HIGH);
  }
  if ( 5.5 <= distance && distance < 8.7){
    digitalWrite(third_row, HIGH);
  }
  if (distance < 7){
    digitalWrite(fourth_row, HIGH);
  }
  if (distance <= 5){
    digitalWrite(fifth_row, HIGH);
  }
  delay(200);
}
