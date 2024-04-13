#ifndef _BTCONFIG_H_
#define _BTCONFIG_H_

#include <BluetoothSerial.h>
#include "Drive/UART.h"

extern BluetoothSerial BTSerial;
String BTcom();
void BluetoothInit();


#endif
