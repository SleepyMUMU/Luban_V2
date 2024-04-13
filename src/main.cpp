#include <Arduino.h>
#include <EEPROM.h>

#include "Drive/Peripherals.h"
#include "Drive/WiFiConfig.h"
#include "Drive/UART.h"
#include "Dynamics/Servo.h"
#include "Communication/JsonConfig.h"
#include "Dynamics/FKIK.h"

#include "FashionStar_UartServo.h"
#include "FashionStar_UartServoProtocol.h"

#include "Drive/BTConfig.h"

void Task1code(void *pvParameters);
void Task2code(void *pvParameters);

String cmd;

bool flag = 0;

void MainLogicTree();



void setup()
{
    // BluetoothInit();
    UARTinit();
    PeripheralsInit();
    WiFiInit();
    TCPServer();
    OTAconfig();
    // httpServer();
    JsonInit();
    ServoInit();

    xTaskCreatePinnedToCore(Task1code, "Task1", 10000, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(Task2code, "Task2", 10000, NULL, 1, NULL, 1);
}

void Task1code(void *pvParameters)
{
    String temp;
    while (1)
    {
        if (WiFi.isConnected())
        {
            if (client.available())
            {
                cmd = TCPcom();
                temp = cmd;
            }
            if (Nano.available())
            {
                client.write(Nano.read());
            }
        }

        if (STM32COM.available())
        {
            while (STM32COM.available())
            {
                cmd += char(STM32COM.read());
            }
            temp = cmd;
        }

        if (temp.length() != 0)
        {
            flag = 1;
            DebugSerial.println(temp);
            temp = "";
        }

        delay(1);
    }
}

void Task2code(void *pvParameters)
{

    DebugSerial.print("*********Robot-LUBAN*********");

    while (1)
    {
        if (flag)
        {
            MainLogicTree();
        }
        ArduinoOTA.handle();
        delay(1);
    }
}

void ChangeToHttp()
{
    DebugSerial.printf("Change to HttpCOM,Enter any key to switch to TCPCOM.\n");
    cmd = "";
    while (cmd.length() == 0)
    {
        DebugSerial.printf("");
        DebugSerial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if (httpCode > 0)
        {
            // HTTP header has been send and Server response header has been handled
            DebugSerial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if (httpCode == HTTP_CODE_OK)
            {

                String payload = http.getString();
                DebugSerial.println(payload);
                cJSON *Basic = cJSON_Parse(payload.c_str());

                if (Basic == NULL)
                {
                    DebugSerial.println("[JSON]Error to Analyse Json,Check the JsonString.");
                }
                else
                {
                    DebugSerial.println("[JSON]Sueeced read the data.");
                    cJSON *MotorConfig = cJSON_GetObjectItem(Basic, "MotorConfig");
                    if (cJSON_IsString(MotorConfig))
                    {
                        STM32COM.println(MotorConfig->valuestring);
                        // DebugSerial.println("[JSON]Get \"DefaultModeConfig\":" + DefaultTnitMode);
                    }
                    else
                    {
                        DebugSerial.println("[JSON]ERROR! \"MotorConfig\" is not a string.");
                    }
                }
            }
        }
        else
        {
            DebugSerial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
    }
}

void ChangeConfigLogicTree()
{
    while (flag != 0)
    {

        DebugSerial.println("[Config]Please choose the Config:");

        DebugSerial.println("1.DefaultAngleArray\t2.PosAArray\t.3PosBArray\t.4PosCArray\t5.PosDArray 6.");

        DebugSerial.println("Enter [exit] to exit.");

        cmd = "";
        while (!cmd.length())
        {
            delay(10);
        }

        if (!strcmp(cmd.c_str(), "exit"))
        {
            DebugSerial.println("Already exited.");
            break;
        }
        else if (cmd.toInt() <= 5) // 记得添加选项后修改范围
        {
            u8_t PosID = cmd.toInt() - 1;
            while (flag != 0)
            {
                DebugSerial.println(PosName[PosID]);
                for (size_t i = 0; i < sizeof(PosArray[PosID]) / sizeof(PosArray[PosID][0]); i++)
                {
                    DebugSerial.print("#");
                    DebugSerial.print(int(i + 1));
                    DebugSerial.println(":" + String(PosArray[PosID][i], 1));
                }
                DebugSerial.println("Please enter the ID of the Servo:");
                cmd = "";
                while (!cmd.length())
                {
                    delay(10);
                }

                if (!strcmp(cmd.c_str(), "exit"))
                {
                    DebugSerial.println("Already exited.");
                    break;
                }
                else if (cmd.toInt() <= 10) // 记得添加选项后修改范围
                {

                    u8_t ServoID = cmd.toInt() - 1;
                    DebugSerial.println("Please enter the new Config");

                    cmd = "";
                    while (!cmd.length())
                    {
                        delay(10);
                    }

                    if (!strcmp(cmd.c_str(), "exit"))
                    {
                        DebugSerial.println("Already exited.");
                        break;
                    }
                    else
                    {
                        // DebugSerial.println("Already exited.");
                        PosArray[PosID][ServoID] = cmd.toFloat();
                    }
                }
            }
        }
    }
}

void MainLogicTree()
{
    if (!strcmp(cmd.c_str(), "save"))
    {
        SaveConfigAsJsonToEeprom();
    }
    else if (!strcmp(cmd.c_str(), "beep"))
    {
        Beep(150);
    }
    else if (!strcmp(cmd.c_str(), "changeconfig"))
    {
        ChangeConfigLogicTree();
    }
    else if (!strcmp(cmd.c_str(), "restart"))
    {
        esp_restart();
    }
    else if (!strcmp(cmd.c_str(), "get"))
    {
        DebugSerial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if (httpCode > 0)
        {
            // HTTP header has been send and Server response header has been handled
            DebugSerial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if (httpCode == HTTP_CODE_OK)
            {
                String payload = http.getString();
                DebugSerial.println(payload);
            }
        }
        else
        {
            DebugSerial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
    }
    else if (!strcmp(cmd.c_str(), "json"))
    {
    }
    else if (!strcmp(cmd.c_str(), "up"))
    {
        RobotUp();
    }
    else if (!strcmp(cmd.c_str(), "ik"))
    {
        float angle[2];
        IK(90, -125, angle);
        DebugSerial.print("angle[0]:");
        DebugSerial.println(angle[0]);
        DebugSerial.print("angle[1]:");
        DebugSerial.println(angle[1]);
    }
    else if (!strcmp(cmd.c_str(), "stm"))
    {
        while (!DebugSerial.available())
        {
            DebugSerial.print("[STM32]Please enter the String you wanna to send：");
            cmd = "";
            while (!cmd.length())
            {
                delay(10);
            }
            if (!strcmp(cmd.c_str(), "exit"))
            {
                DebugSerial.println("Already exited.");
                break;
            }
            STM32COM.print(cmd);
            DebugSerial.println(String("[STMDebugSend]" + cmd));
            while (!(STM32COM.available() || DebugSerial.available()))
                ;
            if (STM32COM.available())
            {
                cmd = "";
                cmd = STMcom();
                DebugSerial.print("[STM]Report:");
                DebugSerial.println(cmd);
            }
            else
            {
                DebugSerial.printf("[TimeOut]STM message Time Out.\n");
            }

            // DebugSerial.println(String("Sended:" + Data));
        }
    }
    else if (!strcmp(cmd.c_str(), "setankle"))
    {
        while (flag != 0)
        {
            DebugSerial.println("[Ankle]Please enter the angle of ANKLE.Enter [exit] to exit.");

            cmd = "";
            while (!cmd.length())
            {
                delay(10);
            }
            if (!strcmp(cmd.c_str(), "exit"))
            {
                DebugSerial.println("Already exited.");
                break;
            }
            RobotAnkleAngle(cmd.toFloat());
        }
    }
    else if (!strcmp(cmd.c_str(), "setfoot"))
    {
        while (!DebugSerial.available())
        {
            DebugSerial.println("[Foot]Please enter the angle of FOOT.Enter [exit] to exit.");
            cmd = "";
            while (!cmd.length())
            {
                delay(10);
            }
            if (!strcmp(cmd.c_str(), "exit"))
            {
                DebugSerial.println("Already exited.");
                break;
            }
            RobotFootAngle(cmd.toFloat());
        }
    }
    else if (!strcmp(cmd.c_str(), "reportangleloop"))
    {
        while (!DebugSerial.available())
        {
            GetServoAngle();
            delay(1000);
            if (!strcmp(cmd.c_str(), "exit"))
            {
                break;
            }
        }
    }
    else if (!strcmp(cmd.c_str(), "http"))
    {
        ChangeToHttp();
    }
    else if (!strcmp(cmd.c_str(), "hi"))
    {
        DebugSerial.print("[Luban]Hi!");
    }
    else if (!strcmp(cmd.c_str(), "powerdown"))
    {
        Left.setTorque(false);
        Right.setTorque(false);
    }
    else if (!strcmp(cmd.c_str(), "cyc"))
    {
        while (!DebugSerial.available())
        {
            DebugSerial.println("[Forword Ankle]Please enter the angle of FORWORD Ankle.Enter [exit] to exit.");

            cmd = "";
            while (!cmd.length())
            {
                delay(10);
            }
            if (!strcmp(cmd.c_str(), "exit"))
            {
                DebugSerial.println("Already exited.");
                break;
            }
            RobotForwordAnkleAngle(cmd.toFloat());
        }
    }
    else if (!strcmp(cmd.c_str(), "trot"))
    {
        Trot();
    }
    else if (!strcmp(cmd.c_str(), "setdamp"))
    {
        Left.setDamping(1000);
        Right.setDamping(1000);
    }
    else if (!strcmp(cmd.c_str(), "setangle"))
    {
        while (flag != 0)
        {
            DebugSerial.println("Enter the ID of the Servo:");
            cmd = "";
            while (!cmd.length())
            {
                delay(10);
            }
            if (!strcmp(cmd.c_str(), "exit"))
            {
                DebugSerial.println("Already exited.");
                break;
            }
            DebugSerial.print("Current angle is:");
            switch (cmd.toInt())
            {
            case 1:

                DebugSerial.println(LFF.queryAngle(), 1);
                break;
            case 2:

                DebugSerial.println(LFA.queryAngle(), 1);
                break;

            case 3:

                DebugSerial.println(RFF.queryAngle(), 1);
                break;

            case 4:

                DebugSerial.println(RFA.queryAngle(), 1);
                break;

            case 5:

                DebugSerial.println(LHF.queryAngle(), 1);
                break;

            case 6:

                DebugSerial.println(LHA.queryAngle(), 1);
                break;

            case 7:

                DebugSerial.println(RHF.queryAngle(), 1);
                break;

            case 8:

                DebugSerial.println(RHA.queryAngle(), 1);
                break;

            default:
                break;
            }
            DebugSerial.println("Enter the FIX angle:");
            u8_t ServoID = cmd.toInt();
            cmd = "";
            while (!cmd.length())
            {
                delay(10);
            }
            if (!strcmp(cmd.c_str(), "exit"))
            {
                DebugSerial.println("Already exited.");
                break;
            }
            DebugSerial.print("Change angle to:");
            switch (ServoID)
            {
            case 1:
                LFF.setAngle(cmd.toFloat(), DefaultTime);
                DebugSerial.println(cmd.toFloat());
                delay(DefaultTime);
                break;
            case 2:
                LFA.setAngle(cmd.toFloat(), DefaultTime);
                DebugSerial.println(cmd.toFloat());
                delay(DefaultTime);
                break;

            case 3:
                RFF.setAngle(cmd.toFloat(), DefaultTime);
                DebugSerial.println(cmd.toFloat());
                delay(DefaultTime);
                break;

            case 4:
                RFA.setAngle(cmd.toFloat(), DefaultTime);
                DebugSerial.println(cmd.toFloat());
                delay(DefaultTime);
                break;

            case 5:
                LHF.setAngle(cmd.toFloat(), DefaultTime);
                DebugSerial.println(cmd.toFloat());
                delay(DefaultTime);
                break;

            case 6:
                LHA.setAngle(cmd.toFloat(), DefaultTime);
                DebugSerial.println(cmd.toFloat());
                delay(DefaultTime);
                break;

            case 7:
                RHF.setAngle(cmd.toFloat(), DefaultTime);
                DebugSerial.println(cmd.toFloat());
                delay(DefaultTime);
                break;

            case 8:
                RHA.setAngle(cmd.toFloat(), DefaultTime);
                DebugSerial.println(cmd.toFloat());
                delay(DefaultTime);
                break;

            default:
                break;
            }
        }
    }
    else if (!strcmp(cmd.c_str(), "forwordfoot"))
    {
        while (!DebugSerial.available())
        {
            DebugSerial.println("[Forword Foot]Please enter the angle of FORWORD FOOT.Enter [exit] to exit.");

            cmd = "";
            while (!cmd.length())
            {
                delay(10);
            }
            if (!strcmp(cmd.c_str(), "exit"))
            {
                DebugSerial.println("Already exited.");
                break;
            }
            RobotForwordFootAngle(cmd.toFloat());
        }
    }
    else if (!strcmp(cmd.c_str(), "forwordankle"))
    {
        while (!DebugSerial.available())
        {
            DebugSerial.println("[Forword Ankle]Please enter the angle of FORWORD Ankle.Enter [exit] to exit.");

            cmd = "";
            while (!cmd.length())
            {
                delay(10);
            }
            if (!strcmp(cmd.c_str(), "exit"))
            {
                DebugSerial.println("Already exited.");
                break;
            }
            RobotForwordAnkleAngle(cmd.toFloat());
        }
    }
    else if (!strcmp(cmd.c_str(), "bhindankle"))
    {
        while (!DebugSerial.available())
        {
            DebugSerial.println("[Bhind Ankle]Please enter the angle of Bhind Ankle.Enter [exit] to exit.");

            cmd = "";
            while (!cmd.length())
            {
                delay(10);
            }
            if (!strcmp(cmd.c_str(), "exit"))
            {
                DebugSerial.println("Already exited.");
                break;
            }
            RobotBhindAnkleAngle(cmd.toFloat());
        }
    }
    else if (!strcmp(cmd.c_str(), "bhindfoot"))
    {
        while (!DebugSerial.available())
        {
            DebugSerial.println("[Bhind Foot]Please enter the angle of Bhind Foot.Enter [exit] to exit.");
            cmd = "";
            while (!cmd.length())
            {
                delay(10);
            }

            if (!strcmp(cmd.c_str(), "exit"))
            {
                DebugSerial.println("Already exited.");
                break;
            }
            RobotBhindFootAngle(cmd.toFloat());
        }
    }
    else if (!strcmp(cmd.c_str(), "trackb"))
    {
        RobotTrackB();
    }
    else if (!strcmp(cmd.c_str(), "trackb"))
    {
        RobotTrackB();
    }
    else if (!strcmp(cmd.c_str(), "world"))
    {
        while (flag != 0)
        {
            /* code */
            int Time = 200;
            DebugSerial.println("Enter the Num:");
            cmd = "";
            while (!cmd.length())
            {
                delay(10);
            }
            if (!strcmp(cmd.c_str(), "exit"))
            {
                break;
            }

            u8_t CaseNum = cmd.toInt();

            DebugSerial.println("Enter the XS:");
            cmd = "";
            while (!cmd.length())
            {
                delay(10);
            }
            if (!strcmp(cmd.c_str(), "exit"))
            {
                break;
            }
            int DebugXS = cmd.toInt();

            DebugSerial.println("Enter the YS:");
            cmd = "";
            while (!cmd.length())
            {
                delay(10);
            }
            if (!strcmp(cmd.c_str(), "exit"))
            {
                break;
            }

            int DebugYS = cmd.toInt();

            switch (CaseNum)
            {
            case 1:
                IK_LFMove(DebugXS, DebugYS, Time);
                break;
            case 2:
                IK_RFMove(DebugXS, DebugYS, Time);
                break;
            case 3:
                IK_RHMove(DebugXS, DebugYS, Time);
                break;
            case 4:
                IK_LBMove(DebugXS, DebugYS, Time);
                break;
            default:
                break;
            }
            delay(Time);
        }
    }
    else if (!strcmp(cmd.c_str(), "round"))
    {
        cmd = "";
        while (!cmd.length())
        {
            delay(10);
        }
        u8_t Num = cmd.toInt();
        roundPos(Num);
    }
    else if (!strcmp(cmd.c_str(), "restm"))
    {
        digitalWrite(18, LOW);
        digitalWrite(19, LOW);
    }
    else if (!strcmp(cmd.c_str(), "down"))
    {
        RobotDown();
    }
    else
    {
        DebugSerial.println("[ERROR]Unknowed CMD!");
    }

    cmd.clear();
    flag = 0;
}

void loop()
{
}
