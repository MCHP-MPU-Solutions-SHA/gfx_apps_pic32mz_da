#include "gfx/legato/generated/le_gen_assets.h"

uint32_t leGlobalPalette_data[256] =
{
    0x000000FF,0x800000FF,0x008000FF,0x808000FF,0x000080FF,0x800080FF,0x008080FF,0xC0C0C0FF,0x808080FF,0xFF0000FF,0x00FF00FF,0xFFFF00FF,0x0000FFFF,0xFF00FFFF,0x00FFFFFF,0xFFFFFFFF,
    0x080808FF,0x121212FF,0x1C1C1CFF,0x262626FF,0x303030FF,0x3A3A3AFF,0x444444FF,0x4E4E4EFF,0x585858FF,0x626262FF,0x606060FF,0x626262FF,0x767676FF,0x808080FF,0x8A8A8AFF,0x949494FF,
    0x9E9E9EFF,0xA8A8A8FF,0xB2B2B2FF,0xBCBCBCFF,0xC6C6C6FF,0xD0D0D0FF,0xDADADAFF,0xE4E4E4FF,0x000000FF,0x050505FF,0x080808FF,0x0B0B0BFF,0x0F0F0FFF,0x131313FF,0x161616FF,0x181818FF,
    0x1D1D1DFF,0x1F1F1FFF,0x2B2B2BFF,0x2F2F2FFF,0x383838FF,0x404040FF,0x4B4B4BFF,0x4E4E4EFF,0x5A5A5AFF,0x5E5E5EFF,0x646464FF,0x686868FF,0x6C6C6CFF,0x737373FF,0xC55A11FF,0xC55B16FF,
    0x777777FF,0xC55C1AFF,0xC65D1EFF,0xC65F24FF,0xC76020FF,0xC76222FF,0xC76227FF,0xD45A00FF,0xBD722AFF,0xC86629FF,0xC8652DFF,0xC86632FF,0xC9672CFF,0xC96930FF,0x808080FF,0xC96935FF,
    0xCA6B33FF,0xC96A3BFF,0x828282FF,0xC96B3EFF,0xCB6C36FF,0xCA6D3BFF,0xC96D41FF,0xCB6E38FF,0xCA6E3EFF,0xCB703FFF,0xCC703CFF,0xCA6F45FF,0xCC733FFF,0xCC7244FF,0xCB7248FF,0xCD7442FF,
    0x888888FF,0xCC7447FF,0xCC744BFF,0xCD7647FF,0x5BB45BFF,0xCC764FFF,0xCD774CFF,0xCE7949FF,0xCC7853FF,0xCD7950FF,0xCE7A4DFF,0x8D8D8DFF,0xCE7B55FF,0xCF7E51FF,0x8F8F8FFF,0xCE7D58FF,
    0xCF7F57FF,0x919191FF,0x8C9294FF,0xCF7F5CFF,0xCE7F5FFF,0xD0825EFF,0x0000FFFF,0xD1845CFF,0xD08362FF,0xCF8365FF,0x959595FF,0xCE836AFF,0xD18660FF,0xD18565FF,0xD08569FF,0xD18767FF,
    0xD0876CFF,0xD38964FF,0xD28967FF,0xD1896CFF,0xD38B68FF,0xD48D6BFF,0xD28D70FF,0xD18D77FF,0xD38E74FF,0x00B3B3FF,0xD59070FF,0xFF8001FF,0xD59174FF,0xD2907CFF,0xD69475FF,0xD59478FF,
    0xA2A2A2FF,0xD5957EFF,0xD6977CFF,0xA4A4A4FF,0xA6A6A6FF,0xD99A7BFF,0xD89B80FF,0xD79C86FF,0xD99D82FF,0xD69C8CFF,0xDAA084FF,0xD9A088FF,0xACACACFF,0xD9A18CFF,0xDBA288FF,0xDAA591FF,
    0xDCA68DFF,0x8181FFFF,0xD9A698FF,0xDDA890FF,0xB3B3B3FF,0xDDAA92FF,0xB5B5B5FF,0xDEAC95FF,0xDFAD98FF,0xDDAE9FFF,0xDFAF9DFF,0xE0B19CFF,0xDEB1A4FF,0xE0B2A0FF,0xE1B39DFF,0xBCBCBCFF,
    0xE1B4A0FF,0xDFB3A6FF,0xE2B6A0FF,0xBEBEBEFF,0xE2B7A3FF,0xE1B7A6FF,0xE3B9A5FF,0xE2BAABFF,0xE4BBA7FF,0xC2C2C2FF,0xE0BAB1FF,0xE2BBAEFF,0xE5BDAAFF,0xC4C4C4FF,0xE5BEAFFF,0xE3BEB2FF,
    0xC6C6C6FF,0xE6C0AEFF,0xE6C1B1FF,0xE6C2B4FF,0xE7C4B4FF,0xCBCBCBFF,0xE7C5B7FF,0xE9C7B4FF,0xE5C5BDFF,0xE9C8B9FF,0xCECECEFF,0xE9C9BCFF,0xC8D0D4FF,0xE6C9C1FF,0xEACBBCFF,0xD1D1D1FF,
    0xE8CBC3FF,0xEBCDBEFF,0xE8CCC6FF,0xEACDC3FF,0xD3D3D3FF,0xE9CEC8FF,0xEDD1C1FF,0xECD0C4FF,0xD5D5D5FF,0xEDD3C7FF,0xD8D8D8FF,0xEDD5CAFF,0xDADADAFF,0xEFD7C9FF,0xEED7CDFF,0xF0DAD0FF,
    0xEFDBD3FF,0xF1DCD1FF,0xE0E0E0FF,0xD6E3E7FF,0xF1DED6FF,0xE2E2E2FF,0xF3E0D5FF,0xF2E0D8FF,0xE5E5E5FF,0xF4E4DCFF,0xE7E7E7FF,0xF5E6DEFF,0xF4E6E1FF,0xE9E9E9FF,0xF5E8E2FF,0xEBEBEBFF,
    0xF6EAE4FF,0xEDEDEDFF,0xF7EDE8FF,0xEFEFEFFF,0xFEFFC1FF,0xF1F1F1FF,0xF9F1ECFF,0xFAF3F0FF,0xFBF5F1FF,0xF6F6F6FF,0xFBF6F4FF,0xF9F9F9FF,0xFCF9F7FF,0xFCFBFBFF,0xFDFDFDFF,0xFFFFFEFF,
};

const lePalette leGlobalPalette =
{
    {
        LE_STREAM_LOCATION_ID_INTERNAL, // data location id
        (void*)leGlobalPalette_data, // data variable pointer
        1024, // data size
    },
    256,
    LE_COLOR_MODE_RGBA_8888
};
