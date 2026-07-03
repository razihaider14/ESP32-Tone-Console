#include "active_buzzer.h"
#include "config.h"
#include <Arduino.h>

static int onTimes[MAX_TONES];
static int offTimes[MAX_TONES];
static int toneCount = 0;
static int repeatCount = 0;
static int currentTone = 0;
static int currentRepeat = 0;
static bool isOnPhase = true;
static bool running = false;
static unsigned long phaseStart = 0;

void activeBuzzerInit() {
    pinMode(PIN_ACTIVE_BUZZER, OUTPUT);
    digitalWrite(PIN_ACTIVE_BUZZER, LOW);
}

void activeBuzzerStart(int newOnTimes[], int newOffTimes[], int newToneCount, int newRepeatCount) {
    if (newToneCount <= 0 || newToneCount > MAX_TONES) return;
    for (int i = 0; i < newToneCount; i++) {
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
    digitalWrite(PIN_ACTIVE_BUZZER, HIGH);
}

void activeBuzzerStop() {
    running = false;
    digitalWrite(PIN_ACTIVE_BUZZER, LOW);
}

bool activeBuzzerIsRunning() {
    return running;
}

void activeBuzzerUpdate() {
    if (!running) return;

    unsigned long now = millis();
    unsigned long duration = isOnPhase ? onTimes[currentTone] : offTimes[currentTone];
    
    if (now - phaseStart < duration) return;

    if (isOnPhase) {
        isOnPhase = false;
        digitalWrite(PIN_ACTIVE_BUZZER, LOW);
        phaseStart = now;
    }
    else {
        currentTone++;
        if (currentTone >= toneCount) {
            currentTone = 0;
            currentRepeat++;
            if (currentRepeat >= repeatCount) {
                activeBuzzerStop();
                return;
            }
        }
        isOnPhase = true;
        digitalWrite(PIN_ACTIVE_BUZZER, HIGH);
        phaseStart = now;
    }
}