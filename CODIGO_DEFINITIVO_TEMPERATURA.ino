#include <TimerOne.h>
#include "DHT.h"

#define DHTTYPE DHT22

#define BUZZER 10
#define BOT1 6
#define BOT2 7
#define ledVerde 8
#define ledRojo 9
#define DHTpin 5

#define INIT 0
#define SETTING 1
#define PRINCIPAL 2

DHT dht(DHTpin, DHTTYPE);

int latchPinA = 3;
int clockPinA = 2;
int dataPinA = 4;
int latchPinB = 12;
int clockPinB = 11;
int dataPinB = 13;
byte info;
String lectura;

int estadoHumedad;
int temperaturaUmbral = 0;
int temperatura;
int numA;
int numB;
int dataA;
int dataB;

int mSeg;
boolean contar = false;

void ISR_TIMER() {
  if (contar == true) {
    mSeg = mSeg + 1;
  } else if (contar == false) {
    mSeg = 0;
  }
}

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(BUZZER, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(BOT1, INPUT_PULLUP);
  pinMode(BOT2, INPUT_PULLUP);

  pinMode(latchPinA, OUTPUT);
  pinMode(clockPinA, OUTPUT);
  pinMode(dataPinA, OUTPUT);
  pinMode(latchPinB, OUTPUT);
  pinMode(clockPinB, OUTPUT);
  pinMode(dataPinB, OUTPUT);

  Timer1.initialize(1000); //interrumpe cada un mili segundo
  Timer1.attachInterrupt(ISR_TIMER);
}

void loop() {
  maquinaHumedad();

  //lectura temperatura
}

void maquinaHumedad() {
  switch (estadoHumedad) {
    case INIT:
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledRojo, HIGH);
      estadoHumedad = SETTING;
      contar = true;
      break;

    case SETTING:
      if (digitalRead(BOT1) == LOW) {
        temperaturaUmbral = temperaturaUmbral + 1;
      }
      if (digitalRead(BOT2) == LOW) {
        temperaturaUmbral = temperaturaUmbral - 1;
      }
      
      //mostrar 7 segmentos
      numA = temperaturaUmbral / 10; //con esto sacamos la decena
      numB = temperaturaUmbral - (numA * 10); //con esto sacamos la unidad
      equivalencia();
      digitalWrite(latchPinA, LOW);
      shiftOut(dataPinA, clockPinA, LSBFIRST, dataA);
      digitalWrite(latchPinA, HIGH);
      digitalWrite(latchPinB, LOW);
      shiftOut(dataPinB, clockPinB, LSBFIRST, dataB);
      digitalWrite(latchPinB, HIGH);
      
      if (mSeg >= 15000) {
        digitalWrite(ledRojo, LOW);
        digitalWrite(ledVerde, HIGH);
        estadoHumedad = PRINCIPAL;
      }
      break;

    case PRINCIPAL:
      temperatura = dht.readTemperature();
      
      //mostrar 7 segmentos
      numA = temperatura / 10; //con esto sacamos la decena
      numB = temperatura - (numA * 10); //con esto sacamos la unidad
      equivalencia();
      digitalWrite(latchPinA, LOW);
      shiftOut(dataPinA, clockPinA, LSBFIRST, dataA);
      digitalWrite(latchPinA, HIGH);
      digitalWrite(latchPinB, LOW);
      shiftOut(dataPinB, clockPinB, LSBFIRST, dataB);
      digitalWrite(latchPinB, HIGH);

      if (temperatura < temperaturaUmbral){
        digitalWrite(BUZZER, LOW);
      }
      if (temperatura > temperaturaUmbral){
        digitalWrite(BUZZER, HIGH);
      }
      break;
  }
}
void equivalencia() {
  switch (numA) {
    case 9:
      dataA = 32;
      break;
    case 8:
      dataA = 0;
      break;
    case 7:
      dataA = 102;
      break;
    case 6:
      dataA = 16;
      break;
    case 5:
      dataA = 48;
      break;
    case 4:
      dataA = 104;
      break;
    case 3:
      dataA = 36;
      break;
    case 2:
      dataA = 132;
      break;
    case 1:
      dataA = 110;
      break;
    case 0:
      dataA = 2;
      break;
  }
  switch (numB) {
    case 9:
      dataB = 2;
      break;
    case 8:
      dataB = 0;
      break;
    case 7:
      dataB = 54;
      break;
    case 6:
      dataB = 128;
      break;
    case 5:
      dataB = 130;
      break;
    case 4:
      dataB = 70;
      break;
    case 3:
      dataB = 34;
      break;
    case 2:
      dataB = 40;
      break;
    case 1:
      dataB = 118;
      break;
    case 0:
      dataB = 16;
      break;
  }
}
