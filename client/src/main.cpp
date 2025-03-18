#include <Arduino.h>
#include "ESP8266WiFi.h"
#include "ESPAsyncWebServer.h"
#include <esp8266httpclient.h>
#include <Wire.h>

const char *ssid = "ESP32-Access-Point";
const char *password = "123456789";
const char *MainServerName = "http://192.168.4.1/post";
IPAddress local_IP(192, 168, 4, 101);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 0, 0);

const int rpin = 13,
          ypin = 12,
          gpin = 14;

const String colors[] = {"r", "ry", "g", "mg", "y"};
const int tsleep[] = {5, 2, 7, 2, 3};
const byte sendcode[] = {0x00, 0x01, 0x02, 0x03, 0x04};
static int s = 2;
static int t = millis();
char todo = 'g';

AsyncWebServer server(80);
void Server()
{
  server.on(
      "/post",
      HTTP_POST,
      [](AsyncWebServerRequest *request) {},
      NULL,
      [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
      {
        todo = data[0];
        switch (todo)
        {
        case 'r':
          s = 4;
          break;
        case 'g':
          s = 1;
          break;

        default:
          break;
        }

        request->send(200);
      });

  server.begin();
}

void setup()
{
  Serial.begin(115200);

  pinMode(rpin, OUTPUT);
  pinMode(ypin, OUTPUT);
  pinMode(gpin, OUTPUT);

  // if (!WiFi.config(local_IP, gateway, subnet)) {
  //   Serial.println("STA Failed to configure");
  // }
  WiFi.begin(ssid, password);
  // Serial.println("Connecting");
  // int wifit = millis();
  // while (WiFi.status() != WL_CONNECTED && millis() - wifit < 5000)
  // {
  //   delay(500);
  //   // Serial.print(".");
  // }
  // Serial.println("");
  // Serial.print("Connected to WiFi network with IP Address: ");
  // Serial.println(WiFi.localIP());

  Server(); // запустили сервер для входящих POST-запросов
}

void nosignal();
void worcking();

void loop()
{
  switch (todo)
  {
  case 's':
    nosignal();
    break;
  case 'r':
    worcking();
    break;
  case 'g':
    worcking();
    break;

  default:
    break;
  }
}

void nosignal()
{
  s = 0;
  digitalWrite(rpin, HIGH);
  digitalWrite(ypin, LOW);
  digitalWrite(gpin, LOW);
  Serial.write(sendcode[s]);
}

void worcking()
{
  if (millis() - t > tsleep[s] * 1000)
  {
    t = millis();

    s = s + 1 >= 5 ? 0 : s + 1;
  }
  switch (s)
  {
  case 0:
    digitalWrite(rpin, HIGH);
    digitalWrite(ypin, LOW);
    digitalWrite(gpin, LOW);
    break;
  case 1:
    digitalWrite(rpin, HIGH);
    digitalWrite(ypin, HIGH);
    digitalWrite(gpin, LOW);
    break;
  case 2:
    digitalWrite(rpin, LOW);
    digitalWrite(ypin, LOW);
    digitalWrite(gpin, HIGH);
    break;
  case 3:
    if (!(millis() % 1000 > 500))
    {
      digitalWrite(rpin, LOW);
      digitalWrite(ypin, LOW);
      digitalWrite(gpin, HIGH);
    }
    else
    {
      digitalWrite(rpin, LOW);
      digitalWrite(ypin, LOW);
      digitalWrite(gpin, LOW);
    }
    break;
  case 4:
    digitalWrite(rpin, LOW);
    digitalWrite(ypin, HIGH);
    digitalWrite(gpin, LOW);
    break;

  default:
    break;
  }

  Serial.write(sendcode[s]);
}
