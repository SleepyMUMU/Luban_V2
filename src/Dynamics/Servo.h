#ifndef _SERVO_H_
#define _SERVO_H_

#include <Arduino.h>
#include "Drive/UART.h"
#include "FashionStar_UartServo.h"
#include "FashionStar_UartServoProtocol.h"
#include "Drive/WiFiConfig.h"

#define ServoSerialBaudRate 500000

/******左前机械臂******/
#define Left_Forefoot_Servo 1
#define Left_ForeAnkle_Servo 2
/******右前机械臂******/
#define Right_Forefoot_Servo 3
#define Right_ForeAnkle_Servo 4
/******左后机械臂******/
#define Left_Hindfoot_Servo 5
#define Left_HindAnkle_Servo 6
/******右后机械臂******/
#define Right_Hindfoot_Servo 8
#define Right_HindAnkle_Servo 7

#define Cam_X_Servo 9
#define Cam_Y_Servo 10

#define Find_X_Servo 11
#define Find_Y_Servo 12

#define LFFR 10.0
#define LFAR -16.6
#define RFFR 2.9
#define RFAR 1.5
#define LHFR -13.5
#define LHAR -20.4
#define RHFR 14.4
#define RHAR -2.5

#define LFFabs -1
#define LFAabs 1
#define RFFabs 1
#define RFAabs -1
#define LHFabs 1
#define LHAabs -1
#define RHFabs -1
#define RHAabs 1

// 摆线参数
#define XS 90.0   // X原点   40 ~ 140             (50)
#define YS -150.0 // Y原点   x = 90:-88 ~ -172    (42)

// 摆线参数
#define XSLF 40.0   // X原点   40 ~ 140             (50)
#define YSLF -160.0 // Y原点   x = 90:-88 ~ -172    (42)

// 摆线参数
#define XSRF 150.0 // X原点   40 ~ 140             (50)
#define YSRF 120.0 // Y原点   x = 90:-88 ~ -172    (42)

// 摆线参数
#define XSLH 150.0  // X原点   40 ~ 140             (50)
#define YSLH -130.0 // Y原点   x = 90:-88 ~ -172    (42)\

// 摆线参数
#define XSRH 140.0  // X原点   40 ~ 140             (50)
#define YSRH -135.0 // Y原点   x = 90:-88 ~ -172    (42)

#define XMOVE 40 // X距离
#define H 20     // 抬腿高度

extern FSUS_Servo LFF; // 创建Left Foot左前脚舵机
extern FSUS_Servo LFA; // 创建Left Ankle左前踝舵机
extern FSUS_Servo LHF; // 创建Left Foot左前脚舵机
extern FSUS_Servo LHA;

extern FSUS_Servo RFF; // 创建Left Foot左前脚舵机
extern FSUS_Servo RFA;
extern FSUS_Servo RHF; // 创建Left Foot左前脚舵机
extern FSUS_Servo RHA;
extern FSUS_Servo Right;
extern FSUS_Servo Left;

void ServoInit();
void GetServoAngle();
void ServoBackup();
void RobotUp();
void RobotAnkleAngle(float Angle);
void RobotFootAngle(float Angle);
void RobotBhindAnkleAngle(float Angle);
void RobotBhindFootAngle(float Angle);
void RobotForwordAnkleAngle(float Angle);
void RobotForwordFootAngle(float Angle);
void RobotTrackB();
void RobotDown();
void Trot();
void RobotTrackB();
void roundPos(uint8_t Num);

void IK_LFMove(float FootAngle, float AnklerAngle, uint16_t Time);
void IK_RFMove(float FootAngle, float AnklerAngle, uint16_t Time);
void IK_LBMove(float FootAngle, float AnklerAngle, uint16_t Time);
void IK_RHMove(float FootAngle, float AnklerAngle, uint16_t Time);
#endif