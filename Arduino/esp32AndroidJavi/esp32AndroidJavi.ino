#include <BluetoothSerial.h>

class BluetoothSerialPlus : public BluetoothSerial {
public:
  void write(String message) {
    int size = message.length();
    const uint8_t* text = (const uint8_t*)message.c_str();
    this->BluetoothSerial::write(text, size);
  }
};

BluetoothSerialPlus BT;  // 10 RX, 11 TX.
byte led_1 = 14;
char CharIN = ' ';
int state = 0;

void setup() {
  Serial.begin(9600);
  BT.begin(9600);

  pinMode(led_1, OUTPUT);
  digitalWrite(led_1, LOW);

  Serial.println("Iniciando...");
}

void loop() {
  if (BT.available()) {
    CharIN = BT.read();
    //Comunicación
    if (CharIN == 'a') {
      state = 0;
    } else if (CharIN == 'b') {
      state = 1;
    } else if (CharIN == 'c') {
      state = 2;
    } else if (CharIN == 'd') {
      state = 3;
    } else if (CharIN == 'r') {
      BT.write("Que tal! Soy el dispositivo esp32. Un gusto en conocerte.\nEsto es una prueba para ver que tan largo pueden ser los mensajes por bluetooth antes de que empieze a haber problemas.");
    }
    Serial.println(CharIN);
  }
  // Parpadeo LED
  if (state) {
    digitalWrite(led_1, HIGH);
    delay(1000 * state);
    digitalWrite(led_1, LOW);
    delay(1000 * state);
  } else {
    digitalWrite(led_1, LOW);
    delay(100);
  }
}
