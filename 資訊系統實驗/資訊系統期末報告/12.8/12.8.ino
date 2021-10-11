//上傳程式碼之前，請取下藍芽模組，以免藍芽模組損壞!!! Copyright © 2018 LanMooTech All rights reserved.
#include <deprecated.h>
#include <MFRC522.h> //RFID會用到
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include "LiquidCrystal_74HC595.h" //LCD會用到
#define PWM1_PIN 5 //車子馬達會用到6個
#define DIR1_PIN 4 //左邊輪
#define PWM2_PIN 11
#define DIR2_PIN 7 //右邊輪
#define EN_PIN 8
#define BUZZER_PIN 6
#define RST_PIN A0 // 讀卡機的重置腳位
#define SS_PIN  10 // 晶片選擇腳位
const byte tri  = A2; //超聲波傳感器會用到3個
const byte ech[3] = {A3,A4,A5}; //ech1 = A3, ech2 = A4, ech3 = A5
byte temp=11;
long dur[3], cm[3]; //dur1,dur2,dur3,cm1,cm2,cm3 超聲波傳感器會用到測距離
LiquidCrystal_74HC595 lcd(2, 3, 9); // Data pin is 2, Clock is 3 and Latch is 9
MFRC522 mfrc522(SS_PIN, RST_PIN);  // 建立MFRC522物件
const int motor_speed = 200;
char command;
unsigned long current = millis();
const byte LEDR = 4;
const byte LEDL = 7;
void setup()
{
    Serial.begin(9600);
    lcd.begin(16, 2);
    pinMode(tri, OUTPUT);
    pinMode(ech[0], INPUT);
    pinMode(ech[1], INPUT);
    pinMode(ech[2], INPUT);
    pinMode(LEDR, OUTPUT);
    pinMode(LEDL, OUTPUT);
    initial(); //設定4個馬達為輸出
    carStop(); //讓車子停下
    SPI.begin(); //RFID
    mfrc522.PCD_Init(); // 初始化MFRC522讀卡機模組
    delay(1000);
}
void initial()
{
    pinMode(PWM1_PIN, OUTPUT);//左後
    pinMode(DIR1_PIN, OUTPUT);//左前
    pinMode(PWM2_PIN, OUTPUT);//右後
    pinMode(DIR2_PIN, OUTPUT);//右前
    pinMode(EN_PIN, OUTPUT);
}
void getcard(){
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()){//是否感應到新的卡片和讀取卡片的資料
        alarm(BUZZER_PIN, 800, 300);
        byte *id = mfrc522.uid.uidByte;   // 取得卡片的UID
        byte idSize = mfrc522.uid.size;   // 取得UID的長度
        Serial.print("PICC type: ");      // 顯示卡片類型
        MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);// 根據卡片回應的SAK值（mfrc522.uid.sak）判斷卡片類型
        //Serial.println(mfrc522.PICC_GetTypeName(piccType));
        //Serial.print("UID: ");       // 顯示卡片的UID長度值
        //for (byte i = 0; i < idSize; i++)  // 逐一顯示UID碼
            //Serial.print(id[i], HEX);       // 以16進位顯示UID值
        mfrc522.PICC_HaltA();  // 讓卡片進入停止模式
        temp=id[0];
    }
}
void loop()
{
    if(temp==11){ //模式1，白色卡片，用手機操控車子
        lcd.setCursor(0,0);
        lcd.print("Mode 1");
        lcd.setCursor(0,1);
        if (Serial.available()){
            command = Serial.read();
            switch (command){
                case 'W':
                    
                    forward();
                    lcd.print("Forward");
                    lcd.setCursor(7,1);
                    lcd.print("         ");
                    digitalWrite(LEDL, HIGH);
                    digitalWrite(LEDR, HIGH);
                    break;
                case 'X':
                    backward();
                    lcd.print("Backward");
                    lcd.setCursor(8,1);
                    lcd.print("        ");
                    digitalWrite(LEDL, LOW);
                    digitalWrite(LEDR, LOW);
                    break;
                case 'Q':
                    forwardLeft();
                    lcd.print("ForwardLeft");
                    lcd.setCursor(11,1);
                    lcd.print("     ");
                    digitalWrite(LEDL, HIGH);
                    digitalWrite(LEDR, LOW);
                    break;
                case 'E':
                    forwardRight();
                    lcd.print("ForwardRight");
                    lcd.setCursor(12,1);
                    lcd.print("    ");
                    digitalWrite(LEDL, LOW);
                    digitalWrite(LEDR, HIGH);
                    break;
                case 'Z':
                    backwardLeft();
                    lcd.print("BackwardLeft");
                    lcd.setCursor(12,1);
                    lcd.print("    ");
                    break;
                case 'C':
                    backwardRight();
                    lcd.print("BackwardRight");
                    lcd.setCursor(13,1);
                    lcd.print("   ");
                    break;
                case 'S':
                    carStop();
                    lcd.print("Stop");
                    lcd.setCursor(4,1);
                    lcd.print("            ");
                    digitalWrite(LEDL, LOW);
                    digitalWrite(LEDR, LOW);
                    break;
            }
        }
        if ((command == 'X') && (millis() - current > 1000)){
            current = millis();
            alarm(BUZZER_PIN, 800, 300);
        }
        delay(100);
        getcard();
    }else if(temp!=11){ //模式2，除了白色卡片之外，用物體操控車子
        lcd.setCursor(0,0);
        lcd.print("Mode 2");
        lcd.setCursor(0,1);
        for(int i=0;i<3;i++){
            //digitalWrite(tri, LOW);
            //delayMicroseconds(5);
            digitalWrite(tri, HIGH);
            delayMicroseconds(10);
            digitalWrite(tri, LOW);
            dur[i] = pulseIn(ech[i], HIGH);
            cm[i] = (dur[i] / 2) / 29.1;
            delay(500);
            
        }
       for(int i=0;i<3;i++){
            Serial.print("Distance");
            Serial.print(i+1);
            Serial.print(":");  
            Serial.print(cm[i]);
            Serial.print("cm");
            Serial.println();
        }
        if((cm[0]>10&&cm[1]>10&&cm[2]>10)||(cm[0]<10&&cm[1]<10&&cm[2]<10)){
            carStop();
            lcd.print("Stop");
            lcd.setCursor(4,1);
            lcd.print("            ");
            digitalWrite(LEDL, LOW);
            digitalWrite(LEDR, LOW);
        }if(cm[1] < 10 && cm[1] != 0){
            forward();
            lcd.print("Forward");
            lcd.setCursor(7,1);
            lcd.print("         ");
            digitalWrite(LEDL, HIGH);
            digitalWrite(LEDR, HIGH);
        }if(cm[0] < 10 && cm[0] != 0){
            forwardRight();
            lcd.print("ForwardRight");
            lcd.setCursor(12,1);
            lcd.print("    ");
            digitalWrite(LEDL, LOW);
            digitalWrite(LEDR, HIGH);
        }if(cm[2] < 10 && cm[2] != 0){
            forwardLeft();
            lcd.print("ForwardLeft");
            lcd.setCursor(11,1);
            lcd.print("     ");
            digitalWrite(LEDL, HIGH);
            digitalWrite(LEDR, LOW);
        }
        delay(100);
        getcard();
   }
}
void alarm(byte pin, uint16_t frequency, uint16_t duration){
   unsigned long startTime = millis();
   unsigned long halfPeriod = 1000000L / frequency / 2;
   pinMode(pin, OUTPUT);
   while (millis() - startTime < duration){
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
   digitalWrite(DIR1_PIN, LOW);
   digitalWrite(DIR2_PIN, HIGH);//右
   analogWrite(PWM1_PIN, motor_speed);
   analogWrite(PWM2_PIN, motor_speed);
   digitalWrite(EN_PIN, HIGH);
}
void forwardRight()
{
   digitalWrite(DIR1_PIN, HIGH);//左
   digitalWrite(DIR2_PIN, LOW);
   analogWrite(PWM1_PIN, motor_speed);
   analogWrite(PWM2_PIN, motor_speed);
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
