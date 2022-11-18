#include <Arduino.h>
#include "sensors.h"
// Temperature and humidity sensor

float* temperature; 
float* humidity; 
int DHT_SENSOR_PIN = 4;
#define DHT_SENSOR_TYPE DHT_TYPE_11

DHT_sensor DHT_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

// Ultrasonic sensor
int TRIG_PIN = 10;
int ECHO_PIN = 9;
int distance_treshold = 1000;
ultrasonic_sensor ultrasonic_sensor(TriG_PIN, ECHO_PIN, distance_treshold);





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
int SDcardPin = 10;


File myFile;

void writeFile(File myFile, String data) {
  if (myFile) {
    myFile.println(data);
  } else {
    Serial.println("error opening test.txt");
  }
}


void setup() {

}

void loop() {

  
}