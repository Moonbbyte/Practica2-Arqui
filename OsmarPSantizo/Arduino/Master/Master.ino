#include <LiquidCrystal.h>
#include <Wire.h>

// LCD
int rs = 47, rw = 45, en = 43, d4 = 29, d5 = 27, d6 = 25, d7 = 23;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //Rs, en ,D3,D2,D1,D0
// Mensaje
long unsigned tiempo_msj;
// Variables
int contador_loop = 0;
int btn_barrido = 24;
int btn_ataque = 26;
int btn_pausa = 28;
bool btn_barrido_pres, btn_ataque_pres, btn_pausa_pres;


int distancia[10];
int distancia2[10];
int grados[10];
bool iguales = true;
int nPos = 0;

void setup() {
  Serial.begin(9600);
  // LCD
  for (int i = 23; i < 30; i++) pinMode(i, OUTPUT);
  for (int i = 45; i < 48; i++) pinMode(i, OUTPUT);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  // I2C - Mensaje Inicial (respuesta esclavo)
  Wire.begin();
  Wire.beginTransmission(2);
  Wire.write(0);
  Wire.endTransmission();
  Wire.requestFrom(2, 1);
  char c = Wire.read();
  if ((int)c == 1) {
    mensajeInicial();
    tiempo_msj = millis();
  }
}

void loop() {
  mensajeReinicioLoop();
  resetearParametros();
  esperarInstrucciones();
}

void mensajeInicial() {
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

void mensajeReinicioLoop() {
  while (true) {
    if ((millis() - tiempo_msj) > 10000) {
      contador_loop++;
      lcd.setCursor(0, 0);
      lcd.print("Entrando al Loop");
      lcd.setCursor(0, 1);
      lcd.print("Contador: ");
      lcd.print(contador_loop);
      delay(1000);
      break;
    }
  }
}

void resetearParametros() {
  // LCD
  lcd.clear();
  lcd.setCursor(0, 0);
}

void esperarInstrucciones() {
  while (true) {
    // Instruccion barrido
    if (!estaPresionado(btn_barrido)) {

      btn_barrido_pres = true;
    }
    if (estaPresionado(btn_barrido) && btn_barrido_pres) {
      recibirDistancias();

      btn_barrido_pres = false;
    }
    // Instruccion ataque
    if (!estaPresionado(btn_ataque)) {
      btn_ataque_pres = true;
    }
    if (estaPresionado(btn_ataque) && btn_ataque_pres) {
      Serial.println("PRESIONASTE");
      Wire.beginTransmission(2);
      Wire.write(4);
      Wire.endTransmission();
      btn_ataque_pres = false;
    }
    // Instruccion pausa
    if (!estaPresionado(btn_pausa)) {
      btn_pausa_pres = true;
    }
    if (estaPresionado(btn_pausa) && btn_pausa_pres) {
      Wire.beginTransmission(2);
      Wire.write(4);
      Wire.endTransmission();
      btn_pausa_pres = false;
    }
    // Si no tiene instrucciones
    if (!estaPresionado(btn_barrido) && !estaPresionado(btn_ataque) && !estaPresionado(btn_pausa)) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Esperando");
      lcd.setCursor(0, 1);
      lcd.print("instruccion...");
    }
    delay(1000);
  }
}

bool estaPresionado(int idBtn) {
  int estadoBtn = digitalRead(idBtn);
  return (estadoBtn == HIGH);
}

void recibirDistancias() {
  int contador = 0;
  int contadorlista = 0;
  while (contador <= 180) {
    Wire.beginTransmission(2);
    Wire.write(1);
    Wire.endTransmission();
    Wire.requestFrom(2, 1);

    char c = Wire.read();
    //    Serial.println((int)c);
    distancia[contadorlista] = (int)c;
    grados[contadorlista] = contador;
    contadorlista++;
    delay(2000);
    contador += 18;


  }
  contador = 0;
  contadorlista = 0;
  while (contador <= 180) {
    Wire.beginTransmission(2);
    Wire.write(1);
    Wire.endTransmission();
    Wire.requestFrom(2, 1);

    char c = Wire.read();
    //    Serial.println((int)c);
    distancia2[contadorlista] = (int)c;
    contadorlista++;
    delay(2000);
    contador += 18;

  }


  for (int i = 0 ; i < 10; i++) {
    Serial.println(distancia[i]);
  }
  Serial.println("--------");

  for (int i = 0 ; i < 10; i++) {
    Serial.println(distancia2[i]);
  }
  if (compararPosiciones()) {
    Serial.println("Son iguales");
    ordenarLista();
  } else {
    Serial.println("No son iguales");
  }
  for (int i = 0 ; i < 10; i++) {
    Serial.println(distancia[i]);
  }
  
  enviarDistancias();
}

// ------------------------------------- INICIO AGREGADO -----------------------------------------------
// Comparando si cada una de las posiciones son iguales
bool compararPosiciones() {
  for (int i = 0; i < 10; i++) {
    int resultado= abs(distancia2 [i ]-distancia [ i]);
    if (resultado > 2){
      return false;
    }
  }
  return true;
}



// Metodo de Burbuja para ordenar el arreglo
void ordenarLista() {
  int temp = 0;
  int temp2 = 0;
  for (int i = 1; i < 10; i++) {
    for (int j = 10 - 1; j >= i; j--) {
      if (distancia[j - 1] > distancia[j]) {
        temp = distancia[j - 1];
        distancia[j - 1] = distancia[j];
        distancia[j] = temp;
        temp2 = grados[j - 1];
        grados[j - 1] = grados[j];
        grados[j] = temp2;
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
    Wire.write(grados[i]);  // Enviando el valor de la posicion
  }
  Wire.write(100); // terminando la transmision de datos
  Wire.endTransmission();
}
