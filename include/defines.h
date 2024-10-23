#ifndef RSGLED_DEFINES_H
#define RSGLED_DEFINES_H

#define MIDI_INPUT_LED 2

// MAIN DEFINE FOR LETTER
#define R
#define DEFAULT_TEMPO 120
#define TEST_MODE false

typedef struct {
    byte MIDI_CHANNEL;
    byte LED_NUM;
    byte LED_GROUP_INDEX_1_START;
    byte LED_GROUP_INDEX_1_END;
    byte LED_GROUP_INDEX_2_START;
    byte LED_GROUP_INDEX_2_END;
    byte LED_GROUP_INDEX_3_START;
    byte LED_GROUP_INDEX_3_END;
    byte LED_GROUP_INDEX_4_START;
    byte LED_GROUP_INDEX_4_END;
    byte LED_GROUP_INDEX_5_START;
    byte LED_GROUP_INDEX_5_END;
    byte LED_GROUP_INDEX_6_START;
    byte LED_GROUP_INDEX_6_END;
    byte LED_GROUP_INDEX_7_START;
    byte LED_GROUP_INDEX_7_END;
    byte LED_GROUP_INDEX_8_START;
    byte LED_GROUP_INDEX_8_END;
    byte LED_GROUP_INDEX_9_START;
    byte LED_GROUP_INDEX_9_END;
    byte LED_GROUP_INDEX_10_START;
    byte LED_GROUP_INDEX_10_END;
} Config;

inline Config getConfig(const byte index) {
    switch (index) {
        case 0:
        default:
            return (Config){
                .MIDI_CHANNEL = 12, // has to be MidiChannel - 1
                .LED_NUM = 55,
                .LED_GROUP_INDEX_1_START = 0,
                .LED_GROUP_INDEX_1_END = 3,
                .LED_GROUP_INDEX_2_START = 4,
                .LED_GROUP_INDEX_2_END = 7,
                .LED_GROUP_INDEX_3_START = 8,
                .LED_GROUP_INDEX_3_END = 11,
                .LED_GROUP_INDEX_4_START = 12,
                .LED_GROUP_INDEX_4_END = 15,
                .LED_GROUP_INDEX_5_START = 16,
                .LED_GROUP_INDEX_5_END = 24,
                .LED_GROUP_INDEX_6_START = 25,
                .LED_GROUP_INDEX_6_END = 31,
                .LED_GROUP_INDEX_7_START = 32,
                .LED_GROUP_INDEX_7_END = 36,
                .LED_GROUP_INDEX_8_START = 37,
                .LED_GROUP_INDEX_8_END = 42,
                .LED_GROUP_INDEX_9_START = 43,
                .LED_GROUP_INDEX_9_END = 47,
                .LED_GROUP_INDEX_10_START = 48,
                .LED_GROUP_INDEX_10_END = 54
            };
        case 1:
            return (Config){
                .MIDI_CHANNEL = 13, // has to be MidiChannel - 1
                .LED_NUM = 55,
                .LED_GROUP_INDEX_1_START = 0,
                .LED_GROUP_INDEX_1_END = 3,
                .LED_GROUP_INDEX_2_START = 4,
                .LED_GROUP_INDEX_2_END = 7,
                .LED_GROUP_INDEX_3_START = 8,
                .LED_GROUP_INDEX_3_END = 11,
                .LED_GROUP_INDEX_4_START = 12,
                .LED_GROUP_INDEX_4_END = 15,
                .LED_GROUP_INDEX_5_START = 16,
                .LED_GROUP_INDEX_5_END = 24,
                .LED_GROUP_INDEX_6_START = 25,
                .LED_GROUP_INDEX_6_END = 31,
                .LED_GROUP_INDEX_7_START = 32,
                .LED_GROUP_INDEX_7_END = 36,
                .LED_GROUP_INDEX_8_START = 37,
                .LED_GROUP_INDEX_8_END = 42,
                .LED_GROUP_INDEX_9_START = 43,
                .LED_GROUP_INDEX_9_END = 47,
                .LED_GROUP_INDEX_10_START = 48,
                .LED_GROUP_INDEX_10_END = 54
            };
        case 2:
            return (Config){
                .MIDI_CHANNEL = 14, // has to be MidiChannel - 1
                .LED_NUM = 55,
                .LED_GROUP_INDEX_1_START = 0,
                .LED_GROUP_INDEX_1_END = 3,
                .LED_GROUP_INDEX_2_START = 4,
                .LED_GROUP_INDEX_2_END = 7,
                .LED_GROUP_INDEX_3_START = 8,
                .LED_GROUP_INDEX_3_END = 11,
                .LED_GROUP_INDEX_4_START = 12,
                .LED_GROUP_INDEX_4_END = 15,
                .LED_GROUP_INDEX_5_START = 16,
                .LED_GROUP_INDEX_5_END = 24,
                .LED_GROUP_INDEX_6_START = 25,
                .LED_GROUP_INDEX_6_END = 31,
                .LED_GROUP_INDEX_7_START = 32,
                .LED_GROUP_INDEX_7_END = 36,
                .LED_GROUP_INDEX_8_START = 37,
                .LED_GROUP_INDEX_8_END = 42,
                .LED_GROUP_INDEX_9_START = 43,
                .LED_GROUP_INDEX_9_END = 47,
                .LED_GROUP_INDEX_10_START = 48,
                .LED_GROUP_INDEX_10_END = 54
            };
    }
}


#endif //RSGLED_DEFINES_H
