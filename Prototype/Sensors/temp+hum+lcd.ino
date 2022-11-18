// include the library code:
#include <LiquidCrystal.h>
#include "DHT.h"

// set the DHT Pin
#define DHTPIN 8

class LCD_screen {
  public:
  int RS;
  int E;
  int D4;
  int D5;
  int D6;
  int D7;
  int DHTPIN;
  LCD_screen(int RS, int E, int D4, int D5, int D6, int D7) {
    RS = RS;
    E = E;
    D4 = D4;
    D5 = D5;
    D6 = D6;
    D7 = D7;
    LiquidCrystal(RS, E, D4, D5, D6, D7);
  }
  void setup{
    // initialize the library by associating any needed LCD interface pin
    // with the arduino pin number it is connected to
    // initialize the library by associating any needed LCD interface pin
    // with the arduino pin number it is connected to
    
    lcd.begin(16, 2);
    lcd.print("Inialization");
    delay(1000);
    lcd.print("Temp:  Humidity:");
  }
  void update_data(int temperature, int humidity) {
    // Print temperature and humidity
    lcd.setCursor(0, 1);
    lcd.print(f);
    lcd.setCursor(7,1);
    lcd.print(h);  
  }
}

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  dht.begin();
  
  // Print a message to the LCD.
  lcd.print("Temp:  Humidity:");
}

void loop() {
  delay(500);
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // read humidity
  float h = dht.readHumidity();
  //read temperature in Fahrenheit
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(f)) {
    lcd.print("ERROR");
    return;
  }

  lcd.print(f);
  lcd.setCursor(7,1);
  lcd.print(h);  
}
