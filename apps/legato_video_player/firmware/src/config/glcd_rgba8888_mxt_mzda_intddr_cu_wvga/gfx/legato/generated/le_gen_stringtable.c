#include "gfx/legato/generated/le_gen_assets.h"

/*****************************************************************************
 * Legato String Table
 * Encoding        ASCII
 * Language Count: 1
 * String Count:   19
 *****************************************************************************/

/*****************************************************************************
 * string table data
 * 
 * this table contains the raw character data for each string
 * 
 * unsigned short - number of indices in the table
 * unsigned short - number of languages in the table
 * 
 * index array (size = number of indices * number of languages
 * 
 * for each index in the array:
 *   unsigned byte - the font ID for the index
 *   unsigned byte[3] - the offset of the string codepoint data in
 *                      the table
 * 
 * string data is found by jumping to the index offset from the start
 * of the table
 * 
 * string data entry:
 *     unsigned short - length of the string in bytes (encoding dependent)
 *     codepoint data - the string data
 ****************************************************************************/

const uint8_t stringTable_data[264] =
{
    0x13,0x00,0x01,0x00,0x01,0x50,0x00,0x00,0x01,0x6E,0x00,0x00,0x01,0x78,0x00,0x00,
    0x01,0x80,0x00,0x00,0x00,0x98,0x00,0x00,0x01,0x9E,0x00,0x00,0x01,0xA6,0x00,0x00,
    0x01,0xAA,0x00,0x00,0x02,0xAE,0x00,0x00,0x01,0xB4,0x00,0x00,0x01,0xBE,0x00,0x00,
    0x01,0xCA,0x00,0x00,0x01,0xD4,0x00,0x00,0x00,0xD8,0x00,0x00,0x00,0xE0,0x00,0x00,
    0x01,0xE6,0x00,0x00,0x01,0xEC,0x00,0x00,0x00,0xF6,0x00,0x00,0x01,0xFE,0x00,0x00,
    0x1B,0x00,0x49,0x6E,0x73,0x65,0x72,0x74,0x0A,0x55,0x53,0x42,0x20,0x46,0x6C,0x61,
    0x73,0x68,0x0A,0x6F,0x72,0x20,0x53,0x44,0x20,0x43,0x61,0x72,0x64,0x00,0x08,0x00,
    0x52,0x47,0x42,0x41,0x38,0x38,0x38,0x38,0x05,0x00,0x46,0x6C,0x61,0x73,0x68,0x00,
    0x16,0x00,0x43,0x61,0x6E,0x6E,0x6F,0x74,0x20,0x6F,0x70,0x65,0x6E,0x20,0x76,0x69,
    0x64,0x65,0x6F,0x20,0x66,0x69,0x6C,0x65,0x04,0x00,0x73,0x69,0x7A,0x65,0x06,0x00,
    0x52,0x47,0x42,0x35,0x36,0x35,0x02,0x00,0x31,0x32,0x01,0x00,0x38,0x00,0x03,0x00,
    0x6D,0x61,0x78,0x00,0x07,0x00,0x53,0x44,0x20,0x43,0x61,0x72,0x64,0x00,0x09,0x00,
    0x55,0x53,0x42,0x20,0x44,0x72,0x69,0x76,0x65,0x00,0x07,0x00,0x34,0x30,0x30,0x78,
    0x32,0x34,0x30,0x00,0x02,0x00,0x32,0x34,0x06,0x00,0x66,0x6F,0x72,0x6D,0x61,0x74,
    0x03,0x00,0x66,0x70,0x73,0x00,0x03,0x00,0x2D,0x2D,0x2D,0x00,0x07,0x00,0x38,0x30,
    0x30,0x78,0x34,0x38,0x30,0x00,0x05,0x00,0x6D,0x65,0x64,0x69,0x61,0x00,0x07,0x00,
    0x33,0x32,0x30,0x78,0x32,0x34,0x30,0x00,
};

/* font asset pointer list */
leFont* fontList[3] =
{
    (leFont*)&NotoSans_Medium,
    (leFont*)&NotoSans_Medium_Select,
    (leFont*)&NotoSans_MediumItalic,
};

const leStringTable stringTable =
{
    {
        LE_STREAM_LOCATION_ID_INTERNAL, // data location id
        (void*)stringTable_data, // data address pointer
        264, // data size
    },
    (void*)stringTable_data, // string table data
    fontList, // font lookup table
    LE_STRING_ENCODING_ASCII // encoding standard
};


// string list
leTableString string_InsertUSBDrive;
leTableString string_Format_RGBA8888;
leTableString string_Source_Flash;
leTableString string_CannotOpenString;
leTableString string_Size;
leTableString string_Format_RGB565;
leTableString string_FPS_12;
leTableString string_FPS_8;
leTableString string_max;
leTableString string_Source_SD;
leTableString string_Source_USB;
leTableString string_Size_400x240;
leTableString string_FPS_24;
leTableString string_Format;
leTableString string_FrameRate;
leTableString string_NoSelect;
leTableString string_Size_800x480;
leTableString string_Media;
leTableString string_Size_320x240;

void initializeStrings(void)
{
    leTableString_Constructor(&string_InsertUSBDrive, stringID_InsertUSBDrive);
    leTableString_Constructor(&string_Format_RGBA8888, stringID_Format_RGBA8888);
    leTableString_Constructor(&string_Source_Flash, stringID_Source_Flash);
    leTableString_Constructor(&string_CannotOpenString, stringID_CannotOpenString);
    leTableString_Constructor(&string_Size, stringID_Size);
    leTableString_Constructor(&string_Format_RGB565, stringID_Format_RGB565);
    leTableString_Constructor(&string_FPS_12, stringID_FPS_12);
    leTableString_Constructor(&string_FPS_8, stringID_FPS_8);
    leTableString_Constructor(&string_max, stringID_max);
    leTableString_Constructor(&string_Source_SD, stringID_Source_SD);
    leTableString_Constructor(&string_Source_USB, stringID_Source_USB);
    leTableString_Constructor(&string_Size_400x240, stringID_Size_400x240);
    leTableString_Constructor(&string_FPS_24, stringID_FPS_24);
    leTableString_Constructor(&string_Format, stringID_Format);
    leTableString_Constructor(&string_FrameRate, stringID_FrameRate);
    leTableString_Constructor(&string_NoSelect, stringID_NoSelect);
    leTableString_Constructor(&string_Size_800x480, stringID_Size_800x480);
    leTableString_Constructor(&string_Media, stringID_Media);
    leTableString_Constructor(&string_Size_320x240, stringID_Size_320x240);
}
