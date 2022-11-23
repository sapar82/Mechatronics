#include <Arduino.h>
#include <string.h>

// Temperature and humidity sensor
#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Ultrasonic sensor
#include "SR04.h"
int TRIG_PIN = 2;
int ECHO_PIN = 3;
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
int distance;
bool someone_was_there = false;
const int distance_treshold = 30;
int counter = 0;


//Real time clock
#include <Wire.h>
#include <DS3231.h>
DS3231 clock;
RTCDateTime dt;
String time;
String date;
String filename = String("data.txt");

// SD card
#include <SPI.h>
#include <SD.h>

File myFile;

void initialization_sd(){
  // Create a file on the SD card
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  if (SD.exists("data.txt")) {
    Serial.println("data.txt exists. Appending data.");
  } else {
    Serial.println("data.txt doesn't exist. Creating a new file.");
    myFile = SD.open("data.txt", FILE_WRITE);
    myFile.println("Date,Time, Temperature,Humidity,Number of people");
    myFile.close();
  }
}

void write_data( String date, String time, float temperature, float humidity, int counter){
  myFile = SD.open("data.txt", FILE_WRITE);
  if (myFile) {
    myFile.println(date + String(",") + time + String(",") + String(temperature) + String(",") + String(humidity) + String(",") + String(counter));
    myFile.close();
    Serial.println("Data written to file");
  } else {
    Serial.println("error opening data.txt");
  }
}

// Define pins
int DistanceSensorTriggerPin = 3;
int SDcardPin = 10;




void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");
  clock.begin();
  dht.begin();
  clock.setDateTime(__DATE__, __TIME__);    // Set the RTC to the compile time
  Serial.print("Time: ");
  Serial.println(__TIME__);
  initialization_sd();
  Serial.print("Setup completed");
  delay(1000);
}

void loop() {
  distance=sr04.Distance();
  if (distance < distance_treshold){
    if (someone_was_there == false){
      counter++;
    }
    someone_was_there = true;
  }
  else{
    someone_was_there = false;
  }

  dt = clock.getDateTime();
  if ( dt.second == 0 || dt.second == 20 || dt.second == 40 || dt.second == 30)
  {
    // Get time
    time = String(dt.hour) + String(":") + String(dt.minute) + String(":") + String(dt.second);
    date = String(dt.day) + String("/") + String(dt.month) + String("/") + String(dt.year);

    // Get temperature and humidity
    float humidity = dht.readHumidity();
    //read temperature in Fahrenheit
    float temperature = dht.readTemperature(true);

    // Write to SD card
    write_data(date, time, temperature, humidity, counter);
    Serial.println("Data written to file:");
    Serial.println(date + String(",") + time + String(",") + String(temperature) + String(",") + String(humidity) + String(",") + String(counter));
    delay(1000);
  }
}