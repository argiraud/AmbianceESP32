#include <Arduino.h>
#include <AsyncUDP.h>
#include <WiFi.h>
#include <FastLED.h>

#define LED_PIN 15
#define NUM_LEDS 144
#define MIC_LOW 0
#define MIC_HIGH 644

#define SAMPLE_SIZE 20
#define LONG_TERM_SAMPLES 250
#define BUFFER_DEVIATION 400
#define BUFFER_SIZE 3

CRGB leds[NUM_LEDS];

const char * ssid = "test";
const char * password = "password";
int udpPort = 3333;

String received_message;
boolean fade = false;

struct averageCounter{
  uint16_t *samples;
  uint16_t sample_size;
  uint8_t counter;

  averageCounter(uint16_t size) {
    counter = 0;
    sample_size = size;
    samples = (uint16_t*) malloc(sizeof(uint16_t) * sample_size);
  }

  bool setSample(uint16_t val) {
    if (counter < sample_size) {
      samples[counter++] = val;
      return true;
    }
    else {
      counter = 0;
      return false;
    }
  }

  int computeAverage() {
    int accumulator = 0;
    for (int i = 0; i < sample_size; i++) {
      accumulator += samples[i];
    }
    return (int)(accumulator / sample_size);
  }

};

struct averageCounter *samples;
struct averageCounter *longTermSamples;
struct averageCounter* sanityBuffer;

float globalHue;
float globalBrightness = 255;
int hueOffset = 120;
float fadeScale = 1.3;
float hueIncrement = 0.7;

AsyncUDP udp;

enum Options {
  On,
  Off,
  ChillMode,
  AmbianceMode,
  ColorMode,
  OptionInvalid
};

Options resolveOption(String input){
  if (input == "On") return On;
  if (input == "Off") return Off;
  if (input == "ChillMode") return ChillMode;
  if (input == "Test") return AmbianceMode;
  if (input == "ColorMode") return ColorMode;
  return OptionInvalid;
}

void allWhite() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(255, 255, 235);
  }
  delay(5);
  FastLED.show();
}

void allOff() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 0);
  }
  delay(5);
  FastLED.show();
}

void chillFade() {
  static int fadeVal = 0;
  static int counter = 0;
  static int from[3] = {0, 234, 255};
  static int to[3]   = {255, 0, 214};
  static int i, j;
  static double dsteps = 500.0;
  static double s1, s2, s3, tmp1, tmp2, tmp3;
  static bool reverse = false;
  if (fade == false) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(from[0], from[1], from[2]);
    }
    s1 = double((to[0] - from[0])) / dsteps; 
    s2 = double((to[1] - from[1])) / dsteps; 
    s3 = double((to[2] - from[2])) / dsteps; 
    tmp1 = from[0], tmp2 = from[1], tmp3 = from[2];
    fade = true;
  }

  if (!reverse) 
  {
    tmp1 += s1;
    tmp2 += s2; 
    tmp3 += s3; 
  }
  else 
  {
    tmp1 -= s1;
    tmp2 -= s2; 
    tmp3 -= s3; 
  }

  for (j = 0; j < NUM_LEDS; j++)
    leds[j] = CRGB((int)round(tmp1), (int)round(tmp2), (int)round(tmp3)); 
  FastLED.show(); 
  delay(5);

  counter++;
  if (counter == (int)dsteps) {
    reverse = !reverse;
    tmp1 = to[0], tmp2 = to[1], tmp3 = to[2];
    counter = 0;
  }
}

float fscale(float originalMin, float originalMax, float newBegin, float newEnd, float inputValue, float curve)
{

  float OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  boolean invFlag = 0;

  // condition curve parameter
  // limit range

  if (curve > 10)
    curve = 10;
  if (curve < -10)
    curve = -10;

  curve = (curve * -.1);  // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output
  curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function

  // Check for out of range inputValues
  if (inputValue < originalMin)
  {
    inputValue = originalMin;
  }
  if (inputValue > originalMax)
  {
    inputValue = originalMax;
  }

  // Zero Refference the values
  OriginalRange = originalMax - originalMin;

  if (newEnd > newBegin)
  {
    NewRange = newEnd - newBegin;
  }
  else
  {
    NewRange = newBegin - newEnd;
    invFlag = 1;
  }

  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal = zeroRefCurVal / OriginalRange; // normalize to 0 - 1 float

  // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine
  if (originalMin > originalMax)
  {
    return 0;
  }

  if (invFlag == 0)
  {
    rangedValue = (pow(normalizedCurVal, curve) * NewRange) + newBegin;
  }
  else // invert the ranges
  {
    rangedValue = newBegin - (pow(normalizedCurVal, curve) * NewRange);
  }

  return rangedValue;
}

void soundReactive(int analogRaw) {
 Serial.println("debut");
 int sanityValue = sanityBuffer->computeAverage();
 Serial.println("AvantIf");
 if (!(abs(analogRaw - sanityValue) > BUFFER_DEVIATION)) {
    sanityBuffer->setSample(analogRaw);
 }
 Serial.println("ApresIf");
  analogRaw = fscale(MIC_LOW, MIC_HIGH, MIC_LOW, MIC_HIGH, analogRaw, 0.4);

  if (samples->setSample(analogRaw))
    return;
    Serial.println("Apres2If");
  uint16_t longTermAverage = longTermSamples->computeAverage();
  uint16_t useVal = samples->computeAverage();
  longTermSamples->setSample(useVal);

  int diff = (useVal - longTermAverage);
  if (diff > 5)
  {
    if (globalHue < 235)
    {
      globalHue += hueIncrement;
    }
  }
  else if (diff < -5)
  {
    if (globalHue > 2)
    {
      globalHue -= hueIncrement;
    }
  }

Serial.println("Avantfscale");
  int curshow = fscale(MIC_LOW, MIC_HIGH, 0.0, (float)NUM_LEDS, (float)useVal, 0);
  //int curshow = map(useVal, MIC_LOW, MIC_HIGH, 0, NUM_LEDS)
Serial.println("Apresfascale");
  for (int i = 0; i < NUM_LEDS; i++)
  {
    if (i < curshow)
    {
      leds[i] = CHSV(globalHue + hueOffset + (i * 2), 255, 255);
    }
    else
    {
      leds[i] = CRGB(leds[i].r / fadeScale, leds[i].g / fadeScale, leds[i].b / fadeScale);
    }
    
  }
  delay(5);
  FastLED.show(); 
}

void changeMode(String msg){
  switch (resolveOption(msg))
  {
  case On:
    allWhite();
    break;

  case Off:
    allOff();
    break;
  
  case ChillMode:
    chillFade();
    break;

  case AmbianceMode:
    Serial.println("AmbianceMode");
    fade = false;
    soundReactive(random(200));
    break;

  case ColorMode:
    allOff();
    break;
  
  default:
    Serial.print("Default");
    Serial.println(msg);
    allOff();
    break;
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Tentative de connexion...");
  while(WiFi.status() != WL_CONNECTED)
  {
	  Serial.print(".");
	  delay(100);
  }
  Serial.println("\n");

  if(udp.listen(udpPort)){
    Serial.println("Ecoute sur le port: " + String(udpPort));
    udp.onPacket([](AsyncUDPPacket packet){
      Serial.println((char*)packet.data());
      received_message = String((char*)packet.data());
      received_message.trim();
      Serial.println(received_message);
      // changeMode(msg);
    });
  }

  globalHue = 0;
  samples = new averageCounter(SAMPLE_SIZE);
  longTermSamples = new averageCounter(LONG_TERM_SAMPLES);
  sanityBuffer    = new averageCounter(BUFFER_SIZE);
  
  while(sanityBuffer->setSample(250) == true) {}
  while (longTermSamples->setSample(200) == true) {}
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
}

void loop() {
  changeMode(received_message);
}
