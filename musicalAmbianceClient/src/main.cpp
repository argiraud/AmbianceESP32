#include <Arduino.h>
#include <AsyncUDP.h>
#include <WiFi.h>
#include <FastLED.h>

#define LED_PIN 15
#define NUM_LEDS 144
CRGB leds[NUM_LEDS];

const char * ssid = "test";
const char * password = "password";
int udpPort = 3333;

String received_message;

AsyncUDP udp;

enum Options {
  On,
  Off,
  ChillMode,
  AmbianceMode,
  ColorMode,
  OptionInvalid
};

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
      Serial.println("Source: " + packet.remoteIP());
      Serial.print("Message reçu: ");
      Serial.write(packet.data(), packet.length());
      Serial.println("\n");
      String msg = String((char*)packet.data());
      changeMode(msg);
    });
  }
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
}

void loop() {
  // put your main code here, to run repeatedly:
}

Options resolveOption(String input){
  if (input == "On") return On;
  if (input == "Off") return Off;
  if (input == "ChillMode") return ChillMode;
  if (input == "AmbianceMode") return AmbianceMode;
  if (input == "ColorMode") return ColorMode;
  return OptionInvalid;
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
    allOff();
    break;

  case AmbianceMode:
    allOff();
    break;

  case ColorMode:
    allOff();
    break;
  
  default:
    break;
  }
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