
class: center, middle

# Arduino Ecosystem

---

## Arduino &mdash; Ecosystem

### Arduino Foundation
- Open source software & hardware
- Cross-platform: runs and builds on Win / Mac / Linux
- Arduino Programming Language (based on Wiring, very similar to C++)
- Arduino IDE (based on Processing)
- Suitable for all levels of expertise

### Other Software
- [Fritzing](http://fritzing.org/home/) (circuit design)

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

