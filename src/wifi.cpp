#include <ESP8266WiFi.h>
#include "wifi.h"

// Replace with your network credentials
// Two Wi-Fi operation mode are possible:
// 1. ESP is Wi-Fi access point (AP). (default)
// 2. ESP is Wi-Fi client.
// To switch to mode 2 remove the next line.
#define AP
const char *ssid = "ESPCompass";
const char *passphrase = "";

#if defined AP
#define DNS
IPAddress apIP(192, 168, 178, 1);
#endif

#if defined DNS
#include <DNSServer.h>
const byte DNS_PORT = 53;
DNSServer dnsServer;
#endif

void wifi_initAP()
{
#if defined AP
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, passphrase);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
#else
    WiFi.begin(ssid, passphrase);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }

    // Print ESP32 Local IP Address
    Serial.println(WiFi.localIP());
#endif

#if defined DNS
    dnsServer.start(DNS_PORT, "*", apIP);
#endif
}

void wifi_loop()
{
#if defined DNS
    dnsServer.processNextRequest();
#endif
}