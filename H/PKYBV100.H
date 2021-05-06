#include <dos.h>

#define SHIFT_VALUE ((unsigned int)(peek(0x40, 0x17)))

//shifts

const unsigned int kbRightShift = 0x0001;
const unsigned int kbLeftShift = 0x0002;
const unsigned int kbCtrlShift = 0x0004;
const unsigned int kbAltShift = 0x0008;
const unsigned int kbScrollState = 0x0010;
const unsigned int kbNumState	= 0x0020;
const unsigned int kbCapsState = 0x0040;
const unsigned int kbInsState = 0x0080;

//combos

const unsigned int kbAltA   = 0x1E00,     kbAltEqual  = 0x8300;
const unsigned int kbAltB   = 0x3000,     kbAltMinus  = 0x8200;
const unsigned int kbAltC   = 0x2E00,     kbAltSpace  = 0x0200;
const unsigned int kbAltD   = 0x2000,     kbBack      = 0x0E08;
const unsigned int kbAltE   = 0x1200,     kbCtrlBack  = 0x0E7F;
const unsigned int kbAltF   = 0x2100,     kbCtrlDel   = 0x0600;
const unsigned int kbAltG   = 0x2200,     kbCtrlEnd   = 0x7500;
const unsigned int kbAltH   = 0x2300,     kbCtrlEnter = 0x1C0A;
const unsigned int kbAltI   = 0x1700,     kbCtrlHome  = 0x7700;
const unsigned int kbAltJ   = 0x2400,     kbCtrlIns   = 0x0400;
const unsigned int kbAltK   = 0x2500,     kbCtrlLeft  = 0x7300;
const unsigned int kbAltL   = 0x2600,     kbCtrlPgDn  = 0x7600;
const unsigned int kbAltM   = 0x3200,     kbCtrlPgUp  = 0x8400;
const unsigned int kbAltN   = 0x3100,     kbCtrlPrtSc = 0x7200;
const unsigned int kbAltO   = 0x1800,     kbCtrlRight = 0x7400;
const unsigned int kbAltP   = 0x1900,     kbDel       = 0x5300;
const unsigned int kbAltQ   = 0x1000,     kbDown      = 0x5000;
const unsigned int kbAltR   = 0x1300,     kbEnd       = 0x4F00;
const unsigned int kbAltS   = 0x1F00,     kbEnter     = 0x1C0D;
const unsigned int kbAltT   = 0x1400,     kbEsc       = 0x011B;
const unsigned int kbAltU   = 0x1600,     kbGrayMinus = 0x4A2D;
const unsigned int kbAltV   = 0x2F00,     kbGrayPlus  = 0x4E2B;
const unsigned int kbAltW   = 0x1100,     kbHome      = 0x4700;
const unsigned int kbAltX   = 0x2D00,     kbIns       = 0x5200;
const unsigned int kbAltY   = 0x1500,     kbLeft      = 0x4B00;
const unsigned int kbAltZ   = 0x2C00,     kbNoKey     = 0x0000;
const unsigned int                        kbPgDn      = 0x5100;
const unsigned int                        kbPgUp      = 0x4900;
const unsigned int                        kbRight     = 0x4D00;
const unsigned int                        kbShiftDel  = 0x0700;
const unsigned int                        kbShiftIns  = 0x0500;
const unsigned int                        kbShiftTab  = 0x0F00;
const unsigned int                        kbTab       = 0x0F09;
const unsigned int                        kbUp        = 0x4800;

//F-keys
const unsigned int kbAlt1   = 0x7800,     kbF1     = 0x3B00;
const unsigned int kbAlt2   = 0x7900,     kbF2     = 0x3C00;
const unsigned int kbAlt3   = 0x7A00,     kbF3     = 0x3D00;
const unsigned int kbAlt4   = 0x7B00,     kbF4     = 0x3E00;
const unsigned int kbAlt5   = 0x7C00,     kbF5     = 0x3F00;
const unsigned int kbAlt6   = 0x7D00,     kbF6     = 0x4000;
const unsigned int kbAlt7   = 0x7E00,     kbF7     = 0x4100;
const unsigned int kbAlt8   = 0x7F00,     kbF8     = 0x4200;
const unsigned int kbAlt9   = 0x8000,     kbF9     = 0x4300;
const unsigned int kbAlt0   = 0x8100,     kbF10    = 0x4400;
const unsigned int kbShiftF1  = 0x5400,   kbCtrlF1  = 0x5E00;
const unsigned int kbShiftF2  = 0x5500,   kbCtrlF2  = 0x5F00;
const unsigned int kbShiftF3  = 0x5600,   kbCtrlF3  = 0x6000;
const unsigned int kbShiftF4  = 0x5700,   kbCtrlF4  = 0x6100;
const unsigned int kbShiftF5  = 0x5800,   kbCtrlF5  = 0x6200;
const unsigned int kbShiftF6  = 0x5900,   kbCtrlF6  = 0x6300;
const unsigned int kbShiftF7  = 0x5A00,   kbCtrlF7  = 0x6400;
const unsigned int kbShiftF8  = 0x5B00,   kbCtrlF8  = 0x6500;
const unsigned int kbShiftF9  = 0x5C00,   kbCtrlF9  = 0x6600;
const unsigned int kbShiftF10 = 0x5D00,   kbCtrlF10 = 0x6700;
const unsigned int kbAltF1  = 0x6800;
const unsigned int kbAltF2  = 0x6900;
const unsigned int kbAltF3  = 0x6A00;
const unsigned int kbAltF4  = 0x6B00;
const unsigned int kbAltF5  = 0x6C00;
const unsigned int kbAltF6  = 0x6D00;
const unsigned int kbAltF7  = 0x6E00;
const unsigned int kbAltF8  = 0x6F00;
const unsigned int kbAltF9  = 0x7000;
const unsigned int kbAltF10 = 0x7100;
