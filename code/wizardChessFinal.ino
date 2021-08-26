/*
  Created By: Divyank Shah
  15th March 2021
  
  Wizard Chess Final Program
  
  The following program is the final program implemented allowing the Arduino to parse incoming bluetooth data and be able to translate that information to     instructions that the Arduino can use to move the stepper motors.
  
*/

#include "Servo.h"
Servo myservo;
int pos = 0;

String voice, pieceType;
int val[8] = {0, 208, 416, 624, 832, 1040, 1248, 1456};
int x1, y1, x2, y2;

const int stepPinX = 2;
const int stepPinY = 3;
const int stepPinZ = 4;
const int dirPinX = 5;
const int dirPinY = 6;
const int dirPinZ = 7;
bool knightDir;

void servoUp() {
  myservo.write(100);
  delay(500);
}

void servoDown() {
  myservo.write(135);
  delay(500);
}

void stepMotorX(int xMotor, int zMotor, int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(xMotor, HIGH);
    delayMicroseconds(1000);
    digitalWrite(xMotor, LOW);
    delayMicroseconds(1000);
    digitalWrite(zMotor, HIGH);
    delayMicroseconds(1000);
    digitalWrite(zMotor, LOW);
    delayMicroseconds(1000);
  }
}

void linear(int xMotor, int yMotor, int zMotor, int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(xMotor, HIGH);
    delayMicroseconds(1000);
    digitalWrite(xMotor, LOW);
    delayMicroseconds(1000);
    digitalWrite(yMotor, HIGH);
    delayMicroseconds(1000);
    digitalWrite(yMotor, LOW);
    delayMicroseconds(1000);
    digitalWrite(zMotor, HIGH);
    delayMicroseconds(1000);
    digitalWrite(zMotor, LOW);
    delayMicroseconds(1000);
  }
}

void stepMotorY(int yMotor, int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(yMotor, HIGH);
    delayMicroseconds(1000);
    digitalWrite(yMotor, LOW);
    delayMicroseconds(1000);
  }
}

void xOneStep() {
  digitalWrite(stepPinX, HIGH);
  delayMicroseconds(1000);
  digitalWrite(stepPinX, LOW);
  delayMicroseconds(1000);
  digitalWrite(stepPinZ, HIGH);
  delayMicroseconds(1000);
  digitalWrite(stepPinZ, LOW);
  delayMicroseconds(1000);
}

void yOneStep() {
  digitalWrite(stepPinY, HIGH);
  delayMicroseconds(1000);
  digitalWrite(stepPinY, LOW);
  delayMicroseconds(1000);
}

void knightMove(bool knightDir) {
  if (knightDir) {
    for (int i = 0; i < y2 - y1; i++) {
      xOneStep();
      xOneStep();
      yOneStep();
    }
    } else if (!knightDir) {
      for (int i = 0; i < x2 - x1; i++) {
        xOneStep();
        yOneStep();
        yOneStep();
      }
    }
  }

  void moveXYSteppers(int pathType) {
    Serial.println("LESSGO BABY");
    if (x2 - x1 > 0) {
      digitalWrite(dirPinX, LOW);
      digitalWrite(dirPinZ, LOW);
    } else if (x2 - x1 < 0) {
      digitalWrite(dirPinX, HIGH);
      digitalWrite(dirPinZ, HIGH);
    } else if (y2 - y1 > 0) digitalWrite(dirPinY, LOW);
    else if (y2 - y1 < 0) digitalWrite(dirPinY, HIGH);
    else return;

    Serial.println("moving to position");
    stepMotorX(stepPinX, stepPinZ, x1);
    stepMotorY(stepPinY, y1);
    servoUp();
    Serial.println("at x1 position");
    if (pathType == 0) {
      stepMotorX(stepPinX, stepPinZ, x2 - x1);
      stepMotorY(stepPinY, y2 - y1);
    } else if (pathType == 1) linear(stepPinX, stepPinZ, stepPinY, x2 - x1);
    else if (pathType == 2) knightMove(knightDir);

    Serial.println("moving servo down");
    servoDown();
    digitalWrite(dirPinX, HIGH);
    digitalWrite(dirPinZ, HIGH);
    digitalWrite(dirPinY, HIGH);
    stepMotorX(stepPinX, stepPinZ, x2);
    stepMotorY(stepPinY, y2);
      
    Serial.println("Move Complete");

  }

  int convertToXStepValues(String value) {
    Serial.println("doing num");
    if (value == "1" || value == "ONE") return val[0];
    else if (value == "2" || value == "TWO" || value == "TO") return val[1];
    else if (value == "3" || value == "THREE") return val[2];
    else if (value == "4" || value == "FOUR") return val[3];
    else if (value == "5" || value == "FIVE" || value == "V") return val[4];
    else if (value == "6" || value == "SIX") return val[5];
    else if (value == "7" || value == "SEVEN") return val[6];
    else if (value == "8" || value == "EIGHT") return val[7];
    else return -1;
  }

  int convertToYStepValues(String value) {
    value.toUpperCase();
    Serial.println("doing word");
    if (value == "AZKABAN") return val[0];
    else if (value == "BUTTERBEER") return val[1];
    else if (value == "CHARM") return val[2];
    else if (value == "DEMENTOR") return val[3];
    else if (value == "ENCHANTMENT") return val[4];
    else if (value == "FIREBOLT") return val[5];
    else if (value == "GRYFFINDOR") return val[6]; 
    else if (value == "HUFFLEPUFF") return val[7];
    else return -1;
  }

  int parseData(String data) {

    int moveIndex = data.indexOf(' ');
    int pieceIndex = data.indexOf(' ', moveIndex + 1);
    int prepositionIndex_1 = data.indexOf(' ', pieceIndex + 1);
    int yIndex_1 = data.indexOf(' ', prepositionIndex_1 + 1);
    int xIndex_1 = data.indexOf(' ', xIndex_1 + 1);
    int prepositionIndex_2 = data.indexOf(' ', yIndex_1 + 1);
    int yIndex_2 = data.indexOf(' ', prepositionIndex_2 + 1);
    int xIndex_2 = data.indexOf(' ', xIndex_2 + 1);

    String moveType = data.substring(0, moveIndex);
    pieceType = data.substring(moveIndex + 1, pieceIndex);
    String preposition_1 = data.substring(pieceIndex + 1, prepositionIndex_1);
    String y1String = data.substring(prepositionIndex_1 + 1, xIndex_1);
    String x1String = data.substring(xIndex_1 + 1, yIndex_1);
    String preposition_2 = data.substring(yIndex_1 + 1, prepositionIndex_2);
    String y2String = data.substring(prepositionIndex_2 + 1, xIndex_2);
    String x2String = data.substring(xIndex_2 + 1, yIndex_2);

    moveType.trim();
    pieceType.trim();
    preposition_1.trim();
    y1String.trim();
    x1String.trim();
    preposition_2.trim();
    y2String.trim();
    x2String.trim();
    
    y1 = convertToXStepValues(y1String);
    x1 = convertToYStepValues(x1String);
    y2 = convertToXStepValues(y2String);
    x2 = convertToYStepValues(x2String);
    if (x1 == -1 || y1 == -1 || x2 == -1 || y2 == -1) {
      Serial.print("invalid number"); return -1;
    }

    preposition_1.toLowerCase();
    preposition_2.toLowerCase();

    if (preposition_1 == "to" && preposition_2 == "from") {
      int temp;
      temp = x1;
      x1 = x2;
      x2 = temp;

      temp = y1;
      y1 = y2;
      y2 = temp;
    } else if (!(preposition_1 == "from" && preposition_2 == "to")) {
      return -1;
    }
    pieceValidation(pieceType);

  }

  void pawn() {
    if ((abs(y2 - y1) == val[1] && x2 - x1 == 0) || (abs(y2 - y1) == val[2] && x2 - x1 == 0 && (y1 == 100 || y1 == 600)))
      moveXYSteppers(0);
    else {
      Serial.print("Illegal move");
      return;
    }
  }

  void rook() {
    if ((x2 - x1 == 0 && y2 - y1 != 0) || (x2 - x1 != 0 && y2 - y1 == 0)) moveXYSteppers(0);
    else {
      Serial.print("Illegal move");
      return;
    }
  }

  void bishop() {
    if (abs(x2 - x1) == abs(y2 - y1)) moveXYSteppers(1);
    else {
      Serial.print("Illegal Move");
      return;
    }
  }

  void queen() {
    if ((x2 - x1 == 0 && y2 - y1 != 0) || (x2 - x1 != 0 && y2 - y1 == 0))
      moveXYSteppers(0);
    else if (abs(x2 - x1) == abs(y2 - y1))
      moveXYSteppers(1);
    else {
      Serial.print("Illegal Move");
      return;
    }
  }

  void king() {
    if ((abs(x2 - x1) == val[1] && abs(y2 - y1) == 0) || (abs(x2 - x1) == 0 && abs(y2 - y1) == val[1]))
      moveXYSteppers(0);
    else if (abs(x2 - x1) == abs(y2 - y1))
      moveXYSteppers(1);
    else {
      Serial.print("Illegal Move");
      return;
    }
  }

  void knight() {
    if (abs(x2 - x1) == 2 * abs(y2 - y1)) {
      moveXYSteppers(2);
      knightDir = false;
    } else if (2 * abs(x2 - x1) == abs(y2 - y1)) {
      moveXYSteppers(2);
      knightDir = true;
    } else {
      Serial.print("Illegal Move");
      return;
    }
  }

  void pieceValidation(String piece) {
    piece.toUpperCase();
    if (piece == "PAWN" || piece == "PAIN") pawn();
    else if (piece == "ROOK" || piece == "BROOKE") rook();
    else if (piece == "BISHOP") bishop();
    else if (piece == "KNIGHT") knight();
    else if (piece == "KING") king();
    else if (piece == "QUEEN") queen();
    else {
      Serial.print("Invalid Piece");
      return;
    }
  }

  void setup() {
    Serial.begin(9600);

    pinMode(stepPinX, OUTPUT);
    pinMode(stepPinY, OUTPUT);
    pinMode(stepPinZ, OUTPUT);
    pinMode(dirPinX, OUTPUT);
    pinMode(dirPinY, OUTPUT);
    pinMode(dirPinZ, OUTPUT);
    myservo.attach(1);
    pinMode(9, OUTPUT);
    servoDown();

    pinMode(8, OUTPUT);
    digitalWrite(8, LOW);
  }

  void loop() {

    while (Serial.available()) {
      delay(10);
      char c = Serial.read();
      if (c == '#') break;
      voice += c;
    }

    if (voice.length() > 0) {

      Serial.println(voice);
      parseData(voice);

    }
    voice = "";
  }
