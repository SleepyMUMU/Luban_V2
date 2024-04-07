#ifndef _WIFICONFIG_H_
#define _WIFICONFIG_H_

#include "UART.h"

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <HTTPClient.h>

extern WiFiClient client;
extern WiFiClient Nano;
extern HTTPClient http;

void WiFiInit();
void OTAconfig();
void TCPServer();
void httpServer();
String TCPcom();
String STMcom();
#endif