#include <string.h>
#include <LiquidCrystal.h>

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
  }
  LiquidCrystal lcd(int RS,int E,int D4,int D5,int D6, int D7)
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
    lcd.print(temperature);
    lcd.setCursor(7,1);
    lcd.print(humidity);  
  }
}


#include "SR04.h"

class Ultrasonic_sensor {
  public:
  int trigPin;
  int echoPin;
  int number_of_people;
  int distance_threshold;
  bool someone_in_distance;

  Ultrasonic_sensor(int trigPin, int echoPin, int distance_threshold) {
    trigPin = trigPin;
    echoPin = echoPin;
    distance_threshold = distance_threshold;
    someone_in_distance = false;
    number_of_people = 0;
    SR04(trigPin, echoPin);
  }
  void update_data() {
    //Everytime this method is called, it will update the number of people in the room by
    //checking if someone is in the distance threshold and if the sensor is not already detecting someone.
    if (SR04.Distance() > distance_thresold){
      if (someone_in_distance == true){
        number_of_people++;
        someone_in_distance = false;
      }
    }
    else{
      someone_in_distance = true;
    }
  }
}

#include <SPI.h>
#include <SD.h>

class SD_file{
  public:
  int PinChipSelect;
  String file_name;
  SD_file(int chipSelect, String file_name) {
    chipSelect = PinChipSelect;
    file_name = file_name;
    SD.begin(PinChipSelect);
  }
  void write_data(String timestamp, int temperature, int humidity, int number_of_people){
    // create a file if it doesn't exist, and open it in write mode:
    File dataFile = SD.open(file_name, FILE_WRITE);
    // if the file opened okay, write to it:
    if (dataFile) {
      dataFile.print(timestamp);
      dataFile.print(",");
      dataFile.print(temperature);
      dataFile.print(",");
      dataFile.print(humidity);
      dataFile.print(",");
      dataFile.println(number_of_people);
      // close the file:
      dataFile.close();
    } else {
      // if the file didn't open, print an error:
      if (Serial) {
        Serial.println("error opening datalog.txt");
      }
    }
  }
}

#include <dht_nonblocking.h>

class DHT_sensor {
  public:
  int DHTPIN;
  #define DHTTYPE DHT_TYPE_11
  DHT_nonblocking dht;

  DHT_sensor(int DHTPIN) {
    DHTPIN = DHTPIN;
    DHTTYPE = DHTTYPE;
    dht = DHT_nonblocking(DHTPIN, DHTTYPE);
    }
  }
}
