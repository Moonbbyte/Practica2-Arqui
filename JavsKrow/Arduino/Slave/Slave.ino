#include <Wire.h>
int TRIG = 12;
int ECO = 11;
int DURACION;
int DISTANCIA;
void setup()
{
   Wire.begin(2);                // Este Esclavo es el número 2
  Wire.onRequest(requestEvent); // Cuando el Maestro le hace una petición,
     pinMode(TRIG,OUTPUT);
  pinMode(ECO, INPUT);                            // realiza el requestEvent
}

void loop()
{
  delay(500);
}

// Esto es lo que envía cuando le hace la petición.
void requestEvent()
{
//lcd.setCursor(0,0);
//digitalWrite(TRIG,LOW);
//delayMicroseconds(2);
//digitalWrite(TRIG,HIGH);
//delayMicroseconds(10);
//digitalWrite(TRIG,LOW);
//DURACION = pulseIn(ECO,HIGH);
//
//DISTANCIA = (DURACION *0.034)/2;
//
//S
//lcd.print("Distancia: ");
//Wire.write(DISTANCIA);
//lcd.print(" cm");
  
Wire.write("Distancia: "); // Envía 10 byte              
}
