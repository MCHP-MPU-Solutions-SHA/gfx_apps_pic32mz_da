#include "gfx/legato/generated/le_gen_assets.h"

/*****************************************************************************
 * Legato String Table
 * Encoding        ASCII
 * Language Count: 1
 * String Count:   30
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

const uint8_t stringTable_data[474] =
{
    0x1E,0x00,0x01,0x00,0x04,0x7C,0x00,0x00,0x03,0x80,0x00,0x00,0x05,0x88,0x00,0x00,
    0x05,0x9C,0x00,0x00,0x06,0xAC,0x00,0x00,0x05,0xBE,0x00,0x00,0x03,0xC4,0x00,0x00,
    0x05,0xCE,0x00,0x00,0x05,0xE6,0x00,0x00,0x06,0xF2,0x00,0x00,0x05,0x00,0x01,0x00,
    0x02,0x10,0x01,0x00,0x05,0x1C,0x01,0x00,0x03,0x2A,0x01,0x00,0x06,0x30,0x01,0x00,
    0x03,0x40,0x01,0x00,0x05,0x48,0x01,0x00,0x03,0x54,0x01,0x00,0x08,0x64,0x01,0x00,
    0x07,0x68,0x01,0x00,0x04,0x7E,0x01,0x00,0x04,0x8A,0x01,0x00,0x05,0x8E,0x01,0x00,
    0x05,0x98,0x01,0x00,0x01,0xA6,0x01,0x00,0x04,0xB2,0x01,0x00,0x04,0xB6,0x01,0x00,
    0x04,0xBA,0x01,0x00,0x03,0xC6,0x01,0x00,0x00,0xCE,0x01,0x00,0x01,0x00,0x3C,0x00,
    0x05,0x00,0x53,0x69,0x7A,0x65,0x3A,0x00,0x11,0x00,0x46,0x72,0x61,0x6D,0x65,0x20,
    0x55,0x70,0x64,0x61,0x74,0x65,0x20,0x28,0x48,0x7A,0x29,0x00,0x0D,0x00,0x50,0x72,
    0x65,0x2D,0x70,0x72,0x6F,0x63,0x65,0x73,0x73,0x65,0x64,0x00,0x0F,0x00,0x49,0x6D,
    0x61,0x67,0x65,0x20,0x52,0x65,0x6E,0x64,0x65,0x72,0x69,0x6E,0x67,0x00,0x04,0x00,
    0x4E,0x65,0x78,0x74,0x07,0x00,0x47,0x50,0x55,0x20,0x4F,0x66,0x66,0x00,0x15,0x00,
    0x44,0x69,0x73,0x70,0x6C,0x61,0x79,0x20,0x52,0x65,0x66,0x72,0x65,0x73,0x68,0x20,
    0x28,0x48,0x7A,0x29,0x3A,0x00,0x09,0x00,0x52,0x61,0x77,0x20,0x33,0x32,0x62,0x69,
    0x74,0x00,0x0B,0x00,0x4D,0x6F,0x74,0x69,0x6F,0x6E,0x20,0x46,0x69,0x6C,0x6C,0x00,
    0x0D,0x00,0x52,0x61,0x77,0x20,0x52,0x4C,0x45,0x20,0x33,0x32,0x62,0x69,0x74,0x00,
    0x0A,0x00,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x0C,0x00,0x4A,0x50,
    0x47,0x20,0x28,0x32,0x34,0x2D,0x62,0x69,0x74,0x29,0x04,0x00,0x4E,0x75,0x6D,0x3A,
    0x0E,0x00,0x53,0x74,0x72,0x69,0x6E,0x67,0x20,0x55,0x70,0x64,0x61,0x74,0x65,0x73,
    0x06,0x00,0x47,0x50,0x55,0x20,0x4F,0x6E,0x0A,0x00,0x46,0x75,0x6C,0x6C,0x53,0x63,
    0x72,0x65,0x65,0x6E,0x0D,0x00,0x53,0x74,0x72,0x69,0x6E,0x67,0x20,0x49,0x6E,0x64,
    0x65,0x78,0x3A,0x00,0x01,0x00,0x61,0x00,0x13,0x00,0x30,0x31,0x32,0x33,0x34,0x35,
    0x36,0x37,0x38,0x39,0x20,0x61,0x76,0x67,0x63,0x75,0x72,0x72,0x2D,0x00,0x0A,0x00,
    0x39,0x38,0x37,0x36,0x35,0x34,0x33,0x32,0x31,0x30,0x01,0x00,0x2D,0x00,0x07,0x00,
    0x50,0x4E,0x47,0x38,0x38,0x38,0x38,0x00,0x0B,0x00,0x30,0x31,0x32,0x33,0x34,0x35,
    0x36,0x37,0x38,0x39,0x78,0x00,0x0A,0x00,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,
    0x38,0x39,0x01,0x00,0x3E,0x00,0x01,0x00,0x2B,0x00,0x0A,0x00,0x37,0x38,0x39,0x30,
    0x31,0x32,0x33,0x34,0x35,0x36,0x05,0x00,0x54,0x79,0x70,0x65,0x3A,0x00,0x0A,0x00,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,
};

/* font asset pointer list */
leFont* fontList[9] =
{
    (leFont*)&ArialLarge,
    (leFont*)&ArialMed,
    (leFont*)&ArialSmall,
    (leFont*)&ArialNarrow,
    (leFont*)&ArialNarrowMed,
    (leFont*)&ArialTiny,
    (leFont*)&ArialTitle,
    (leFont*)&ArialLittle,
    (leFont*)&ArialTinyAve,
};

const leStringTable stringTable =
{
    {
        LE_STREAM_LOCATION_ID_INTERNAL, // data location id
        (void*)stringTable_data, // data address pointer
        474, // data size
    },
    (void*)stringTable_data, // string table data
    fontList, // font lookup table
    LE_STRING_ENCODING_ASCII // encoding standard
};


// string list
leTableString string_Previous;
leTableString string_Size;
leTableString string_FPS;
leTableString string_PreProcessedBlit;
leTableString string_ImageRendering;
leTableString string_Next;
leTableString string_GPUOff;
leTableString string_RefreshRate;
leTableString string_Raw565;
leTableString string_MotionUpdates;
leTableString string_RawRLE565;
leTableString string_NumsSmall;
leTableString string_JPG24Bit;
leTableString string_RectanglesNum;
leTableString string_DynamicString;
leTableString string_GPUOn;
leTableString string_Fullscreen;
leTableString string_StringSize;
leTableString string_ave;
leTableString string_NumsLittle;
leTableString string_FPSCountString;
leTableString string_Minus;
leTableString string_PNG8888;
leTableString string_NumsTiny;
leTableString string_NumsMed;
leTableString string_NextSymbol;
leTableString string_Plus;
leTableString string_RefreshRateCount;
leTableString string_ImageType;
leTableString string_NumsLarge;

void initializeStrings(void)
{
    leTableString_Constructor(&string_Previous, stringID_Previous);
    leTableString_Constructor(&string_Size, stringID_Size);
    leTableString_Constructor(&string_FPS, stringID_FPS);
    leTableString_Constructor(&string_PreProcessedBlit, stringID_PreProcessedBlit);
    leTableString_Constructor(&string_ImageRendering, stringID_ImageRendering);
    leTableString_Constructor(&string_Next, stringID_Next);
    leTableString_Constructor(&string_GPUOff, stringID_GPUOff);
    leTableString_Constructor(&string_RefreshRate, stringID_RefreshRate);
    leTableString_Constructor(&string_Raw565, stringID_Raw565);
    leTableString_Constructor(&string_MotionUpdates, stringID_MotionUpdates);
    leTableString_Constructor(&string_RawRLE565, stringID_RawRLE565);
    leTableString_Constructor(&string_NumsSmall, stringID_NumsSmall);
    leTableString_Constructor(&string_JPG24Bit, stringID_JPG24Bit);
    leTableString_Constructor(&string_RectanglesNum, stringID_RectanglesNum);
    leTableString_Constructor(&string_DynamicString, stringID_DynamicString);
    leTableString_Constructor(&string_GPUOn, stringID_GPUOn);
    leTableString_Constructor(&string_Fullscreen, stringID_Fullscreen);
    leTableString_Constructor(&string_StringSize, stringID_StringSize);
    leTableString_Constructor(&string_ave, stringID_ave);
    leTableString_Constructor(&string_NumsLittle, stringID_NumsLittle);
    leTableString_Constructor(&string_FPSCountString, stringID_FPSCountString);
    leTableString_Constructor(&string_Minus, stringID_Minus);
    leTableString_Constructor(&string_PNG8888, stringID_PNG8888);
    leTableString_Constructor(&string_NumsTiny, stringID_NumsTiny);
    leTableString_Constructor(&string_NumsMed, stringID_NumsMed);
    leTableString_Constructor(&string_NextSymbol, stringID_NextSymbol);
    leTableString_Constructor(&string_Plus, stringID_Plus);
    leTableString_Constructor(&string_RefreshRateCount, stringID_RefreshRateCount);
    leTableString_Constructor(&string_ImageType, stringID_ImageType);
    leTableString_Constructor(&string_NumsLarge, stringID_NumsLarge);
}