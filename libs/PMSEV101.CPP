#include <dos.h>

#include "pmsev101.h"

unsigned char MouseObject::ColdReset(int &NumButtons)

{
   REGPACK regs;

   regs.r_ax = 0x0000; //Cold reset for mouse
   intr(0x33, &regs);

   switch (regs.r_bx)
   {
      case 0x0000 : NumButtons = 0;
                    break;
      case 0x0002 :
      case 0xFFFF : NumButtons = 2;
                    break;
      case 0x0003 : NumButtons = 3;
      default     : ;
   }

   return regs.r_ax == 0xFFFF; //Returns a false if an error occured
}

unsigned char MouseObject::ColdReset(void)

{
   REGPACK regs;

   regs.r_ax = 0x0000; //Cold reset for mouse
   intr(0x33, &regs);

   return regs.r_ax == 0xFFFF; //Returns a false if an error occured
}

void MouseObject::ShowMouse(void)

{
   REGPACK regs;

   regs.r_ax = 0x0001; //Calls BIOS interrupt to show mouse
   intr(0x33, &regs);
}

void MouseObject::HideMouse(void)

{
   REGPACK regs;

   regs.r_ax = 0x0002; //Calls BIOS interrupt for hiding mouse
   intr(0x33, &regs);
}

void MouseObject::GetClicks(char &RtBut, char &MidBut, char &LtBut, int &row, int &col)

{
   REGPACK regs;

   regs.r_ax = 0x0003; //calls BIOS interrupt service for mouse status
   intr(0x33, &regs);
   if ((regs.r_bx >> 2) > 0) //Checks return bits to see if the middle button (if it exists) is down
      MidBut = 1;   //The return value is true
   else
       MidBut = 0;  //The return value is false
   if ((regs.r_bx >> 1) % 2 > 0)  //Checks to see if the right button is down
      RtBut = 1;   //The return value is true
   else
       RtBut = 0;   //The return value is false
   if (regs.r_bx % 2 > 0)  //Checks to see if the left button is down
      LtBut = 1;   //The return value is true
   else
       LtBut = 0;   //The return value is false
   row = regs.r_dx; //The row of the mouse cursor is returned
   col = regs.r_cx; //The column of the mouse cursor is returned
}

void MouseObject::MoveCursor(int row, int col)

{
   REGPACK regs;

   regs.r_ax = 0x0004; //Calls BIOS interrupt service to position the cursor
   regs.r_cx = col;    //Sends BIOS the new column of the cursor
   regs.r_dx = row;    //Sends BIOS the new row of the cursor
   intr(0x33, &regs);  //Sends all data to the BIOS register, where it is processed
}

int MouseObject::LastPress(int ButNum, int  &row, int &col)

{
   REGPACK regs;

   regs.r_ax = 0x0005; //Calls BIOS interrupt service for specific button press data
   regs.r_bx = ButNum; //Sends BIOS the button to analyze
   intr(0x33, &regs);  //Sends BIOS all data
   row = regs.r_dx;    //Returns the row location of the last press
   col = regs.r_cx;    //Returns the column location of the last press
   return regs.r_bx;   //Returns the number of times the specified button has been pressed since the last call
}

int MouseObject::LastRelease(int ButNum, int &row, int &col)

{
   REGPACK regs;

   regs.r_ax = 0x0006;  //Calls BIOS interrupt service for specific button release data
   regs.r_bx = ButNum;  //Sends BIOS the button to analyze
   intr(0x33, &regs);   //Sends BIOS all data
   row = regs.r_dx;     //Returns the row location of the last release
   col = regs.r_cx;     //Returns the column location of the last release
   return regs.r_bx;    //Returns the number of times the specified button has been released since the last call}
}

void MouseObject::HorizLimit(int Min, int Max)

{
   REGPACK regs;

   regs.r_ax = 0x0007; //Calls BIOS interrupt service for horizontal limit settings
   regs.r_cx = Min;    //Sends BIOS the minimum column that can be attained by the mouse cursor
   regs.r_dx = Max;    //Sends BIOS the maximum column that can be attained by the mouse cursor
   intr(0x33, &regs);  //Sends BIOS all data
}

void MouseObject::VertLimit(int Min, int Max)

{
   REGPACK regs;

   regs.r_ax = 0x0008; //Calls BIOS interrupt service for the vertical limit settings
   regs.r_cx = Min;    //Sends BIOS the minimum row that can be attained by the mouse cursor
   regs.r_dx = Max;    //Sends BIOS the maximum row that can be attained by the mouse cursor
   intr(0x33, &regs);  //Sends BIOS all data
}

void MouseObject::SetSensitivity(int Horiz, int Vert, int DblSpeed)

{
   REGPACK regs;

   regs.r_ax = 0x001A;   //Calls BIOS interrupt service for setting sensitivity settings
   regs.r_bx = Horiz;    //Sends BIOS horizontal sensitivity
   regs.r_cx = Vert;     //Sends BIOS vertical sensitivity
   regs.r_dx = DblSpeed; //Sends BIOS the double speed threshold
   intr(0x33, &regs);    //Sends BIOS all data
}

void MouseObject::GetSensitivity(int &Horiz, int &Vert, int &DblSpeed)

{
   REGPACK regs;

   regs.r_ax = 0x001B;    //Calls BIOS interrupt service for getting sensitivity settings
   intr(0x33, &regs);
   Horiz = regs.r_bx;     //Returns horizontal sensitivity
   Vert = regs.r_cx;      //Returns vertical sensitivity
   DblSpeed = regs.r_dx;  //Returns double speed threshold
}

void MouseObject::DefineMicPixRatio(int Horiz, int Vert)

{
   REGPACK regs;

   regs.r_ax = 0x000F; //Calls BIOS interrupt service for setting mickey to pix. ratio
   regs.r_cx = Horiz;  //Sends BIOS horizontal mic/pix ratio
   regs.r_dx = Vert;   //Sends BIOS vertical mic/pix ratio
   intr(0x33, &regs);  //Sends BIOS all data
}

unsigned char MouseObject::SoftReset(int &NumButtons)

{
   REGPACK regs;

   regs.r_ax = 0x0021; //Calls BIOS interrupt service for software reset
   intr(0x33, &regs);
   NumButtons = regs.r_bx; //The number of buttons on the mouse is returned
   return regs.r_ax == 0xFFFF; //Checks for errors.  If an error occurred, then the function returns false.
}

unsigned char MouseObject::SoftReset(void)

{
   REGPACK regs;

   regs.r_ax = 0x0021; //Calls BIOS interrupt service for software reset
   intr(0x33, &regs);
   return regs.r_ax == 0xFFFF; //Checks for errors.  If an error occurred, then the function returns false.
}

