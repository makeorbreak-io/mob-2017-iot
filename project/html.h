#ifndef HTML_H
#define HTML_H

#include <Arduino.h>

String query(String request, String variable);
String beginHTML();
String endHTML();
String submit(String label);
String input(String type, String name, String label, String value);
String range(String name, String label, int value);
String div(String content);
String form(String content);
String p(String content);

#endif

