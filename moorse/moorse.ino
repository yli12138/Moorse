#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int cursorIndex = 0;

boolean clearedBuffer = true;

const unsigned long unit = 150;
const unsigned long space = 2000;
const unsigned long deleteTime = 2000;
const int flexThreshold = 750;
const int deleteThreshold = 880;

boolean flexPressing = false;
boolean flexDeleting = false;

unsigned long millisNow = 0;
unsigned long millisPause = 0;
unsigned long millisDelete = 0;

int morseBuffer[5];
int morseBufferIndex = 0;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  //pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (clearedBuffer == false && millis() - millisPause >= space) {
    Serial.println("Clearing!");
    pushMorseBuffer();
    clearedBuffer = true;
  }
  
  int flexVal = analogRead(A0);

  if (flexVal <= flexThreshold) {
    // Pressing
    if (flexPressing == false) {
      Serial.println("Pressing");
      flexPressing = true;
      millisNow = millis();
      millisPause = millis();
    }
  } else if (flexVal >= deleteThreshold) {
    // Deleting
    if(flexDeleting == false) {
      flexDeleting = true;
      millisDelete = millis();
    } else {
      if(millis() - millisDelete >= deleteTime) {
        deleteLastLetter();
        flexDeleting = false;
      }
    }
  } else {
    if (flexPressing == true) {
      Serial.println("Released");
      int typeOfPress = determinePressType(millis() - millisNow);
      if (typeOfPress != -1) {
        Serial.println("Valid value: " + String(typeOfPress));
        morseBuffer[morseBufferIndex] = typeOfPress;
        morseBufferIndex += 1;
        clearedBuffer = false;
      } else {
        // invalid press
      }

      flexPressing = false;

      millisPause = millis();
    }

    if(flexDeleting == true) {
      flexDeleting = false;
    }
  }
}

// returns 1 for short press, 2 for long press, -1 for invalid
int determinePressType(unsigned long millisDiff) {
  if (millisDiff >= 3 * unit) return 2;
  if (millisDiff >= unit && millisDiff < 3 * unit) return 1;
  return -1;
}

void pushLetter(String letter) {
  Serial.println("Pushing " + letter + ", index " + cursorIndex);
  lcd.setCursor(cursorIndex, 0);
  lcd.print(letter);
  cursorIndex++;
}

void deleteLastLetter() {
  lcd.setCursor(cursorIndex - 1, 0);
  lcd.print(" ");
  cursorIndex--;
}

void pushMorseBuffer() {

  int sAM = 0;
  for (int i = 0; i < 5; i = i + 1) {
    if (morseBuffer[i] != 0) {
      sAM += 1;
    }
  }

  if (sAM == 1) { // sAM 1
    if (morseBuffer[0] == 1) {
      pushLetter("E");
    } else {
      pushLetter("T");
    }
  } else if (sAM == 2) { // sAM 2
    if (morseBuffer[0] == 1) {
      if (morseBuffer[1] == 1) {
        pushLetter("I");
      } else {
        pushLetter("A");
      }
    } else {
      if (morseBuffer[1] == 1) {
        pushLetter("N");
      } else {
        pushLetter("M");
      }
    }
  } else if (sAM == 3) { // sAM 3
    if (morseBuffer[0] == 1) {
      if (morseBuffer[1] == 1) {
        if (morseBuffer[2] == 1) {
          pushLetter("S");
        } else {
          pushLetter("U");
        }
      } else {
        if (morseBuffer[2] == 1) {
          pushLetter("R");
        } else {
          pushLetter("W");
        }
      }
    } else {
      if (morseBuffer[1] == 1) {
        if (morseBuffer[2] == 1) {
          pushLetter("D");
        } else {
          pushLetter("K");
        }
      } else {
        if (morseBuffer[2] == 1) {
          pushLetter("G");
        } else {
          pushLetter("O");
        }
      }
    }
  } else if (sAM == 4) { // sAM 4
    if (morseBuffer[0] == 1) {
      if (morseBuffer[1] == 1) {
        if (morseBuffer[2] == 1) {
          if (morseBuffer[3] == 1) {
            pushLetter("H");
          } else {
            pushLetter("V");
          }
        } else {
          if (morseBuffer[3] == 1) {
            pushLetter("F");
          } else {
            pushLetter("U");
          }
        }
      } else {
        if (morseBuffer[2] == 1) {
          if (morseBuffer[3] == 1) {
            pushLetter("L");
          } else {
            pushLetter("U");
          }
        } else {
          if (morseBuffer[3] == 1) {
            pushLetter("P");
          } else {
            pushLetter("J");
          }
        }
      }
    } else {
      if (morseBuffer[1] == 1) {
        if (morseBuffer[2] == 1) {
          if (morseBuffer[3] == 1) {
            pushLetter("B");
          } else {
            pushLetter("X");
          }
        } else {
          if (morseBuffer[3] == 1) {
            pushLetter("C");
          } else {
            pushLetter("Y");
          }
        }
      } else {
        if (morseBuffer[2] == 1) {
          if (morseBuffer[3] == 1) {
            pushLetter("Z");
          } else {
            pushLetter("Q");
          }
        } else {
          if (morseBuffer[3] == 1) {
            pushLetter("S");
          } else {
            pushLetter("U");
          }
        }
      }
    }
  }

  memset(morseBuffer, 0, sizeof(morseBuffer));
  morseBufferIndex = 0;
}

