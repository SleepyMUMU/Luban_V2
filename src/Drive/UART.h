#ifndef _UART_H_
#define _UART_H_

#include <Arduino.h>
#include "WiFiConfig.h"

#define DebugSerial client
#define DebugSerialBaudRate 115200

#define STM32COM Serial
#define STM32COMBaudRate 115200

#define STM32COM_RX 3
#define STM32COM_TX 1

#define ServoSerial1 Serial1
#define ServoSerial1BaudRate 500000

#define ServoSerial1_RX 32
#define ServoSerial1_TX 33

#define ServoSerial2 Serial2
#define ServoSerial2BaudRate 500000

#define ServoSerial2_RX 26
#define ServoSerial2_TX 25

void UARTinit();

#endif