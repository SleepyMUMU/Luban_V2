#include "Peripherals.h"

volatile byte BeepState = LOW;

hw_timer_t *Beeptimer = NULL;

/// @brief 蜂鸣器中断函数
/// @return NULL
void IRAM_ATTR BeepTimerFS()
{
    // BeepState = !BeepState;
    pinMode(BuzzerPin, INPUT_PULLUP);
    // digitalWrite(BuzzerPin, BeepState);
    timerAlarmDisable(Beeptimer);
    // timerStop(Beeptimer);
    // DebugSerial.println("Beep Down.");
}

void Beep(unsigned long BeepTime)
{
    //DebugSerial.println("[Beep]\tBeeping.");
    pinMode(BuzzerPin, OUTPUT);
    digitalWrite(BuzzerPin, HIGH);
    //  设置定时器的时间间隔和定时器模式
    timerAlarmWrite(Beeptimer, BeepTime * 1000, true); // 将毫秒转换为微秒
    timerAlarmEnable(Beeptimer);
    // timerStart(Beeptimer); // 手动启动定时器
}

/// @brief 外设初始化
void PeripheralsInit()
{
    Beeptimer = timerBegin(0, 80, true); // 0分频，80MHz的时钟，true表示重复触发
    timerAttachInterrupt(Beeptimer, &BeepTimerFS, true);
    timerAlarmWrite(Beeptimer, 1000000, true);
    timerAlarmEnable(Beeptimer);
    // 定时器默认处于关闭状态
    // timerStop(Beeptimer);
    timerAlarmDisable(Beeptimer); //	使能定时器
    // pinMode(BuzzerPin, OUTPUT);

    //DebugSerial.println("[Beep]\tBeep inited.");
    Beep(100);
    delay(200);
    Beep(100);
}