#include <WiFi.h>
#include <HTTPClient.h>

#include "Easings.h"
#include "Leds.h"
#include "Sensor.h"

const char* net_ssid = "VM2685D8";
const char* net_password = "2pJejmkganFu";
const char* server_endpoint = "http://192.168.0.37:5000/api/data";

uint32_t previous_post = 0;
uint32_t post_interval = 1000;

float temperature_val = 0;
char led_mode = 'b';
uint32_t led_set = 0, led_delay = 1000;

void PostCallback(TimerHandle_t) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, server_endpoint);
  http.addHeader("Content-Type", "application/json");

  String post_payload = String("{\"temperature\":\"") + String(Temp.readValue(), 1) + String("\"}");
  int response_code = http.POST(post_payload);

  sscanf(http.getString().c_str(), "%c;%d;%d", &led_mode, &led_set, &led_delay);
  Easings.updateDelay(led_delay);

  http.end();
}

void setup() {
  Serial.println("=== COM3505 IoT ESP32-S3 Firmware ===");

  Serial.begin(115200);
  WiFi.begin(net_ssid, net_password);

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

    case 'm':
      LEDs.writeAll(led_set);
      break;
  }

  vTaskDelay(10);
}