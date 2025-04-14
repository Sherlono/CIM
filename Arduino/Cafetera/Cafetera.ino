/* Placa: Arduino Mega 2560
 * Sensor: Parallax Ping)))
 * L298N Dual H-Bridge Motor Controller
 * 5V Dual-Channel Relay Module */

#include <Servo.h>

#define INONEPIN    5   // Pin In1  Relay
#define SENSORPIN   7   // Pin Sensor
#define INTHREEPIN  8   // Pin In3  H Bridge
#define INFOURPIN   9   // Pin In4  H Bridge
#define PWMPIN      10  // Pin PWM
#define PULSEPIN    12  // Pin Servo
constexpr int ANGLEOFFSET = 50;  // Angulo de ajuste valvula
constexpr int DUCICOEFF   = 10;  // Coeficiente para el duty cicle

class Ultrasonic{
  public:
    Ultrasonic(int pin): this_pin(pin){}
    // Begin the detection and get the pulse back signal
    void DistanceMeasure(void){
        pinMode(this_pin, OUTPUT);
        digitalWrite(this_pin, LOW);
        delayMicroseconds(2);
        digitalWrite(this_pin, HIGH);
        delayMicroseconds(5);
        digitalWrite(this_pin, LOW);
        pinMode(this_pin, INPUT);
        duration = pulseIn(this_pin, HIGH);
    }
    // The measured distance from the range 0 to 400 Centimeters
    long microsecondsToCentimeters(){ return 8 - duration/29/2;}
    
  private:
    int this_pin;   //pin number of Arduino that is connected with SIG pin of Ultrasonic Ranger.
    long duration;  // the Pulse time received;
};

class Valvula : public Servo{
  public:
    Valvula(bool noCl): _normClosed(noCl), _isopen(true), cicle(0){}
    
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

class Bomba{
  public:
    Bomba(int pwm, int i1, int i2): _pwmPin(pwm), _in1Pin(i1), _in2Pin(i2){}
    
    void forward(int input){
      analogWrite(_pwmPin, input);
      digitalWrite(_in1Pin, HIGH);
      digitalWrite(_in2Pin, LOW);
    }
    void stop(){
      analogWrite(_pwmPin, 0);
      digitalWrite(_in1Pin, LOW);
      digitalWrite(_in2Pin, HIGH);
    }

    void update(int input){
      if(input > 0){
        forward(input);
      }else{
        stop();
      }
    }
    private:
      int _pwmPin, _in1Pin, _in2Pin;
};

// Salida del controlador
int u1 = 0;
// Distancia en centimetros
float x1 = 0;
// Variables control
float k[3];  // k[0] = Kp, k[1] = Ki, k[2] = Kd
float referencia, memory = 0;
float error = 0, prev_error = 0;
unsigned long prev_time = 0;

Ultrasonic ultrasonic(SENSORPIN);
Valvula valvula(false);
Bomba bomba(PWMPIN, INTHREEPIN, INFOURPIN);

void setup() {
  Serial.begin(9600);
  // Servo
  valvula.attach(PULSEPIN);
  delay(500);
  valvula.close();
  // Bomba
  pinMode(INONEPIN, OUTPUT);
  digitalWrite(INONEPIN, LOW);
  delay(10);
  pinMode(INTHREEPIN, OUTPUT);
  pinMode(INFOURPIN, OUTPUT);
  pinMode(PWMPIN, OUTPUT);
  bomba.stop();
  delay(10);

  // Pedir las ganancias PID
  for(int i = 0; i < 3; i++){
      do{
        if(i == 0){ Serial.println("Ingrese ganancia P.");}
        else if(i == 1){ Serial.println("Ingrese ganancia I.");}
        else{ Serial.println("Ingrese ganancia D.");}

        while(Serial.available() == 0){}
        k[i] = Serial.parseFloat();
        if(k[i] < 0){
          Serial.println("Debe ser mayor o igual a 0.");
        }
      }while(k[i] < 0);
      Serial.println(k[i]);
  }
  // Pedir la referencia
  do{
    Serial.println("Ingrese la referencia.");
    while(Serial.available() == 0){}
    referencia = Serial.parseFloat();
    if(referencia < 0 || referencia > 8){
      Serial.println("Debe ser mayor o igual a 0 y menor o igual a 8.");
    }
  }while(referencia < 0 || referencia > 8);
}

void loop() {
  // Medir estado de la variable
  ultrasonic.DistanceMeasure();
  x1 = ultrasonic.microsecondsToCentimeters();
  x1 = truncar(x1, 0, 20);

  // Calcular error
  error = referencia - x1;

  // Calcular señales de control
  u1 = Controlador(x1); // PID
  u1 = truncar(u1, -100, 100);

  // Actuar valvula y bomba
  if(u1 < 0){
    bomba.stop();  // Bomba de agua Detenida
    valvula.open(); // Valvula Abierta
  } else {
    bomba.update(u1);  // Bomba activada
    valvula.close();    // Valvula Cerrada
  }

  // Graficar datos
  Serial.print(u1);
  Serial.print(",");
  Serial.print(x1);
  Serial.print("\n");
}

int Controlador(float x){
  float dt = (millis() - prev_time)/1000;

  memory += error * dt;
  float p_out = k[0] * error;
  float i_out = k[1] * memory;
  float d_out = k[2] * (error - prev_error) / dt;

  prev_error = error;
  prev_time = millis();
  return round(p_out + i_out + d_out);
}

int truncar(int in, int min, int max){
  int out = (in > max) ? max : in;
  out = (out < min) ? min : out;
  return out;
}
