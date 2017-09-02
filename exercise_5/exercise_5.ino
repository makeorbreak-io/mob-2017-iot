#include <ESP8266WiFi.h>

#include "pins.h"
#include "wifi.h"
#include "html.h"
#include "secrets.h"

WiFiServer server(80);
byte b1 = LOW;
byte b1_prev = LOW;
byte b2 = LOW;
byte b2_prev = LOW;
int counter = 0;

void setup() {
  // Initialize serial
  Serial.begin(115200);

  // Initialize gpio pins
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);

  // Connect to WiFi network
  connectToWiFi(WLAN_SSID, WLAN_PASS);
  printWifiStatus();

  // Start the TCP server
  server.begin();
  Serial.println("\nServer started");
}

void loop() {

  // Read value from gpio pins
  b1 = digitalRead(D1);
  b2 = digitalRead(D2);

  // Check which pins go from HIGH to LOW or LOW to HIGH
  if (b1_prev == LOW && b1 == HIGH) {
    counter--;
    Serial.println(counter);
    b1_prev = HIGH;
  } else if (b1_prev == HIGH && b1 == LOW) {
    b1_prev = LOW;
  }

  if (b2_prev == LOW && b2 == HIGH) {
    counter++;
    Serial.println(counter);
    b2_prev = HIGH;
  } else if (b2_prev == HIGH && b2 == LOW) {
    b2_prev = LOW;
  }

  // Check if there are clients connect to our TCP server
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Read (and discard) the client request data
  while (!client.available()) {
    client.read();
  }

  // Send HTTP reply to the client
  client.flush();
  client.println(beginHTML());
  client.print("<h1>");
  client.print(counter);
  client.println("</h1>");
  client.println(endHTML());

}
