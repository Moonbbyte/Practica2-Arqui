#include <Wire.h>
#include <Servo.h>

int TRIG = 12;
int ECO = 11;
int ESTADO = 0;

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
}
