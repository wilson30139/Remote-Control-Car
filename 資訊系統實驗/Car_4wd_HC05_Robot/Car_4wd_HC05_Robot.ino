/*
 * 使用手機的藍芽裝置控制 Robot 四輪小車
 * 警告：//////////////////////////////上傳程式碼之前，請取下藍芽模組，以免藍芽模組損壞!!!
 * 
 * Copyright © 2018 LanMooTech All rights reserved.
 */
#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#define PWM1_PIN 5
#define DIR1_PIN 4
#define PWM2_PIN 11
#define DIR2_PIN 7
#define EN_PIN 8
#define BUZZER_PIN 6
#include "LiquidCrystal_74HC595.h"
#define RST_PIN      A0        // 讀卡機的重置腳位
#define SS_PIN       10        // 晶片選擇腳位

const byte tri = A2;
const byte ech1 = A3;
const byte ech2 = A4;
const byte ech3 = A5;
long dur1, dur2, dur3, cm1, cm2, cm3;
LiquidCrystal_74HC595 lcd(2, 3, 9);// Data pin is 2, Clock is 3 and Latch is 9
MFRC522 mfrc522(SS_PIN, RST_PIN);  // 建立MFRC522物件
const int motor_speed = 200;

char command;
unsigned long current = millis();

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(tri, OUTPUT);
  pinMode(ech1, INPUT);
  pinMode(ech2, INPUT);
  pinMode(ech3, INPUT);
  initial();
  carStop();
  SPI.begin();
  mfrc522.PCD_Init();   // 初始化MFRC522讀卡機模組
  delay(1000);
}

void initial()
{
  pinMode(PWM1_PIN, OUTPUT);
  pinMode(DIR1_PIN, OUTPUT);
  pinMode(PWM2_PIN, OUTPUT);
  pinMode(DIR2_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);
}

void loop()
{
  digitalWrite(tri, HIGH);
  delayMicroseconds(1000);
  digitalWrite(tri, LOW);
  dur1 = pulseIn(ech1, HIGH);
  cm1 = (dur1 / 2) / 29.1;
  digitalWrite(tri, HIGH);
  delayMicroseconds(1000);
  digitalWrite(tri, LOW);
  /*digitalWrite(tri, LOW);
  delayMicroseconds(5);
  digitalWrite(tri, HIGH);
  delayMicroseconds(10);
  digitalWrite(tri, LOW);*/
  dur2 = pulseIn(ech2, HIGH);
  cm2 = (dur2 / 2) / 29.1;
  digitalWrite(tri, HIGH);
  delayMicroseconds(1000);
  digitalWrite(tri, LOW);
  dur3 = pulseIn(ech3, HIGH);
  cm3 = (dur3 / 2) / 29.1;
  lcd.print("Distance1:");
  lcd.print(cm1);
  if(cm1 < 10){
    lcd.setCursor(11,0);
    lcd.print("cm");
    lcd.setCursor(13,0);
    lcd.print("   ");
    /*digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);*/
  }else if(cm1 > 9 && cm1 <100){
    lcd.setCursor(12,0);
    lcd.print("cm");
    lcd.setCursor(14,0);
    lcd.print("  ");
    /*digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);*/  
  }else{
    lcd.setCursor(13,0);
    lcd.print("cm");
    lcd.setCursor(15,0);
    lcd.print(" ");
    /*digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);*/  
  }
  lcd.setCursor(0,1);
  lcd.print("Distance2:");
  lcd.print(cm2);
  if(cm2 < 10){
    lcd.setCursor(11,1);
    lcd.print("cm");
    lcd.setCursor(13,1);
    lcd.print("   ");
    /*digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);*/
  }else if(cm2 > 9 && cm2 <100){
    lcd.setCursor(12,1);
    lcd.print("cm");
    lcd.setCursor(14,1);
    lcd.print("  ");
    /*digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);*/  
  }else{
    lcd.setCursor(13,1);
    lcd.print("cm");
    lcd.setCursor(15,1);
    lcd.print(" ");
    /*digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);*/  
  }
  Serial.print("Distance1:");  
  Serial.print(cm1);
  Serial.print("cm");
  Serial.println();
  Serial.print("Distance2:");  
  Serial.print(cm2);
  Serial.print("cm");
  Serial.println();
  Serial.print("Distance3:");  
  Serial.print(cm3);
  Serial.print("cm");
  Serial.println();
  if (Serial.available())
  {
    command = Serial.read();
    
    switch (command)
    {
      case 'W':
        forward();
        break;
      case 'X':
        backward();
        break;
      case 'Q':
        forwardLeft();
        break;
      case 'E':
        forwardRight();
        break;
      case 'Z':
        backwardLeft();
        break;
      case 'C':
        backwardRight();
        break;
      case 'S':
        carStop();
        break;
    }
  }

  if ((command == 'X') && (millis() - current > 1000))
  {
    current = millis();
    
    alarm(BUZZER_PIN, 800, 300);
  }
  
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      byte *id = mfrc522.uid.uidByte;   // 取得卡片的UID
      byte idSize = mfrc522.uid.size;   // 取得UID的長度
      Serial.print("PICC type: ");      // 顯示卡片類型
      // 根據卡片回應的SAK值（mfrc522.uid.sak）判斷卡片類型
      MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
      Serial.println(mfrc522.PICC_GetTypeName(piccType));
      Serial.print("UID: ");       // 顯示卡片的UID長度值
      //Serial.println(idSize);
      for (byte i = 0; i < idSize; i++) {  // 逐一顯示UID碼
        /*Serial.print("id[");
        Serial.print(i);
        Serial.print("]: ");*/
        Serial.print(id[i], HEX);       // 以16進位顯示UID值
      }
      if(id[0] == 11)
        tone(6, 500);
      else
        noTone(6);
      mfrc522.PICC_HaltA();  // 讓卡片進入停止模式
      
    }
}

void alarm(byte pin, uint16_t frequency, uint16_t duration)
{
  unsigned long startTime = millis();
  unsigned long halfPeriod = 1000000L / frequency / 2;
  
  pinMode(pin, OUTPUT);
  
  while (millis() - startTime < duration)
  {
    digitalWrite(pin, HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(pin, LOW);
    delayMicroseconds(halfPeriod);
  }
  
  pinMode(pin, INPUT);
}

void forward()
{
  digitalWrite(DIR1_PIN, HIGH);
  digitalWrite(DIR2_PIN, HIGH);
  analogWrite(PWM1_PIN, motor_speed);
  analogWrite(PWM2_PIN, motor_speed);

  digitalWrite(EN_PIN, HIGH);
}

void forwardLeft()
{
  int speed = motor_speed + 40;

  speed = (speed > 255 ? 255 : speed);
  
  digitalWrite(DIR1_PIN, LOW);
  digitalWrite(DIR2_PIN, HIGH);
  analogWrite(PWM1_PIN, speed);
  analogWrite(PWM2_PIN, speed);

  digitalWrite(EN_PIN, HIGH);
}

void forwardRight()
{
  int speed = motor_speed + 40;

  speed = (speed > 255 ? 255 : speed);
  
  digitalWrite(DIR1_PIN, HIGH);
  digitalWrite(DIR2_PIN, LOW);
  analogWrite(PWM1_PIN, speed);
  analogWrite(PWM2_PIN, speed);

  digitalWrite(EN_PIN, HIGH);
}

void backward()
{
  digitalWrite(DIR1_PIN, LOW);
  digitalWrite(DIR2_PIN, LOW);
  analogWrite(PWM1_PIN, motor_speed);
  analogWrite(PWM2_PIN, motor_speed);

  digitalWrite(EN_PIN, HIGH);
}

void backwardLeft()
{
  int speed = motor_speed + 40;

  speed = (speed > 255 ? 255 : speed);
  
  digitalWrite(DIR1_PIN, HIGH);
  digitalWrite(DIR2_PIN, LOW);
  analogWrite(PWM1_PIN, speed);
  analogWrite(PWM2_PIN, speed);

  digitalWrite(EN_PIN, HIGH);
}

void backwardRight()
{
  int speed = motor_speed + 40;

  speed = (speed > 255 ? 255 : speed);
  
  digitalWrite(DIR1_PIN, LOW);
  digitalWrite(DIR2_PIN, HIGH);
  analogWrite(PWM1_PIN, speed);
  analogWrite(PWM2_PIN, speed);

  digitalWrite(EN_PIN, HIGH);
}

void carStop()
{
  digitalWrite(EN_PIN, LOW);
}
