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
  Serial.begin(9600);
}

void loop()
{
  delay(500);
}

// Esto es lo que envía cuando le hace la petición.
void requestEvent()
{

//digitalWrite(TRIG,LOW);
//delayMicroseconds(2);
//digitalWrite(TRIG,HIGH);
//delayMicroseconds(10);
//digitalWrite(TRIG,LOW);
//DURACION = pulseIn(ECO,HIGH);
//
//DISTANCIA = (DURACION *0.034)/2;
//Serial.println(byte(DISTANCIA));



char letras[13]= {B01000111,B01010010,B01010101,B01010000,B01001111,B00100000,B00110000,B00110100,B00100000,B01000001,B01000011,B01000101,B00110001};
char cheque[3]={1,2,3};
//char letras[3] = {B00110010,B00110000,B00100000};
                    

Wire.write(letras);


  
        
}
