//EL CAMBIO EN EL 7 SEG SE HACE CADA DOS NUMEROS EN INFO. Osea, si aparece una rayita en info = 0; en info = 2 se prenderan dos. En Info = 1, seguira habíando una sola rayita prendida.
/*  _______ 7 SEGMENTOS B _________
  Numero - info
  9 = 2
  8 = 0
  7 = 54
  6 = 128
  5 = 130
  4 = 70
  3 = 34
  2 = 40
  1 = 118
  0 = 16
*/

int latchPinA = 3;
int clockPinA = 2;
int dataPinA = 4;

/*  _______ 7 SEGMENTOS A _________
  Numero - info
  9 = 32
  8 = 0
  7 = 102
  6 = 16
  5 = 48
  4 = 104
  3 = 36
  2 = 132
  1 = 110
  0 = 2
*/

int latchPinB = 12;
int clockPinB = 11;
int dataPinB = 13;


byte info;
String lectura;

void setup() {
  //7 SEG 1
  pinMode(latchPinA, OUTPUT);
  pinMode(clockPinA, OUTPUT);
  pinMode(dataPinA, OUTPUT);

  //7 SEG 2
  pinMode(latchPinB, OUTPUT);
  pinMode(clockPinB, OUTPUT);
  pinMode(dataPinB, OUTPUT);

  Serial.begin(9600);
}
int y = 1;

int modo = 1;

void loop() {
  if (modo == 1){
    MANUAL();
  } else if (modo == 2){
    AUTO();
  }
}

void seg (int segmentos) {
  if (segmentos == 1) {
    digitalWrite(latchPinA, LOW);
    shiftOut(dataPinA, clockPinA, LSBFIRST, info); // 0 es todo prendido - A partir de 253 se prende
    //return the latch pin high to signal chip that it
    //no longer needs to listen for information
    digitalWrite(latchPinA, HIGH);
  } else {
    digitalWrite(latchPinB, LOW);
    shiftOut(dataPinB, clockPinB, LSBFIRST, info); // 0 es todo prendido - A partir de 253 se prende
    //return the latch pin high to signal chip that it
    //no longer needs to listen for information
    digitalWrite(latchPinB, HIGH);
  }
}

void MANUAL() {
  if (Serial.available() > 0) {
    lectura = Serial.readString();
    Serial.println(lectura);
    info = lectura.toInt();
  }

  seg(1); // poner 2 para el seg B, 1 para el seg A
}

void AUTO() {
  for (int x = 0; x < 254; x = x + 2) {
    info = x;
    seg(1);
    Serial.print("turno número ");
    Serial.print(y);
    Serial.println(": ");
    Serial.println(x);
    delay(1000);
    y = y + 1;
  }
}
