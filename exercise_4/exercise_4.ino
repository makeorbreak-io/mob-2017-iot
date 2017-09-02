#include <DHT.h>

#include "pins.h"
#include "wifi.h"
#include "secrets.h"

#define HTTP_HOST "api.thingspeak.com"
#define HTTP_PORT 80

DHT sensor(D1, DHT11);
float hum = 0, temp = 0;

void setup() {
  // initialize serial
  Serial.begin(115200);

  // initialize sensor
  sensor.begin();

  // connect to wifi
  connectToWiFi(WLAN_SSID, WLAN_PASS);
  printWifiStatus();
}

void loop() {
  hum = sensor.readHumidity();
  temp = sensor.readTemperature();

  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print("%; Temperature: ");
  Serial.print(temp);
  Serial.println("ºC");

  WiFiClient client;
  if (!client.connect(HTTP_HOST, HTTP_PORT)) {
    Serial.println("connection failed");
    return;
  }

  /* post thingspeak update payload to the server */
  String message = String("") + F("field1=") + String(hum) + F("&field2=") + String(temp);
  client.print("POST /update HTTP/1.1\r\n");
  client.print("Host: api.thingspeak.com\r\n");
  client.print("Connection: close\r\n");
  client.print("X-ThingSpeakAPIKey: ");
  client.print(THINGSPEAK_API_KEY);
  client.print("\r\n");
  client.print("Content-Type: application/x-www-form-urlencoded\r\n");
  client.print("Content-Length: ");
  client.print(message.length());
  client.print("\r\n\r\n");
  client.print(message);
  client.print("\r\n");

  /* read response from the server and print it to serial */
  while (client.connected()) {
    if (client.available()) {
      Serial.print((char)client.read());
    }
  }
  Serial.println("");

  delay(20000);
}
