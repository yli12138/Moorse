#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int push = 0, de = 1;
const int ledPin = 8;

int buttonState = 0;

int cursorTrans = 0;
int cursorChecker = 0;
int seconds = 0;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  pinMode(push, INPUT);
  pinMode(de, INPUT);

  //pinMode(ledPin, OUTPUT);
}

void loop() {
  buttonState = digitalRead(push);
  
  if (buttonState == HIGH) {
    //digitalWrite(ledPin, HIGH);
    lcd.clear();
    
    lcd.setCursor(0, 1);
    lcd.print("released");

    cursorTrans = 0;
    cursorChecker = cursorTrans;
  } else {
    //digitalWrite(ledPin, LOW);
    if(cursorChecker == cursorTrans){
      lcd.clear();
      seconds = millis();
      cursorChecker --;
    }
    
    lcd.setCursor(0, 1);
    lcd.print("pushing");

    int current = millis() - seconds;
    if(900 < current && current < 1100){
      lcd.setCursor(cursorTrans,0);
      lcd.print("-");
      cursorTrans ++;
      seconds = millis();
    }
  }
  
}
