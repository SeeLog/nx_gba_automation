#define A_PIN 10
#define B_PIN 16
#define SELECT_PIN 2
#define START_PIN 3
#define RIGHT_PIN 4
#define LEFT_PIN 5
#define UP_PIN 6
#define DOWN_PIN 7 
#define R_PIN 14
#define L_PIN 8

enum Button {
  A, B, SELECT, START, RIGHT, LEFT,
  UP, DOWN, L, R
};

typedef struct {
  int pinNum;
  Button button;
  int data;
} ButtonData;

typedef struct {
  int pinNumArray[];
  Button buttonArray[];
  int data;
  int len;
} HatData;

const int buttonListNum = 6;
ButtonData buttonList[] = {
  { A_PIN, A, 0x0004 },
  { B_PIN, B, 0x0002 },
  // Switch "-" -> GBA "Select"
  { SELECT_PIN, SELECT, 0x0100 },
  // Switch "+" -> GBA "Start"
  { START_PIN, START, 0x0200 },
  { L_PIN, L, 0x0010 },
  { R_PIN, R, 0x0020 },
};

const int bufSize = 11;
int timeoutCount = 0;
int receivedBuffer[bufSize];
int receivedNum = 0;

void clearBuffer() {
  for (int i = 0; i < bufSize; i++) {
    receivedBuffer[i] = 0;
  }
  receivedNum = 0;
}

void setup() {
  Serial.begin(9600);
  pinMode(A_PIN, OUTPUT);
  digitalWrite(A_PIN, HIGH);
  pinMode(B_PIN, OUTPUT);
  digitalWrite(B_PIN, HIGH);
  pinMode(SELECT_PIN, OUTPUT);
  digitalWrite(SELECT_PIN, HIGH);
  pinMode(START_PIN, OUTPUT);
  digitalWrite(START_PIN, HIGH);
  pinMode(RIGHT_PIN, OUTPUT);
  digitalWrite(RIGHT_PIN, HIGH);
  pinMode(LEFT_PIN, OUTPUT);
  digitalWrite(LEFT_PIN, HIGH);
  pinMode(UP_PIN, OUTPUT);
  digitalWrite(UP_PIN, HIGH);
  pinMode(DOWN_PIN, OUTPUT);
  digitalWrite(DOWN_PIN, HIGH);
  pinMode(R_PIN, OUTPUT);
  digitalWrite(R_PIN, HIGH);
  pinMode(L_PIN, OUTPUT);
  digitalWrite(L_PIN, HIGH);
}

void pressButton(ButtonData data) {
  digitalWrite(data.pinNum, LOW);
}

void releaseButton(ButtonData data) {
  digitalWrite(data.pinNum, HIGH);
}

void pressHat(int bData) {
  switch(bData) {
    case 0:
      digitalWrite(UP, LOW);
      digitalWrite(RIGHT, HIGH);
      digitalWrite(DOWN, HIGH);
      digitalWrite(LEFT, HIGH);
      break;
    case 1:
      digitalWrite(UP, LOW);
      digitalWrite(RIGHT, LOW);
      digitalWrite(DOWN, HIGH);
      digitalWrite(LEFT, HIGH);
      break;
    case 2:
      digitalWrite(UP, HIGH);
      digitalWrite(RIGHT, LOW);
      digitalWrite(DOWN, HIGH);
      digitalWrite(LEFT, HIGH);
      break;
    case 3:
      digitalWrite(UP, HIGH);
      digitalWrite(RIGHT, LOW);
      digitalWrite(DOWN, LOW);
      digitalWrite(LEFT, HIGH);
      break;
    case 4:
      digitalWrite(UP, HIGH);
      digitalWrite(RIGHT, HIGH);
      digitalWrite(DOWN, LOW);
      digitalWrite(LEFT, HIGH);
      break;
    case 5:
      digitalWrite(UP, HIGH);
      digitalWrite(RIGHT, HIGH);
      digitalWrite(DOWN, LOW);
      digitalWrite(LEFT, LOW);
      break;
    case 6:
      digitalWrite(UP, HIGH);
      digitalWrite(RIGHT, HIGH);
      digitalWrite(DOWN, HIGH);
      digitalWrite(LEFT, LOW);
      break;
    case 7:
      digitalWrite(UP, LOW);
      digitalWrite(RIGHT, HIGH);
      digitalWrite(DOWN, HIGH);
      digitalWrite(LEFT, LOW);
      break;
    case 8:
      digitalWrite(UP, HIGH);
      digitalWrite(RIGHT, HIGH);
      digitalWrite(DOWN, HIGH);
      digitalWrite(LEFT, HIGH);
      break;
  }
}

void loop() {
  while (Serial.available() > 0 && receivedNum < bufSize) {
    timeoutCount = 0;
    receivedBuffer[receivedNum++] = Serial.read();
  }

  if (timeoutCount > 1000) {
    timeoutCount = 0;
    clearBuffer();
  }

  if (receivedNum == bufSize) {
    // Start parse
    int dataType = receivedBuffer[0];

    if (dataType == 0xAB) {
      int btnData = receivedBuffer[1] | receivedBuffer[2] << 8;
      int hatData = receivedBuffer[3];
      for (int i = 0; i < buttonListNum; i++) {
        ButtonData data = buttonList[i];
        if ((data.data & btnData) > 0) {
          pressButton(data);
        } else {
          releaseButton(data);
        }
      }
      pressHat(hatData);
    }
    clearBuffer();
  }
}
