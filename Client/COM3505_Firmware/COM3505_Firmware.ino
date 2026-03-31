#include <WiFi.h>
#include <HTTPClient.h>
#include <Preferences.h>

#include "Easings.h"
#include "Leds.h"
#include "Sensor.h"

// const char* default_net_ssid = "thing";
// const char* default_net_password = "password";
// const char* default_server_endpoint = "http://10.61.23.177:5000/api/data";

String net_ssid, net_password, server_endpoint;
Preferences net_preferences;

uint32_t previous_post = 0;
uint32_t post_interval = 1000;

float temperature_val = 0;
char led_mode = 'b';
uint32_t led_set = 0, led_delay = 1000;

void PostCallback(TimerHandle_t) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, server_endpoint.c_str());
  http.addHeader("Content-Type", "application/json");

  temperature_val = Temp.readValue();
  String post_payload = String("{\"temperature\":\"") + String(temperature_val, 1) + String("\"}");
  int response_code = http.POST(post_payload);

  sscanf(http.getString().c_str(), "%c;%d;%d", &led_mode, &led_set, &led_delay);
  Easings.updateDelay(led_delay);

  http.end();
}

void setup() {
  delay(1000);
  Serial.println("=== COM3505 IoT ESP32-S3 Firmware ===");

  net_preferences.begin("network-creds", false);

  Serial.print("Enter network ssid: ");

  for (int timeout = 1000; Serial.available() == 0 && timeout > 0; timeout--) delay(10);
  net_ssid = Serial.readStringUntil('\n');
  Serial.println(net_ssid);

  Serial.print("Enter network password: ");
  for (int timeout = 1000; Serial.available() == 0 && timeout > 0; timeout--) delay(10);
  net_password = Serial.readStringUntil('\n');
  Serial.println(net_password);

  Serial.print("Enter server address: ");
  for (int timeout = 1000; Serial.available() == 0 && timeout > 0; timeout--) delay(10);
  server_endpoint = Serial.readStringUntil('\n');
  Serial.println(server_endpoint);
  server_endpoint.trim();
  server_endpoint = String("http://") + server_endpoint + String(":5000/api/data");

  if (net_ssid.length() > 1 && net_password.length() >= 8) {
    Serial.println("Saving network creds");
    net_preferences.putString("ssid", net_ssid.c_str());
    net_preferences.putString("pass", net_password.c_str());
    net_preferences.putString("ip", server_endpoint.c_str());
  } else {
    Serial.println("Defaultingto previous creds");
    net_ssid = net_preferences.getString("ssid", String("default_ssid"));
    net_password = net_preferences.getString("pass", String("default_pass"));
    server_endpoint = net_preferences.getString("ip", String("http://127.0.0.1:5000/api/data"));
  }

  Serial.begin(115200);
  WiFi.begin(net_ssid.c_str(), net_password.c_str());

  LEDs.initialise();
  Temp.initialise();

  Serial.println("Initalised WiFi, LEDs, and sensors. Waiting on connection");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  };
  Serial.println("\nConnection esablished.");

  xTimerStart(xTimerCreate(
    "PostTimer",
    post_interval / portTICK_PERIOD_MS, 
    pdTRUE,
    NULL,
    PostCallback
  ), 0);
}

void loop() {
  // LED things
  int progress = millis() % led_delay;   // progress into this round
  int overflows = millis() / led_delay;  // updates every <led_delay>
  switch (led_mode) {
    case 'b':
      if (progress > led_delay / 2.0f) LEDs.resetAll();
      else LEDs.setAll();
      break;

    case 'l':
      LEDs.showScale(Easings.linearEase(progress));
      break;

    case 'i':
      LEDs.showScale(Easings.easeInEase(progress));
      break;

    case 'o':
      LEDs.showScale(Easings.easeOutEase(progress));
      break;

    case 'x':
      LEDs.showScale(Easings.easeInOutEase(progress));
      break;

    case 'r':
      LEDs.highlightGroup(progress / (led_delay / 3.0f));
      break;

    case 'f':
      LEDs.highlightRandom(overflows);
      break;

    case 'c':
      LEDs.writeAll(overflows);
      break;

    case 't':
      LEDs.showScale((temperature_val - 15));
      break;

    case 'w':
      if (progress > led_delay / 2.0f) LEDs.writeAll(2730);
      else LEDs.writeAll(1365);
      break;
      break;

    case 'm':
      LEDs.writeAll(led_set);
      break;
  }

  vTaskDelay(10);
}