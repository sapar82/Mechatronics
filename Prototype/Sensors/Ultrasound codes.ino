//www.elegoo.com
//2016.12.08
#include "SR04.h"
#define TRIG_PIN 10
#define ECHO_PIN 9

SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);

int dist;
const int threshold = 1000;
int counter = 0;

void setup() {
   Serial.begin(9600);
   delay(500);
}

void loop() {
  dist=sr04.Distance();
  if (dist < thresold){
    if (state == false){
      counter++;
      state = true;
    }
  }
  else{
    state = false;
  }
}














