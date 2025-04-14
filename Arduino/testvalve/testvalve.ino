#include <Servo.h>

#define PULSEPIN    12  // Pin Servo
const int ANGLEOFFSET = 55;  // Angulo de ajuste valvula
const int DUCICOEFF   = 10;  // Coeficiente para el duty cicle

class Valve : public Servo{
  public:
    Valve(bool noCl): _normClosed(noCl), _isopen(true), cicle(0){}
    
    void open(){
      if(!_isopen){ // s1: 0 s2: 90
        this->write(125 - !_normClosed*90 + ANGLEOFFSET);
        _isopen = true;
      }
    }
    void close(){
      if(_isopen){ // s1: 35 s2: 125
        this->write(_normClosed*90 + ANGLEOFFSET);
        _isopen = false;
      }
    }
    void set(bool s){
      if(s){
        open();
      }else{
        close();
      }
      cicle = 0;
    }
    void update(int dCycle){
      if(cicle < -dCycle*DUCICOEFF){
        open();
      }else if(cicle < 100*DUCICOEFF){
        close();
      }else{
        cicle = 0;
        return;
      }
      cicle++;
    }

    bool IsNormallyClosed() { return _normClosed;}
    bool IsOpen() { return _isopen;}

  private:
    bool _normClosed, // Si esta valvula esta normalmente cerrada o no
        _isopen;      // El estado actual de la valvula (Abierto o "No Abierto")
    int cicle;        // El instante actual en el ciclo de la señal PWM
};

Valve valve1(false);

void setup() {
  Serial.begin(9600);
  // Servo
  valve1.attach(PULSEPIN);
  delay(500);
  valve1.close();
}

void loop() {
  valve1.open();
  delay(5000);
  valve1.close();
  delay(2000);
}
