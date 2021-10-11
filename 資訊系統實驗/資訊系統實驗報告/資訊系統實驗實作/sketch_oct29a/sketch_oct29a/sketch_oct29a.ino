#include <LiquidCrystal.h>
#include <EEPROM.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

/*
Right        Value = 0
Up           Value = 1
Down         Value = 2
Left         Value = 3
Select       Value = 4
*/
int  adc_key_val[5] ={
 30, 150, 360, 535, 760 };

int NUM_KEYS = 5;
int adc_key_in;
int key=-1;
int oldkey=-1;

int lcdTopRow = 0;
int lcdBottomRow = 1;
int DropCount = 0;       // default Starting point
int pulseWidth0 = 0;    //Valve ON Time  (Drop Size)
int pulseWidth1 = 0;    //Valve ON Time  (Drop Size)
int pulseWidth2 = 0;    //Valve ON Time  (Drop Size)
int pulseSep0 = 0;      // Drop Seperation time  (Time Between Drops), valve off time
int pulseSep1 = 0;      // Drop Seperation time  (Time Between Drops), valve off time

int reading = 0;

int VLVOFF = LOW;
int VLVON = HIGH;
int backLightON = HIGH;
int backLightOFF = LOW;
int ledpinON = HIGH;
int ledpinOFF = LOW;
int triggerON = LOW;
int triggerOFF = HIGH;
int dropsAwayON = LOW;
int dropsAwayOFF = HIGH;
int backLightControl = 10;   // Output to control back light on LCD
int valveControl =11;    // Output pin to turn valve ON & OFF
int ledpin = 13;         // Heartbeat indicator
int timerTrigger = 2;    // Output pin to start flash trigger timer
int dropsAway = 3;       // Output pin to show signal to valve
int cursorValue = 0;

void setup()
{
 pinMode(timerTrigger,OUTPUT);   //flash timer trigger
 pinMode(backLightControl,OUTPUT);   //Backlight control
 pinMode(valveControl,OUTPUT);   //valve control
 pinMode(ledpin, OUTPUT);  //we'll use the debug LED to output a heartbeat
 pinMode(dropsAway,OUTPUT);      // indicater for valve signal

 lcd.begin(16,2);
 Serial.begin(9600);

 //load our variables from EEPROM
 DropCount = EEPROM.read(0);
 pulseWidth0 = EEPROM.read(1);
 pulseSep0 = EEPROM.read(2);
 pulseWidth1 = EEPROM.read(3);
 pulseSep1 = EEPROM.read(4);
 pulseWidth2 = EEPROM.read(5);

 digitalWrite(dropsAway,dropsAwayOFF);          // Turn off the indicator
 digitalWrite(valveControl,VLVOFF);             // Valve OFF
 digitalWrite(ledpin, ledpinOFF);               // Turn OFF board LED
 digitalWrite(backLightControl,backLightON);    // Turn ON LCD Backlight
 lcd.clear();


 if (DropCount == 1)
 {
   lcd.setCursor(0,lcdTopRow);
   lcd.print("Drop Count");
   lcd.setCursor(0,lcdBottomRow);
   lcd.print(DropCount);
   lcd.print(" ");
   lcd.print(pulseWidth0);
 }

 else if (DropCount == 2)
 {
   lcd.setCursor(0,lcdTopRow);
   lcd.print("Drop Count");
   lcd.setCursor(0,lcdBottomRow);
   lcd.print(DropCount);
   lcd.print(" ");
   lcd.print(pulseWidth0);
   lcd.print(" ");
   lcd.print(pulseSep0);
   lcd.print(" ");
   lcd.print(pulseWidth1);
 }

 else if (DropCount == 3)
 {
   lcd.setCursor(0,lcdTopRow);
   lcd.print("Drop Count");
   lcd.setCursor(0,lcdBottomRow);
   lcd.print(DropCount);
   lcd.print(" ");
   lcd.print(pulseWidth0);
   lcd.print(" ");
   lcd.print(pulseSep0);
   lcd.print(" ");
   lcd.print(pulseWidth1);
   lcd.print(" ");
   lcd.print(pulseSep1);
   lcd.print(" ");
   lcd.print(pulseWidth2);
 }
 lcd.setCursor(0,lcdBottomRow);
 lcd.cursor();   //Turn on the cursor
}

void loop()
{

 adc_key_in = analogRead(0);    // read the value from the sensor  

 key = get_key(adc_key_in);    // convert into key press
 if (key != oldkey) // if keypress is detected
 {
   delay(50);      // wait for debounce time
   adc_key_in = analogRead(0);    // read the value from the sensor  
   key = get_key(adc_key_in);    // convert into key press
   if (key != oldkey)            
   {          
     oldkey = key;
     if (key >=0)    
     {
       rightkey();
     }
     if (key == 1)     //up key
     {
       upkey();

     }

     else if (key == 2)   //down  key
     {
       downkey();

     }
     else if (key ==3)    //left key

     {
       leftkey();
     }

     else if (key == 4)     //select key
     {
       outloop();
       delay(500);
     }
   }
 }    
}




// Convert ADC value to key number

int get_key(unsigned int input)
{  
 int k;
 for (k = 0; k < NUM_KEYS; k++)
 {
   if (input < adc_key_val[k])
   {  
     return k;  
   }
 }
 if (k >= NUM_KEYS)
   k = -1;     // No valid key pressed
 return k;
}
/*






*/
void rightkey()
{


 if (key == 0)    //right key
 {
   cursorValue++;
   if (cursorValue >= 15)
   {
     cursorValue = 15;
   }

   lcd.setCursor (cursorValue,1);
   //lcd.print (cursorValue);

   if (cursorValue == 2 && DropCount == 3)
   {

     lcd.setCursor(0,lcdTopRow);
     lcd.print("                ");
     lcd.setCursor(0,lcdTopRow);
     lcd.print("Drop 1 Size");
     lcd.setCursor(cursorValue,1);
   }
   // lcd.print(pulseWidth0);

   else if (cursorValue == 5 && DropCount == 3)
   {
     lcd.setCursor(0,lcdTopRow);
     lcd.print("Drop 1 & 2 Sep");
     lcd.setCursor(cursorValue,1);
   }

   else if (cursorValue == 8 && DropCount == 3)
   {
     lcd.setCursor(0,lcdTopRow);
     lcd.print("               ");
     lcd.setCursor(0,lcdTopRow);
     lcd.print("Drop 2 Size");
     lcd.setCursor(cursorValue,1);
   }
   else if (cursorValue == 11 && DropCount == 3)
   {
     lcd.setCursor(0,lcdTopRow);

     lcd.print("Drop 2 & 3 Sep");
     lcd.setCursor(cursorValue,1);
   }
   else if (cursorValue == 14 && DropCount == 3)
   {
     lcd.setCursor(0,lcdTopRow);
     lcd.print("               ");
     lcd.setCursor(0,lcdTopRow);
     lcd.print("Drop 3 Size");
     lcd.setCursor(cursorValue,1);

   }
   if (cursorValue == 2 && DropCount == 2)
   {
     lcd.setCursor(0,lcdTopRow);
     lcd.print("                ");
     lcd.setCursor(0,lcdTopRow);
     lcd.print("Drop 1 Size");
     lcd.setCursor(cursorValue,1);
   }


   else if (cursorValue == 5 && DropCount == 2)
   {
     lcd.setCursor(0,lcdTopRow);
     lcd.print("Drop 1 & 2 Sep");
     lcd.setCursor(cursorValue,1);
   }

   else if (cursorValue == 8 && DropCount == 2)
   {
     lcd.setCursor(0,lcdTopRow);
     lcd.print("               ");
     lcd.setCursor(0,lcdTopRow);
     lcd.print("Drop 2 Size");
     lcd.setCursor(cursorValue,1);
   }
   else if (cursorValue == 2 && DropCount == 1)
   {
     lcd.setCursor(0,lcdTopRow);
     lcd.print("                ");
     lcd.setCursor(0,lcdTopRow);
     lcd.print("Drop 1 Size");
     lcd.setCursor(cursorValue,1);
   }
 }  
}
/*


*/
void leftkey()
{
 cursorValue--;
 if (cursorValue <= 0)
 {
   cursorValue = 0;
 }  
 lcd.setCursor (cursorValue,1);

 if (cursorValue == 0)
 {
   lcd.setCursor(0,lcdTopRow);
   lcd.print("                ");
   lcd.setCursor(0,lcdTopRow);
   lcd.print ("Drop Count");
   lcd.setCursor(cursorValue,1);
 }
 if (cursorValue == 2 && DropCount == 3)
 {

   lcd.setCursor(0,lcdTopRow);
   lcd.print("                ");
   lcd.setCursor(0,lcdTopRow);
   lcd.print("Drop 1 Size");
   lcd.setCursor(cursorValue,1);
 }

 else if (cursorValue == 5 && DropCount == 3)
 {
   lcd.setCursor(0,lcdTopRow);
   lcd.print("Drop 1 & 2 Sep");
   lcd.setCursor(cursorValue,1);
 }

 else if (cursorValue == 8 && DropCount == 3)
 {
   lcd.setCursor(0,lcdTopRow);
   lcd.print("               ");
   lcd.setCursor(0,lcdTopRow);
   lcd.print("Drop 2 Size");
   lcd.setCursor(cursorValue,1);
 }
 else if (cursorValue == 11 && DropCount == 3)
 {
   lcd.setCursor(0,lcdTopRow);

   lcd.print("Drop 2 & 3 Sep");
   lcd.setCursor(cursorValue,1);
 }
 else if (cursorValue == 14 && DropCount == 3)
 {
   lcd.setCursor(0,lcdTopRow);
   lcd.print("               ");
   lcd.setCursor(0,lcdTopRow);
   lcd.print("Drop 3 Size");
   lcd.setCursor(cursorValue,1);              
 }

 if (cursorValue == 2 && DropCount == 2)
 {
   lcd.setCursor(0,lcdTopRow);
   lcd.print("                ");
   lcd.setCursor(0,lcdTopRow);
   lcd.print("Drop 1 Size");
   lcd.setCursor(cursorValue,1);
 }
 // lcd.print(pulseWidth0);

 else if (cursorValue == 5 && DropCount == 2)
 {
   lcd.setCursor(0,lcdTopRow);
   lcd.print("Drop 1 & 2 Sep");
   lcd.setCursor(cursorValue,1);
 }

 else if (cursorValue == 8 && DropCount == 2)
 {
   lcd.setCursor(0,lcdTopRow);
   lcd.print("               ");
   lcd.setCursor(0,lcdTopRow);
   lcd.print("Drop 2 Size");
   lcd.setCursor(cursorValue,1);
 }
 if (cursorValue == 2 && DropCount == 1)
 {
   lcd.setCursor(0,lcdTopRow);
   lcd.print("                ");
   lcd.setCursor(0,lcdTopRow);
   lcd.print("Drop 1 Size");
   lcd.setCursor(cursorValue,1);
 }
}
/*


*/


void upkey()
{

 if (DropCount == 0)
 {
   lcd.setCursor(0,lcdTopRow);
   lcd.print ("                ");
   lcd.setCursor(0,lcdTopRow);
   lcd.print ("Drop Count");
   lcd.setCursor(0,lcdBottomRow);


 }
 if (cursorValue == 0)
 {




   DropCount = DropCount++;

   if (DropCount == 2  && cursorValue == 0)

   {
     lcd.print (DropCount);
     lcd.print (" ");
     lcd.print (pulseWidth0);
     lcd.print (" ");
     lcd.print (pulseSep0);
     lcd.print (" ");
     lcd.print (pulseWidth1);
     lcd.print ("      ");
     lcd.setCursor(0,lcdBottomRow);

   }
   else if (DropCount == 3  && cursorValue == 0)

   {
     lcd.print (DropCount);
     lcd.print (" ");
     lcd.print (pulseWidth0);
     lcd.print (" ");
     lcd.print (pulseSep0);
     lcd.print (" ");
     lcd.print (pulseWidth1);
     lcd.print (" ");
     lcd.print (pulseSep1);
     lcd.print (" ");
     lcd.print (pulseWidth2);
   }


   lcd.setCursor(0,lcdBottomRow);
   lcd.print(DropCount);
   lcd.setCursor(cursorValue,1);
 }

 if (DropCount >= 4)              
 {                                
   DropCount = 4;                
   lcd.setCursor(0,lcdTopRow);    
   lcd.print("Max Drops = 3");    
   lcd.setCursor(0,lcdBottomRow);            
   lcd.print(DropCount);          
   lcd.setCursor(0,lcdBottomRow);            
 }                                


 else if (cursorValue == 2)  //increment tens of drop size 1
 {
   pulseWidth0 = pulseWidth0 + 10;

   if (pulseWidth0 >= 100)
   {
     pulseWidth0 = 99;
   }

   lcd.print(pulseWidth0);
   lcd.setCursor(cursorValue,1);
 }  
 else if (cursorValue == 3)  //increment units of drop size 1
 {
   pulseWidth0 = pulseWidth0 +1;

   if (pulseWidth0 >= 100)
   {
     pulseWidth0 = 99;
   }

   if (pulseWidth0 <= 9)
   {
     lcd.setCursor(cursorValue -1,1);
     lcd.print(" ");
   }
   else if (pulseWidth0 >= 10)
   {
     lcd.setCursor(cursorValue -1,1);
   }  
   lcd.print (pulseWidth0);
   lcd.setCursor(cursorValue,1);
 }  

 else if (cursorValue == 5)    //increment tens of drop seperation 1
 {
   pulseSep0 = pulseSep0 + 10;

   if (pulseSep0 >= 100)
   {
     pulseSep0 = 99;
   }
   lcd.print(pulseSep0);
   lcd.setCursor(cursorValue,1);
 }  
 else if (cursorValue == 6)  //increment units of drop size 1
 {
   pulseSep0 = pulseSep0 +1;

   if (pulseSep0 >= 100)
   {
     pulseSep0 = 99;
   }

   if (pulseSep0 <= 9)
   {
     lcd.setCursor(cursorValue -1,1);
     lcd.print(" ");
   }
   else if (pulseSep0 >= 10)
   {
     lcd.setCursor(cursorValue -1,1);
   }  
   lcd.print (pulseSep0);
   lcd.setCursor(cursorValue,1);
 }

 else if (cursorValue == 8)  //increment tens of drop size 2
 {
   pulseWidth1 = pulseWidth1 + 10;

   if (pulseWidth1 >= 100)
   {
     pulseWidth1 = 99;
   }

   lcd.print(pulseWidth1);
   lcd.setCursor(cursorValue,1);
 }  
 else if (cursorValue == 9)  //increment units of drop size 2
 {
   pulseWidth1 = pulseWidth1 +1;

   if (pulseWidth1 >= 100)
   {
     pulseWidth1 = 99;
   }
   if (pulseWidth1 <= 9)
   {
     lcd.setCursor(cursorValue -1,1);
     lcd.print(" ");
   }
   else if (pulseWidth1 >= 10)
   {
     lcd.setCursor(cursorValue -1,1);
   }
   lcd.print (pulseWidth1);
   lcd.setCursor(cursorValue,1);
 }  

 else if (cursorValue == 11)    //increment tens of drop seperation 2
 {
   pulseSep1 = pulseSep1 + 10;

   if (pulseSep1 >= 100)
   {
     pulseSep1 = 99;
   }
   lcd.print(pulseSep1);
   lcd.setCursor(cursorValue,1);
 }  
 else if (cursorValue == 12)      //increment units of drop seperation 2
 {
   pulseSep1 = pulseSep1 +1;

   if (pulseSep1 >= 100)
   {
     pulseSep1 = 99;
   }
   if (pulseSep1 <= 9)
   {
     lcd.setCursor(cursorValue -1,1);
     lcd.print(" ");
   }
   else if (pulseSep1 >= 10)
   {
     lcd.setCursor(cursorValue -1,1);
   }
   lcd.print (pulseSep1);
   lcd.setCursor(cursorValue,1);
 }

 else if (cursorValue == 14)  //increment tens of drop size 3
 {
   pulseWidth2 = pulseWidth2 + 10;

   if (pulseWidth2 >= 100)
   {
     pulseWidth2 = 99;
   }

   lcd.print(pulseWidth2);
   lcd.setCursor(cursorValue,1);
 }  
 else if (cursorValue == 15)  //increment units of drop size 3
 {
   pulseWidth2 = pulseWidth2 +1;

   if (pulseWidth2 >= 100)
   {
     pulseWidth2 = 99;
   }
   if (pulseWidth2 <= 9)
   {
     lcd.setCursor(cursorValue -1, 1);
     lcd.print(" ");
   }
   else if (pulseWidth2 >= 10)
   {
     lcd.setCursor(cursorValue -1,1);
   }
   lcd.print (pulseWidth2);
   lcd.setCursor(cursorValue,1);
 }  
 return;
}
/*






*/
void downkey()
{
 if (DropCount == 4)
 {
   lcd.setCursor(0,lcdTopRow);
   lcd.print ("                ");
   lcd.setCursor(0,lcdTopRow);
   lcd.print ("Drop Count");
   lcd.setCursor(0,lcdBottomRow);

 }

 if (cursorValue == 0)
 {


   DropCount = DropCount--;

   if (DropCount <= 0)
   {

     DropCount = 0;
     lcd.setCursor(0,lcdTopRow);
     lcd.print("Min Drops = 1");
     lcd.setCursor(0,lcdBottomRow);
     lcd.print(DropCount);
     lcd.setCursor(0,lcdBottomRow);
   }

   if (DropCount == 2  && cursorValue == 0)

   {

     lcd.print (DropCount);
     lcd.print (" ");
     lcd.print (pulseWidth0);
     lcd.print (" ");
     lcd.print (pulseSep0);
     lcd.print (" ");
     lcd.print (pulseWidth1);
     lcd.print ("      ");
     lcd.setCursor(0,lcdBottomRow);

   }
   if (DropCount == 1  && cursorValue == 0)

   {
     lcd.print (DropCount);
     lcd.print (" ");
     lcd.print (pulseWidth0);
     lcd.print ("            ");
   }

   lcd.print(DropCount);
   lcd.setCursor(cursorValue,1);

 }







 else if (cursorValue == 2)  //decrement tens of drop size 1
 {
   pulseWidth0 = pulseWidth0 - 10;

   if (pulseWidth0 <= 9)
   {

     lcd.setCursor(cursorValue,1);  // put cursor in correct position to print just units
     // when the value is less than ten
     lcd.print(" ");
     cursorValue ++;
     lcd.setCursor(cursorValue ,1);
     lcd.print(pulseWidth0);
     lcd.setCursor(cursorValue,1);
   }

   if (pulseWidth0 == 0) //make sure pulswidth never goes to zero
   {
     pulseWidth0 = 1;
   }


   lcd.print(pulseWidth0);
   lcd.setCursor(cursorValue,1);
 }  
 else if (cursorValue == 3)  //decrement units of drop size 1
 {
   pulseWidth0 = pulseWidth0 -1;

   if (pulseWidth0 == 0)     // make sure pulse width never goes to zerp
   {
     pulseWidth0 = 1;
   }

   if (pulseWidth0 <= 9)
   {
     lcd.setCursor(cursorValue -1,1);
     lcd.print(" ");
   }
   else if (pulseWidth0 >= 10)
   {
     lcd.setCursor(cursorValue -1,1);

   }

   lcd.print (pulseWidth0);
   lcd.setCursor(cursorValue,1);

 }


 else if (cursorValue == 5)    //decrement tens of drop seperation 1
 {
   pulseSep0 = pulseSep0 - 10;

   if (pulseSep0 <= 9)
   {

     lcd.setCursor(cursorValue,1);      // put cursor in correct position to print just units
     // when the value is lees than ten
     lcd.print(" ");
     cursorValue ++;
     lcd.setCursor(cursorValue ,1);
     lcd.print(pulseSep0);
     lcd.setCursor(cursorValue,1);
   }

   if (pulseSep0 == 0)                 //never let the seperation go to zero
   {
     pulseSep0 = 1;
   }


   lcd.print(pulseSep0);
   lcd.setCursor(cursorValue,1);
   //pulseSep0 = pulseSep0 - 10;
   // lcd.print(pulseSep0);
   lcd.setCursor(cursorValue,1);
 }  
 else if (cursorValue == 6)      //decrement units of drop seperation 1
 {
   pulseSep0 = pulseSep0 -1;

   if (pulseSep0 == 0)     // make sure pulse width never goes to zerp
   {
     pulseSep0 = 1;
   }

   if (pulseSep0 <= 9)
   {
     lcd.setCursor(cursorValue -1,1);
     lcd.print(" ");
   }
   else if (pulseSep0 >= 10)
   {
     lcd.setCursor(cursorValue -1,1);

   }
   lcd.print (pulseSep0);
   lcd.setCursor(cursorValue,1);
 }
 else if (cursorValue == 8)  //decrement tens of drop size 1
 {
   pulseWidth1 = pulseWidth1 - 10;

   if (pulseWidth1 <= 9)
   {

     lcd.setCursor(cursorValue,1);  // put cursor in correct position to print just units
     // whne the value is lees than ten
     lcd.print(" ");
     cursorValue ++;
     lcd.setCursor(cursorValue ,1);
     lcd.print(pulseWidth1);
     lcd.setCursor(cursorValue,1);
   }

   if (pulseWidth1 == 0) //make sure pulswidth never goes to zero
   {
     pulseWidth1 = 1;
   }


   lcd.print(pulseWidth1);
   lcd.setCursor(cursorValue,1);
 }  

 else if (cursorValue == 9)  //decrement units of drop size 1
 {
   pulseWidth1 = pulseWidth1 -1;

   if (pulseWidth1 == 0)     // make sure pulse width never goes to zerp
   {
     pulseWidth1 = 1;
   }

   if (pulseWidth1 <= 9)
   {
     lcd.setCursor(cursorValue -1,1);
     lcd.print(" ");
   }
   else if (pulseWidth1 >= 10)
   {
     lcd.setCursor(cursorValue -1,1);

   }

   lcd.print (pulseWidth1);
   lcd.setCursor(cursorValue,1);

 }


 else if (cursorValue == 11)    //decrement tens of drop seperation 2
 {
   pulseSep1 = pulseSep1 - 10;

   if (pulseSep1 <=9)

   {  
     lcd.setCursor(cursorValue,1);  // put cursor in correct position to print just units
     // when the value is less than ten
     lcd.print(" ");
     cursorValue ++;
     lcd.setCursor(cursorValue ,1);
     lcd.print(pulseSep1);
     lcd.setCursor(cursorValue,1);
   }

   if (pulseSep1 == 0) //make sure pulswidth never goes to zero
   {
     pulseSep1 = 1;
   }


   lcd.print(pulseSep1);
   lcd.setCursor(cursorValue,1);
 }


 else if (cursorValue == 12)      //decrement units of drop seperation 2
 {
   pulseSep1 = pulseSep1 -1;
   if (pulseSep1 == 0)     // make sure pulse width never goes to zerp
   {
     pulseSep1 = 1;
   }

   if (pulseSep1 <= 9)
   {
     lcd.setCursor(cursorValue -1,1);
     lcd.print(" ");
   }
   else if (pulseSep1 >= 10)
   {
     lcd.setCursor(cursorValue -1,1);

   }

   lcd.print (pulseSep1);
   lcd.setCursor(cursorValue,1);
 }

 else if (cursorValue == 14)  //decrement tens of drop size 3
 {
   pulseWidth2 = pulseWidth2 - 10;

   if (pulseWidth2 <= 9)
   {

     lcd.setCursor(cursorValue,1);  // put cursor in correct position to print just units
     // when the value is less than ten
     lcd.print(" ");
     cursorValue ++;
     lcd.setCursor(cursorValue ,1);
     lcd.print(pulseWidth2);
     lcd.setCursor(cursorValue,1);
   }

   if (pulseWidth2 == 0) //make sure pulswidth never goes to zero
   {
     pulseWidth0 = 1;
   }


   lcd.print(pulseWidth2);
   lcd.setCursor(cursorValue,1);
 }  
 else if (cursorValue == 15)  //decrement units of drop size 3
 {

   pulseWidth2 = pulseWidth2 -1;

   if (pulseWidth2 == 0)     // make sure pulse width never goes to zerp
   {
     pulseWidth2 = 1;
   }

   if (pulseWidth2 <= 9)
   {
     lcd.setCursor(cursorValue -1,1);
     lcd.print(" ");
   }
   else if (pulseWidth2 >= 10)
   {
     lcd.setCursor(cursorValue -1,1);

   }


   lcd.print (pulseWidth2);
   lcd.setCursor(cursorValue,1);
 }  

 return;

}






void outloop()

{

 digitalWrite(backLightControl,backLightOFF);      //Backlight OFF

   delay(1500);

 digitalWrite (timerTrigger,triggerON);   //Trigger the timer box



 if (DropCount == 1)

   //  Output 1 drop  

 {
   digitalWrite(ledpin,ledpinON);        // heartbeat
   digitalWrite(valveControl,VLVON);   //Valve ON
   digitalWrite(dropsAway,dropsAwayON);     //indicator LED on

   delay (pulseWidth0);              //set drop 1 size

   digitalWrite(valveControl,VLVOFF);      //Valve OFF
   digitalWrite(dropsAway,dropsAwayOFF);    //Indicator OFF
   digitalWrite(ledpin,VLVON);
   digitalWrite(timerTrigger,triggerOFF);   //reset the timer trigger

   EEPROM.write(0,DropCount);         // store our variables for next startup
   EEPROM.write(1,pulseWidth0);

   delay(2000);                     //  Make sure Camera is done
   digitalWrite(backLightControl,backLightON);   //Backlight ON
 }


 else if (DropCount == 2)

   // output 2 drops

 {
   digitalWrite(ledpin,ledpinON);
   digitalWrite(valveControl,VLVON);   //Valve ON
   digitalWrite(dropsAway,dropsAwayON);     //indicator LED on

   delay (pulseWidth0);              //set drop 1 size

   digitalWrite(valveControl,VLVOFF);  //Valve OFF
   digitalWrite(timerTrigger,triggerOFF);
   digitalWrite(ledpin,ledpinOFF);
   digitalWrite(dropsAway,dropsAwayOFF);    //Indicator OFF

     delay(pulseSep0);                  //this is the delay that separates the drop 1 & 2

   digitalWrite(ledpin,ledpinON);
   digitalWrite(valveControl,VLVON);   //Valve ON
   digitalWrite(dropsAway,dropsAwayON);     //indicator LED on

   delay (pulseWidth1);              //set drop 2 size

   digitalWrite(valveControl,VLVOFF);  //Valve OFF
   digitalWrite(ledpin,ledpinOFF);
   digitalWrite(dropsAway,dropsAwayOFF);    //Indicator OFF

   EEPROM.write(0,DropCount);        // store our variables for next startup
   EEPROM.write(1,pulseWidth0);
   EEPROM.write(2,pulseSep0);
   EEPROM.write(3,pulseWidth1);

   delay(2000);                      //Make sure Camera is done
   digitalWrite(backLightControl,backLightON);     //Backlight ON
 }



 else if (DropCount == 3)    

   //  Output 3 drops  

 {
   digitalWrite(ledpin,ledpinON);
   digitalWrite(valveControl,VLVON);   //Valve ON
   digitalWrite(dropsAway,dropsAwayON);     //indicator LED on

   delay (pulseWidth0);              //set drop size

   digitalWrite(valveControl,VLVOFF);  //Valve OFF
   digitalWrite(ledpin,ledpinOFF);
   digitalWrite(dropsAway,dropsAwayOFF);    //Indicator OFF
   digitalWrite(timerTrigger,triggerOFF);   // flash timer reset

   delay(pulseSep0);                  //set drop separation

   digitalWrite(ledpin,ledpinON);
   digitalWrite(valveControl,VLVON);    //Valve ON
   digitalWrite(dropsAway,dropsAwayON);     //indicator LED on

   delay (pulseWidth1);               //set drop 2 size

   digitalWrite(valveControl,VLVOFF);   //Valve OFF
   digitalWrite(dropsAway,dropsAwayOFF);    //Indicator OFF
   digitalWrite(ledpin,ledpinOFF);

   delay(pulseSep1);                  //set delay between drop 2 and 3

   digitalWrite(ledpin,ledpinON);
   digitalWrite(valveControl,VLVON);   //Valve ON
   digitalWrite(dropsAway,dropsAwayON);     //indicator LED on

   delay (pulseWidth2);

   digitalWrite(valveControl,VLVOFF);  //Valve OFF
   digitalWrite(dropsAway,dropsAwayOFF);    //Indicator OFF
   digitalWrite(ledpin,ledpinOFF);

   EEPROM.write(0,DropCount);        // store our variables for next startup
   EEPROM.write(1,pulseWidth0);
   EEPROM.write(2,pulseSep0);
   EEPROM.write(3,pulseWidth1);
   EEPROM.write(4,pulseSep1);
   EEPROM.write(5,pulseWidth2);
   
   delay(2000);                      //  Make sure Camera is done
   
   digitalWrite(backLightControl,backLightON);     //Backlight ON

 }            
}
 
