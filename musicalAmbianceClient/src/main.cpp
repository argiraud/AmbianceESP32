#include <Arduino.h>
#include <AsyncUDP.h>
#include <WiFi.h>

const char * ssid = "test";
const char * password = "password";
int udpPort = 3333;

String received_message;

AsyncUDP udp;

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

  Serial.println("Listening on Port: " + String(udpPort));
  if(udp.listen(udpPort)){
    Serial.println("UDP Listening on IP: " + WiFi.localIP());
    udp.onPacket([](AsyncUDPPacket packet){
      received_message = packet.readStringUntil('\n');
      received_message.trim();
      Serial.println(received_message);
    });
  }

}

void loop() {
  // put your main code here, to run repeatedly:
}