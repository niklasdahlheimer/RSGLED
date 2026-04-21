#include <WiFi.h>
#include <WiFiUdp.h>
#include <AppleMIDI.h>
#include "midiControllerRtp.h"
#include "logging.h"
#include "led.h"
#include "midiConsts.h"
#include "../credentials.h"

static MidiData *midiData;
static byte midiChannel;
static unsigned long lastNoteOn = 0;
static bool isConnected = false;

APPLEMIDI_CREATE_INSTANCE(WiFiUDP, MIDI_RTP, "RSG-LED", 5004);

// Forward declarations
static void handleNoteOnRtp(byte channel, byte note, byte velocity);
static void handleNoteOffRtp(byte channel, byte note, byte velocity);
static void handleControlChangeRtp(byte channel, byte number, byte value);

void MIDICRTP_init(const byte _midiChannel, const char letter, const int ip, MidiData *_midiData) {
    LOGN("RTP MIDI: Init RTP MIDI Controller");
    midiChannel = _midiChannel;
    midiData = _midiData;

    WiFiClass::mode(WIFI_STA);
    WiFi.setTxPower(WIFI_POWER_19_5dBm);

    const IPAddress local_IP(wifiConfig.network[0], wifiConfig.network[1], wifiConfig.network[2], ip);
    const IPAddress gateway(wifiConfig.gateway[0], wifiConfig.gateway[1], wifiConfig.gateway[2], wifiConfig.gateway[3]);
    const IPAddress subnet(wifiConfig.subnet[0], wifiConfig.subnet[1], wifiConfig.subnet[2], wifiConfig.subnet[3]);

    if (!WiFi.config(local_IP, gateway, subnet)) {
        LOGN("RTP MIDI: STA Failed to configure");
    }

    LOGD("RTP MIDI: Connecting to %s\n", wifiConfig.ssid);
    WiFi.begin(wifiConfig.ssid, wifiConfig.password);

    connectionLED.turnON();
    midiLED.turnON();
    
    const int connectionResult = WiFi.waitForConnectResult(10000);
    if (connectionResult == WL_CONNECTED) {
        LOGN("RTP MIDI: WiFi Connected");
        isConnected = true;
    } else {
        LOGD("RTP MIDI: WiFi Connection failed: %d\n", connectionResult);
    }

    connectionLED.turnOFF();
    midiLED.turnOFF();

    char hostname[50];
    snprintf(hostname, sizeof(hostname), "RSG-LED-%c-%02d", letter, ip);

    MIDI_RTP.begin(MIDI_CHANNEL_OMNI);

    AppleMIDI_RTP.setHandleConnected([](const appleMidi::ssrc_t& ssrc, const char* name) {
        LOGD("RTP MIDI: Connected to session %s\n", name);
    });
    AppleMIDI_RTP.setHandleDisconnected([](const appleMidi::ssrc_t& ssrc) {
        LOGD("RTP MIDI: Disconnected from session\n");
    });

    MIDI_RTP.setHandleNoteOn(handleNoteOnRtp);
    MIDI_RTP.setHandleNoteOff(handleNoteOffRtp);
    MIDI_RTP.setHandleControlChange(handleControlChangeRtp);

    LOGD("RTP MIDI: IP address: %s\n", WiFi.localIP().toString().c_str());
}

static void handleNoteOnRtp(byte channel, byte note, byte velocity) {
    LOGD("RTP MIDI: Note on %03d, velocity %03d, channel %d\n", note, velocity, channel);
    if (channel != midiChannel && channel != MIDI_CHANNEL_ALL) return;
    midiData->noteOn[note] = 2 * velocity;
    lastNoteOn = millis();
    LED_blinkOnce(&midiLED);
}

static void handleNoteOffRtp(byte channel, byte note, byte velocity) {
    LOGD("RTP MIDI: Note off %03d, channel %d\n", note, channel);
    if (channel != midiChannel && channel != MIDI_CHANNEL_ALL) return;
    midiData->noteOn[note] = 0;
}

static void handleControlChangeRtp(byte channel, byte number, byte value) {
    LOGD("RTP MIDI: Control change on %03d, value %03d, channel %d\n", number, value, channel);
    if (channel != midiChannel && channel != MIDI_CHANNEL_ALL) return;
    midiData->controls[number] = value * 2;
    LED_blinkOnce(&midiLED);

    if (number == ALL_NOTE_OFF_CC && value == ALL_NOTE_OFF_VAL) {
        LOGD("RTP MIDI: All Note Off command received!\n");
        for (unsigned char &i: midiData->noteOn) {
            i = 0;
        }
    }
}

void MIDICRTP_loop() {
    if (isConnected) {
        MIDI_RTP.read();
    }
}

void MIDICRTP_disconnect() {
    LOGN("RTP MIDI: Disconnecting WiFi...");
    WiFi.disconnect(true);
    isConnected = false;
}

unsigned long MIDICRTP_lastNoteOn() {
    return lastNoteOn;
}
