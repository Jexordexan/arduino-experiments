#include <FastLED.h>

#define NUM_LEDS 300
#define LED_PIN 7

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


void loop() {
  static int cursor_one = 0;
  static int cursor_two = NUM_LEDS - 1;
  static byte hue_one = 126;
  static byte hue_two = 0;

  clearLed();
  if (cursor_one >= NUM_LEDS) {
    cursor_one = 0;
  }
  if (cursor_two < 0) {
    cursor_two = NUM_LEDS - 1;
  }

  cursor_one += 1;
  cursor_two -= 1;
  
  setFade(cursor_one, 50, hue_one++, 1);
  setFade(cursor_two, 25, hue_two--, -1);
  showLed();
  delay(10);
}
