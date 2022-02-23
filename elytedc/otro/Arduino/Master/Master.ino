#include <LiquidCrystal.h>
#include <Wire.h>
#include <Servo.h>

int btns = A1;
int btnd= A4;
int btnp= A2;
int a=0;
bool scanea;
bool dispara;
bool pausa;
bool estado=false;
bool estado1=false;
bool aux=true;
bool inicio=true;

//Sensor
int TRIG = 52;
int ECO = 53;
int DURACION;
int DISTANCIA;
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
  while (inicio){
    String cadena = "";lcd.setCursor(0, 1); //Columna, Linea
  lcd.write(byte(0));
  lcd.setCursor(1, 1);
  lcd.print(cadena);
  cadena = "";
  Serial.println(cadena);
  lcd.print("GRUPO 04 ACE");
  lcd.createChar(1, cheque);
  lcd.setCursor(13, 1); //Columna, Linea
  lcd.write(byte(1));
  //I2C
  int cadena2;
  delay(500);
  inicio=false;
  }
   
while (aux){
   if(analogRead(btns)==0){
    //lcd.setCursor(1, 0);
    //lcd.print("prueba");
  }else{
     lcd.setCursor(1, 0);
    lcd.print("uu");
    estado=true;
    aux=false;
  
  }

  if(analogRead(btnp)==0){
    //lcd.setCursor(1, 0);
    //lcd.print("aaaaa");
  }else{
     lcd.setCursor(1, 0);
    lcd.print("disparar");
    estado1=true;
    aux=false;
  
  }

  }

if(estado==true){
    Wire.beginTransmission(2);
    Wire.write(0);
    Wire.endTransmission();
  }

  if(estado1==true){
    Wire.beginTransmission(2);
    Wire.write(1);
    Wire.endTransmission();
  }
  
  while (false){
    Wire.requestFrom(2,1);
    char c = Wire.read();
    
    Serial.println((int)c-32);
    lcd.setCursor(0, 1);
    lcd.print((int)c);
    delay(1000);
  }
  
  
}
