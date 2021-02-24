#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <AsyncUDP.h>

const char *ssid = "test";
const char *password = "password";

const int led = 13;
const int micpin = 32;

AsyncWebServer server(80);

AsyncUDP udp;
const char * udpAdress = "192.168.4.255";
const int udpPort = 3333;

boolean micOn=false;

void setup()
{
  //----------------------------------------------------Serial
  Serial.begin(115200);
  Serial.println("\n");

  //----------------------------------------------------GPIO
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  pinMode(micpin, INPUT);

  //----------------------------------------------------SPIFFS
  if(!SPIFFS.begin())
  {
    Serial.println("Erreur SPIFFS...");
    return;
  }

  File root = SPIFFS.open("/");
  Serial.println(root.name());
  File file = root.openNextFile();

  while(file)
  {
    Serial.print("File: ");
    Serial.println(file.name());
    file.close();
    file = root.openNextFile();
  }

  //----------------------------------------------------WIFI
  Serial.println("Creation du Point d'Acces ...");
  WiFi.softAP(ssid, password);
  Serial.print("Adresse IP: ");
  Serial.println(WiFi.softAPIP());  
  // WiFi.begin(ssid, password);
	// Serial.print("Tentative de connexion...");
	
	// while(WiFi.status() != WL_CONNECTED)
	// {
	// 	Serial.print(".");
	// 	delay(100);
	// }
	
	// Serial.println("\n");
	// Serial.println("Connexion etablie!");
	// Serial.print("Adresse IP: ");
	// Serial.println(WiFi.localIP());

  //----------------------------------------------------UDP
  udp.listen(udpPort);

  //----------------------------------------------------SERVER
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/script.js", "text/javascript");
  });

  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("udp");
    micOn = false;
    udp.broadcast("On");
    digitalWrite(led, HIGH);
    request->send(200);
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("udp");
    micOn = false;
    udp.broadcast("Off");
    digitalWrite(led, HIGH);
    request->send(200);
  });

  server.on("/chillMode", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("udp");
    micOn = false;
    udp.broadcast("ChillMode");
    request->send(200);
  });

    server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("udp");
    micOn = true;
    udp.broadcast("a/2000");
    request->send(200);
  });

  server.on("/colorMode:", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("udp");
    micOn = false;
    udp.broadcast("ColorMode:");
    request->send(200);
  });

  server.on("/colorMode/green", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("green");
    micOn = false;
    udp.broadcast("ColorMode/g");
    request->send(200);
  });

  server.on("/colorMode/yellow", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("yellow");
    micOn = false;
    udp.broadcast("ColorMode/y");
    request->send(200);
  });

  server.on("/colorMode/red", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("red");
    micOn = false;
    udp.broadcast("ColorMode/r");
    request->send(200);
  });

  server.on("/colorMode/blue", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("blue");
    micOn = false;
    udp.broadcast("ColorMode/b");
    request->send(200);
  });

  server.begin();
  Serial.println("Serveur actif!");
}

void loop()
{
  //Serial.println(analogRead(micpin));
  if(micOn){
    //Serial.println(analogRead(micpin));
    //  String msgString = "a/";
    //String msgString = "a/" + analogRead(micpin);
    // String msgString = String('a/' + (uint16_t)analogRead(micpin));
    char text[6];
    sprintf(text, "a/%d", analogRead(micpin));
    //Serial.println(text);
//now use text as string  
    // Serial.println(msgString);
    // char charBuf[50] = "";
    // msgString.toCharArray(charBuf, 50);  
    // Serial.println(msgString);
    // Serial.println(charBuf);
    udp.broadcast(text);
    delay(500);
  }
}
