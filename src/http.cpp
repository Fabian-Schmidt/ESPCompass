#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include "sensor.h"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
FS *filesystem = &SPIFFS;

void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

void http_init()
{
    filesystem->begin();

    server.serveStatic("/", SPIFFS, "/");
    server.serveStatic("/", SPIFFS, "/html/").setDefaultFile("index.html");
    server.serveStatic("/img/", SPIFFS, "/img/");
    server.serveStatic("/css/", SPIFFS, "/css/");
    server.serveStatic("/js/", SPIFFS, "/js/");

    server.on("/compass", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (sensor_ready())
        {
            request->send(200, "text/plain", String(sensor_readDegress()));
        }
        else
        {
            request->send(500, "text/plain", String());
        }
    });

    server.onNotFound(notFound);

    // Start server
    server.begin();
}