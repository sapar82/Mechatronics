#include <Arduino.h>
// Temperature and humidity sensor
#include <dht_nonblocking.h>
//Initialize humidity and temperature values, create sensor object
public float temperature; 
public float humidity; 
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_TYPE_11 );


// SD card
#include <SPI.h>
#include <SD.h>
// Distance sensor
#include "SR04.h"
//Real time clock
#include <Wire.h>
#include <DS3231.h>
DS3231 clock;
RTCDateTime dt;
String time;
String date;


// Define pins
int DistanceSensorTriggerPin = 3;
int DHT_SENSOR_PIN = 2;
int HumiditySensorPin = 4;
int SDcardPin = 10;


#define DHT_SENSOR_TYPE DHT_TYPE_11
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

File myFile;

void writeFile(File myFile, String data) {
  if (myFile) {
    myFile.println(data);
    Serial.println("Data written to file");
  } else {
    Serial.println("error opening test.txt");
  }
}


void setup() {
  clock.begin();
  clock.setDateTime(__DATE__, __TIME__);    
  if (!SD.begin(SDcardPin)) {
    Serial.println("initialization failed!");
    while (1);}
  Serial.begin(9600);
  myFile = SD.open("test.txt", FILE_WRITE);
  writeFile(myFile, "Hello World!");
  // put your setup code here, to run once:
}

void loop() {

  myFile = SD.open("test.txt", FILE_WRITE);
  for (int i = 0; i < 10; i++) {
    dt = clock.getDateTime();
    time = String(dt.hour) + ":" + String(dt.minute) + ":" + String(dt.second);
    date = String(dt.day) + "/" + String(dt.month) + "/" + String(dt.year);
    Serial.println(time);
    Serial.println(date);
    writeFile(myFile, time);
    delay(1000);
  }
  myFile.close();

  // put your main code here, to run repeatedly:
  // will read temperature and humidity values
  dht_sensor.measure(temperature, humidity);

  
}