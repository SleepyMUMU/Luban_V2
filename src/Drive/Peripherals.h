#ifndef _PERIPHERALS_H_
#define _PERIPHERALS_H_

#include <Arduino.h>
#include "UART.h"



#define BuzzerPin 23

extern volatile byte BeepState;
extern hw_timer_t *timer;


void Beep(unsigned long BeepTime);

void PeripheralsInit();

#endif