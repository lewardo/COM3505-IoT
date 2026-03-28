#include <WiFi.h>
#include <HTTPClient.h>

#include "Easings.h"
#include "Leds.h"
#include "Sensor.h"

const char* net_ssid = "VM2685D8";
const char* net_password = "2pJejmkganFu";
const char* server_endpoint = "http://192.168.0.37:5000/api/data";

String post_payload;

uint32_t previous_post = 0;
uint32_t post_interval = 2000;

float temperature_val = 0;
char led_mode = 'm';
uint32_t led_set = 0, led_delay = 1000;

void setup() {
  Serial.begin(115200);
  WiFi.begin(net_ssid, net_password);

  LEDs.initialise();
  Temp.initialise();

  while (WiFi.status() != WL_CONNECTED) {};
}

void loop() {
  int current_time = millis();
  if (previous_post - current_time > post_interval && WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, server_endpoint);
    http.addHeader("Content-Type", "application/json");

    int response_code = http.POST(post_payload);
    sscanf(http.getString().c_str(), "%c;%d;%d", &led_mode, &led_set, &led_delay);

    Easings.updateDelay(led_delay);

    http.end();

    if (response_code == 200) {
      previous_post = current_time;
    };
  }

  // temperature sensor
  temperature_val = Temp.readValue();
  post_payload = String(temperature_val, 3);

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

  delay(10);
}