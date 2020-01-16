//Break-o-meter developed by Michael Kuhn

#include <Wire.h>;
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f, 20, 4); //set Adress of LCd to 0x3f and set LCD size
const int timereset = 7;
const int breakagodisplay = 8;
const int buttonled = 5;
unsigned long interval = 3000000;  //3000000 = 50 min
unsigned long previousmillis = 0;
unsigned long breaksincemillis = 0;
unsigned long breakagosincemillis = 0;
int breaksince = 0;
bool breaktime;
unsigned long breaksinceinterval = 60000; //60000 = 1 min
unsigned long breakagointerval = 10000;

void setup() {
  Serial.begin(9600);
  pinMode(timereset, INPUT_PULLUP);
  pinMode(breakagodisplay, INPUT);
  pinMode(buttonled, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(4, 1);
  lcd.print("Make-a-break");
  lcd.setCursor(7, 2);
  lcd.print("Timer");
  delay(5000);
  lcd.clear();
  lcd.noBacklight();
}

void loop() {
  int resetstate = digitalRead(timereset); //Button Status
  int breakagostate = digitalRead(breakagodisplay);
  unsigned long currentmillis = millis();
  unsigned long currentbreakmillis = millis();

  //Serial.println(breaktime);
  //Serial.println(resetstate);

  if (currentmillis - previousmillis >= interval) {
    //timer();
    breaktime = 1;
    digitalWrite(buttonled, LOW);
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Hey,Time for a break");
    lcd.setCursor(0, 2);
    lcd.print(breaksince);
    if (breaksince < 10) {                // If/else used for spacing
      lcd.setCursor(2, 2);
      lcd.print("minutes");
    }
    else {
      lcd.setCursor(3, 2);
      lcd.print(" minutes");
    }
    lcd.setCursor(0, 3);
    lcd.print("since the last break");
  }

  else {
    //timer();
    //breaksincemillis = millis();
    digitalWrite(buttonled, HIGH);
  }

  if (breaktime == 1) {
    if (resetstate == LOW) {
      previousmillis = currentmillis;
      lcd.clear();
      lcd.noBacklight();
      breaksince = 0;
      breaktime = 0;
      breaksincemillis = currentbreakmillis;
      Serial.println(breaksince);
      Serial.println(currentbreakmillis);
      Serial.println(breaksincemillis);
    }
  }

  if (currentbreakmillis - breaksincemillis >= breaksinceinterval) {
    breaksince++;
    breaksincemillis = currentbreakmillis;
    Serial.println(breaksince);
  }
  if (breakagostate == HIGH) {
    breakagosincemillis = breaksincemillis;
    displayago();
    }
}

void displayago()  {
  unsigned long currentbreakagodisplay = millis();

  if (currentbreakagodisplay - breakagosincemillis <= breakagointerval) {
    lcd.backlight();
    lcd.setCursor(0, 2);
    lcd.print(breaksince);
    lcd.print(" minutes"); 
    lcd.setCursor(0, 3);
    lcd.print("since last break.");
  }
  else {
    lcd.clear();
    lcd.noBacklight();
  }
}
