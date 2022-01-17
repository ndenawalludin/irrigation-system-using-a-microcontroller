#include "DHT.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#define DHTPIN 2

#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);


float humidityData;
float temperatureData;


const char* ssid = "Nden Awalludin";// 
const char* password = "poiuytre";
//WiFiClient client;
char server[] = "172.20.10.4";   //eg: 192.168.0.222

void setup()
{
 Serial.begin(9600);
  delay(10);
  dht.begin();
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
//  server.begin();
  Serial.println("Server started");
  Serial.print(WiFi.localIP());
  delay(1000);
  Serial.println("connecting to");
  Serial.println(server);
 }
void loop()
{ 
  humidityData = dht.readHumidity();
  temperatureData = dht.readTemperature(); 
  Sending_To_phpmyadmindatabase(); 
  delay(30000); // interval
 }

 void Sending_To_phpmyadmindatabase()   //CONNECTING WITH MYSQL
 {
   WiFiClient client;
   const int port = 80;
   
   if (client.connect(server, port)) {
    Serial.println("connected");
    // Make a HTTP request:
    Serial.print("GET /etilang/kirimdata.php?humidity=");
    client.print("GET /etilang/kirimdata.php?humidity=");     //YOUR URL
    Serial.println(humidityData);
    client.print(humidityData);
    client.print("&temperature=");
    Serial.println("&temperature=");
    client.print(temperatureData);
    Serial.println(temperatureData);
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: 172.20.10.4");
    client.println("Connection: close");
    client.println();

  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
 }
