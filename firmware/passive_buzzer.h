#pragma once

void passiveBuzzerInit();
void passiveBuzzerStart(int frequencies[], int onTimes[], int[offTimes], int toneCount, int repeatCount);
void passiveBuzzerStop();
void passiveBuzzerUpdate();
bool passiveBuzzerIsRunning();