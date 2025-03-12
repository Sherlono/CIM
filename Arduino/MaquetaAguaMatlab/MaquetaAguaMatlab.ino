// Placa: Arduino UNO (china)
#include <Servo.h>

#define TRIGGER1 2
#define ECHO1 3
#define TRIGGER2 4
#define ECHO2 5
#define PULSO1 10
#define PULSO2 11

class Valve : public Servo{
public:
  Valve(bool noCl): _normClosed(noCl), _isopen(true){}
  
  void open(){
    if(!_isopen){ // s1: 0 s2: 90
      this->write(_normClosed*90);
      _isopen = true;
    }
  }
  void close(){
    if(_isopen){ // s1: 35 s2: 125
      this->write(125 - !_normClosed*90);
      _isopen = false;
    }
  }
  void set(bool s){
    if(s){
      open();
    }else{
      close();
    }
  }

  bool IsNormallyClosed(){ return _normClosed;}
  bool IsOpen(){ return _isopen;}
private:
  bool _normClosed, // Si esta valvula esta normalmente cerrada o no
       _isopen;     // El estado actual de la valvula (Abierto o "No Abierto")
};

Valve valve1(false), valve2(true);  // Parientes de la clase Servo con funcionalidad extra
bool u1 = false, u2 = false;
long d1 = 0, d2 = 0;        // Distancia en centimetros
int start_t;
String dataString = "";
bool dataComplete = false;
const int dataLength = 2;
int datos[dataLength];
const char separator = ',';

void setup() {
  Serial.begin(9600);
  // Radar
  pinMode(TRIGGER1, OUTPUT);
  pinMode(ECHO1, INPUT);
  digitalWrite(TRIGGER1, LOW);
  pinMode(TRIGGER2, OUTPUT);
  pinMode(ECHO2, INPUT);
  digitalWrite(TRIGGER2, LOW);
  // Servo
  valve1.attach(PULSO1);
  valve2.attach(PULSO2);
  delay(500);
  valve1.close();
  valve2.close();
}

void loop() {
  if (dataComplete){
    // Leer datos Matlab
    for (int i = 0; i < dataLength ; i++){
      int index = dataString.indexOf(separator);
      datos[i] = dataString.substring(0,index).toInt();
      dataString = dataString.substring(index + 1);
    }
    //u1 = datos[0];
    //u2 = datos[1];

    /*MedirEstanques();  // Medir nivel de los estanques
    // Enviar mediciones a Matlab
    Serial.println(d1);
    Serial.println(d2);
    Serial.println(millis());*/
    //d1 = datos[0];
    if((millis()/3000)%3 == 0){Serial.println(0);}
    else if((millis()/3000)%3 == 1){Serial.println(1);}
    else {Serial.println(2);}

    // Actuar valvulas
    //valve1.set(u1);
    //valve2.set(u2);

    // Reiniciar variables auxiliares
    dataString = "";
    dataComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    dataString += inChar;
    if (inChar == '\n') {
      dataComplete = true;
    }
  }
}

void MedirEstanques(){
  d1 = measure(1);
  d2 = measure(2);
}

long measure(int radar){
  int triggerpin = TRIGGER1 + 2*(radar-1);
  int echopin = ECHO1 + 2*(radar-1);
  
  digitalWrite(triggerpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerpin, LOW);
  
  long t = pulseIn(echopin, HIGH); // Tiempo que demora en llegar el eco
  int d = t/59;
  
  return d;             // Escalamos el tiempo a una distancia en cm
}
