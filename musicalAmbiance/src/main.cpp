#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <AsyncUDP.h>

const char *ssid = "test";
const char *password = "password";

const int led = 13;
const int capteurLuminosite = 34;

AsyncWebServer server(80);

AsyncUDP udp;
const char * udpAdress = "192.168.4.255";
const int udpPort = 3333;

void setup()
{
  //----------------------------------------------------Serial
  Serial.begin(115200);
  Serial.println("\n");

  //----------------------------------------------------GPIO
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  pinMode(capteurLuminosite, INPUT);

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

  server.on("/lireLuminosite", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    int val = analogRead(capteurLuminosite);
    String luminosite = String(val);
    request->send(200, "text/plain", luminosite);
  });

  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("udp");
    udp.broadcast("On");
    digitalWrite(led, HIGH);
    request->send(200);
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("udp");
    udp.broadcast("Off");
    digitalWrite(led, HIGH);
    request->send(200);
  });

  server.begin();
  Serial.println("Serveur actif!");
}

void loop()
{
}