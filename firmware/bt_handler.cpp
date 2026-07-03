#include "bt_handler.h"
#include "config.h"
#include <Arduino.h>
#include <BluetoothSerial.h>

static BluetoothSerial btSerial;
static String lineBuffer = "";
static bool lineReady = false;

void btInit() {
    btSerial.begin(BT_DEVICE_NAME);
}

bool btAvailable() {
    while (btSerial.available()) {
        char c = btSerial.read();
        if ( c == '\n' || c == '\r') {
            lineReady = true;
        }
        else {
            lineBuffer += c ;
        }

        if (lineReady) break;
    }
    return lineReady;
}

String btReadLine() {
    String line = lineBuffer;
    line.trim();
    lineBuffer = "";
    lineReady = false;
    return line;
}

void btPrintln(const String& msg) {
    btSerial.println(msg);
}

void btPrint(const String& msg) {
    btSerial.print(msg);
}