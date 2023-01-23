#include <Arduino.h>
#include <string.h>

// Temperature and humidty sensor
#include "DHT.h"
#define DHTPIN 11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float temperature;
float humidity;

// Ultrasonic sensor
#include "SR04.h"
int TRIG_PIN = 5; // assign pin 5 and 6 to first ultrasonic sensor
int ECHO_PIN = 6;
SR04 dist_sensor_in = SR04(ECHO_PIN,TRIG_PIN); // first sensor

int TRIG_PIN_2 = 3; // assign pin 3 and 4 to second ultrasonic sensor
int ECHO_PIN_2 = 4;
SR04 dist_sensor_out = SR04(ECHO_PIN_2,TRIG_PIN_2); // second sensor
// initialize variables for sensors 
int distance_in; // distance measured by sensor 1
int distance_out; // distance measured by sensor 2
int counter = 0; // number of people inside 
bool previous_state_in = false;
bool previous_state_out = false;
const int distance_treshold = 30; // decision threshold 

// Display
#include <epd2in9_V2.h>
#include <epdpaint.h>
#include <imagedata.h>

#define COLORED     0
#define UNCOLORED   1

unsigned char image[1024];
Paint paint(image, 0, 0);    // width should be the multiple of 8 
Epd epd;
unsigned long current_time;
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
    // Verify the SD card is present and can be initialized.
    while (1);
  }
  Serial.println("initialization done.");

  if (SD.exists("data.txt")) { // if the file exists, append data to it: 
    Serial.println("data.txt exists. Appending data.");
  } else {
    Serial.println("data.txt doesn't exist. Creating a new file.");
    myFile = SD.open("data.txt", FILE_WRITE); // else create a file if none detected
    myFile.println("Date, Time, Temperature, Humidity, Number of people");
    myFile.close();
  }
}

// function to write data to SD card
void write_data( String date, String time, float temperature, float humidity, int counter){
  myFile = SD.open("data.txt", FILE_WRITE);
  if (myFile) {
    myFile.println(date + String(",") + time + String(",") + String(temperature) + String(",") + String(humidity) + String(",") + String(counter));
    myFile.close();
  } else {
    Serial.println("error opening data.txt");
  }
}

// function to initialize e-paper screen
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

  current_time = millis();      
}

// function to update the screen with new measurements every 5 min
void loop_screen(String time, String counter, String temperature, String humidity) {
  // Update for time
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
  // update for temperature
  char temp_string[] = {'T', 'e', 'm', 'p', 'e', 'r', 'a', 't', 'u', 'r', 'e', ':', ' ', '-', '-', '.', '-', 'F', '\0'};

  temp_string[13] = temperature[0];
  temp_string[14] = temperature[1];
  temp_string[16] = temperature[3];

  paint.SetWidth(32);
  paint.SetHeight(220);
  paint.SetRotate(ROTATE_90);

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 4, temp_string, &Font16, COLORED);
  epd.SetFrameMemory_Partial(paint.GetImage(), 70, 0, paint.GetWidth(), paint.GetHeight());
  // update for humidity
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
  // update for person counter 
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
}

// function of the sensors that update the counter when persons go in and out of detected area
void call_sensor(){
    distance_in = dist_sensor_in.Distance(); // Detect when a person enters
  if (distance_in < distance_treshold){
  if (!previous_state_in){
    counter++; // only adds 1 if person just entered the field
    previous_state_in = true; 
    }
  }
  else{
    previous_state_in = false;
  }

  Serial.print("Distance sensor in: "); // for debugging and knowing the rate of detection of sensor
  Serial.print(distance_in);
  Serial.print("\n");

  distance_out = dist_sensor_out.Distance(); // Detect when a person leaves
  if (distance_out < distance_treshold){
    if (!previous_state_out){
      counter--; // only substract 1 if person just entered the field
      previous_state_out = true;
    }
  }
  else{
    previous_state_out = false;
  }

  Serial.print("Distance sensor out: "); // for debugging and knowing the rate of detection of sensor
  Serial.print(distance_out);
  Serial.print("\n");
}
// setup code goes here 
void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");
  clock.begin();
  dht.begin();
  clock.setDateTime(__DATE__, __TIME__);    // Set the RTC to the compile time
  Serial.print("Time: ");
  Serial.println(__TIME__);
  initialization_sd();
  delay(200);
  init_screen();
  Serial.print("Setup completed");
}

// loop code goes here 
void loop() {
  
  call_sensor(); // ultrasound sensors measurements

  dt = clock.getDateTime(); // get data from clock module
  if ( dt.second % 315 == 0 ) // every 5 min 
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
  // string for person counter
  String counter_string = String(counter/1000) + String(counter/100 % 10) + String(counter/10 % 10) + String(counter % 10);
    loop_screen(String(time), String(counter), String(temperature), String(humidity)); // update screen with current values
  delay(1000); // idk if we need this
}
