#include "Servo.h"
#include "Communication\JsonConfig.h"
#include "FKIK.h"

extern String cmd;

FSUS_Protocol RoboticArmGroup_1(&ServoSerial1, ServoSerial1BaudRate); // 协议
FSUS_Protocol RoboticArmGroup_2(&ServoSerial2, ServoSerial2BaudRate); // 协议

FSUS_Servo LFF(Left_Forefoot_Servo, &RoboticArmGroup_2);  // 创建Left Foot左前脚舵机
FSUS_Servo LFA(Left_ForeAnkle_Servo, &RoboticArmGroup_2); // 创建Left Ankle左前踝舵机
FSUS_Servo LHF(Left_Hindfoot_Servo, &RoboticArmGroup_2);  // 创建Left Foot左前脚舵机
FSUS_Servo LHA(Left_HindAnkle_Servo, &RoboticArmGroup_2);

FSUS_Servo RFF(Right_Forefoot_Servo, &RoboticArmGroup_1); // 创建Left Foot左前脚舵机
FSUS_Servo RFA(Right_ForeAnkle_Servo, &RoboticArmGroup_1);
FSUS_Servo RHF(Right_Hindfoot_Servo, &RoboticArmGroup_1); // 创建Left Foot左前脚舵机
FSUS_Servo RHA(Right_HindAnkle_Servo, &RoboticArmGroup_1);
FSUS_Servo CAM_X(Cam_X_Servo, &RoboticArmGroup_1);
FSUS_Servo CAM_Y(Cam_Y_Servo, &RoboticArmGroup_1);
FSUS_Servo Find_X(Find_X_Servo, &RoboticArmGroup_2);
FSUS_Servo Find_Y(Find_Y_Servo, &RoboticArmGroup_2);

FSUS_Servo Right(255, &RoboticArmGroup_1);
FSUS_Servo Left(255, &RoboticArmGroup_2);

String Temp;

void RobotTrackB();
void ServoInit()
{
    // delay(10);
    LFF.init();
    LFA.init();
    RFF.init();
    RFA.init();
    LHF.init();
    LHA.init();
    RHF.init();
    RHA.init();
    CAM_X.init();
    CAM_Y.init();
    Find_X.init();
    Find_Y.init();
    delay(100);
    Find_X.setAngleRange(-90, +90);
    Find_X.setAngle(0);
    Find_Y.setAngle(0);
    CAM_X.setAngleRange(-90, +90);
    CAM_X.setAngle(+45, 300);
    delay(800);
    CAM_X.setAngle(-45, 300);
    delay(800);
    CAM_X.setAngle(0, 300);
    delay(300);
    CAM_Y.setAngle(45, 200);
    delay(200);
    CAM_Y.setAngle(0, 200);
    delay(200);
    CAM_Y.setAngle(45, 200);
    delay(200);
    CAM_Y.setAngle(0, 200);
    delay(10);
    RobotTrackB();
    digitalWrite(HIGH, 2);

    // crawlerB();
    //  CAM_X.setAngle(1000);
    //  /CAM_Y.setAngle();
    //  ALL.setDamping(1000);
}

void ServoBackup()
{
    int backuptime = 2000;
    LFF.setAngle(PosArray[0][0], backuptime);
    LFA.setAngle(PosArray[0][1], backuptime);
    RFF.setAngle(PosArray[0][2], backuptime);
    RFA.setAngle(PosArray[0][3], backuptime);
    LHF.setAngle(PosArray[0][4], backuptime);
    LHA.setAngle(PosArray[0][5], backuptime);
    RHF.setAngle(PosArray[0][6], backuptime);
    RHA.setAngle(PosArray[0][7], backuptime);
    RHA.wait();
    delay(backuptime);
}

void ServoAngleCorrection()
{
    GetServoAngle();
    client.println("[Debug]选择你要矫正的机械臂:");
    while (!client.available())
        ;

    if (client.available())
    {
        Temp += String(client.read());
    }

    if (!strcmp(Temp.c_str(), "LH"))
    {
        /* code */
    }
}

void GetServoAngle()
{

    client.println("*****左前机械臂*****");
    client.println(String("LFF:" + (String(LFF.queryAngle(), 1))));
    client.println(String("LFA:" + (String(LFA.queryAngle(), 1))));

    client.println("*****右前机械臂*****");
    client.println(String("RFF:" + (String(RFF.queryAngle(), 1))));
    client.println(String("RFA:" + (String(RFA.queryAngle(), 1))));
    client.println("*****左后机械臂*****");
    client.println(String("LHF:" + (String(LHF.queryAngle(), 1))));
    client.println(String("LHA:" + (String(LHA.queryAngle(), 1))));
    client.println("*****右后机械臂*****");
    client.println(String("RHF:" + (String(RHF.queryAngle(), 1))));
    client.println(String("RHA:" + (String(RHA.queryAngle(), 1))));
}

void RobotFootAngle(float Angle)
{
    int RangeTime = DefaultTime;
    LFF.setAngle((LFFabs * (Angle)) + PosArray[0][0], RangeTime);

    RFF.setAngle((RFFabs * (Angle)) + PosArray[0][2], RangeTime);

    LHF.setAngle((LHFabs * (Angle)) + PosArray[0][4], RangeTime);

    RHF.setAngle((RHFabs * (Angle)) + PosArray[0][6], RangeTime);

    delay(RangeTime);
}

void RobotTrackA()
{
    int RangeTime = DefaultTime;
    LFF.setAngle(PosArray[0][0], RangeTime);
    LFA.setAngle(PosArray[0][1], RangeTime);
    RFF.setAngle(PosArray[0][2], RangeTime);
    RFA.setAngle(PosArray[0][3], RangeTime);
    LHF.setAngle(PosArray[0][4], RangeTime);
    LHA.setAngle(PosArray[0][5], RangeTime);
    RHF.setAngle(PosArray[0][6], RangeTime);
    RHA.setAngle(PosArray[0][7], RangeTime);

    delay(RangeTime);
}

void RobotTrackB()
{
    int RangeTime = DefaultTime;
    LFF.setAngle(PosArray[0][0], RangeTime);
    LFA.setAngle(PosArray[0][1], RangeTime);
    RFF.setAngle(PosArray[0][2], RangeTime);
    RFA.setAngle(PosArray[0][3], RangeTime);
    LHF.setAngle(PosArray[0][4], RangeTime);
    LHA.setAngle(PosArray[0][5], RangeTime);
    RHF.setAngle(PosArray[0][6], RangeTime);
    RHA.setAngle(PosArray[0][7], RangeTime);

    delay(RangeTime);
}

void RobotDown()
{
    int RangeTime = DefaultTime;
    LFF.setAngle((LFFabs * (-45)) + PosArray[0][0], RangeTime);
    LFA.setAngle((LFAabs * (-45)) + PosArray[0][1], RangeTime);
    RFF.setAngle((RFFabs * (-45)) + PosArray[0][2], RangeTime);
    RFA.setAngle((RFAabs * (-45)) + PosArray[0][3], RangeTime);
    LHF.setAngle((LHFabs * (-45)) + PosArray[0][4], RangeTime);
    LHA.setAngle((LHAabs * (-45)) + PosArray[0][5], RangeTime);
    RHF.setAngle((RHFabs * (-45)) + PosArray[0][6], RangeTime);
    RHA.setAngle((RHAabs * (-45)) + PosArray[0][7], RangeTime);

    delay(RangeTime);
}

void RobotUp()
{
    int RangeTime = DefaultTime;
    LFF.setAngle((LFFabs * (90)) + PosArray[0][0], RangeTime);
    LFA.setAngle((LFAabs * (-90)) + PosArray[0][1], RangeTime);
    RFF.setAngle((RFFabs * (90)) + PosArray[0][2], RangeTime);
    RFA.setAngle((RFAabs * (-90)) + PosArray[0][3], RangeTime);
    LHF.setAngle((LHFabs * (90)) + PosArray[0][4], RangeTime);
    LHA.setAngle((LHAabs * (-90)) + PosArray[0][5], RangeTime);
    RHF.setAngle((RHFabs * (90)) + PosArray[0][6], RangeTime);
    RHA.setAngle((RHAabs * (-90)) + PosArray[0][7], RangeTime);

    delay(RangeTime);
}

void RobotForwordFootAngle(float Angle)
{
    int RangeTime = DefaultTime;
    LFF.setAngle((LFFabs * (Angle)) + PosArray[0][0], RangeTime);

    RFF.setAngle((RFFabs * (Angle)) + PosArray[0][2], RangeTime);

    delay(RangeTime);
}

void RobotForwordAnkleAngle(float Angle)
{
    int RangeTime = DefaultTime;

    LFA.setAngle((LFAabs * (Angle)) + PosArray[0][1], RangeTime);

    RFA.setAngle((RFAabs * (Angle)) + PosArray[0][3], RangeTime);
    delay(RangeTime);
}

void RobotBhindFootAngle(float Angle)
{
    int RangeTime = DefaultTime;
    LHF.setAngle((LHFabs * (Angle)) + PosArray[0][0], RangeTime);
    RHF.setAngle((RHFabs * (Angle)) + PosArray[0][2], RangeTime);
    delay(RangeTime);
}

void RobotBhindAnkleAngle(float Angle)
{
    int RangeTime = DefaultTime;
    LHA.setAngle((LHAabs * (Angle)) + PosArray[0][5], RangeTime);
    RHA.setAngle((RHAabs * (Angle)) + PosArray[0][7], RangeTime);
    delay(RangeTime);
}

void RobotAnkleAngle(float Angle)
{
    int RangeTime = DefaultTime;
    // LFF.setAngle((LFFabs*(Angle))+LFFR, RangeTime);
    LFA.setAngle((LFAabs * (Angle)) + PosArray[0][1], RangeTime);
    // RFF.setAngle((RFFabs*(Angle))+RFFR, RangeTime);
    RFA.setAngle((RFAabs * (Angle)) + PosArray[0][3], RangeTime);
    // LHF.setAngle((LHFabs*(Angle))+LHFR, RangeTime);
    LHA.setAngle((LHAabs * (Angle)) + PosArray[0][5], RangeTime);
    // RHF.setAngle((RHFabs*(Angle))+RHFR, RangeTime);
    RHA.setAngle((RHAabs * (Angle)) + PosArray[0][7], RangeTime);
    delay(RangeTime);
}

void FK_LFMove(float FootAngle, float AnklerAngle, uint16_t Time)
{
    LFF.setAngle(LFFabs * FootAngle + PosArray[PosDefNum][Left_Forefoot_Servo], Time);
    // delay(6);
    LFA.setAngle(LFAabs * AnklerAngle + PosArray[PosDefNum][Left_ForeAnkle_Servo], Time);
}
void FK_LBMove(float FootAngle, float AnklerAngle, uint16_t Time)
{
    LHF.setAngle(LHFabs * FootAngle + PosArray[PosDefNum][Left_Hindfoot_Servo], Time);
    // delay(6);
    LHA.setAngle(LHAabs * AnklerAngle + PosArray[PosDefNum][Left_HindAnkle_Servo], Time);
}

void FK_RHMove(float FootAngle, float AnklerAngle, uint16_t Time)
{
    RHF.setAngle(RHFabs * FootAngle + PosArray[PosDefNum][Right_Hindfoot_Servo], Time);
    // delay(6);
    RHA.setAngle(RHAabs * AnklerAngle + PosArray[PosDefNum][Right_HindAnkle_Servo], Time);
}
void FK_RFMove(float FootAngle, float AnklerAngle, uint16_t Time)
{
    RFF.setAngle(RFFabs * FootAngle + PosArray[PosDefNum][Right_Forefoot_Servo], Time);
    // delay(6);
    RFA.setAngle(RFAabs * AnklerAngle + PosArray[PosDefNum][Right_ForeAnkle_Servo], Time);
}
void IK_RHMove(float x, float y, uint16_t Time)
{
    float angle[2] = {};

    // IK_RUPoint[0] = x;
    // IK_RUPoint[1] = y;

    IK(x, y, angle); // 逆解

    // DebugSerial.println("angle[0]");
    // DebugSerial.println(angle[0]);
    // DebugSerial.println("angle[1]");
    // DebugSerial.println(angle[1]);

    FK_RHMove(angle[1], angle[0], Time);
}

void IK_LFMove(float x, float y, uint16_t Time)
{
    float angle[2] = {};

    // IK_RUPoint[0] = x;
    // IK_RUPoint[1] = y;

    IK(x, y, angle); // 逆解

    // DebugSerial.println("angle[0]");
    // DebugSerial.println(angle[0]);
    // DebugSerial.println("angle[1]");
    // DebugSerial.println(angle[1]);

    FK_LFMove(angle[1], angle[0], Time);
}

void IK_LBMove(float x, float y, uint16_t Time)
{
    float angle[2] = {};

    // IK_RUPoint[0] = x;
    // IK_RUPoint[1] = y;

    IK(x, y, angle); // 逆解

    // DebugSerial.println("angle[0]");
    // DebugSerial.println(angle[0]);
    // DebugSerial.println("angle[1]");
    // DebugSerial.println(angle[1]);

    FK_LBMove(angle[1], angle[0], Time);
}

void IK_RFMove(float x, float y, uint16_t Time)
{
    float angle[2] = {};

    // IK_RUPoint[0] = x;
    // IK_RUPoint[1] = y;

    IK(x, y, angle); // 逆解

    // DebugSerial.println("angle[0]");
    // DebugSerial.println(angle[0]);
    // DebugSerial.println("angle[1]");
    // DebugSerial.println(angle[1]);

    FK_RFMove(angle[1], angle[0], Time);
}

/*
 * Trot小跑步态
 */
void Trot()
{

    u8_t TrotStatus = 1;
    uint8_t dir = 1;  // 方向
    uint8_t DSD = 40; // 小跑步态中的延迟时间（Delay in Single Duty）
    // String cmd;

    float sigma, xep_b /*摆动相（swing phase）*/, xep_z /*支撑相（stance phase）*/, yep;
    float t = 0;

    float speed = 0.04; // 步频

    float Ts = 1, fai = 0.5; // 周期T， 占空比fai(支撑相)
    float offset = 50;       // 步态的步幅（stride）

    float xs = XS - offset, xf = XS + offset; // 起始坐标，终点坐标
    float ys = -150, yh = 35;                 // 起始坐标，抬腿高度

    // 足端坐标
    float x1 = XS, x2 = XS, x3 = XS, x4 = XS;
    float y1 = YS, y2 = YS, y3 = YS, y4 = YS;

    // cmd = "";
    // DebugSerial.println("Enter the dir:");
    // while (!cmd.length())
    // {
    //     delay(10);
    // }
    // dir = cmd.toInt();

    // cmd = "";
    // DebugSerial.println("Enter the DSD:");
    // while (!cmd.length())
    // {
    //     delay(10);
    // }
    // DSD = cmd.toInt();

    // cmd = "";
    // DebugSerial.println("Enter the Speed:");
    // while (!cmd.length())
    // {
    //     delay(10);
    // }
    // speed = cmd.toFloat();

    // cmd = "";
    // DebugSerial.println("Enter the yh:");
    // while (!cmd.length())
    // {
    //     delay(10);
    // }
    // yh = cmd.toInt();

    // cmd = "";
    // DebugSerial.println("Enter the offset:");
    // while (!cmd.length())
    // {
    //     delay(10);
    // }
    // offset = cmd.toInt();

    DebugSerial.println(F("启动Trot模式..."));

    while (TrotStatus)
    {
        // 2.单个步态周期
        while (t < 1)
        {
            // 前半周期
            if (t <= Ts * fai)
            {
                sigma = 2 * PI * t / (fai * Ts);
                yep = yh * (1 - cos(sigma)) / 2 + ys; // y轴坐标

                xep_b = (xf - xs) * (sigma - sin(sigma)) / (2 * PI) + xs; // 摆动相x轴坐标
                xep_z = (xs - xf) * (sigma - sin(sigma)) / (2 * PI) + xf; // 支撑相x轴坐标

                if (dir != 2)
                {
                    y1 = yep;
                    y2 = ys;
                    y3 = yep;
                    y4 = ys;
                }

                if (dir == 1)
                {
                    x1 = xep_b;
                    x2 = xep_z;
                    x3 = xep_z;
                    x4 = xep_b;
                }
                else if (dir == 0)
                {
                    x1 = xep_z;
                    x2 = xep_b;
                    x3 = xep_b;
                    x4 = xep_z;
                }
                else if (dir == 2)
                {
                    x1 = XS;
                    x2 = XS;
                    x3 = XS;
                    x4 = XS;
                }
            }
            // 后半周期
            if (Ts * fai < t && t < Ts)
            {
                sigma = 2 * PI * (t - Ts * fai) / (fai * Ts);
                yep = yh * (1 - cos(sigma)) / 2 + ys; // y轴坐标

                xep_b = (xf - xs) * (sigma - sin(sigma)) / (2 * PI) + xs; // 摆动相x轴坐标
                xep_z = (xs - xf) * (sigma - sin(sigma)) / (2 * PI) + xf; // 支撑相x轴坐标

                if (dir != 2)
                {
                    y1 = ys;
                    y2 = yep;
                    y3 = ys;
                    y4 = yep;
                }

                if (dir == 1)
                {
                    x1 = xep_z;
                    x2 = xep_b;
                    x3 = xep_b;
                    x4 = xep_z;
                }
                else if (dir == 0)
                {
                    x1 = xep_b;
                    x2 = xep_z;
                    x3 = xep_z;
                    x4 = xep_b;
                }
                else if (dir == 2)
                {
                    x1 = XS;
                    x2 = XS;
                    x3 = XS;
                    x4 = XS;
                }
            }

            t = t + speed;

            // DebugSerial.print("[x1]");
            // DebugSerial.println(x1);
            // DebugSerial.print("[x2]");
            // DebugSerial.println(x2);
            // DebugSerial.print("[x3]");
            // DebugSerial.println(x3);
            // DebugSerial.print("[x4]");
            // DebugSerial.println(x4);
            // DebugSerial.print("[y1]");
            // DebugSerial.println(y1);
            // DebugSerial.print("[y2]");
            // DebugSerial.println(y2);
            // DebugSerial.print("[y3]");
            // DebugSerial.println(y3);
            // DebugSerial.print("[y4]");
            // DebugSerial.println(y4);
            IK_LFMove(x1 + (XSLF - XS), y1 + (YSLF - YS), DSD);
            IK_RFMove(x2 + (XSRF - XS), y2 + (YSRF - YS), DSD);
            IK_RHMove(x3 + (XSRH - XS), y3 + (YSRH - YS), DSD);
            IK_LBMove(x4 + (XSLH - XS), y4 + (YSLH - YS), DSD);
            // IK_LFMove(x1, y1, DSD);
            // IK_RFMove(x2, y2, DSD);
            // IK_RHMove(x3, y3, DSD);
            // IK_LBMove(x4, y4, DSD);
            // LFF.wait();
            delay(DSD);
        }

        if (!strcmp(cmd.c_str(), "exit"))
        {
            TrotStatus = 0;
            break;
        }
        if (TrotStatus)
        {
            t = 0;
        }
    }
}

void PosToPoint(float x, float y, uint16_t Time)
{
    IK_LFMove(XSLF - x, YSLF + y, Time);
    IK_RFMove(XSRF - x, YSRF - y, Time);
    IK_RHMove(XSRH + x, YSRH - y, Time);
    IK_LBMove(XSLH + x, YSLH - y, Time);
}

void roundPos(uint8_t Num)
{
    uint8_t DSD = 30;

    float r = 40;
    float speed = 5;
    float deg = 0;
    float theta;
    float xr, yr;

    for (uint8_t i = 0; i < Num; i++)
    {
        while (deg <= 360)
        {
            theta = D2R(deg);

            xr = r * cos(theta);
            yr = r * sin(theta);

            PosToPoint(xr, yr, DSD);
            delay(DSD);

            deg += speed;
        }

        deg = 0;
    }

    delay(500);
    PosToPoint(0, 0, 200);
}

void GetCycloidPoints(float CPoints[][2], float xs, float xf, float ys, float yh)
{
    uint8_t count = 0;

    float offset = 0.06; // 生成10个点
    float sigma, xep, yep;
    float t;

    float Ts = 1, fai = 0.5; // 周期T， 占空比fai(支撑相)

    /* ############### 计算部分 ############### */
    for (t = 0; t <= Ts * fai; t += offset)
    {
        sigma = 2 * PI * t / (fai * Ts);
        xep = (xf - xs) * (sigma - sin(sigma)) / (2 * PI) + xs; // x轴坐标
        yep = yh * (1 - cos(sigma)) / 2 + ys;                   // y轴坐标

        CPoints[count][0] = xep;
        CPoints[count][1] = yep;
        count += 1;
    }

    CPoints[count][0] = xf;
    CPoints[count][1] = ys;
    count += 1;
    /* ######################################## */
}

void LegCycloid(float CPoints[][2], uint8_t LEGNum)
{
    uint8_t Num = 10;
    uint8_t DSD = 250 / Num;

    for (uint8_t i = 0; i < 10; i++)
    {
        // switch (LEGNum)
        // {
        // case 1:
        //     // IK_RUMove(CPoints[i][0], CPoints[i][1], DSD);
        //     DebugSerial.print("CPoints[i][0]:");
        //     DebugSerial.println(CPoints[i][0]);
        //     DebugSerial.print("CPoints[i][1]:");
        //     DebugSerial.println(CPoints[i][1]);
        //     break;
        // case 2:
        //     IK_RBMove(CPoints[i][0], CPoints[i][1], DSD);
        //     break;
        // case 3:
        //     IK_LBMove(CPoints[i][0], CPoints[i][1], DSD);
        //     break;
        // case 4:
        //     IK_LUMove(CPoints[i][0], CPoints[i][1], DSD);
        //     break;
        // }
        DebugSerial.print("CPoints[i][0]:");
        DebugSerial.println(CPoints[i][0]);
        DebugSerial.print("CPoints[i][1]:");
        DebugSerial.println(CPoints[i][1]);
        delay(DSD);
    }
}
