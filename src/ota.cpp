#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include "led.h"
#include "../credentials.h"

void OTA_init(char letter, int ip) {
    WiFi.mode(WIFI_STA);

    // Set your Static IP address
// https://randomnerdtutorials.com/esp32-static-fixed-ip-address-arduino-ide/
    IPAddress local_IP(192, 168, 100, ip);
    IPAddress gateway(192, 168, 100, 1);
    IPAddress subnet(255, 255, 255, 0);
    // Configures static IP address
    if (!WiFi.config(local_IP, gateway, subnet)) {
        Serial.println("STA Failed to configure");
    }

    // Hostname defaults to esp3232-[MAC]
    char hostname[50];  // Buffer for the final hostname
    snprintf(hostname, sizeof(hostname), "RSG-LED-%c-%02d", letter, ip);
    ArduinoOTA.setHostname(hostname);

    // Connect to Wi-Fi network with SSID and password
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    connectionLED.turnON();
    midiLED.turnON();
    int connectionResult = WiFi.waitForConnectResult(10000);
    /*WL_CONNECTED after successful connection is established
    WL_NO_SSID_AVAIL in case configured SSID cannot be reached
    WL_CONNECT_FAILED if connection failed
    WL_CONNECT_WRONG_PASSWORD if password is incorrect
    WL_IDLE_STATUS when Wi-Fi is in process of changing between statuses
    WL_DISCONNECTED if module is not configured in station mode
    -1 on timeout*/
    Serial.printf("connection result: %d", connectionResult);
    connectionLED.turnOFF();
    midiLED.turnOFF();

    // Port defaults to 3232
    // ArduinoOTA.setPort(3232);

    // No authentication by default
    // ArduinoOTA.setPassword("admin");

    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

    ArduinoOTA
            .onStart([]() {
                String type;
                if (ArduinoOTA.getCommand() == U_FLASH) {
                    type = "sketch";
                } else {  // U_SPIFFS
                    type = "filesystem";
                }

                // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
                Serial.println("Start updating " + type);
            })
            .onEnd([]() {
                Serial.println("\nEnd");
            })
            .onProgress([](unsigned int progress, unsigned int total) {
                Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
            })
            .onError([](ota_error_t error) {
                Serial.printf("Error[%u]: ", error);
                if (error == OTA_AUTH_ERROR) {
                    Serial.println("Auth Failed");
                } else if (error == OTA_BEGIN_ERROR) {
                    Serial.println("Begin Failed");
                } else if (error == OTA_CONNECT_ERROR) {
                    Serial.println("Connect Failed");
                } else if (error == OTA_RECEIVE_ERROR) {
                    Serial.println("Receive Failed");
                } else if (error == OTA_END_ERROR) {
                    Serial.println("End Failed");
                }
            });

    ArduinoOTA.begin();

    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void OTA_loop() {
    ArduinoOTA.handle();
}