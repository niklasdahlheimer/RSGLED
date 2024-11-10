#ifndef RSGLED_CONFIG_H
#define RSGLED_CONFIG_H

#define MAX_LED_NUM                 500
#define MAX_LINE_NUM                60
#define MAX_PIXEL_PER_LINE_NUM      20
#define MAX_GROUP_NUM               11

typedef struct {
    char LETTER;
    byte MIDI_CHANNEL;
    short lines[MAX_LINE_NUM][MAX_PIXEL_PER_LINE_NUM];
    short groups[MAX_GROUP_NUM][MAX_LINE_NUM];
} Config;

inline Config getConfig(const int index) {
    switch (index) {
        case 0:
        default:
            return (Config){
                .LETTER = 'R',
                .MIDI_CHANNEL = 12, // has to be MidiChannel - 1
                .lines = {
                    {1, 107, 108, 212, 213, 314, 315, 420}, //1
                    {2,106,109,211, 214, 313,316,419}, //2
                    {3,105,110,210, 215, 312,317,418}, //3
                    {4,104,111,209, 216, 311,318,417}, //4
                    {5,103,112,208, 217, 310,319,416}, //5
                    {6,102,113,207, 218, 309,320,415}, //6
                    {7,101,114,206, 219, 308,321,414}, //7
                    {8,100,115,205, 220, 307,322,413}, //8
                    {9, 99,116,204, 221, 306,323,412}, //9
                    {10, 98,117,203, 222, 305,324,411}, //10
                    {11, 97,118,202, 223, 304,325,410}, //11
                    {12, 96,119,201, 224, 303,326,409}, //12
                    {13, 95,120,200, 225, 302,327,408}, //13
                    {14, 94,121,199, 226, 301,328,407}, //14
                    {15, 93,122,198, 227, 300,329,406}, //15
                    {16, 92,123,197, 228, 299,330,405}, //16
                    {17, 91,124,196, 229, 298,331,404}, //17
                    {18, 90,125,195, 230, 297,332,403}, //18
                    {19, 89,126,194, 231, 296,333,402}, //19
                    {20, 88,127,193, 232, 295,334,401}, //20
                    {21, 87,128,192, 233, 294,335,400}, //21
                    {22, 86,129,191, 234, 293,336,399}, //22
                    {23, 85,130,190, 235, 292,337,398}, //23
                    {24, 84,131,189, 236, 291,338,397}, //24
                    {25, 83,132,188, 237, 290,339,396}, //25
                    {26, 82,133,187, 238, 289,340,395}, //26
                    {27, 81,134,186, 239, 288,341,394}, //27
                    {28, 80,135,185, 240, 287,342,393}, //28
                    {29, 79,136,184, 241, 286,343,392}, //29
                    {30, 78,137,183, 242, 285,344,391}, //30
                    {31, 77,138,182, 243, 284,345,390}, //31
                    {32, 76,139,181, 244, 283,346,389}, //32
                    {33, 75,140,180, 245, 282,347,388}, //33
                    {34, 74,141,179, 246, 281,348,387}, //34
                    {35, 73,142,178, 247, 280,349,386}, //35
                    {36, 72,143,177, 248, 279,350,385}, //36
                    {37, 71,144,176, 249, 278,351,384}, //37
                    {38, 70,145,175, 250, 277,352,383}, //38
                    {39, 69,146,174, 251, 276,353,382}, //39
                    {40, 68,147,173, 252, 275,354,381}, //40
                    {41, 67,148,172, 253, 274,355,380}, //41
                    {42, 66,149,171, 254, 273,356,379}, //42
                    {43, 65,150,170, 255, 272,357,378}, //43
                    {44, 64,151,169, 256, 271,358,377}, //44
                    {45, 63,152,168, 257, 270,359,376}, //45
                    {46, 62,153,167, 258, 269,360,375}, //46
                    {47, 61,154,166, 259, 268,361,374}, //47
                    {48, 60,155,165, 260, 267,362,373}, //48
                    {49, 59,156,164, 261, 266,363,372}, //49
                    {50, 58,157,163, 262, 265,364,371}, //50
                    {51, 57,158,162, 263, 264,365,370}, //51
                    {52, 56,159,161, 264, 263,366,369}, //52
                    {53, 55,160,160, 265, 262,367,368}, //53
                    {54, 54, 161,159, 266, 261,368,367}, //54
                },
                .groups = {
                    {}, // 0 default all group
                    {1, 2, 3, 4}, // 1
                    {5, 6, 7, 8}, // 2
                    {9, 10, 11, 12}, // 3
                    {13, 14, 15, 16}, // 4
                    {17, 18, 19, 20, 21, 22, 23, 24, 25}, // 5
                    {26, 27, 28, 29, 30, 31, 32}, // 6
                    {33, 34, 35, 36, 37}, // 7
                    {38, 39, 40, 41, 42, 43}, // 8
                    {44, 45, 46, 47, 48}, // 9
                    {49, 50, 51, 52, 53, 54} // 10
                },

            };
        case 1:
            return (Config){
                    .LETTER = 'S',
                .MIDI_CHANNEL = 13, // has to be MidiChannel - 1
               .lines = {
                    {7,8,9,10,11,104,105,106,198,288,287,199,200,107,12}, //1
                    {6,103,197,287,288,200,108,13},
                    {5,102,196,286,289,201,109,14},
                    {4,101,195,285,290,202,110,15},
                    {3,100,194,284,291,203,111,16},
                    {2,99,193,283,292,204,112,17},
                    {1, 98, 192, 282, 293, 205, 113, 18},
                    {97,191,281,368,294,206,114,19},
                       {96,190,280,367,295,207,115,20},
                        {95,189,279,366,296,208,116,21}, //10
                        {94,188,278,365,297,209,117,22},
                        {93,187,277,364,298,210,118,23},
                        {92,186,276,363,299,211,119,24},
                        {91,185,275,362,300,212,120,25},
                        {90,184,274,361,301,213,121,26},
                        {89,183,273,360,302,214,122,27},
                        {88,182,272,359,303,215,123,28},
                        {87,181,271,358,304,216,124,29},
                        {86,180,270,357,305,217,125,30},
                        {85,179,269,356,306,218,126,31}, //20
                        {84,178,268,355,307,219,127,32},
                        {83,177,267,354,308,220,128,33},
                        {82,176,266,353,309,221,129,34},
                        {81,175,265,352,310,222,130,35},
                        {80,174,264,351,311,223,131,36},
                        {79,173,263,350,312,224,132,37},
                        {78,172,262,349,313,225,133,38},
                        {77,171,261,348,314,226,134,39},
                        {76,170,260,347,315,227,135,40},
                        {75,169,259,346,316,228,136,41}, //30
                        {74,168,258,345,317,229,137,42},
                        {73,167,257,344,318,230,138,43},
                        {72,166,256,343,319,231,139,44},
                        {71,165,255,342,320,232,140,45},
                        {70,164,254,341,321,233,141,46},
                        {69,163,253,340,322,234,142,47},
                        {68,162,252,339,323,235,143,48},
                        {67,161,251,338,324,236,144,49},
                        {66,160,250,337,325,237,145,50},
                        {65,159,249,336,326,238,146,51}, //40
                        {64,158,248,335,327,239,147,52},
                        {63,157,247,334,328,240,148,53},
                        {62,156,246,333,329,241,149,54},
                        {61,155,245,332,330,242,150,55},
                        {60,59,58,57,56,151,152,153,154,243,244,331} //45

                },
                .groups = {
                    {}, // 0 default all group
                    {5,6,7,8}, // 1
                    {1,2,3,4},
                    {23,24,25,26,27,28},
                    {29,30,31,32},
                    {33,34,35,36,37},
                    {38,39,40,41},
                    {42,43,44,45},
                    {18,19,20,21,22},
                    {14,15,16,17},
                    {9,10,11,12,13},
                },

            };
        case 2:
            return (Config){
                    .LETTER = 'G',
                .MIDI_CHANNEL = 14, // has to be MidiChannel - 1
                 .lines = {
                    {1, 93,184,273,358,359,271,272,181,182,183,91,92, 361,360, 371,370,369,379,380},
                    {2, 94,185,274,357,270,180,90,  362,368,372,378},
                    {3, 95,186,275,356,269,179,89,     363,367,373,377},
                    {4, 96,187,276,355,268,178,88,     364,365,366,374,375,376},
                    {5, 97,188,277,354,267,177,87},
                    {6, 98,189,278,353,266,176,86},
                    {7, 99,190,279,352,265,175,85},
                    {8, 100,191,280,351,264,174,84},
                    {9, 101,192,281,350,263,173,83, 349, 262,172,82},
                    {10, 102,193,282,348,261,171,81, 347, 260,170,80},
                    {11, 103,194,283,346,259,169,79, 345, 258,168,78},
                    {12, 104,195,284,344,257,167,77, 343, 256,166,76},
                    {13, 105,196,285,342,255,165,75, 341, 254,164,74},
                    {14, 106,197,286,340,253,163,73, 339, 252,162,72},
                    {15, 107,198,287,338,251,161,71, 337, 250,160,70},
                    {16, 110, 203, 293,337, 248,156,65},
                    {17, 111, 204, 294,336, 247,155,64},
                    {18, 112, 205, 295,335, 246,154,63},
                    {19, 113, 206, 296,334, 245,153,62},
                    {20, 114, 207, 297,333, 244,152,61},
                    {21, 115, 208, 298,332, 243,151,60},
                    {22, 116, 209, 299,331, 242,150,59},
                    {23, 117, 210, 300,330, 241,149,58},
                    {24, 118, 211, 301,329, 240,148,57,  328, 239, 147, 56},
                    {25, 119, 212, 302,303,327, 238,146,55,  326, 237, 145, 54},
                    {26, 120, 213, 304,305,325, 236,144,53,  324, 235, 143, 52},
                    {27, 121, 214,215, 306,307,323, 234,142,51,  322, 233, 141, 50},
                    {28, 122, 215, 216,307,308,321, 232,140,49,  320, 231, 139, 48},
                    {29, 123, 216, 217,308,309,319, 230,138,46,47,  318, 229, 137, 46},
                    {30, 124, 217,218, 309,310,318, 228,136,45,  319, 227, 135, 44},
                    {31, 125, 219,220, 226,134,43,   225, 133, 42,313,312,316,317},
                    {32, 126, 220,221,315,316, 313, 224,132,41,  312, 223, 131, 40},
                    {33,34,35,36,37,38,39,127,128,129,130,131,221,222,223,224,313,314,315}
                },
                .groups = {
                    {}, // 0 default all group
                    {12,13,14},
                    {15,16,17,},
                    {18,19,20},
                    {21,22,23},
                    {24,25,26},
                    {27,28,29,30},
                    {31,32,33},
                    {1,2,3,4},
                    {5,6,7},
                    {8,9,10,11},

                },
            };
    }
}


#endif //RSGLED_CONFIG_H
