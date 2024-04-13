#include <BluetoothSerial.h>
#include "Drive/UART.h"
#include "BTConfig.h"
BluetoothSerial BTSerial;

void BluetoothInit()
{
    BTSerial.begin("Luban Robot #1"); // 如果没有参数传入则默认是蓝牙名称是: "ESP32"
    BTSerial.setPin("6666");          // 蓝牙连接的配对码
    DebugSerial.printf("[Bluetooth]BT initial ok and ready to pair. \r\n");
}

String BTcom()
{
    String ReceiveData;
    while (BTSerial.available())
    {
        ReceiveData = ReceiveData + char(BTSerial.read());
    }
    return ReceiveData;
}
