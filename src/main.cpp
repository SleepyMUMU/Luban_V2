#include <Arduino.h>
#include <FashionStar_UartServo.h>
#include <FashionStar_UartServoProtocol.h>

#define CORE1 1
#define stackSize 10000

// 初始化舵机通讯协议
FSUS_Protocol protocol;

// 初始化舵机 并绑定协议
FSUS_Servo servo = FSUS_Servo(1, &protocol);

String CMD;

void Serialrecv()
{

  while (Serial.available())
  {
    CMD += char(Serial.read());
  }
  delay(1);
  Serial.print(CMD);
}
void task2(void *pvParameters)
{
  for (;;)
  {
    Serialrecv();
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}
void task1(void *pvParameters)
{
  for (;;)
  {

    if (CMD.length() != 0)
    {
      Serial.println(CMD.length());
      Serial.println(CMD);
      Serial.println(CMD.compareTo("ping"));
    }

    if (CMD.compareTo("ping") == 0)
    {
      // if (servo.ping())
      // {
      //   Serial.println("Servo is online");
      // }
      // else
      // {
      Serial.println("Servo is offline");
      // }
    }

    CMD = ""; // Clear CMD after processing
    CMD.clear(); // Reset CMD after processing
    if (CMD.length() == 0)
    {
     Serial.println("CMD is empty");
    }
    delay(100); // Add a small delay to avoid excessive looping
  }
}
// void task2(void *pvParameters)
// {
//   for (;;)
//   {
//     Serial.println("Task 2");
//     vTaskDelay(2000 / portTICK_PERIOD_MS);
//   }
// }

void setup()
{
  Serial.begin(115200);
  // 初始化舵机

  // servo.init();

  xTaskCreatePinnedToCore(
      task1,
      "Task 1",
      10000,
      NULL,
      1,
      NULL,
      CORE1);
  xTaskCreatePinnedToCore(
      task2,
      "Task 2",
      stackSize,
      NULL,
      1,
      NULL,
      CORE1);
}

void loop()
{
}
