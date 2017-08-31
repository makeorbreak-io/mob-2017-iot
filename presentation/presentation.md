class: center, middle

# Introduction to the Arduino ecosystem and the Internet of Things

---

# Agenda

1. Introduction
2. Electrical Circuits
3. Arduino Ecosystem
4. Setup
5. Material we're using
  1. Breadboards
  2. NodeMCU development board
  3. Single color / RGB LEDs
  4. DHT11 temperature / humidity sensor
  5. Tactile buttons
  6. Resistor

6. Exercises
  1. Connect to the internet
  2. Toggle an LED on / off
  3. Control RGB LED color with 3 sliders
  4. Read values from DHT11 sensor
  5. Control RGB LED color with values from DHT11

---

class: center, middle

# Introduction

---

class: center, middle

# Electrical Circuits

---

class: center, middle

# Material

---

## Material &mdash; Breadboards

<img src="/images/breadboards.jpg" width="100%" />

---

## Material &mdash; NodeMCU Development Board

.center[<img src="/images/nodemcu_pins.png" width="500" />]

---

## Material &mdash; LEDs

---

## Material &mdash; DHT11

---

## Material &mdash; Tactile Buttons

---

## Material &mdash; Resistors

---

class: center, middle

# Arduino Ecosystem

---

## Arduino &mdash; Ecosystem

- Open source software & hardware
- Cross-platform: runs and builds on Win / Mac / Linux
- Arduino Programming Language (based on Wiring, very similar to C++)
- Arduino IDE (based on Processing)
- Suitable for all levels of expertise

.center[<img src="/images/sensor_actuator.png" width="300" />]

---

## Arduino &mdash; Boards

- 
note: talk about the arduino ecosystem: what it is: the microcontroller boards,
the ide and what every program has in common

### Digital IO pins

Can be inputs or outputs, specified by the sketch

### Analog IN pins

Dedicated analog input pins take voltage and convert to integer ranging from 0-1023

### Analog OUT pins

Dedicated analog output pins, read voltage and convert to an integer between 0-1023

---

## Arduino &mdash; Sketch organization

The entry point for your application will be an `.ino` file.

Due to limitations with the Arduino IDE, this file must be named the same as its parent folder.

Support files need to be at the same level as your `.ino` file for local `#import` statements to work.

```shell
project
├── project.ino
├── wifi.h
├── wifi.cpp
└── ...
```
---

## Arduino &mdash; Sketch code skeleton

```c
void setup() {
  // put your setup code here, to run once
  //
  // used for setting up pin modes, serial monitor,
  // and one-time initialization functions

  Serial.begin(115200);
  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println("LED is on");
  digitalWrite(LED, HIGH);
  delay(500);

  Serial.println("LED is off");
  digitalWrite(LED, LOW);
  delay(500);
}
```

---

## Arduino &mdash; Important functions

### I/O

```c
// mode = INPUT | OUTPUT
void pinMode(int pin, int mode)

// returns HIGH | LOW (1 | 0)
int digitalRead(int pin)

// value should be HIGH | LOW (1 | 0)
void digitalWrite(int pin, int value)

// value should be between 0-255 or 0-1023; uses PWM (Pulse Width Modulation)
void analogWrite(int pin, int value)
```

---

## Arduino &mdash; Important functions

### Timing

```c
int millis()

int micros()

void delay(int millis)

void delayMicroseconds(int micros)
```

---

class: center, middle

# Setup

---

## Setup &mdash; Install

- Install the Arduino IDE from [https://www.arduino.cc/en/Main/Software](https://www.arduino.cc/en/Main/Software)
- Install the (CH340G) drivers for the development board
  - Mac OS X: [https://github.com/adrianmihalko/ch340g-ch34g-ch34x-mac-os-x-driver](https://github.com/adrianmihalko/ch340g-ch34g-ch34x-mac-os-x-driver)
  - Windows (if needed): [http://www.wch.cn/download/CH341SER_ZIP.html](http://www.wch.cn/download/CH341SER_ZIP.html)
  - Linux (if needed): [http://www.wch.cn/download/CH341SER_LINUX_ZIP.html](http://www.wch.cn/download/CH341SER_LINUX_ZIP.html)
- Open the Arduino IDE, and under "Preferences" > "Additional Boards Manager URLs" add [http://arduino.esp8266.com/stable/package_esp8266com_index.json](http://arduino.esp8266.com/stable/package_esp8266com_index.json)

---

## Setup &mdash; Connect

Connect your board to your laptop with the provided USB cable, and use the following settings under Tools:

<dl>
  <dt>Board</dt>
  <dd>NodeMCU 1.0 (ESP-12E Module)</dd>

  <dt>CPU Frequency</dt>
  <dd>80 MHz</dd>

  <dt>Flash Size</dt>
  <dd>4M (3M SPIFFS)</dd>

  <dt>Upload Speed</dt>
  <dd>115200</dd>

  <dt>Port</dt>
  <dd>Depends on your machine</dd>
</dl>

---

class: center, middle

# Exercises

---

## Exercises &mdash; 1. Connect to the Internet

We'll be using `wttr.in`, an awesome weather forecast service to test
our connectivity

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
socket so we can have a browser connect to our microcontroller.

---

## Exercises &mdash; 3. RGB LED

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


