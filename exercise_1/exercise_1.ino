#include <ESP8266WiFi.h>

#include "secrets.h"

#define HTTP_HOST "wttr.in"
#define HTTP_PORT 80

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to wireless network ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // print the SSID of the network you're attached to:
  Serial.print("\n\nConnected to ");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void loop() {
  Serial.print("connecting to ");
  Serial.println(HTTP_HOST);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(HTTP_HOST, HTTP_PORT)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/Porto?T";

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + String(HTTP_HOST) + "\r\n" +
               "User-Agent: curl\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");

	delay(5000);
}
