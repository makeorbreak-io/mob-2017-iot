#ifndef HTML_H
#define HTML_H

#include <Arduino.h>

String beginHTML();
String endHTML();
String input(String type, String name, String value);
String submit(String label);

#endif

