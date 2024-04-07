#include "UART.h"

void UARTinit()
{
    // DebugSerial.begin(DebugSerialBaudRate);
    // DebugSerial.println("*****Debug Serial Connected.*****");
    STM32COM.begin(STM32COMBaudRate,
                       SERIAL_8N1,
                       STM32COM_RX,
                       STM32COM_TX);
    //STM32COM.print("X0Y0W0");

    ServoSerial1.begin(ServoSerial1BaudRate,
                       SERIAL_8N1,
                       ServoSerial1_RX,
                       ServoSerial1_TX);
    ServoSerial2.begin(ServoSerial1BaudRate,
                       SERIAL_8N1,
                       ServoSerial2_RX,
                       ServoSerial2_TX);
}