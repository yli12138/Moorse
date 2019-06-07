#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int sendButton = 1;
int sendButtonState = 0;

String currentLine = "";
String oldLine = "";

boolean finish = false;

const unsigned long unit = 50;
const int flexThreshold = 750;

boolean flexPressing = false;

unsigned long millisNow = 0;

void sendLine(){
  oldLine = "" + currentLine;
  currentLine = "";

//  lcd.clear();
  lineUpDate();
}

void lineUpDate(){//call after sendLine
  lcd.setCursor(0,0);
  lcd.print(oldLine);
  lcd.setCursor(0,1);
  lcd.print(currentLine);
}

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  pinMode(sendButton, INPUT);

  //pinMode(ledPin, OUTPUT);
//  Serial.begin(9600);
}

void loop() {
  sendButtonState = digitalRead(sendButton);
  int flexVal = analogRead(A0);

  if(flexVal <= flexThreshold) {
    // Pressing
    if(!flexPressing) {
      flexPressing = true;
      millisNow = millis();
    }
  } else {
    if(flexPressing && !finish) {
      finish = true;
      flexPressing = false;
      int typeOfPress = determinePressType(millis() - millisNow);
      if(typeOfPress == -1) {
        currentLine = "invalid!";
        lineUpDate();
      } else if(typeOfPress == 0) {
        currentLine = "short press!";
        lineUpDate();
      } else {
        currentLine = "long press!";
        lineUpDate();
      }
    }
  }

//  if(flexPressing) {
//    lcd.setCursor(0,0);
//    lcd.print("pressing!");
//  } else {
//    lcd.clear();
//  }
  
  if (sendButtonState == LOW) {
    finish = false;
    sendLine();
  } 
  //else {
//    //digitalWrite(ledPin, LOW);
//    lcd.setCursor(0,0);
//    lcd.print(flexVal);
//  }
}

// returns 0 for short press, 1 for long press, -1 for invalid
int determinePressType(unsigned long millisDiff) {
  if(millisDiff >= 3*unit) return 1;
  if(millisDiff >= unit && millisDiff < 3*unit) return 0;
  return -1;
}

