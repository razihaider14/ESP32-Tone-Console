#pragma once
#include <Arduino.h>

void btInit();
bool btAvailable();
String btReadLine();
void btPrintln(const String& msg);
void btPrint(const String& msg);