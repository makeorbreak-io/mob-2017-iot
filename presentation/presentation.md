class: center, middle

# Introduction to the Arduino ecosystem and the Internet of Things

---

# Agenda

1. Introduction
2. Electrical Circuits
3. Material we're using
  1. Breadboards
  2. NodeMCU development board
  3. Single color / RGB LEDs
  4. DHT11 temperature / humidity sensor
  5. Resistances

4. Exercises
  1. Connect to the internet
  2. Toggle an LED on / off
  3. Control RGB LED color with 3 sliders
  4. Read values from DHT11 sensor
      1. Print values to serial
      2. Print values to web page
      3. Log values to web service
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

<img src="/images/nodemcu_pins.png" width="300" />

---

## Material &mdash; LEDs

---

## Material &mdash; DHT11

---

## Material &mdash; Resistors

---

class: center, middle

# Exercises

---

## Exercises &mdash; 1. Connect to the Internet

---

## Exercises &mdash; 2. Toggle an LED

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


