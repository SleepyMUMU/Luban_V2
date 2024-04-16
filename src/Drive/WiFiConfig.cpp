#include "WiFiConfig.h"
#include "Peripherals.h"
// #include <string>

const char *SSID_1 = "PLA·Studio🐣";
const char *Password_1 = "woaiwuxie1";

const char *SSID = "MUMUSNITRO";
const char *Password = "yssahttn";

const char *NitroIP = "192.168.137.1";

const char *ServerIP = "192.168.137.1";
const int ServerPort = 1347;

const char *NanoIP = "192.168.31.193";
const int NanoPort = 1348;

String url = "http://192.168.31.193:8088/robotData/data"; // HTTP服务地址

IPAddress INTROIP(192, 168, 137, 99);
IPAddress local_IP(192, 168, 31, 99);

IPAddress gateway(192, 168, 137, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(223, 5, 5, 5);
IPAddress secondaryDNS(119, 29, 29, 29);

WiFiClient client;
WiFiClient Nano;
HTTPClient http;

void WiFiInit()
{
  u64_t TimeOut = 10000, NowTime = 0;
  u32_t DelayTime = 500;
  WiFi.config(INTROIP, gateway, subnet, primaryDNS, secondaryDNS);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, Password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED && TimeOut > NowTime)
  {
    delay(DelayTime);
    NowTime += DelayTime;
  }
  WiFi.setAutoReconnect(true);
  Beep(300);
}

String readFromStream(Stream &stream)
{
  String receiveData;

  if (stream.available())
  {
    receiveData = stream.readString();
  }

  return receiveData;
}

String TCPcom()
{
  return readFromStream(client);
}

String STMcom()
{
  return readFromStream(STM32COM);
}

void httpServer()
{
  DebugSerial.print("[HTTP] begin...\n");
  http.begin(url); // 访问服务器地址
  DebugSerial.print("[HTTP] GET...\n");
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

void TCPServer()
{
  // DebugSerial.println("[TCP]\tConnecting to server...");
  if (client.connect(NitroIP, ServerPort))
  {
    DebugSerial.println("[TCP]Connected to server");
    DebugSerial.println("[ESP]Hi,ESP connected.");
    // client.println("Hello, Server!"); // 向服务器发送数据

    // while (client.connected())
    // {
    //     if (client.available())
    //     {
    //         char c = client.read();
    //         Serial.write(c); // 将从服务器接收到的数据打印到串口
    //     }
    // }

    // client.stop();
    // DebugSerial.println("[TCP]\tDisconnected from server");
  }
  else
  {
    DebugSerial.println("[TCP]\tFailed to connect to server");
  }
  if (Nano.connect(NanoIP, NanoPort))
  {
    DebugSerial.println("[Nano]Nano connected.");
    Nano.printf("[ESP]ESP Connected.\n");
  }
  else
  {
    DebugSerial.println("[Nano]Failed to connected Nano.");
  }

  // delay(5000); // 等待5秒后重新连接服务器
}

void OTAconfig()
{
  // ArduinoOTA.setPort(2049);

  ArduinoOTA.setHostname("ESP32");
  ArduinoOTA.setPassword("yssahttn");
  ArduinoOTA.begin();

  DebugSerial.println("[OTA]Ready");
  DebugSerial.print("[OTA]IP address: ");
  DebugSerial.println(WiFi.localIP());
  DebugSerial.printf("[OTA]esp_get_free_heap_size : %d  \n", esp_get_free_heap_size());
}
