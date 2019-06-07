#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int sendButton = 1;
const int ledPin = 8;

int buttonState = 0;
int flexVal = 0;

String currentLine = "";
String oldLine = "";

const int unit = 500;

void sendLine(){
  oldLine = currentLine;
  currentLine = "";

  lineUpDate();
}

void lineUpDate(){//call after sendLine
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print(oldLine);
}

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  pinMode(sendButton, INPUT);

  //pinMode(ledPin, OUTPUT);
}

void loop() {
  buttonState = digitalRead(sendButton);
  
  if (buttonState == HIGH) {
    lcd.clear();
  } else {
    //digitalWrite(ledPin, LOW);
    lcd.setCursor(0,0);
    flexVal = analogRead(A0);
    lcd.print(flexVal);
}
