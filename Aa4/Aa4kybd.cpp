#include <dos.h>                   // for interrupts
#include <conio.h>                 // getch and kbhit
#include <stdlib.h>                // atexit

#include "Aa4Kybd.H"               // header for this module


const int IntNum =  0x15;          // keyboard constants

const int _UP =     0x48;
const int _DOWN =   0x50;
const int _LEFT =   0x4B;
const int _RIGHT =  0x4D;
const int _CTRL =   0x1D;
const int _ESC =    0x01;
const int _ALT =    0x38;
const int _TAB =    0x0F;
const int _P =      0x19;

void interrupt (far *OldKbdVec)(...);     // old keyboard interrupt
char CtrlFlag = 0,      // key status flags
     UpFlag = 0,
     DownFlag = 0,
	  LeftFlag = 0,
	  RightFlag = 0,
	  EscFlag = 0,
	  AltFlag = 0,
	  TabFlag = 0,
	  PFlag = 0;

void KbdExitProc(void)
// Pre: None.
// Post: keyboard restored to original state.
{
	if (_dos_getvect(IntNum) != OldKbdVec)   // if not yet restored
		_dos_setvect(IntNum, OldKbdVec);  // restore it.
}

void interrupt far keyboard(...)
// Pre: Interrupt 0x15 triggered.
// Post: All data analyzed.
{
	int reg_ax = _AX;     // get value of ax register.

	if ((reg_ax >> 8) == 0x4F)       // if al = 0x4f (keyboard)
   	switch (reg_ax & 0x00FF)      // process its value.
      {
			case _UP           : UpFlag     = 1;     // set/unset flags
         							break;
         case _DOWN         : DownFlag   = 1;
         							break;
         case _LEFT         : LeftFlag   = 1;
         							break;
         case _RIGHT        : RightFlag  = 1;
         							break;
         case _CTRL         : CtrlFlag   = 1;
         							break;
         case _ESC          : EscFlag    = 1;
         							break;
         case _ALT          : AltFlag    = 1;
         							break;
         case _TAB          : TabFlag    = 1;
										break;
			case _P            : PFlag = 1;
										break;
         case _UP + 128     : UpFlag     = 0;
         							break;
         case _DOWN + 128   : DownFlag   = 0;
         							break;
         case _LEFT + 128   : LeftFlag   = 0;
         							break;
			case _RIGHT + 128  : RightFlag  = 0;
										break;
			case _CTRL + 128   : CtrlFlag   = 0;
										break;
			case _ALT + 128    : AltFlag    = 0;
         							break;
			case _ESC + 128    : EscFlag    = 0;
         							break;
         case _TAB + 128    : TabFlag    = 0;
										break;
			case _P + 128      : PFlag = 0;
                              break;
			default				 : ;
		}
	if (reg_ax > 127)       // if necessary, remove the keypress
		while (kbhit())
			getch();
}

void InstallKyb(void)
// Pre: None.
// Post: Keyboard ready to be activated.
{
	OldKbdVec = _dos_getvect(IntNum);    // get old keyboard interrupt vector
	atexit(KbdExitProc);                 // set the new exit procedure
}

void UnsetKyb(void)
// Pre: Keyboard handler is installed (with InstallKyb).
// Post: Keyboard status (interrupt vector) is restored.
{
	_dos_setvect(IntNum, OldKbdVec);    // set vector back to original.
}

void SetKyb(void)
// Pre: Keyboard handler is installed (with InstallKyb).
// Post: Keyboard handler is activated.
{
	_dos_setvect(IntNum, keyboard);
}

void ClrKybBuf(void)
// Pre: None.
// Post: Keyboard buffer is cleared.
{
	while (kbhit())
		getch();
}

void ClrFlags(void)
// Pre: None.
// Post: All keyboard flags are cleared.
{
	CtrlFlag = 0;
   UpFlag = 0;
   DownFlag = 0;
	LeftFlag = 0;
	RightFlag = 0;
	EscFlag = 0;
	AltFlag = 0;
	TabFlag = 0;
   PFlag = 0;
}
