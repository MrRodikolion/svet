#include <Arduino.h>
#include "ESP8266WiFi.h"
#include "ESPAsyncWebServer.h"
#include <esp8266httpclient.h>
#include <ESPAsyncTCP.h>
#include <LittleFS.h>

const char *ssid = "ESP32-Access-Point";
const char *password = "123456789";
const char *SlaveServerName1 = "http://192.168.4.2/post";
const char *SlaveServerName2 = "http://192.168.4.3/post";

const int rpin = 13,
          ypin = 12,
          gpin = 14;

const String colors[] = {"r", "ry", "g", "mg", "y"};
const int tsleep[] = {5, 2, 7, 2, 3};
const byte sendcode[] = {0x00, 0x01, 0x02, 0x03, 0x04};
static int s = 0;
static int t = millis() + tsleep[s] * 1000 + 1;
char todo = 's';
bool isupdated = false;

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
            //      IsReadyForRead = false;
            // for (size_t i = 0; i < len; i++)
            // {
            //     Serial.write(data[i]);
            // }

            // Serial.println();

            request->send(200);
        });

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "index.html", "text/html"); });

    // server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request)
    //           { request->send(LittleFS, "/index.css", "text/css"); });

    server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "/index.js", "text/javascript"); });

    server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "/styles.css", "text/css"); });

    server.on("/svet0", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/json", "{\"clr\": \"" + colors[s] + "\"}"); });

    server.on("/stop", HTTP_GET,
              [](AsyncWebServerRequest *request)
              {
                  todo = 's';
                  isupdated = false;
                  //   WiFiClient client;
                  //   HTTPClient http;

                  //   http.begin(client, SlaveServerName);

                  //   http.addHeader("Content-Type", "text/plain");
                  //   http.POST("s");
                  //   http.end();
                  request->send(200, "text/html", "ok");
              });
    server.on("/start", HTTP_GET,
              [](AsyncWebServerRequest *request)
              {
                  todo = 'g';
                  isupdated = false;
                  //   WiFiClient client;
                  //   HTTPClient http;

                  //   http.begin(client, SlaveServerName);

                  //   http.addHeader("Content-Type", "text/plain");
                  //   http.POST("r");
                  //   http.end();
                  request->send(200, "text/html", "ok");
              });

    server.begin();
}

void setup()
{
    Serial.begin(115200);

    pinMode(rpin, OUTPUT);
    pinMode(ypin, OUTPUT);
    pinMode(gpin, OUTPUT);

    if (!LittleFS.begin())
    {
        Serial.println("An Error has occurred while mounting LittleFS");
        return;
    }

    // Serial.print("Setting AP (Access Point)…");
    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();
    // Serial.print("AP IP address: ");
    // Serial.println(IP);

    Server();
}

void TestSender();

struct station_info *stat_info;
struct ip4_addr *IPaddress;
IPAddress address;

void dumpClients()
{
    Serial.print(" Clients:\r\n");
    stat_info = wifi_softap_get_station_info();
    while (stat_info != NULL)
    {
        IPaddress = &stat_info->ip;
        address = IPaddress->addr;
        Serial.print("\t");
        Serial.print(address);
        Serial.print("\r\n");
        stat_info = STAILQ_NEXT(stat_info, next);
    }
}

void loop()
{
    // if (millis() - t > tsleep[s] * 1000)
    // {
    //     t = millis();
    //     TestSender();
    //     // dumpClients();
    // }
    // Serial.write(sendcode[s]);
    switch (todo)
    {
    case 's':
        Serial.write(sendcode[0]);
        digitalWrite(rpin, HIGH);
        digitalWrite(ypin, LOW);
        digitalWrite(gpin, LOW);
        if (!isupdated)
        {
            WiFiClient client;
            HTTPClient http;

            http.begin(client, SlaveServerName1);

            http.addHeader("Content-Type", "text/plain");
            http.POST("s");
            http.end();

            WiFiClient client1;
            HTTPClient http1;

            http1.begin(client1, SlaveServerName2);

            http1.addHeader("Content-Type", "text/plain");
            http1.POST("s");
            http1.end();
            isupdated = true;
        }
        break;
    case 'g':
        Serial.write(sendcode[2]);
        digitalWrite(rpin, LOW);
        digitalWrite(ypin, LOW);
        digitalWrite(gpin, HIGH);
        if (!isupdated)
        {
            WiFiClient client;
            HTTPClient http;

            http.begin(client, SlaveServerName1);

            http.addHeader("Content-Type", "text/plain");
            http.POST("r");
            http.end();

            WiFiClient client1;
            HTTPClient http1;

            http1.begin(client1, SlaveServerName2);

            http1.addHeader("Content-Type", "text/plain");
            http1.POST("g");
            http1.end();
            isupdated = true;
        }
        break;

    default:
        break;
    }
}

void TestSender()
{
    s = s + 1 >= 3 ? 0 : s + 1;
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
        if (millis() % 1000 > 500)
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

    // WiFiClient client;
    // HTTPClient http;

    // http.begin(client, SlaveServerName);

    // http.addHeader("Content-Type", "text/plain");
    // http.POST(colors[s]);
    // http.end();
}
