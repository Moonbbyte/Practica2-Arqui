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
DURACION = 1000;
DISTANCIA = (DURACION *0.034)/2;
//Serial.println(byte(DISTANCIA));
if(DISTANCIA > 0 && DISTANCIA < 100){
  int unidad, decena;
  unidad = DISTANCIA % 10;
  decena = (DISTANCIA - unidad) / 10;
  //centena = (DISTANCIA - unidad - decena) / 100;
  char distancia_bt[2]={
    //centena + '0',
    decena + '0',
    unidad + '0' 
  };
  Wire.write(distancia_bt);
}
//
//char distancia_bt[tem.length()];
//for(int i = 0; i < tem.length(); i++){
//  distancia_bt[i] = ( + '0');
//}
//char letras[] = {1 + '0', 9 + '0'};
//char letras[2] = {B00110010,B00110000};



  
        
}
