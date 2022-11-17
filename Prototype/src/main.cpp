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



void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // will read temperature and humidity values
  dht_sensor.measure(temperature, humidity);

  
}