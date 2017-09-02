#include <ESP8266WiFi.h>

#include "pins.h"
#include "wifi.h"
#include "http.h"
#include "html.h"

#include "secrets.h"

#define LED D6
WiFiServer server(80);

void setup() {
  // Initialize serial
  Serial.begin(115200);

  // Initialize gpio pins
  pinMode(LED, OUTPUT);

  // Connect to WiFi network
  connectToWiFi(WLAN_SSID, WLAN_PASS);
  printWifiStatus();

  // Start the server
  server.begin();
  Serial.println("\nServer started");
}

void loop() {
  // Wait until a client connects to our server
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the connected client sends the request data
  while (!client.available()) {
    delay(1);
  }

  // Get the first line of the HTTP request
  String request = client.readStringUntil('\r');

  // Ignore browser requests for favicon.ico
  if (request.indexOf("favicon.ico") >= 0) {
    return;
  }

  Serial.println();
  Serial.println(request);

  // Parse the query string in the GET line
  // GET /?led=ON
  String led = query(request, "led");
  bool isOn = led.indexOf("ON") > -1;

  digitalWrite(LED, isOn ? HIGH : LOW);

  // Send HTTP reply to the client
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println(beginHTML());
  client.println("<form>");
  client.println(input("hidden", "led", isOn ? "OFF" : "ON"));
  client.println(submit(isOn ? "Turn Off" : "Turn On"));
  client.println("</form>");
  client.println(endHTML());
}
