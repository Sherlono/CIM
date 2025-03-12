#include <Servo.h>

#define PULSO1 10
#define PULSO2 11

Servo myservo1, myservo2;
int pos = 0;

void setup() {
  Serial.begin(9600);
  // Servo
  myservo1.attach(PULSO1);
  myservo2.attach(PULSO2);
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo1.write(pos);                // tell servo to go to position in variable 'pos'
    myservo2.write(pos);
    delay(15);                          // waits 15ms for the servo to reach the position
    Serial.println(pos);
  }
  Serial.println("180º Alcanzados");
  delay(2000);
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo1.write(pos);                // tell servo to go to position in variable 'pos'
    myservo2.write(pos);
    delay(15);                          // waits 15ms for the servo to reach the position
    Serial.println(pos);  
  }
  Serial.println("0º Alcanzados");
  delay(2000);
}
