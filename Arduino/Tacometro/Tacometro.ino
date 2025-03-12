#include <Wire.h>
#include <VL53L0X.h>

#define pi 3.141592655

VL53L0X sensor;
int prev_data = 999, current_data = 0, pulses = 0, i = 0;
unsigned long prev_time, current_time;
float speed = 0;

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }
  
  sensor.startContinuous();
}

void loop()
{
  current_data = sensor.readRangeContinuousMillimeters();
  current_time = millis();

  if(prev_data - current_data < 18 ){
    pulses++;
  }
  /*if(i == 200){
    calc_speed();
    prev_time = current_time;
    pulses = 0;
    i = 0;
  }*/

  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  /*Serial.print("Distancia: ");
  Serial.print(current_data);
  Serial.print(" ");
  Serial.print(current_data - prev_data);
  Serial.print(" ");
  Serial.print(pulses);
  Serial.print(" ");
  Serial.println(speed);*/
  Serial.print("Distancia: ");
  Serial.print(current_data);
  Serial.print(" Diferencia: ");
  Serial.print(current_data - prev_data);
  Serial.print(" Pulsos: ");
  Serial.println(pulses);

  prev_data = current_data;
  i++;
}
inline void calc_speed(){
  float delta_time = (current_time - prev_time)/1000; // dt en segundos
  //speed = (pulses*1.12)/delta_time;                 // Velocidad en m/s (6.6 (metros)/5.9(pulsos) = 1.12)
  //speed = (pulses*2*pi*(5/1.6))/(12*delta_time);    // Velocidad en m/s (6.6 (metros)/5.9(pulsos) = 1.12)
  speed = pulses/(0.2*delta_time);                    // rpm (12(pulsos)/60(segundos) = 0.2)
}
