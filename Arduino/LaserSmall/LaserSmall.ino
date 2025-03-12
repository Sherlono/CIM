/*Rango de medicion: 2mm a 1100mm*/

#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;
int current_data = 0;

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

  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  Serial.print("Distancia: ");
  Serial.println(current_data);
} 
