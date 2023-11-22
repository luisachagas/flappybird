#ifndef TIMER_H
#define TIMER_H

#include <unistd.h>

void timerInit(int milliseconds);
void timerDestroy();
int timerTimeOver();
void timerUpdateTimer(int milliseconds);
void timerSleep(int milliseconds);
void timerPrint();

#endif // TIMER_H
