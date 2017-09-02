class: center, middle

# Exercises

---

## Exercises &mdash; 1. Connect to the Internet

Our first exercise won't have a physical circuit. We'll be using `wttr.in`,
an awesome weather forecast service to test our connectivity to the internet.

### Establishing connectivity to a wireless network

```c
#include <ESP8266WiFi.h>

#define WLAN_SSID "my awesome wlan name"
#define WLAN_PASS "hunter2"

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to wireless network ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  IPAddress ip = WiFi.localIP();
  Serial.print("Connected! IP Address: ");
  Serial.println(ip);
}
```
---

### Querying wttr.in

No high-level HTTP clients 😧. We'll do everything through a TCP socket 👌

A simple HTTP request looks like:

```http
GET /Porto?T HTTP/1.1
Host: wttr.in
User-Agent: curl
Connection: close
```
⚠️ Each of the lines above is terminated with CRLF characters! (`\r\n`).
**Don't forget the blank line in the end** that marks the end of the HTTP request.

Try it out! Send these lines to `wttr.in` with `nc` or `telnet`!

```bash
nc wttr.in 80
```

---

```c
void loop() {
  // `client` is a TCP client socket which we connect to `wttr.in` in port 80
  WiFiClient client;
  if (!client.connect("wttr.in", 80)) {
    Serial.println("connection failed");
    return;
  }
  // send the HTTP request to the server we just connected to
  client.print("GET /Porto?T HTTP/1.1\r\n");
  client.print("Host: wttr.in\r\n");
  client.print("User-Agent: curl\r\n");
  client.print("Connection: close\r\n");
  client.print("\r\n");
  // wait up to 5 seconds for a response
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      client.stop();
      return;
    }
  }
  // read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  // wait before finishing the loop (and starting the next)
  delay(5000);
}
```

---

## Exercises &mdash; 2. Toggle an LED using a web interface

This exercise is a bit different from exercise 1. Instead of using a client socket
to have our microcontroller connect to an internet server, we're using a server
socket so we can have a browser connect to our microcontroller that will act as
an HTTP server.

We'll also set up a small physical circuit using a resistor and an led.

<div style="text-align: center;">
  <img src="/images/exercise_2.png" width="300" />
</div>

---

We'll be using a few functions to reduce the size of the code samples
in this presentation: `connectToWifi`, `printWifiStatus`, `beginHTML` `query`.

```c
// A TCP server listening on port 80 using the wifi hardware
WiFiServer server(80);

void setup() {
  // Initialize serial I/O
  Serial.begin(115200);

  // Initialize GPIO pins
  pinMode(D6, OUTPUT);

  // Connect to WiFi network
  connectToWiFi(WLAN_SSID, WLAN_PASS);
  printWifiStatus();

  // Start the server
  server.begin();
  Serial.println("\nServer started");

  // Get the first line of the HTTP request
  String request = client.readStringUntil('\r');

  // Ignore browser requests for favicon.ico
  if (request.indexOf("favicon.ico") >= 0) return;
}
```
---

```c
void loop() {
  // Wait until a client connects to our server
  WiFiClient client = server.available();
  if (!client) return;

  // Wait until the connected client sends the request data
  while (!client.available()) {
    delay(1);
  }

  // Parse the query string in the GET line
  // GET /?led=ON
  String led = query(request, "led");
  bool isOn = led.indexOf("ON") > -1;

  // Turn LED on or off
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
```

---

## Exercises &mdash; 3. Control an RGB LED's hue and brightness using a web interface

This exercise is very similar to the previous, with some more added complexity
in the code and in the circuit.

The RGB led we're providing is the "common anode" type, so we wire it like the
following diagram.

<div style="text-align: center;">
  <img src="/images/exercise_3.png" width="300" />
</div>

---

## Exercises &mdash; 4. Read values from DHT11

<img src="/images/dht11_sensor.jpg" width="300" />

The DHT11 is a basic, ultra low-cost digital temperature and humidity sensor.
It uses a capacitive humidity sensor and a thermistor to measure the surrounding
air, and spits out a digital signal on the data pin (no analog input pins needed).

---

### DHT11 &mdash; 4.1. Print values to serial monitor

```c
DHT sensor(5, DHT11); // DHT11 connected to GPIO 5
float hum = 0, temp = 0;

void setup() {
  Serial.begin(115200);
  sensor.begin();
}

void loop() {
  hum = sensor.readHumidity();
  temp = sensor.readTemperature();
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print("%; Temperature: ");
  Serial.print(temp);
  Serial.println("ºC");

  delay(20000); // !!!
}
```
---

### DHT11 &mdash; 4.2. Print values to web page

---

### DHT11 &mdash; 4.3. Log values to web service

We'll be using the *ThingSpeak* API to log the values read from the sensor.
ThingSpeak is an API that enables the creation of sensor logging applications
and sharing of data.

<div style="text-align: center;">
<img src="/images/thingspeak.png" width="600" />
</div>
---

```c
#define HTTP_HOST "api.thingspeak.com"
#define HTTP_PORT 80
#define THINGSPEAK_API_KEY "herpderpkey"

void loop() {
  hum = sensor.readHumidity();
  temp = sensor.readTemperature();

  WiFiClient client;
  if (!client.connect(HTTP_HOST, HTTP_PORT)) {
    Serial.println("connection failed");
    return;
  }

  /* post thingspeak update payload to the server */
  String message = String("") + F("field1=") + String(hum) +
    F("&field2=") + String(temp);
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

  delay(20000);
}
```
---



