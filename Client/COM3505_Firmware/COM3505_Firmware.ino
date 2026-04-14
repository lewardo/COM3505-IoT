/**
 * @file COM3505_Firmware.ino
 * @author Edouard Levasseur, Max Manning
 * @brief Main source file for the ESP32-S3 firmware 
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <Preferences.h>

#include "Easings.h"
#include "Leds.h"
#include "Sensor.h"

// Declare the network connection variables, and the persistent storage library
String net_ssid, net_password, server_endpoint;
Preferences net_preferences;

// Timing variables
uint32_t previous_post = 0;
uint32_t post_interval = 1000;

// LED and Temperature parameters
float temperature_val = 0;
char led_mode = '.';
uint32_t led_set = 0, led_delay = 1000;

/**
 * @brief Callback to run at a certain interval, asynchronous as to not block the LED loop
 * @param TimerHandle_t takes a handle to the timer that calls it
 * @return void
 */
void PostCallback(TimerHandle_t) {
  // Initialise network objects
  WiFiClient client; 
  HTTPClient http;

  // Esablish the http connection to the server
  http.begin(client, server_endpoint.c_str());
  http.addHeader("Content-Type", "application/json");

  // Read the temperature of the sensor, format it for the JSON payload, and POST it to the endpoint
  temperature_val = Temp.readValue();
  String post_payload = String("{\"temperature\":\"") + String(temperature_val, 1) + String("\"}");
  int response_code = http.POST(post_payload);

  // Parse the result of the response setting the relevant parameters, and update the LED delays
  sscanf(http.getString().c_str(), "%c;%d;%d", &led_mode, &led_set, &led_delay);
  Easings.updateDelay(led_delay);

  http.end();

  Serial.print("Posted temperature to ");
  Serial.println(server_endpoint);
}

/**
 * @brief Setup function, only run once 
 */
void setup() {
  delay(1000);
  Serial.println("=== COM3505 IoT ESP32-S3 Firmware ===");

  net_preferences.begin("network-creds", false);

  Serial.print("Enter network ssid: ");

  // Parse the SSID for the network, 10s timeout
  for (int timeout = 1000; Serial.available() == 0 && timeout > 0; timeout--) delay(10);
  net_ssid = Serial.readStringUntil('\n');
  Serial.println(net_ssid);

  // Parse the Password for the network, 10s timeout
  Serial.print("Enter network password: ");
  for (int timeout = 1000; Serial.available() == 0 && timeout > 0; timeout--) delay(10);
  net_password = Serial.readStringUntil('\n');
  Serial.println(net_password);

  // Parse the endpoint for the POST request, 10s timeout
  Serial.print("Enter server address: ");
  for (int timeout = 1000; Serial.available() == 0 && timeout > 0; timeout--) delay(10);
  server_endpoint = Serial.readStringUntil('\n');
  Serial.println(server_endpoint);
  
  // Format the IP as the endpoint
  server_endpoint.trim();
  server_endpoint = String("http://") + server_endpoint + String(":5000/api/data");

  // Validate the network credentials, and save them to persistent storage if so
  // Otherwise, use the previously saved credentials, falling back to defaults
  if (net_ssid.length() > 2 && net_password.length() >= 8) {
    Serial.println("Saving network creds");
    net_preferences.putString("ssid", net_ssid.c_str());
    net_preferences.putString("pass", net_password.c_str());
  } else {
    Serial.println("Defaulting to previous creds");
    net_ssid = net_preferences.getString("ssid", String("default_ssid"));
    net_password = net_preferences.getString("pass", String("default_pass"));
  }

  // Validate that the endpoint was provided and save it to storage, otherwise fallback as above
  if (server_endpoint.length() > 28) {
    net_preferences.putString("ip", server_endpoint.c_str());
  } else {
    Serial.println("Defaulting to previous endpoint");
    server_endpoint = net_preferences.getString("ip", String("http://127.0.0.1:5000/api/data"));
  }

  // Initialise Serial, WiFi, and the custom Driver objects
  Serial.begin(115200);
  WiFi.begin(net_ssid.c_str(), net_password.c_str());

  LEDs.initialise();
  Temp.initialise(NUM_LEDS);

  // Wait for a connection
  Serial.println("Initalised WiFi, LEDs, and sensors. Waiting on connection");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  };

  // Log the current IP
  Serial.print("\nConnection esablished, current ip ");
  Serial.println(WiFi.localIP());

  // Start the POST interval timer now
  xTimerStart(xTimerCreate(
    "PostTimer",
    post_interval / portTICK_PERIOD_MS, 
    pdTRUE,
    NULL,
    PostCallback
  ), 0);
}

/**
 * @brief Looping function, run repeatedly
 */
void loop() {
  // Calculate the parameters for the LED patterns
  int progress = millis() % led_delay;   // progress into this round
  int overflows = millis() / led_delay;  // updates every <led_delay>

  // Manage LEDs depending on pattern and progress
  switch (led_mode) {
    case 'b':
      if (overflows % 2 == 0) LEDs.resetAll();
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
      LEDs.highlightGroup(overflows % 3);
      break;

    case 'f':
      LEDs.highlightRandom(overflows);
      break;

    case 'c':
      LEDs.writeAll(overflows);
      break;

    case 't':
      LEDs.showScale((temperature_val - 15)); // offset so every LED is a degree above 15
      break;

    case 'w':
      if (overflows % 2 == 0) LEDs.writeAll(1365); // 0b010101010101
      else LEDs.writeAll(2730); // 0b101010101010
      break;

    case 'm':
      LEDs.writeAll(led_set);
      break;
  }

  vTaskDelay(10); // delay for a bit, yielding to the callback
}