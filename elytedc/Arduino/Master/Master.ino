#include <LiquidCrystal.h>
#include <Wire.h>
#include <Servo.h>


//Sensor
int TRIG = 52;
int ECO = 53;
int DURACION;
int DISTANCIA;
bool valor = true;
int a[] = {1, 2,3};

byte cara[] = {
  B00000,
  B00000,
  B01010,
  B10101,
  B00000,
  B10001,
  B01110,
  B00000
};
byte cheque[] = {
  B00000,
  B00000,
  B00000,
  B00001,
  B10010,
  B01100,
  B01000,
  B00000
};




//servo
Servo servomec;
//LCS
int rs = 47, rw = 45, en = 43, d4 = 29, d5 = 27, d6 = 25, d7 = 23;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //Rs, en ,D3,D2,D1,D0



void setup()
{
  pinMode(TRIG, OUTPUT);
  pinMode(ECO, INPUT);

  Wire.begin();        // Conexión al Bus I2C
  for (int i = 23; i < 30; i++) pinMode(i, OUTPUT);
  for (int i = 45; i < 48; i++) pinMode(i, OUTPUT);
  lcd.begin(16, 2); //Columnas , Lineas
  Serial.begin(9600);  // Velocidad de conexión
  servomec.attach(32);//Pin
  lcd.createChar(0, cara);
}

void loop()
{
  String cadena = "";
  lcd.setCursor(0, 0); //Columna, Linea
  lcd.print("Probando I2C");
  //Servo
  servomec.write(0);
  //delay(100);
  //  servomec.write(90);
  //  delay(2000);
  //  servomec.write(180);
  //  delay(2000);


  //I2C

  while (Wire.available())   // slave may send less than requested
  {
    char c = Wire.read();

    Serial.print(c);
    cadena += c;
  }
 



  if (valor == true)
  { valor = false;

    Wire.beginTransmission(2);
    Wire.write(4);
    Wire.endTransmission();
    delay(50);
  }

  delay(10);
  Wire.beginTransmission(2);
  //Wire.write(1);
  Wire.endTransmission();

  //Sensor
  //  digitalWrite(TRIG,HIGH);
  //  delay(1);
  //  digitalWrite(TRIG,LOW);
  //  DURACION = pulseIn(ECO,HIGH);
  //  DISTANCIA = DURACION /58.2;
  //
  //  lcd.clear();
  //  lcd.setCursor(0,0); //Columna, Linea
  //  lcd.print(DISTANCIA );
  //


  // Sensor 2
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
  //Serial.println(DISTANCIA);
  //lcd.print("Distancia: ");
  //lcd.print(DISTANCIA);
  //lcd.print(" cm");

  delay(100);
}
