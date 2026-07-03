#include "command_parser.h"
#include "config.h"
#include "bt_handler.h"
#include "active_buzzer.h"
#include "passive_buzzer.h"
#include <Arduino.h>

enum ParserState {
    IDLE,
    ASK_COUNT,
    ASK_FREQ,
    ASK_ON,
    ASK_OFF,
    ASK_REPEAT,
    RUNNING
};

enum BuzzerMode {
    MODE_NONE,
    MODE_ACTIVE,
    MODE_PASSIVE
};

static ParserState state = IDLE;
static BuzzerMode mode = MODE_NONE;

static int toneCount = 0;
static int currentTone = 0;
static int frequencies[MAX_TONES];
static int onTimes[MAX_TONES];
static int offTimes[MAX_TONES];

static void printHelp() {
    btPrintln("Commands:");
    btPrintln("Active: Configure and play active buzzer.");
    btPrintln("Passive: Configure and play passive buzzer.");
    btPrintln("Stop: Stop sound / cancel setup.");
    btPrintln("Help: Show this message.");
}

static void goIdle(const String& message) {
    state = IDLE;
    mode = MODE_NONE;
    if (message.length() > 0) {
        btPrintln(message);
    }
    btPrintln("Enter a command (active, passive, stop, help):");
}

static void handleStop() {
    activeBuzzerStop();
    passiveBuzzerStop();
    goIdle("Stopped.");
}

static bool parseIntSafe(const String& line, int& outValue) {
    if (line.length() == 0) return false;
    for (unsigned int i = 0; i < line.length(); i++) {
        if (!isDigit(line.charAt(i))) return false;
    }
    outValue = line.toInt();
    return true;
}

void commandParserHandleLine(String line) {
    line.trim();
    if (line.length() == 0) return;
    String lower = line;
    lower.toLowerCase();

    if (lower == "stop") {
        handleStop();
        return;
    }

    if (lower == "help") {
        printHelp();
        return;
    }

    switch (state) {
        case IDLE:
            if (lower == "active") {
                mode = MODE_ACTIVE;
                state = ASK_COUNT;
                btPrintln("Active buzzer selected.");
                btPrintln("How many tones?");
            }
            else if (lower == "passive") {
                mode = MODE_PASSIVE;
                state = ASK_COUNT;
                btPrintln("Passive buzzer selected.");
                btPrintln("How many tones?");
            }
            else {
                btPrintln("Unknown command. Type 'help' for options.");
            }
            break;

        case ASK_COUNT: {
            int count;
            if (!parseIntSafe(line, count) || count <= 0 || count >= MAX_TONES) {
                btPrintln("Please enter a valid number (1 - " + String(MAX_TONES) + ").");
                return;
            }
            toneCount = count;
            currentTone = 0;

            if (mode == MODE_PASSIVE) {
                state = ASK_FREQ;
                btPrintln("Tone 1/" + String(toneCount) + ", what frequency (Hz)?");
            }
            else {
                state = ASK_ON;
                btPrintln("Tone 1/" + String(toneCount) + ", how long ON (ms)?");
            }
            break;
        }

        case ASK_FREQ: {
            int val;
            if (!parseIntSafe(line, val) || val <= 0) {
                btPrintln("Please enter a valid frequency in Hz.");
                return;
            }
            frequencies[currentTone] = val; 
            state = ASK_ON;
            btPrintln("Tone " + String(currentTone + 1) + "/" + String(toneCount) + ", how long ON (ms)?");
            break;
        } 

        case ASK_ON: {
            int val;
            if(!parseIntSafe(line, val) || val <= 0) {
                btPrintln("Please enter a valid duration in ms.");
                return;
            }
            onTimes[currentTone] = val;
            state = ASK_OFF;
            btPrintln("Tone " + String(currentTone + 1) + "/" + String(toneCount) + ", how long OFF (ms)?");
            break;
        }

        case ASK_OFF: {
            int val;
            if (!parseIntSafe(line, val) || val <= 0) {
                btPrintln("Please enter a valid duration in ms.");
                return;
            }
            offTimes[currentTone] = val;
            currentTone++;

            if (currentTone < toneCount) {
                if (mode == MODE_PASSIVE) {
                    state = ASK_FREQ;
                    btPrintln("Tone " + String(currentTone + 1) + "/" + String(toneCount) + ", what frequency (Hz)?");
                }
                else {
                    state = ASK_ON;
                    btPrintln("Tone " + String(currentTone + 1) + "/" + String(toneCount) + ", how long ON (ms)?");
                }
            }
            else {
                state = ASK_REPEAT;
                btPrintln("How many times to repeat?");
            }
            break;
        }

        case ASK_REPEAT: {
            int val;
            if (!parseIntSafe(line, val) || val <= 0) {
                btPrintln("Please enter a valid repeat count.");
                return;
            }
            state = RUNNING;

            if (mode == MODE_ACTIVE) {
                activeBuzzerStart(onTimes, offTimes, toneCount, val);
            }
            else {
                passiveBuzzerStart(frequencies, onTimes, offTimes, toneCount, val);
            }
            btPrintln("Playing... (send 'stop' to cancel).");
            break;
        }

        case RUNNING:
            btPrintln("Already playing. Send 'stop' to cancel.");
            break;
    }
}

void commandParserUpdate() {
    if (state != RUNNING) return;
    
    bool stillRunning = (mode == MODE_ACTIVE) ? activeBuzzerIsRunning() : passiveBuzzerIsRunning();
    if (!stillRunning) {
        goIdle("Done.");
    }
}