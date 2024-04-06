// Author: Crisu Radu Andrei 
// 

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include "app_constants.h"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
//create a variable for the DHT sensor
DHT dht(DHTPIN, DHTTYPE);
// Json Variable to Hold Sensor Readings
JSONVar readings;
//

// Timer variables
static unsigned long lastUpdate = 0;
static unsigned long pumpTime=0;

static unsigned long startWateringTime=0;

static unsigned long currentTime=0;

// variables used for monitoring the Sprout
float temperature = 0.0;
float humidity = 0.0;
float luminosity=0.0;
float moisture=0.0;
int pumpStatus=PUMP_OFF;
int lightStatus=LIGHT_ON;

int wateringMode=AUTO_MODE;

 unsigned int moisture_analog_read=0;
 unsigned int luminosity_analog_read=0;

//stores elapsed time  since the last check
unsigned int checkMoistureTime=0;

// Get Sensor Readings and return JSON object
String getSensorReadings() {
  readings["temperature"] = temperature;
  readings["humidity"] = humidity;
  readings["luminosity"] = luminosity;
  readings["moisture"] = moisture;
  readings["pumpStatus"]=pumpStatus;
  readings["lightStatus"]=lightStatus;
  readings["wateringInterval"]=(float)WATERING_INTERVAL/T_SECOND;
  readings["checkMoistureTime"]=(float)checkMoistureTime/T_SECOND;
  String jsonString = JSON.stringify(readings);
  return jsonString;
}

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ESP32_SSID,ESP32_PASSWORD);
  //WiFi.begin(ssid, password);
  //Serial.print("Connecting to WiFi ..");
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  //Serial.println(WiFi.localIP());
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  dht.begin();
  initWiFi();
  pinMode(PUMPPIPN, OUTPUT);

  // Serve the HTML content
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  // Request for the latest sensor readings
  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = getSensorReadings();
    request->send(200, "application/json", json);
    json = String();
  });

  // Start server
  server.begin();
}

void loop() {

  //read the current time
  currentTime=millis();
  
  if (currentTime - lastUpdate > READING_INTERVAL) {
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
    moisture_analog_read=analogRead(MOISTUREPIN);
    luminosity_analog_read=analogRead(LUMINOSITYPIN);

    

    luminosity=((float)luminosity_analog_read/4095.0)*100.0;
    moisture=100-((float)moisture_analog_read/4095.0)*100.0;

    Serial.print("Humidity: ");
    Serial.println(humidity);
    Serial.print("Temp: ");
    Serial.println(temperature);
    Serial.print("Moisture: ");
    Serial.println(moisture);
    Serial.print("Luminosity: ");
    Serial.println(luminosity);

    lastUpdate = currentTime;
  }

  checkMoistureTime=currentTime-pumpTime;

  if(checkMoistureTime>=WATERING_INTERVAL)
  {
    if(pumpStatus==PUMP_OFF && moisture<MOISTURE_THRESHOLD)
    {
      pumpStatus=PUMP_ON;
      startWateringTime=currentTime;
    }
     //variable needed to check if it is time to check if the moisture level is normal
     pumpTime=currentTime;

  }

  

  if(currentTime-startWateringTime>WATERING_PERIOD)
  {
    pumpStatus=PUMP_OFF;
    startWateringTime=currentTime;

  }

  if(pumpStatus==PUMP_ON)
  {
     digitalWrite(PUMPPIPN,HIGH);
  }
  else
  {
    digitalWrite(PUMPPIPN,LOW);
  }
  

}