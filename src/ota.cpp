#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include "ota.h"
#include "logging.h"
#include "led.h"
#include "../credentials.h"

const char* wifiResultToString(const int result) {
    switch (result) {
        case WL_CONNECTED:          return "WL_CONNECTED";
        case WL_NO_SSID_AVAIL:      return "WL_NO_SSID_AVAIL";
        case WL_CONNECT_FAILED:     return "WL_CONNECT_FAILED";
        case WL_DISCONNECTED:       return "WL_DISCONNECTED";
        case WL_IDLE_STATUS:        return "WL_IDLE_STATUS";
        case WL_SCAN_COMPLETED:     return "WL_SCAN_COMPLETED";
        default:                    return "UNKNOWN";
    }
}

void OTA_init(const char letter, const int ip) {
    WiFiClass::mode(WIFI_STA);
    WiFi.setTxPower(WIFI_POWER_19_5dBm); // highest tx power
    // Set your Static IP address
    // https://randomnerdtutorials.com/esp32-static-fixed-ip-address-arduino-ide/
    const IPAddress local_IP(wifiConfig.network[0], wifiConfig.network[1], wifiConfig.network[2], ip);
    const IPAddress gateway(wifiConfig.gateway[0], wifiConfig.gateway[1], wifiConfig.gateway[2], wifiConfig.gateway[3]);
    const IPAddress subnet(wifiConfig.subnet[0], wifiConfig.subnet[1], wifiConfig.subnet[2], wifiConfig.subnet[3]);
    // Configures static IP address
    if (!WiFi.config(local_IP, gateway, subnet)) {
        LOGN("STA Failed to configure");
    }

    // Hostname defaults to esp3232-[MAC]
    char hostname[50]; // Buffer for the final hostname
    snprintf(hostname, sizeof(hostname), "RSG-LED-%c-%02d", letter, ip);
    ArduinoOTA.setHostname(hostname);

    // Connect to Wi-Fi network with SSID and password
    LOGD("Connecting to %s\n", wifiConfig.ssid);
    WiFi.begin(wifiConfig.ssid, wifiConfig.password);

    connectionLED.turnON();
    midiLED.turnON();
    const int connectionResult = WiFi.waitForConnectResult(10000);
    /*
    WL_CONNECTED after successful connection is established
    WL_NO_SSID_AVAIL in case configured SSID cannot be reached
    WL_CONNECT_FAILED if connection failed
    WL_CONNECT_WRONG_PASSWORD if password is incorrect
    WL_IDLE_STATUS when Wi-Fi is in process of changing between statuses
    WL_DISCONNECTED if module is not configured in station mode
    -1 on timeout
    */
    LOGD("connection result: %d (%s)\n", connectionResult, wifiResultToString(connectionResult));
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
                } else {
                    // U_SPIFFS
                    type = "filesystem";
                }

                // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
                LOGN("Start updating " + type);
            })
            .onEnd([]() {
                LOGN("\nEnd");
            })
            .onProgress([](unsigned int progress, unsigned int total) {
                LOGD("Progress: %u%%\r", (progress / (total / 100)));
            })
            .onError([](ota_error_t error) {
                LOGD("Error[%u]: ", error);
                if (error == OTA_AUTH_ERROR) {
                    LOGN("Auth Failed");
                } else if (error == OTA_BEGIN_ERROR) {
                    LOGN("Begin Failed");
                } else if (error == OTA_CONNECT_ERROR) {
                    LOGN("Connect Failed");
                } else if (error == OTA_RECEIVE_ERROR) {
                    LOGN("Receive Failed");
                } else if (error == OTA_END_ERROR) {
                    LOGN("End Failed");
                }
            });

    ArduinoOTA.begin();

    LOGD("IP address: %s\n", WiFi.localIP().toString().c_str());
}

void OTA_loop() {
    ArduinoOTA.handle();
}

void OTA_disconnect() {
    LOGN("Disconnecting WiFi...");
    WiFi.disconnect(true);
}
