// Author : CRISU RADU ANDREI

#ifndef APP_CONSTANTS_H
#define APP_CONSTANTS_H

//other libraries
#include <DHT.h>


// PC hotspot
//const char* PC_SSID = "-----------";
//const char* PC_PASSWORD = "**********";
//ESP32 wifi
const char* ESP32_SSID="Sprout";
const char* ESP32_PASSWORD="TSG2023RKO";

#define T_SECOND 1000 // it has 1000 ms
#define T_MINUTE 60*T_SECOND

//define pump status
#define PUMP_OFF 0
#define PUMP_ON 255 // a value diffrent from 0

#define LIGHT_OFF 0
#define LIGHT_ON 255 // a value diffrent from 0

#define DHTPIN 26
#define DHTTYPE DHT11

#define MOISTUREPIN 39
#define LUMINOSITYPIN 35
#define PUMPPIPN 25

//the value of the time interval for 
//reading new data from sensors
#define READING_INTERVAL 2*T_SECOND

//the value of the time interval for
//checking the moisture level
//and activation of water pump if necessary
#define WATERING_INTERVAL 2*T_MINUTE

//the period while the pump is on 
// and provids water to  the plant
#define WATERING_PERIOD 5* T_SECOND

#define AUTO_MODE 0
#define MANUAL_MODE 255

//the value used to activate the water pump
//if the detected moisture is under 25%  the pump will be activated 
#define MOISTURE_THRESHOLD 25

// HTML content
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 Sensor Data</title>
</head>
<body>
  <h1>ESP32 Sensor Data</h1>
  <p>Temperature: <span id="temperature">0</span></p>
  <p>Humidity: <span id="humidity">0</span></p>

  <script>
    function updateReadings() {
      fetch('/readings')
        .then(response => response.json())
        .then(data => {
          document.getElementById('temperature').innerText = data.temperature;
          document.getElementById('humidity').innerText = data.humidity;
        })
        .catch(error => {
          console.error('Error fetching readings:', error);
        });
    }

    setInterval(updateReadings, 2000);
  </script>
</body>
</html>
)rawliteral";



#endif