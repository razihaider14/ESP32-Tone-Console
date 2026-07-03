#include "passive_buzzer.h"
#include "config.h"
#include <Arduino.h>

static int frequencies[MAX_TONES];
static int onTimes[MAX_TONES];
static int offTimes[MAX_TONES];
static int toneCount = 0;
static int repeatCount = 0;
static int currentTone = 0;
static int currentRepeat = 0;
static bool isOnPhase = true;
static bool running = false;
static unsigned long phaseStart = 0;

void passiveBuzzerInit() {
    pinMode(PIN_PASSIVE_BUZZER, OUTPUT);
    noTone(PIN_PASSIVE_BUZZER);
}

void passiveBuzzerStart(int newFrequencies[], int newOnTimes[], int newOffTimes[], int newToneCount, int newRepeatCount) {
    if (newToneCount <= 0 || newToneCount > MAX_TONES) return;
    for (int i = 0; i < newToneCount; i++) {
        frequencies[i] = newFrequencies[i];
        onTimes[i] = newOnTimes[i];
        offTimes[i] = newOffTimes[i];
    }
    toneCount = newToneCount;
    repeatCount = newRepeatCount;
    currentTone = 0;
    currentRepeat = 0;
    isOnPhase = true;
    running = true;
    phaseStart = millis();
    tone(PIN_PASSIVE_BUZZER, frequencies[currentTone]);
}

void passiveBuzzerStop() {
    running = false;
    noTone(PIN_PASSIVE_BUZZER);
}

bool passiveBuzzerIsRunning() {
    return running;
}

void passiveBuzzerUpdate() {
    if (!running) return;

    unsigned long now = millis();
    unsigned long duration = isOnPhase ? onTimes[currentTone] : offTimes[currentTone];
    if (now - phaseStart < duration) return;
    if (isOnPhase) {
        isOnPhase = false;
        noTone(PIN_PASSIVE_BUZZER);
        phaseStart = now;
    }
    else {
        currentTone++;
        if (currentTone >= toneCount) {
            currentTone = 0;
            currentRepeat++;
            if (currentRepeat >= repeatCount) {
                passiveBuzzerStop();
                return;
            }
        }
        isOnPhase = true;
        tone(PIN_PASSIVE_BUZZER, frequencies[currentTone]);
        phaseStart = now;
    }
}