#include <LiquidCrystal.h>
#include <Wire.h>
#include <Servo.h>


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

// Distancias de los enemigos ---------------------------------------------------------
int distancias1[18] = {3, 1, 9, 3, 4, 5, 6, 7, 8, 9, 88, 11, 12, 13, 101, 15, 16, 200};
int distancias2[18] = {3, 1, 9, 3, 4, 5, 6, 7, 8, 9, 88, 11, 12, 13, 101, 15, 16, 200};
bool iguales = true; // Si los arreglos de las distancias son iguales
int nPos = 0; // Contador del numero de posiciones enviadas


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
  String cadena = ""; lcd.setCursor(0, 1); //Columna, Linea
  lcd.write(byte(0));
  lcd.setCursor(1, 1);
  lcd.print(cadena);
  cadena = "";
  Serial.println(cadena);
  lcd.print("GRUPO 04 ACE");
  lcd.createChar(1, cheque);
  lcd.setCursor(13, 1); //Columna, Linea
  lcd.write(byte(1));
  Wire.beginTransmission(2);
  Wire.write(1);
  Wire.endTransmission();



  //I2C
  int cadena2;


  while (true) {

    //    Wire.requestFrom(2,1);
    //    char c = Wire.read();

    //      int pos = (int)c-32; // posicion del enemigo

    //    Serial.println((int)c-32);
    //    lcd.setCursor(0, 1);
    //    lcd.print((int)c);
    //    delay(1000);


    // ------------------------------------- INICIO AGREGADO -----------------------------------------------
    // ------------------------------------- Descomentar despues -----------------------------------------------

    // Hay que recibir constantemente los datos
    //    if (nPos < 18) { // Llenando la lista 1
    //      distancias1[nPos] = pos;
    //    }
    //    else if (nPos < 36) { // Llenando la lista 2
    //      distancias2[nPos - 18] = pos;
    //    }
    //    else {
    
            // Cuando ya se hayan enviado las dos listas, entrará aquí
            bool c = compararPosiciones(); // Comparando cada una de las posiciones de los dos arreglos para ver si son iguales
            if (c) {  // Son iguales las distancias
              ordenarLista();
              enviarDistancias();
            } else { // Las distancias son diferentes
              Wire.beginTransmission(2); // inciando la transmision de datos
              Wire.write(3);
              Wire.write('n'); // NO son iguales las distancias
              Wire.endTransmission();
            }
            break;
    //}

    // ------------------------------------- Fin Descomentar -----------------------------------------------
    // ------------------------------------- FIN AGREGADO -----------------------------------------------

    delay(500);

  }

  //  while (Wire.available()>0)   // slave may send less than requested
  //  {
  //    char c = Wire.read();
  //    cadena2 = String(c).toInt();
  //    Serial.println(cadena2);
  //
  //    cadena += c;
  //  }
  //   //Columna, Linea
  //
  //  lcd.setCursor(1, 1);
  //  lcd.print(cadena);
  //

  delay(500);
}


// ------------------------------------- INICIO AGREGADO -----------------------------------------------
// Comparando si cada una de las posiciones son iguales
bool compararPosiciones() {
  for (int i = 0; i < 18; i++) {
    if (!(distancias1[i] == distancias2[i])) {
      iguales = false;
      break;
    }
  }
  return iguales;
}

// Metodo de Burbuja para ordenar el arreglo
void ordenarLista() {
  int temp = 0;
  for (int i = 1; i < 18; i++) {
    for (int j = 18 - 1; j >= i; j--) {
      if (distancias1[j - 1] > distancias1[j]) {
        temp = distancias1[j - 1];
        distancias1[j - 1] = distancias1[j];
        distancias1[j] = temp;
      }
    }
  }
}

// Envia cada una de las posiciones del arreglo al esclavo
void enviarDistancias() {
  Wire.beginTransmission(2); // inciando la transmision de datos
  Wire.write(3);
  Wire.write('o'); // o -> ok, las lista está correcta
  for (int i = 0; i < 18; i++) {
    Wire.write(distancias1[i]);  // Enviando el valor de la posicion
  }
  Wire.write(100); // terminando la transmision de datos
  Wire.endTransmission();
}

// ------------------------------------- FIN AGREGADO -----------------------------------------------
