#include <LiquidCrystal.h>
#include <Wire.h>

// LCD
int rs = 47, rw = 45, en = 43, d4 = 29, d5 = 27, d6 = 25, d7 = 23;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //Rs, en ,D3,D2,D1,D0
// Mensaje
long unsigned tiempo_msj;

void setup() {
  Serial.begin(9600);
  // LCD
  for (int i = 23; i < 30; i++) pinMode(i, OUTPUT);
  for (int i = 45; i < 48; i++) pinMode(i, OUTPUT);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  // I2C
  Wire.begin();
  Wire.beginTransmission(2);
  Wire.write(0);
  Wire.endTransmission();
  Wire.requestFrom(2, 1);
  char c = Wire.read();
  if ((int)c == 1) {
    mostrarMensaje();
    tiempo_msj = millis();
  }
}

void loop() {
  if ((millis() - tiempo_msj) > 1000) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Esperando");
    lcd.setCursor(0, 1);
    lcd.print("instruccion...");
    delay(1000);
  }
}

void mostrarMensaje() {
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
  lcd.createChar(0, cara);
  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  lcd.setCursor(1, 0);
  lcd.print("GRUPO 04 ACE");
  lcd.createChar(1, cheque);
  lcd.setCursor(13, 0);
  lcd.write(byte(1));
}
