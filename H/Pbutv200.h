#ifndef __PBUTV200_H
#define __PBUTV200_H

#include "PGphV120.H"

const int LEFT_BUTTON = 0;   //Symbolic constant used for the left button
const int MIDDLE_BUTTON = 2; //Symbolic constant used for the middle button
const int RIGHT_BUTTON = 1;  //Symbolic constant used for the right button

struct _PBut_ListType;

typedef _PBut_ListType *_PBut_ListPtr;

struct _PBut_ListType
{
   char *name, *key;  //"name" contains the label of the
                      //button, and "key" is the string
                      //used to reference the button.  This
                      //string is used in the TrapClick
                      //function.
   unsigned tcol; //Stores the color of the button label.
   int s_l; //Shadow length
   unsigned col;  //Stores the color of the button
   int xs, ys, xe, ye; //xs = upper left x coordinate.
                       //ys = upper left y coordinate.
                       //xe = lower right x coordinate.
                       //ye = lower right y coordinate.
   char down;  //Stores whether or not the button is currently depressed.
   _PBut_ListPtr prev, next;
};

class ButtonObject
{
 public:

   ButtonObject(void);

   void AddButton(int xs, int ys, int xe, int ye, unsigned col, char *name, char *key);
        //Adds a button to the button object, and the display list
   char RemoveButton(char *key);
        //Removes a button from the button object display list.  Returns false if there was an error.
   void ShowAllButtons();
        //Displays all buttons on screen
   char * TrapClick(int ButNum);
        //Waits until the user chooses a button.  It returns the key of the chosen button.
   char * TrapClick_Anim(int ButNum, void far *xor, void far *and, int xdim, int ydim, VirtualScreen vscr);
        //Advanced trap-click incorporating animation on left panel.
   void SetShadowLength(int s);
        //Sets the shadow length of the button, in pixels.  The default length is 2 pixels.
   void SetTextColor(unsigned col);
        //Sets the current text color to the passed value.
   void SetTextFont(unsigned font);
        //Sets the current text font to the passed value.
   void SetTextSize(unsigned size);
        //Sets the current text size to the passed value.
   ~ButtonObject(void);
      //Cleans up after the button unit.  It frees all memory of the linked list.

 private:

   _PBut_ListPtr ButtonList; //private linked list data structure.  It stores all the buttons.
                             //It is private so that it is not accidentally modified.
   int S_L; //Private var. for the shadow length
   unsigned _tcol; //_tfont = current text font
                                   //_tsize = current text size
                                   //_tcol = current text color

   void far *oldimg;

   void DrawCursor(int x_cur, int y_cur, void far *vscr);
   void DrawCursor(int x_cur, int y_cur);
   void DrawButton(_PBut_ListType BDat, char On);
   void DrawButton(_PBut_ListType BDat, char On, void far *vscr);
};
#endif
