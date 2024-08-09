#include <Servo.h>
#include <Stepper.h>
#include <LedControl.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo lid;

int servoPin = 6;
int angle = 90;
int message;
int cnt;
int lid_memory;
int last;
int max;

const int stepsPerRevolution = 1024;   // 스텝모터 한 바퀴는 1024스텝
Stepper myStepper1(stepsPerRevolution, 13, 11, 12, 10);
Stepper myStepper2(stepsPerRevolution, 5, 3, 4, 2);

void setup() {
  Serial.begin(9600);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  myStepper1.setSpeed(17.9); // 모터 1
  myStepper2.setSpeed(17.9); // 모터 2
  lid.attach(servoPin);
  lid.write(90);
  Wire.begin();
  lcd.begin(); // 초기화
  lcd.backlight();
  lcd.setCursor(0, 0);
}

void dots() {
  if (message == 0) {
    lcd.setCursor (5, 0);
    lcd.print("Home");
  }
  else if (message == 1) {
    lcd.setCursor (5, 0);
    lcd.print("1 Class");
  }
  else if (message == 2) {
    lcd.setCursor (5, 0);
    lcd.print("2 Class"); // 빨간불
  }
  else if (message == 3) {
    lcd.setCursor (5, 0);
    lcd.print("3 Class");
  }
}

void fw() {
  while (cnt < max) {
    myStepper1.step(-stepsPerRevolution / 720);
    myStepper2.step(stepsPerRevolution / 720);
    cnt += 1;
  }
}

void bw() {
  while (cnt < max) {
    myStepper1.step(stepsPerRevolution / 720);
    myStepper2.step(-stepsPerRevolution / 720);
    cnt += 1;
  }
}

void loop() {
  lcd.setCursor (5, 0);
  lcd.print("T-Bot");
  lcd.setCursor (3, 1);
  lcd.print("HackerTone");
  if (Serial.available() > 0) {
    message = Serial.read();
    Serial.println(10);
    if (last == 0 && message < 4) {
      max = 720 * message;
      dots();
      fw();
      last = message;
      cnt = 0;
    }
    else if (message < 4) {
      if (last < message) {
        max = 720 * (message - last);
        dots();
        fw();
        last = message;
        cnt = 0;
      }
      else {
        max = 720 * (last - message);
        bw();
        last = message;
        cnt = 0;
      }
    }
    else {
      if (message == 4) {
        for (angle = 70; angle > 0; angle--) {
          lid.write(angle);
          delay(2);
          Serial.println(angle);
        }
        lid_memory = message;
        digitalWrite(A0, HIGH); // 초록불
        digitalWrite(A1, LOW); // 빨간불
      }
      else if (message == 5) {
        if (lid_memory == 4) {
          for (angle = 0; angle < 70; angle++) {
            lid.write(angle);
            delay(2);
            Serial.println(angle);
          }
          Serial.println(message);
          digitalWrite(A0, LOW); // 초록불
          digitalWrite(A1, HIGH); // 빨간불
        }
        else {
          for (angle = 140; angle > 70; angle--) {
            lid.write(angle);
            delay(2);
            Serial.println(angle);
          }
          digitalWrite(A0, LOW); // 초록불
          digitalWrite(A1, HIGH); // 빨간불
          Serial.println(message);
        }
      }
      else {
        for (angle = 70; angle < 140; angle++) {
          lid.write(angle);
          delay(2);
          Serial.println(angle);
        }
        digitalWrite(A0, HIGH); // 초록불
        digitalWrite(A1, LOW); // 빨간불
        lid_memory = message;
        Serial.println(message);
      }
    }
  }
}
