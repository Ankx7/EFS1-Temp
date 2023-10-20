//EL CAMBIO EN EL 7 SEG SE HACE CADA DOS NUMEROS EN INFO. Osea, si aparece una rayita en info = 0; en info = 2 se prenderan dos. En Info = 1, seguira habíando una sola rayita prendida.
/*  _______ 7 SEGMENTOS A _________
  Numero - info
  9 = 2
  8 =
  7 =
  6 =
  5 = 130
  4 =
  3 =
  2 = 40
  1 =
  0 = 16
*/
//7 seg 1
int latchPinA = 3;
int clockPinA = 2;
int dataPinA = 4;

/*  _______ 7 SEGMENTOS B _________
  Numero - info
  9 = 
  8 =
  7 =
  6 =
  5 = 
  4 =
  3 =
  2 = 
  1 =
  0 = 
*/
//7 seg 2
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

void loop() {
  if (Serial.available() > 0) {
    lectura = Serial.readString();
    Serial.println(lectura);
    info = lectura.toInt();
  }
  seg(2); // poner 2 para el seg B, 1 para el seg A
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
