#include "config.h"
#include "bt_handler.h"
#include "active_buzzer.h"
#include "passive_buzzer.h"
#include "command_parser.h"

void setup() {
    Serial.begin(115200);
    btInit();
    activeBuzzerInit();
    passiveBuzzerInit();
    btPrintln("ESP32 Tone Console ready.");
    btPrintln("Type 'help' for commands.");
}

void loop() {
    if (btAvailable()) {
        String line = btReadLine();
        commandParserHandleLine(line);
    }
    activeBuzzerUpdate();
    passiveBuzzerUpdate();
    commandParserUpdate();
}