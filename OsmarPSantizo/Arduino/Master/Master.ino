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
int btn_reset = 30;
bool btn_barrido_pres, btn_ataque_pres, btn_pausa_pres, btn_reset_pres;


int distancia[10];
int distancia2[10];
int grados[10];
bool iguales = true;
int nPos = 0;
int enemigosvivos = 0;
int enemigoseliminados = 0;
bool estaenpausa = false;
long unsigned tiempopausa ;



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

  while (true) {
    if ((millis() - tiempo_msj) > 10000) {

      break;
    }
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
  lcd.clear();
  contador_loop++;
  lcd.setCursor(0, 0);
  lcd.print("Entrando al loop");
  lcd.setCursor(0, 1);
  lcd.print("Contador: ");
  lcd.print(contador_loop);
  delay(3000);

}

void resetearParametros() {
  // LCD
  lcd.clear();
  lcd.setCursor(0, 0);


  //Reseteo de listas
  for (int i = 0; i < 10; i++) {
    distancia[i] = 0;
    distancia2[i] = 0;
    grados[i] = -1;
  }
  iguales = true;
  enemigosvivos = 0;
  enemigoseliminados=0;
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
      long unsigned tiempoataque = millis();
      enviarGrados();
      delay(3000);
      Wire.beginTransmission(2);
      Wire.write(4);
      Wire.endTransmission();
      enemigoseliminados =0;
      while (enemigosvivos > 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Vivos: " + String(enemigosvivos));
        lcd.setCursor(0, 1);
        lcd.print("Muertos: " + String(enemigoseliminados));
        delay(2000);
        enemigosvivos--;
        enemigoseliminados++;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Vivos: " + String(enemigosvivos));
        lcd.setCursor(0, 1);
        lcd.print("Muertos: " + String(enemigoseliminados));
        delay(2000);

      }
      tiempoataque = millis() - tiempoataque;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("T: " +String(tiempoataque/1000));
      for (int i = 0; i<10; i++){
        Serial.println(distancia[i]);
     
        Serial.println(distancia2[i]);
        Serial.println("----");
      }
      int prueba[10] = {22,22,22,22,22,22,22,22,22,22};
      mediana(distancia);
      moda(distancia);
      media(distancia);
      
      
      delay(5000);

      btn_ataque_pres = false;
    }
    // Instruccion reset
    if (!estaPresionado(btn_reset)) {
      btn_reset_pres = true;
    }
    if (estaPresionado(btn_reset) && btn_reset_pres) {

      btn_reset_pres = false;
      break;
    }

    // Si no tiene instrucciones
    if (!estaPresionado(btn_barrido) && !estaPresionado(btn_ataque) && !estaPresionado(btn_reset) && !estaPresionado(btn_pausa) ) {
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
    // Instruccion pausa
    if (!estaPresionado(btn_pausa)) {
      btn_pausa_pres = true;
    }
    if (estaPresionado(btn_pausa) && btn_pausa_pres) {
      estaenpausa = ! estaenpausa;
      btn_pausa_pres = false;
      tiempopausa = millis();
    }


    while (estaenpausa) {
      if ((millis() - tiempopausa) >= 5000) {
        estaenpausa = false;
        break;
      }
    }

    Wire.beginTransmission(2);
    Wire.write(1);
    Wire.endTransmission();
    Wire.requestFrom(2, 1);

    char c = Wire.read();
    if ((int)c < 25 ) {

      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("Detectado");
      lcd.setCursor(0, 1);
      lcd.print("D: " + String((int)c) + " A: " + contador);

    }
    Serial.println((int)c);
    distancia[contadorlista] = (int)c;
    if ((int)c < 25) {
      grados[contadorlista] = contador;
    }

    contadorlista++;
    delay(2000);
    lcd.clear();

    contador += 18;


  }
  contador = 0;
  contadorlista = 0;
  while (contador <= 180) {
    // Instruccion pausa
    if (!estaPresionado(btn_pausa)) {
      btn_pausa_pres = true;
    }
    if (estaPresionado(btn_pausa) && btn_pausa_pres) {
      estaenpausa = ! estaenpausa;
      btn_pausa_pres = false;
      tiempopausa = millis();
    }




    while (estaenpausa) {
      if ((millis() - tiempopausa) >= 5000) {
        estaenpausa = false;
        break;
      }
    }
    Wire.beginTransmission(2);
    Wire.write(1);
    Wire.endTransmission();
    Wire.requestFrom(2, 1);

    char c = Wire.read();
    if ((int)c < 25 ) {

      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("Detectado");
      lcd.setCursor(0, 1);
      lcd.print("D: " + String((int)c) + " A: " + contador);

    }

    Serial.println((int)c);
    distancia2[contadorlista] = (int)c;
    contadorlista++;
    delay(2000);
    lcd.clear();
    contador += 18;

  }

  if (compararPosiciones()) {
    Serial.println("Son iguales");
    ordenarLista();
  } else {
    Serial.println("No son iguales");
  }


}


// ------------------------------------- INICIO AGREGADO -----------------------------------------------
// Comparando si cada una de las posiciones son iguales
bool compararPosiciones() {
  for (int i = 0; i < 10; i++) {
    int resultado = abs(distancia2 [i ] - distancia [ i]);
    if (resultado > 3) {
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
void enviarGrados() {
  int enemigosvivos=0;
  Wire.beginTransmission(2); // inciando la transmision de datos
  Wire.write(3);
  Wire.write('o'); // o -> ok, las lista está correcta
  for (int i = 0; i < 10; i++) {
    if (grados[i] != -1) {
      Wire.write(grados[i]);  // Enviando el valor de la posicion
      enemigosvivos++;
    }
  }
  Wire.write(200); // terminando la transmision de datos
  Wire.endTransmission();
}

void mediana(int ma []){
int longitud=enemigoseliminados;
 int bandera=0;
 int numero=0;
    for(int i=longitud; i>0 && bandera==0;i--) {
        bandera=1;
        for(int y=0;y<i;y++) {
            if(ma[y]>ma[y+1]) {
                numero = ma[y];
               ma[y] = ma[y+1];
                ma[y+1]=numero;
                bandera=0;
            }
        }
    }
     if(longitud%2!=0) {
//        print.serial(ma[longitud/2]);
        lcd.setCursor(6, 0);
        lcd.print("Me: " +String(ma[longitud/2]));
        //printf("\nEL valor de la mediana es : %d",ma[longitud/2]);
    } else {
//        printf("\nEL valor 1 de la mediana es : %d",ma[longitud/2]);
//        printf("\nEL valor 2 de la mediana es : %d",ma[(longitud/2)-1]);
        lcd.setCursor(6, 0);
        lcd.print("Med: " +String((ma[longitud/2] + ma[(longitud/2)-1])/2));
//        print.serial(ma[longitud/2]);
//        print.serial(ma[(longitud/2)-1]);
    }
    
}

void media(int ma []){
     int suma = 0;
      int longitud=10;
    for(int i=0;i<longitud;i++) {
        suma+=ma[i];
    }
    float resultado=suma/longitud;
    lcd.setCursor(0, 1);
    lcd.print("X: " +String(resultado));
    //print.serial(resultado);

}

void moda(int ma [] ){
 int longitud=10;
 int auxiliar[longitud];
   // int ma[10] = {1,2,3,2,20,7,90,83,1,0};
    for(int i=0; i <longitud;i++) {
        auxiliar[i]=0;
    }

    int posicion;
    int numero;
    //leo las veces que se repite cada numero.
    for(int i=0;i<longitud;i++) {
        numero = ma[i];
        posicion = i;
        for(int y=i;y<longitud;y++) {
            if(ma[y]==numero) auxiliar[posicion]++;
        }
    }
// VEO QUIEN ES EL MAYOR
    int mayor=auxiliar[0];
    int posicionmayor = 0;
    for(int i=0;i<longitud;i++) {
        if(auxiliar[i]>mayor) {
            posicionmayor=i;
            mayor=auxiliar[i];
        }
        
    }
    // Visualizar el elemento con mas frecuencia de aparicion
   
    
   // print.serial(ma[posicionmayor]);
    lcd.setCursor(9, 1);
    lcd.print("Mo: " +String(ma[posicionmayor]));
    
    
    
}
