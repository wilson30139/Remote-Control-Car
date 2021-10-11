// include the library code:
#include "LiquidCrystal_74HC595.h"

// Data pin is #3, Clock is #2 and Latch is #4
LiquidCrystal_74HC595 lcd(3, 2, 4);

void setup() {
  // set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Data Structure");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis()/1000);
}
