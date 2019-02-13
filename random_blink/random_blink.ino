#include <FastLED.h>

#define NUM_LEDS 300
#define LED_PIN 7

CRGB led[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:
  randomSeed(analogRead(0));
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

void showLed() { 
  static byte hue = 0;
  static int randActive = random(300);
  static const unsigned long REFRESH_INTERVAL = 50; // ms
  static unsigned long lastRefreshTime = 0;
  
  if(millis() - lastRefreshTime >= REFRESH_INTERVAL) {
    lastRefreshTime += REFRESH_INTERVAL;

    if (randActive) {
      led[randActive].setHue(hue++);
    }
    // print a random number from 0 to 299
    randActive = random(300);
    
    led[randActive] = CHSV(0, 0, 255);
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    if (isOn(led[i])) {
      led[i].fadeToBlackBy(5);
    }
  }
  FastLED.show();
}


void loop() {
  showLed();
}
