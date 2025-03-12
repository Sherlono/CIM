#include <BluetoothSerial.h>

class BluetoothSerialPlus: public BluetoothSerial{
public:
  void write(String message){
    int size = message.length();
    const uint8_t* text = (const uint8_t*)message.c_str();
    this->BluetoothSerial::write(text, size);
  }
};

//#define RXD2 16
//#define TXD2 17

BluetoothSerialPlus BT;
byte led_1 = 14;
String StringIN = " ";
int state = 0;

String commando1 = "JOINTS";
String commando2 = "XYZ";

void setup() {
  Serial.begin(9600);
  BT.begin(9600);
  //Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  
  Serial.println("Iniciando...");
  delay(2000);
}

void loop() {
  if(BT.available()){
    StringIN = BT.readString();
    // --- Comunicación ---
    //BT.write("Que tal!");
    Serial.println(StringIN);
  }
  /*while(Serial2.available()){
    Serial.println(Serial2.read());
  }
  delay(100);*/
}
