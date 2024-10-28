#ifndef RSGLED_CONFIG_H
#define RSGLED_CONFIG_H

typedef struct {
    byte MIDI_CHANNEL;
    int LED_NUM;
    short lines[48][10];
    short groups[11][48];
} Config;

inline Config getConfig(const int index) {
    switch (index) {
        case 0:
        default:
            return (Config){
                .MIDI_CHANNEL = 12, // has to be MidiChannel - 1
                .LED_NUM = 361,
                .lines = {
                    {1, 40, 41, 80}, // 1
                    {2, 39, 42, 79}, // 2
                    {3, 38, 43, 78}, // 3
                    {4, 37, 44, 77}, // 4
                    {5, 36, 45, 76}, // 5
                    {6, 35, 46, 75}, // 6
                    {7, 34, 47, 74}, // 7
                    {8, 33, 48, 73}, // 8
                    {9, 32, 49, 72}, // 9
                    {10, 31, 50, 71}, // 10
                    {11, 30, 51, 70}, // 11
                    {12, 29, 52, 69}, // 12
                    {13, 28, 53, 68}, // 13
                    {14, 27, 54, 67}, // 14
                    {15, 26, 55, 66}, // 15
                    {16, 25, 56, 65}, // 16
                    {17, 24, 57, 64}, // 17
                    {18, 23, 58, 63}, // 18
                    {19, 22, 59, 62}, // 19
                    {20, 21, 60, 61}, // 20
                },
                .groups = {
                    {}, // 0 default all group
                    {1, 2, 3, 4, 5}, // 1
                    {6, 7, 8, 9, 10}, // 2
                    {11, 12, 13, 14, 15}, // 3
                    {16, 17, 18, 19, 20}, // 4
                },

            };
        case 1:
            return (Config){
                .MIDI_CHANNEL = 13, // has to be MidiChannel - 1
                .LED_NUM = 55,
                .lines = {},
                .groups = {
                    {}, // 0 default all group
                    {}, // 1
                },

            };
        case 2:
            return (Config){
                .MIDI_CHANNEL = 14, // has to be MidiChannel - 1
                .LED_NUM = 55,
                .lines = {},
                .groups = {
                    {}, // 0 default all group
                    {}, // 1
                },
            };
    }
}


#endif //RSGLED_CONFIG_H
