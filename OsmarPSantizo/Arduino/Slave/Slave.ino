#include <Wire.h>
#include <Servo.h>
int TRIG = 12;
int ECO = 11;
int DURACION;
int DISTANCIA;
int ESTADO = 0;
int LASER = 9;
char DISTANCIAC;
int enemigos[] = {10,100, 20,30,90};
int grados[10];
int contador = 0;
//servo
Servo servomec;
void setup()
{
  Wire.begin(2);                // Este Esclavo es el número 2

  Wire.onReceive(requestEvent);
  Wire.onRequest(datos); // Cuando el Maestro le hace una petición,
  pinMode(TRIG, OUTPUT);
  pinMode(ECO, INPUT);                            // realiza el requestEvent
  Serial.begin(9600);
  servomec.attach(8);//Pin
  pinMode(LASER,OUTPUT);
  digitalWrite(LASER,LOW);
}

void loop()
{
  if(ESTADO == 4){
    
    Disparar();
  }
  delay(500);

}

void datos() {
  
  if(ESTADO == 0){
    Wire.write(1);
  }else if(ESTADO == 1){
    barrido(); 
  }
  
}

// Esto es lo que envía cuando le hace la petición.
void requestEvent()
{

  int x = Wire.read();    // receive byte as an integer
  if(x==0){
    ESTADO = 0;
  }
  else if (x == 1 ) {
    ESTADO = 1;
    if(contador >180){
      contador =0;
    }
    
  } else if (x == 2) {
    ESTADO = 2;
  }
  else if (x == 3) {
    char ok = Wire.read();

    if (ok == 'o') { // Los arreglos son iguales
      int pos = 0;
      x = Wire.read();
      while (x != 200) { // Mientras no se envie un 100, seguir leyendo lo que envia el maestro
        grados[pos] = x; // Guardando lo enviado por el maestro
        pos++; // incrementando la posicion del arreglo
        x = Wire.read(); // Leyendo lo siguiente que envio el Maestro
      }
 Serial.println("Distancias obtenidas en el Esclavo: ");
      for (int i = 0; i < 10; i++) {
      Serial.print(grados[i]);
        //servomec.write(distancias[i]);
        delay(1000);
        Serial.print(", ");
      }
    }
    else {
//      Serial.println("Hay distancias diferentes");
    }
  }
  else if(x == 4){
//    Serial.println("requestEvent");
    ESTADO = 4;
  }

}



void barrido() {
  
    servomec.write(contador);

    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    DURACION = pulseIn(ECO, HIGH);

    DISTANCIA = (DURACION * 0.034) / 2;

    DISTANCIAC = (char)DISTANCIA;

   Wire.write(DISTANCIAC);
    
    delay(1000);
    
    
//  for (int i = 0 ; i < 180 ; i += 18) {
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
//    Serial.println(DISTANCIA);
//
//    Wire.write(DISTANCIAC);
//    delay(1000);
//  }
//  for (int i = 0 ; i < 180 ; i += 18) {
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
//    Serial.println(DISTANCIA);
//    Wire.write(DISTANCIAC);
//    delay(1000);
//  }

  contador+=18;
  
  
   ESTADO = 0;  
  
}

void Disparar() {
//      Serial.println("Metodo disparar");
      for (int i = 0; i <10 ; i++) {
        servomec.write(grados[i]);
        delay(1000);
        digitalWrite(LASER,HIGH);
        delay(1000);
        digitalWrite(LASER,LOW);
        delay(2000);
      }
      ESTADO = 0;
}
