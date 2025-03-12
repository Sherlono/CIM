#include <Servo.h>

#define TRIGGER1 2
#define ECHO1 3
#define TRIGGER2 4
#define ECHO2 5

void setup() {
  Serial.begin(9600);
  // Radar
  pinMode(TRIGGER1, OUTPUT);
  pinMode(ECHO1, INPUT);
  digitalWrite(TRIGGER1, LOW);
  pinMode(TRIGGER2, OUTPUT);
  pinMode(ECHO2, INPUT);
  digitalWrite(TRIGGER2, LOW);
}

void loop() {
  radarUpdate();
}

void radarUpdate(){
  long t1, t2; // Tiempo que demora en llegar el eco
  long d1, d2; // Distancia en centimetros
 
  digitalWrite(TRIGGER1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER1, LOW);
  
  t1 = pulseIn(ECHO1, HIGH);
  d1 = t1/59;             // Escalamos el tiempo a una distancia en cm
  
  digitalWrite(TRIGGER2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER2, LOW);
  
  t2 = pulseIn(ECHO2, HIGH);
  d2 = t2/59;
  
  Serial.print("Distancia 1: ");
  Serial.print(d1);
  Serial.print("cm. ");
  Serial.print("Distancia 2: ");
  Serial.print(d2);
  Serial.println("cm");
}
