#include <ESP8266WiFi.h>

#include "./lib/pins.h"
#include "./lib/wifi.h"
#include "./lib/html.h"

#define R D5
#define G D6
#define B D7
 
const char* ssid = "<ssid>";
const char* password = "<password>";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
 
  // Connect to WiFi network
  connectToWiFi(ssid, password);
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
  if (request.indexOf("favicon.ico") >= 0) return;

  Serial.println();
  Serial.println(request);

  String r = query(request, "r");
  String g = query(request, "g");
  String b = query(request, "b");

  Serial.println("(" + r + ", " + g + ", " + b + ")");

  analogWrite(R, r.toInt()*4); // NodeMCU PWM is between 0-1023
  analogWrite(G, g.toInt()*4); // NodeMCU PWM is between 0-1023
  analogWrite(B, b.toInt()*4); // NodeMCU PWM is between 0-1023

  // print html
  client.flush();

  client.println(beginHTML());

  client.println("<form>");
  client.println(range("r", "R", r.toInt()));
  client.println(range("g", "G", g.toInt()));
  client.println(range("b", "B", b.toInt()));
  client.println(submit("Set LED Color"));
  client.println("</form>");

  client.println(endHTML());
}
