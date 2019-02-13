#include <FastLED.h>

#define NUM_LEDS 300
#define LED_PIN 7
#define JOY_X A0
#define JOY_Y A1

CRGB led[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, LED_PIN>(led, NUM_LEDS);
  Serial.begin(9600);  

  for (int i = 0; i < NUM_LEDS; i++) {
    led[i] = CRGB(0, 0, 0);
  }

  FastLED.show();
}

void clearLed() {
  for (int i = 0; i < NUM_LEDS; i++) {
    led[i] = CRGB(0, 0, 0);
  }
}

bool isOn(CRGB led) {
  return led != CRGB(0, 0, 0);
}

CRGB mix(CRGB led1, CRGB led2) {
  return (led1 /= 2) + (led2 /= 2);
}

void setFade(int start, int spread, byte hue, int dir) {
  int index = start;
  byte brightness = 0;
  byte saturation = 255;
  for (int i = 0; i < spread; i++) {
    index = start - (i * dir);
    if (index < 0) {
      index = NUM_LEDS + index;
    }
    if (index >= NUM_LEDS) {
      index = index - NUM_LEDS;
    }
    if (i == 0) {
      saturation = 0;
    } else {
      saturation = 255;
    }
    
    brightness = map(i, 0, spread, 255, 0);
    
    
    if (isOn(led[index])) {
      led[index] = mix(led[index], CHSV(hue, saturation, brightness));
    } else {
      led[index] = CHSV(hue, saturation, brightness);
    }
    
  }
}

void showLed() { 
  FastLED.show();
}

const int numReadings = 10;

int getJoyX() {
  static int readings[numReadings];      // the readings from the analog input
  static int readIndex = 0;              // the index of the current reading
  static int total = 0;                  // the running total
  static int average = 0;                // the average

  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(JOY_X);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;

  return average;
}


int getJoyY() {
  static int readings[numReadings];      // the readings from the analog input
  static int readIndex = 0;              // the index of the current reading
  static int total = 0;                  // the running total
  static int average = 0;                // the average

  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(JOY_Y);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;

  return average;
}


void loop() {
  static int cursor1 = 0;
  static int cursor2 = NUM_LEDS - 1;
  static byte hue1 = 126;
  static byte hue2 = 0;

//  int joy_x = getJoyX();
//  int joy_y = getJoyY();

  int joy_x = analogRead(JOY_X);
  int joy_y = analogRead(JOY_Y);

  Serial.println(joy_x);
  int cursor1_v = map(joy_x, 0, 1000, -1, 1);
  int cursor2_v = map(joy_y, 0, 1023, -1, 1);
  clearLed();
  if (cursor1 >= NUM_LEDS) {
    cursor1 = 0;
  }
  if (cursor2 < 0) {
    cursor2 = NUM_LEDS - 1;
  }

  cursor1 += cursor1_v;
  cursor2 += cursor2_v;
  
  setFade(cursor1, 25, hue1++, 1);
  setFade(cursor2, 25, hue2++, -1);
  showLed();
  delay(10);
}
