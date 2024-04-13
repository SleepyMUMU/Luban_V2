#include "JsonConfig.h"
#include "Dynamics/Servo.h"
#include <EEPROM.h>

String DefaultTnitMode = "TrackB";
int DefaultTime = 2000;

String test;

float PosArray[6][10] =
    {
        {LFFR, LFAR, RFFR, RFAR, LHFR, LHAR, RHFR, RHAR},
        {0},
        {0},
        {0},
        {0},
        {0},
};

String PosName[6] =
    {"DefaultPos",
     "PosA",
     "PosB",
     "PosC",
     "PosD",
     "PosQ"};

String EepromRead()
{
    String data;
    u16_t temp = '\0';
    for (size_t addr = 15; addr < EEPROM.length() && (temp = EEPROM.read(addr)) != 255; addr++)
    {
        data += char(temp);
    }
    return data;
}

void EepromSave(String data)
{
    for (size_t addr = 15; addr < data.length() + 15; addr++)
    {
        EEPROM.write(addr, data.charAt(addr - 15));
    }
    EEPROM.commit();
}

void JSONArrayGet(cJSON *root, String item, u8_t ArrayNum)
{
    cJSON *JSONArray = cJSON_GetObjectItem(root, item.c_str());
    // DebugSerial.println("[ANGLE]Get item[" + item + "]:");
    for (size_t i = 0; i < cJSON_GetArraySize(JSONArray); i++)
    {
        PosArray[ArrayNum][i] = cJSON_GetArrayItem(JSONArray, i)->valuedouble;
        String message = String(PosArray[ArrayNum][i], 1);
        // DebugSerial.println(message);
    }
}

// void JsonAnalyse(String ReciveData)
// {
//     ReciveData = EepromRead();

//     cJSON *Basic = cJSON_Parse(ReciveData.c_str());

//     if (Basic == NULL)
//     {
//         DebugSerial.println("[JSON]Error to Analyse Json,Check the JsonString.");
//     }
//     else
//     {
//         DebugSerial.println("[JSON]Sueeced read the data.");
//         cJSON *RobotPos = cJSON_GetObjectItem(Basic, "RobotPos");
//         if (!RobotPos)
//         {
//             DebugSerial.println("[JSON]ERROR!Failed to parse \"RobotPos\"");
//         }
//         if (cJSON_IsString(RobotPos))
//         {
//             DefaultTnitMode.clear();
//             DefaultTnitMode = DefaultModeConfig->valuestring;
//             DebugSerial.println("[JSON]Get \"DefaultModeConfig\":" + DefaultTnitMode);
//         }
//         else
//         {
//             DebugSerial.println("[JSON]ERROR! \"DefaultModeConfig\" is not a string.");
//         }   

//         cJSON *DefaultTimeConfig = cJSON_GetObjectItem(Basic, "DefaultInitTime");
//         DefaultTime = DefaultTimeConfig->valueint;
//         DebugSerial.print("[JSON]Get \"DefaultTime\":");
//         DebugSerial.println(DefaultTime);

//         // float posa[10];

//         JSONArrayGet(Basic, "DefaultAngle", PosDefNum);
//         JSONArrayGet(Basic, "PosA", PosANum);
//         JSONArrayGet(Basic, "PosB", PosBNum);
//         JSONArrayGet(Basic, "PosC", PosCNum);
//         JSONArrayGet(Basic, "PosD", PosDNum);
//         // DebugSerial.println("PosArray[PosDefNum][i]");
//         //  for (size_t i = 0; i < 10; i++)
//         //  {
//         //      DebugSerial.println(PosArray[PosDefNum][i]);
//         //  }
//     }
//     // DebugSerial.println(String(cJSON_Print(Basic)));
//     //  cJSON_AddItemToObject(Basic, "MotorConfig", MotorConfig);
//     //  cJSON_AddStringToObject(MotorConfig, "soure", "Forward"); // 添加字符串类型数据到子叶结构体
// }

void JsonInit()
{

    if (!EEPROM.begin(0x1000))
    {
        DebugSerial.println("[EEPROM]Failed to initialise EEPROM.");
    }

    String ReciveData = EepromRead();
    // String ReciveData;
    // DebugSerial.println("[EEPROM]Readed:");
    // DebugSerial.println(ReciveData);

    cJSON *Basic = cJSON_Parse(ReciveData.c_str());

    if (Basic == NULL)
    {

        DebugSerial.println("[EEPROM]Error to read EEPROM,use default config.");

        cJSON *Basic = cJSON_CreateObject();       // 创建JSON根部结构体
        cJSON *ServoConfig = cJSON_CreateObject(); // 创建JSON子叶结构体

        cJSON *DefaultAngle = cJSON_CreateFloatArray(PosArray[PosDefNum], sizeof(PosArray[PosDefNum]) / sizeof(PosArray[PosDefNum][0]));
        cJSON *PosA = cJSON_CreateFloatArray(PosArray[PosANum], sizeof(PosArray[PosDNum]) / sizeof(PosArray[PosANum][0]));
        cJSON *PosB = cJSON_CreateFloatArray(PosArray[PosBNum], sizeof(PosArray[PosDNum]) / sizeof(PosArray[PosBNum][0]));
        cJSON *PosC = cJSON_CreateFloatArray(PosArray[PosCNum], sizeof(PosArray[PosDNum]) / sizeof(PosArray[PosCNum][0]));
        cJSON *PosD = cJSON_CreateFloatArray(PosArray[PosDNum], sizeof(PosArray[PosDNum]) / sizeof(PosArray[PosDNum][0]));

        cJSON_AddStringToObject(Basic, "DefaultTnitMode", DefaultTnitMode.c_str()); // 添加初始化工作模式到Basic
        cJSON_AddNumberToObject(Basic, "DefaultInitTime", DefaultTime);             // 添加初始化时间到Basic
        cJSON_AddItemToObject(Basic, "DefaultAngle", DefaultAngle);                 // 添加字符串类型数据到子叶结构体
        cJSON_AddItemToObject(Basic, "PosA", PosA);                                 // 添加字符串类型数据到子叶结构体
        cJSON_AddItemToObject(Basic, "PosB", PosB);                                 // 添加字符串类型数据到子叶结构体
        cJSON_AddItemToObject(Basic, "PosC", PosC);                                 // 添加字符串类型数据到子叶结构体
        cJSON_AddItemToObject(Basic, "PosD", PosD);                                 // 添加字符串类型数据到子叶结构体
    }
    else
    {
        DebugSerial.println("[JSON]Sueeced read the EEPROM data.");
        cJSON *DefaultModeConfig = cJSON_GetObjectItem(Basic, "DefaultTnitMode");
        if (!DefaultModeConfig)
        {
            DebugSerial.println("[JSON]ERROR!Failed to parse \"DefaultModeConfig\"");
        }
        if (cJSON_IsString(DefaultModeConfig))
        {
            DefaultTnitMode.clear();
            DefaultTnitMode = DefaultModeConfig->valuestring;
            DebugSerial.println("[JSON]Get \"DefaultModeConfig\":" + DefaultTnitMode);
        }
        else
        {
            DebugSerial.println("[JSON]ERROR! \"DefaultModeConfig\" is not a string.");
        }

        cJSON *DefaultTimeConfig = cJSON_GetObjectItem(Basic, "DefaultInitTime");
        DefaultTime = DefaultTimeConfig->valueint;
        DebugSerial.print("[JSON]Get \"DefaultTime\":");
        DebugSerial.println(DefaultTime);

        // float posa[10];

        JSONArrayGet(Basic, "DefaultAngle", PosDefNum);
        JSONArrayGet(Basic, "PosA", PosANum);
        JSONArrayGet(Basic, "PosB", PosBNum);
        JSONArrayGet(Basic, "PosC", PosCNum);
        JSONArrayGet(Basic, "PosD", PosDNum);
        // DebugSerial.println("PosArray[PosDefNum][i]");
        //  for (size_t i = 0; i < 10; i++)
        //  {
        //      DebugSerial.println(PosArray[PosDefNum][i]);
        //  }
    }
    // DebugSerial.println(String(cJSON_Print(Basic)));
    //  cJSON_AddItemToObject(Basic, "MotorConfig", MotorConfig);
    //  cJSON_AddStringToObject(MotorConfig, "soure", "Forward"); // 添加字符串类型数据到子叶结构体
}

void ShowJsonConfig()
{
    DebugSerial.println("****[ShowJsonConfig]****");
}

void saveinit()
{
    cJSON *Basic = cJSON_CreateObject();       // 创建JSON根部结构体
    cJSON *ServoConfig = cJSON_CreateObject(); // 创建JSON子叶结构体

    cJSON *DefaultAngle = cJSON_CreateFloatArray(PosArray[PosDefNum], sizeof(PosArray[PosDefNum]) / sizeof(PosArray[PosDefNum][0]));
    cJSON *PosA = cJSON_CreateFloatArray(PosArray[PosANum], sizeof(PosArray[PosDNum]) / sizeof(PosArray[PosANum][0]));
    cJSON *PosB = cJSON_CreateFloatArray(PosArray[PosBNum], sizeof(PosArray[PosDNum]) / sizeof(PosArray[PosBNum][0]));
    cJSON *PosC = cJSON_CreateFloatArray(PosArray[PosCNum], sizeof(PosArray[PosDNum]) / sizeof(PosArray[PosCNum][0]));
    cJSON *PosD = cJSON_CreateFloatArray(PosArray[PosDNum], sizeof(PosArray[PosDNum]) / sizeof(PosArray[PosDNum][0]));

    cJSON_AddStringToObject(Basic, "DefaultTnitMode", "TrackB"); // 添加初始化工作模式到Basic
    cJSON_AddNumberToObject(Basic, "DefaultInitTime", 2000);     // 添加初始化时间到Basic
    cJSON_AddItemToObject(Basic, "DefaultAngle", DefaultAngle);  // 添加字符串类型数据到子叶结构体
    cJSON_AddItemToObject(Basic, "PosA", PosA);                  // 添加字符串类型数据到子叶结构体
    cJSON_AddItemToObject(Basic, "PosB", PosB);                  // 添加字符串类型数据到子叶结构体
    cJSON_AddItemToObject(Basic, "PosC", PosC);                  // 添加字符串类型数据到子叶结构体
    cJSON_AddItemToObject(Basic, "PosD", PosD);                  // 添加字符串类型数据到子叶结构体
    // EepromSave(String(cJSON_Print(Basic)));
    test = String(cJSON_Print(Basic));
    DebugSerial.println("[Saveinit]");
    DebugSerial.println(test);
}

void SaveConfigAsJsonToEeprom()
{
    cJSON *Basic = cJSON_CreateObject();       // 创建JSON根部结构体
    cJSON *ServoConfig = cJSON_CreateObject(); // 创建JSON子叶结构体

    cJSON *DefaultAngle = cJSON_CreateFloatArray(PosArray[PosDefNum], sizeof(PosArray[PosDefNum]) / sizeof(PosArray[PosDefNum][0]));
    // DebugSerial.println((char*)PosArray[PosDefNum]);
    cJSON *PosA = cJSON_CreateFloatArray(PosArray[PosANum], sizeof(PosArray[PosDNum]) / sizeof(PosArray[PosANum][0]));
    cJSON *PosB = cJSON_CreateFloatArray(PosArray[PosBNum], sizeof(PosArray[PosDNum]) / sizeof(PosArray[PosBNum][0]));
    cJSON *PosC = cJSON_CreateFloatArray(PosArray[PosCNum], sizeof(PosArray[PosDNum]) / sizeof(PosArray[PosCNum][0]));
    cJSON *PosD = cJSON_CreateFloatArray(PosArray[PosDNum], sizeof(PosArray[PosDNum]) / sizeof(PosArray[PosDNum][0]));

    cJSON_AddStringToObject(Basic, "DefaultTnitMode", "TrackB"); // 添加初始化工作模式到Basic
    cJSON_AddNumberToObject(Basic, "DefaultInitTime", 2000);     // 添加初始化时间到Basic
    cJSON_AddItemToObject(Basic, "DefaultAngle", DefaultAngle);  // 添加字符串类型数据到子叶结构体
    cJSON_AddItemToObject(Basic, "PosA", PosA);                  // 添加字符串类型数据到子叶结构体
    cJSON_AddItemToObject(Basic, "PosB", PosB);                  // 添加字符串类型数据到子叶结构体
    cJSON_AddItemToObject(Basic, "PosC", PosC);                  // 添加字符串类型数据到子叶结构体
    cJSON_AddItemToObject(Basic, "PosD", PosD);                  // 添加字符串类型数据到子叶结构体
    EepromSave(String(cJSON_Print(Basic)));

    DebugSerial.println(String(cJSON_Print(Basic)));
}
