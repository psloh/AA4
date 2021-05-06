#ifndef __PMSEV101_H
#define __PMSEV101_H

class MouseObject
{
   public:

   unsigned char ColdReset();
   unsigned char ColdReset(int &NumButtons);
   void ShowMouse(void);
   void HideMouse(void);
   void GetClicks(char &RtBut, char &MidBut, char &LtBut, int &row, int &col);
   void MoveCursor(int row, int col);
   int LastPress(int ButNum, int  &row, int &col);
       //Gets last button press data
   int LastRelease(int ButNum, int &row, int &col);
       //Gets last release data
   void HorizLimit(int Min, int Max);
        //Sets horizontal cursor limit
   void VertLimit(int Min, int Max);
        //Sets veritcal cursor limit
   void SetSensitivity(int Horiz, int Vert, int DblSpeed);
        //Sets mouse sensitivity
   void GetSensitivity(int &Horiz, int &Vert, int &DblSpeed);
        //Gets mouse sensitivity
   void DefineMicPixRatio(int Horiz, int Vert);
        //Sets mickey to pixel ratio.
        //A mickey is used in measuring mouse motion.
   unsigned char SoftReset(int &NumButtons);
   unsigned char SoftReset(void);
                     //Software reset of mouse
};

#endif