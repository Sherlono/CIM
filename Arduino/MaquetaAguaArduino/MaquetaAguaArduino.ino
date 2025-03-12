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
  Valve(bool noCl): _normClosed(noCl), cicle(0), _isopen(true){}
  
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
  void update(int dCycle){  // La valvula esta abierta mientras el ciclo no supere el duty cycle
    if(cicle <= dCycle){
      open();
    }else if(cicle < 100){
      close();
    }else{
      open();
      cicle = 0;
      return;
    }
    cicle++;
  }
  
private:
  bool _normClosed, // Si esta valvula esta normalmente cerrada o no
       _isopen;     // El estado actual de la valvula (Abierto o "No abierto")
  int cicle;        // El instante actual en el ciclo de la señal PWM (Entre 0 y 100 y vuelve a 0)
};

Valve valve1(false), valve2(true);  // Parientes de la clase Servo con funcionalidad extra
int color1, color2;                 // Representa el duty cicle de cada "Valvula"
String Saludo = "¿Que balance de color desea? (Muy Azul 0 - 100 Muy Rojo)\n";

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
  //drenar();
  MedirEstanques();
  Serial.print(Saludo);
  getValue(color1);       // Obtiene el valor ingresado por usuario
  color2 = 100 - color1;  // color2 es el complemento de color1
  
  Iniciar_llenado();      // Aqui es donde ocurre el duty cycle
  
  // El proceso ha terminado
  valve1.close();
  valve2.close();
  Serial.println("Listo!");
}

void MedirEstanques(){
  long d1 = 0, d2 = 0;        // Distancia en centimetros
  d1 = measure(1);
  d2 = measure(2);
  
  // Mostrar las mediciones   
  /*Serial.print("Distancia 1: ");
  Serial.print(d1);
  Serial.print(" cm. ");
  Serial.print("Distancia 2: ");
  Serial.print(d2);
  Serial.println(" cm.");*/
  
  if(d1 >= 12){
    Serial.println("El estanque 1 esta vacio. Por favor rellenar.");
  }
  if(d2 >= 12){
    Serial.println("El estanque 2 esta vacio. Por favor rellenar.");
  }
  
  while(d1 >= 12 || d2 >= 12){
    d1 = measure(1);
    d2 = measure(2);
    delay(3000);
  }
}

long measure(int radar){
  if(radar == 1 || radar == 2){ // Solo hay dos radares
    long t; // Tiempo que demora en llegar el eco
    int triggerpin = TRIGGER1 + 2*(radar-1);
    
    digitalWrite(triggerpin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerpin, LOW);
    
    t = pulseIn(ECHO1 + 2*(radar-1), HIGH);
    int d = t/59;
    
    return d;             // Escalamos el tiempo a una distancia en cm
  }else{
    Serial.println("Radar incorrecto.");
    return 0;
  }
}

void getValue(int& value){
  do{
    while (!Serial.available()){}
    value = Serial.parseInt();
    if(value > 100){
      Serial.println("Numero demasiado alto. Intenta de nuevo.");
    }else if(value < 0){
      Serial.println("Numero debe ser mayor o igual a 0. Intenta de nuevo.");
    }
  }
  while(value < 0 || value > 100);
  Serial.println(value);
}

void Iniciar_llenado(){
  Serial.print("Llenado en proceso...\n");
  for(int i = 0; i < 5000; i++){
    if(color1){
      valve1.update(color1);
    }
    if(color2){
      valve2.update(color2);
    }
    delay(28);
  }
}

void drenar(){
  valve1.open();
  valve2.open();
}
