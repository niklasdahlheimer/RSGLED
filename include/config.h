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
                    {1, 40, 41, 80, 81, 120, 121, 160}, //1
                    {2, 39, 42, 79, 82, 119, 122, 159}, //2
                    {3, 38, 43, 78, 83, 118, 123, 158}, //3
                    {4, 37, 44, 77, 84, 117, 124, 157}, //4
                    {5, 36, 45, 76, 85, 116, 125, 156}, //5
                    {6, 35, 46, 75, 86, 115, 126, 155}, //6
                    {7, 34, 47, 74, 87, 114, 127, 154}, //7
                    {8, 33, 48, 73, 88, 113, 128, 153}, //8
                    {9, 32, 49, 72, 89, 112, 129, 152}, //9
                    {10, 31, 50, 71, 90, 111, 130, 151}, //10
                    {11, 30, 51, 70, 91, 110, 131, 150}, //11
                    {12, 29, 52, 69, 92, 109, 132, 149}, //12
                    {13, 28, 53, 68, 93, 108, 133, 148}, //13
                    {14, 27, 54, 67, 94, 107, 134, 147}, //14
                    {15, 26, 55, 66, 95, 106, 135, 146}, //15
                    {16, 25, 56, 65, 96, 105, 136, 145}, //16
                    {17, 24, 57, 64, 97, 104, 137, 144}, //17
                    {18, 23, 58, 63, 98, 103, 138, 143}, //18
                    {19, 22, 59, 62, 99, 102, 139, 142}, //19
                    {20, 21, 60, 61, 100, 101, 140, 141}, //20

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
