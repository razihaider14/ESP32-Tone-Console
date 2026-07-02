#pragma once

void activeBuzzerInit();
void activeBuzzerStart(int onTimes[], int offTimes[], int toneCount, int repeatCount);
void activeBuzzerStop();
void activeBuzzerUpdate();
bool activeBuzzerIsRunning();