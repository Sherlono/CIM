/*  Nota Importante
 *   Desde el 13/11/2024 la base de datos Firebase se inició en modo de prueba 
 *   lo que significa de se podra sobre escribir y leer datos libremente durante
 *   30 dias. Seguido de estos se debera volver a iniciar en modo de prueba o
 *   investigar los requerimientos para el modo normal.
 *   Para consultas contactarme a javier.sanchez1501@alumnos.ubiobio.cl
 */
//#include <Config.h>
//#include <FirebaseClient.h>
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>

const char *FIREBASE_HOST = "https://maqueta-agua-default-rtdb.firebaseio.com";    // URL
const char *FIREBASE_AUTH = "6CmEzSJeKbQ6V7mfZUyKLMrF10mD0OSUQDRCS3Uv";            // Secret

const char *ssid = "wifi-ubb";
const char *password = "soporte-dci";
FirebaseData firebaseData;
int value;

void setup() {
  Serial.begin(115200);
  // WiFi
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(250);
  }
  Serial.print("\nConectado al Wi-Fi");
  Serial.println();
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {
  Firebase.getInt(firebaseData, "Value");
  value = firebaseData.intData();
  Serial.println(value);
  delay(250);
}
