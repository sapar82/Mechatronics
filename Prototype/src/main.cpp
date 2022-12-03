#include <Arduino.h>
#include <string.h>

// Temperature and humidity sensor
#include "DHT.h"
#define DHTPIN 11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float temperature;
float humidity;

// Ultrasonic sensor
#include "SR04.h"
int TRIG_PIN = 5;
int ECHO_PIN = 6;
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
int distance;
bool someone_was_there = false;
bool someone_was_there_2 = false;
const int distance_treshold = 30;
int counter = 0;

int TRIG_PIN_2 = 3;
int ECHO_PIN_2 = 4;
SR04 sr04_2 = SR04(ECHO_PIN_2,TRIG_PIN_2);

// Display
#include <epd2in9_V2.h>
#include <epdpaint.h>
#include <imagedata.h>

#define COLORED     0
#define UNCOLORED   1

unsigned char image[1024];
Paint paint(image, 0, 0);    // width should be the multiple of 8 
Epd epd;
unsigned long time_start_ms;
unsigned long time_now_s;

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

  if (!SD.begin(53)) {
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
  } else {
    Serial.println("error opening data.txt");
  }
}

void init_screen()
{Serial.begin(9600);
  if (epd.Init() != 0) {
      Serial.print("e-Paper init failed");
      return;
  }
  
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  

  if (epd.Init() != 0) {
      Serial.print("e-Paper init failed ");
      return;}
  epd.SetFrameMemory_Base(IMAGE_DATA);
  epd.DisplayFrame();

  time_start_ms = millis();      
}

void loop_screen(String time, String counter, String temperature, String humidity) {
  // put your main code here, to run repeatedly:
  char time_string[] = {'C','u','r','r','e','n','t',' ','t','i','m','e',':', '0', '0', ':', '0', '0','\0'};
  time_string[13] = time[0];
  time_string[14] = time[1];
  time_string[16] = time[3];
  time_string[17] = time[4];
  paint.SetWidth(32);
  paint.SetHeight(238);
  paint.SetRotate(ROTATE_90);

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 4, time_string, &Font16, COLORED);
  epd.SetFrameMemory_Partial(paint.GetImage(), 100, 00, paint.GetWidth(), paint.GetHeight());
  
  char temp_string[] = {'T', 'e', 'm', 'p', 'e', 'r', 'a', 't', 'u', 'r', 'e', ':', ' ', '-', '-', '.', '-', 'C', '\0'};

  temp_string[13] = temperature[0];
  temp_string[14] = temperature[1];
  temp_string[16] = temperature[3];

  paint.SetWidth(32);
  paint.SetHeight(220);
  paint.SetRotate(ROTATE_90);

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 4, temp_string, &Font16, COLORED);
  epd.SetFrameMemory_Partial(paint.GetImage(), 70, 0, paint.GetWidth(), paint.GetHeight());

  char humidity_string[] = {'H', 'u', 'm', 'i', 'd', 'i', 't', 'y', ':', ' ', '-', '-', '.','-','-', '%', '\0'};
  humidity_string[10] = humidity[0];
  humidity_string[11] = humidity[1];
  humidity_string[13] = humidity[3];
  humidity_string[14] = humidity[4];



  paint.SetWidth(32);
  paint.SetHeight(220);
  paint.SetRotate(ROTATE_90);

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 4, humidity_string, &Font16, COLORED);
  epd.SetFrameMemory_Partial(paint.GetImage(), 40, 0, paint.GetWidth(), paint.GetHeight());

  char counter_string[] = {'P', 'e', 'o', 'p', 'l', 'e', ' ', 'i', 'n', 's', 'i', 'd', 'e', ':', ' ', '0', '0', '0', '0', '\0'};

  counter_string[15] = counter[0];
  counter_string[16] = counter[1];
  counter_string[17] = counter[2];
  counter_string[18] = counter[3];

  paint.SetWidth(32);
  paint.SetHeight(240);
  paint.SetRotate(ROTATE_90);

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 4, counter_string, &Font16, COLORED);
  epd.SetFrameMemory_Partial(paint.GetImage(), 10, 0, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame_Partial();

  // delay(300);
}


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
  init_screen();
}

void loop() {
  distance = sr04.Distance();
  if (distance < distance_treshold){
    if (!someone_was_there){
      counter++;
      Serial.print("Someone entered the room. Distance: ");
      Serial.println(distance);
      delay(1000);
    }
    someone_was_there = true;
  }
  else{
    someone_was_there = false;
  }
  distance = sr04_2.Distance();
  if (distance < distance_treshold){
    if (!someone_was_there_2){
      counter--;
      Serial.print("Someone left the room. Distance: ");
      Serial.println(distance);
      delay(1000);
    }
    someone_was_there = true;
  }
  else{
    someone_was_there = false;
  }

  dt = clock.getDateTime();
  if ( dt.second % 30 == 0)
  {
    Serial.println("Reading sensor");
    // Get time
    time = String(dt.hour/10) + String(dt.hour % 10) + String(":") + String(dt.minute / 10) + String(dt.minute % 10 )+ String(":") + String(dt.second / 10) + String(dt.second % 10);
    date = String(dt.day) + String("/") + String(dt.month);

    // Get temperature and humidity
    humidity = dht.readHumidity(true);
    //read temperature in Fahrenheit
    temperature = dht.readTemperature(true);

    // Write to SD card
    Serial.println("Writing to SD card");
    write_data(date, time, temperature, humidity, counter);
    Serial.println("Data written to file:");
    Serial.println(date + String(",") + time + String(",") + String(temperature) + String(",") + String(humidity) + String(",") + String(counter));
  }
  String counter_string = String(counter/1000) + String(counter/100 % 10) + String(counter/10 % 10) + String(counter % 10);
  if (dt.second % 5 == 0){
    loop_screen(String(time), String(counter), String(temperature), String(humidity));
  }
  
}