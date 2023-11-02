#include <TimerOne.h>
#include "DHT.h"

#define DHTTYPE DHT11

//ESTADOS ANTIRREBOTE
#define PRESS 0
#define CONFIRM 1
#define FREE 2

int mSegAntiR;
int boton;
int estadoAntirrebote = PRESS;
bool flag1 = 0;
bool flag2 = 0;

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

void ISR_TIMER (void) {
  mSeg++;
  mSegAntiR++;
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

  digitalWrite(latchPinA, LOW);
  shiftOut(dataPinA, clockPinA, LSBFIRST, 2);
  digitalWrite(latchPinA, HIGH);
  digitalWrite(latchPinB, LOW);
  shiftOut(dataPinB, clockPinB, LSBFIRST, 16);
  digitalWrite(latchPinB, HIGH);
}

void loop() {
  maquinaHumedad();
  Serial.println(digitalRead(BOT1));
  switch (estadoAntirrebote) {
    case PRESS:
      if (digitalRead(BOT1) == LOW) {
        boton = BOT1;
        estadoAntirrebote = CONFIRM;
        mSegAntiR = 0;
      }
      if (digitalRead(BOT2) == LOW) {
        boton = BOT2;
        estadoAntirrebote = CONFIRM;
        mSegAntiR = 0;
      }
      break;

    case CONFIRM:
      if (digitalRead(boton) == LOW && mSegAntiR >= 15) {
        estadoAntirrebote = FREE;
      }
      if (digitalRead(boton) == HIGH && mSegAntiR >= 15) {
        estadoAntirrebote = PRESS;
      }
      break;

    case FREE:
      if (boton == BOT1) {
        if (digitalRead(boton) == HIGH ) {
          flag1 = 1;
          estadoAntirrebote = PRESS;
        }
      }
      if (boton == BOT2) {
        if (digitalRead(boton) == HIGH) {
          flag2 = 1;
          estadoAntirrebote = PRESS;
        }
      }
      break;
  }
}

void maquinaHumedad() {
  switch (estadoHumedad) {
    case INIT:
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledRojo, HIGH);
      mSeg = 0;
      estadoHumedad = SETTING;
      break;

    case SETTING:
      if (flag1 == 1) {
        temperaturaUmbral = temperaturaUmbral + 1;
        flag1 = 0;
        mSeg = 0;
      }
      if (flag2 == 1) {
        temperaturaUmbral = temperaturaUmbral - 1;
        flag2 = 0;
        mSeg = 0;
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
      
        digitalWrite(BUZZER, LOW);
        
      if (mSeg >= 15000) {
        digitalWrite(ledRojo, LOW);
        digitalWrite(ledVerde, HIGH);
        estadoHumedad = PRINCIPAL;
      }
      break;

    case PRINCIPAL:
      temperatura = dht.readTemperature();

      if (flag1 == 1) {
        temperaturaUmbral = temperaturaUmbral + 1;
        mSeg = 0;
        digitalWrite(ledRojo, HIGH);
        digitalWrite(ledVerde, LOW);
        estadoHumedad = SETTING;
      }
      if (flag2 == 1) {
        temperaturaUmbral = temperaturaUmbral - 1;
        mSeg = 0;
        digitalWrite(ledRojo, HIGH);
        digitalWrite(ledVerde, LOW);
        estadoHumedad = SETTING;
      }
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

      if (temperatura < temperaturaUmbral) {
        digitalWrite(BUZZER, LOW);
      }
      if (temperatura > temperaturaUmbral) {
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
