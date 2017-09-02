#include <ESP8266WiFi.h>

#include "pins.h"
#include "wifi.h"
#include "http.h"
#include "html.h"

#include "secrets.h"

#define R D5
#define G D6
#define B D7

WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);

  // Connect to WiFi network
  connectToWiFi(WLAN_SSID, WLAN_PASS);
  printWifiStatus();

  // Start the server
  server.begin();
  Serial.println("\nServer started");
}

void loop() {
  WiFiClient client = server.available();

  if (!client) {
    return;
  }

  // Wait until de client sends some data
  while(!client.available()) {
    delay(1);
  }

  // process request
  String request = client.readStringUntil('\r');

  // ignore browser requests for favicon.ico
  if (request.indexOf("favicon.ico") >= 0) {
    return;
  }

  Serial.println();
  Serial.println(request);

  // Read the R, G and B values from the query string
  String r = query(request, "r");
  String g = query(request, "g");
  String b = query(request, "b");

  Serial.println("(" + r + ", " + g + ", " + b + ")");

  // NodeMCU PWM is between 0-1023
  analogWrite(R, 1023 - r.toInt());
  analogWrite(G, 1023 - g.toInt());
  analogWrite(B, 1023 - b.toInt());

  // Send HTTP response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println(beginHTML());
  client.println("<form>");
  client.println(range("r", "R", r.toInt()));
  client.println(range("g", "G", g.toInt()));
  client.println(range("b", "B", b.toInt()));
  client.println(submit("Set LED Color"));
  client.println("</form>");
  client.println(endHTML());

}
