#ifndef _JSONCONFIG_H_
#define _JSONCONFIG_H_

#include "Drive/WiFiConfig.h"
#include "cJSON.h"

extern String DefaultTnitMode;
extern int DefaultTime;
extern float PosArray[6][10];
extern String PosName[6];
#define PosDefNum 0
#define PosANum 1
#define PosBNum 2
#define PosCNum 3
#define PosDNum 4
#define PosQNum 5
// extern cJSON *Basic;       // 创建JSON根部结构体
// extern cJSON *ServoConfig; // 创建JSON子叶结构体
// extern cJSON *MotorConfig; // 创建JSON子叶结构体

void JsonInit();
void SaveConfigAsJsonToEeprom();
void saveinit();

#endif