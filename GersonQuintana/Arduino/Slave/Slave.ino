#include <Wire.h>
#include <Servo.h>

int TRIG = 12;
int ECO = 11;
int ESTADO = 0;

int distancias[18];

void setup() {
  Serial.begin(9600);
  Serial.println("ESCLAVO");
  // Sensor
  pinMode(TRIG, OUTPUT);
  pinMode(ECO, INPUT);
  // I2C
  Wire.begin(2);
  Wire.onReceive(requestEvent);
  Wire.onRequest(datos);
}

void loop() {

}

void datos() {
  if (ESTADO == 0) {
    Wire.write(1);
    Serial.println("Si pide mensaje");
  }
  else if (ESTADO == 1) {
    barrido();
  }
}

void requestEvent() {
  int x = Wire.read();    // receive byte as an integer
  if (x == 0) {
    ESTADO = 0;
  }
  else if (x == 1) {
    ESTADO = 1;
  }
  else if (x == 2) {
    ESTADO = 2;
  }
  else if (x == 3) {
    char ok = Wire.read();

    if (ok == 'o') { // Los arreglos son iguales
      int pos = 0;
      x = Wire.read();
      while (x != 100) { // Mientras no se envie un 100, seguir leyendo lo que envia el maestro
        distancias[pos] = x; // Guardando lo enviado por el maestro
        pos++; // incrementando la posicion del arreglo
        x = Wire.read(); // Leyendo lo siguiente que envio el Maestro
      }
      Serial.println("Distancias obtenidas en el Esclavo: ");
      for (int i = 0; i < 18; i++) {
        Serial.print(distancias[i]);
        Serial.print(", ");
      }
    }
    else {
      Serial.println("Hay distancias diferentes");
    }
  }
  
}

void barrido() {
  Serial.println("Se esta haciendo el barrido");  
//  for (int i = 0 ; i < 180 ; i += 10) {
//    servomec.write(i);
//    digitalWrite(TRIG, LOW);
//    delayMicroseconds(2);
//    digitalWrite(TRIG, HIGH);
//    delayMicroseconds(10);
//    digitalWrite(TRIG, LOW);
//    DURACION = pulseIn(ECO, HIGH);
//
//    DISTANCIA = (DURACION * 0.034) / 2;
//
//    DISTANCIAC = (char)DISTANCIA;
//
//    Serial.println(DISTANCIAC);
//
//    //    Wire.write(53);
//    delay(1000);
//  }
//
//
//  for (int i = 180 ; i >= 0 ; i -= 10) {
//    servomec.write(i);
//    digitalWrite(TRIG, LOW);
//    delayMicroseconds(2);
//    digitalWrite(TRIG, HIGH);
//    delayMicroseconds(10);
//    digitalWrite(TRIG, LOW);
//    DURACION = pulseIn(ECO, HIGH);
//
//    DISTANCIA = (DURACION * 0.034) / 2;
//    DISTANCIAC = (char)DISTANCIA;
//    Serial.println(DISTANCIAC );
//
//    delay(1000);
//  }
}
