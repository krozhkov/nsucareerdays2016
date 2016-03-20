#include <map>
#include <algorithm>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid     = "arbuz";
const char* password = "arbuz321";
const int httpPort = 80;
const char* host = "ngurestexample.us-east-1.elasticbeanstalk.com";

// http://stackoverflow.com/questions/33450946/esp8266-for-arduino-ide-xtensa-lx106-elf-gcc-and-stdmap-linking-error
std::map <char*,int> lights = {{"light01", 5}, {"light02", 4},{"light03", 2}};

void setupPins() {
  for (auto it = lights.begin(); it != lights.end(); ++it)
  {
    pinMode(it->second, OUTPUT);
    digitalWrite(it->second, LOW);
  }
}

void setup() {
  Serial.begin(115200);

  setupPins();
  
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  
  while (true) {
    Serial.println("Attempting to connect to wifi.\n");
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() == WL_CONNECTED) {
      break;
    }
    delay(10000);
  }
}

void loop() {
  HTTPClient http;
  http.begin(host, httpPort, "/");

  int httpCode = http.GET();
  if(httpCode) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if(httpCode == 200) {
          String payload = http.getString();
          Serial.println(payload);
          DynamicJsonBuffer jsonBuffer;
          JsonObject& root = jsonBuffer.parseObject(payload, 1);

          for (auto it = lights.begin(); it != lights.end(); ++it)
          {
              boolean lightOn = root[it->first].as<boolean>();
              Serial.printf("%s: %s\n", it->first, lightOn ? "true" : "false");
              digitalWrite(it->second, lightOn ? HIGH : LOW);
          }
      }
  } else {
      Serial.print("[HTTP] GET... failed, no connection or no HTTP server\n");
  }

  delay(1000);
}

