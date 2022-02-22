#include <Wire.h>
#include <Servo.h>
int TRIG = 12;
int ECO = 11;
int DURACION;
int DISTANCIA;
int ESTADO = 0;
char DISTANCIAC;

int distancias[18]; // Se guardan las distancias enviadas desde el Maestro

//servo
Servo servomec;

void setup()
{
  Wire.begin(2);                // Este Esclavo es el número 2

  Wire.onReceive(requestEvent); // Se ejecuta el metodo que esta entre parentesis
  Wire.onRequest(datos); // Cuando el Maestro le hace una petición,
  pinMode(TRIG, OUTPUT);
  pinMode(ECO, INPUT);                            // realiza el requestEvent
  Serial.begin(9600);
  servomec.attach(8);//Pin
}

void loop()
{
  if (ESTADO == 0) {
    mensaje();
  }
  else if (ESTADO == 1) {
    barrido();
  }
  else if ( ESTADO == 2 ) {
    Disparar();
  }
  delay(500);

}

void datos() {
  if (ESTADO == 0) {
    Wire.write(5);
  }
  else if (ESTADO == 1) {
    Wire.write(DISTANCIAC + 32);
  }

}

// Esto es lo que envía cuando le hace la petición.
void requestEvent()
{
  int x = Wire.read();    // receive byte as an integer
  if (x == 0) {
    ESTADO = 0;
  }
  else if (x == 1 ) {
    ESTADO = 1 ;
  }
  else if (x == 2) {
    ESTADO = 2;
  }
  
  // ------------------------------------- INICIO AGREGADO -----------------------------------------------
  else if (x == 3) { // Recibir distancias del arreglo
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
  // ------------------------------------- FIN AGREGADO -----------------------------------------------

}

void mensaje() {
  Serial.println("Hola");
}

void barrido() {
  for (int i = 0 ; i < 180 ; i += 10) {
    servomec.write(i);
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    DURACION = pulseIn(ECO, HIGH);

    DISTANCIA = (DURACION * 0.034) / 2;

    DISTANCIAC = (char)DISTANCIA;

    Serial.println(DISTANCIAC);

    //    Wire.write(53);
    delay(1000);
  }


  for (int i = 180 ; i >= 0 ; i -= 10) {
    servomec.write(i);
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    DURACION = pulseIn(ECO, HIGH);

    DISTANCIA = (DURACION * 0.034) / 2;
    DISTANCIAC = (char)DISTANCIA;
    Serial.println(DISTANCIAC );


    delay(1000);
  }
}

void Disparar() {
  Serial.println("HOLA");
}
