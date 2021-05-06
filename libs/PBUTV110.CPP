#include <graphics.h>
#include <dos.h>
#include <iostream.h>
#include <string.h>

#include "pbutv110.h"


ButtonObject::ButtonObject()

{
   ButtonList = new _PBut_ListType; //Allocates memory for the button linked list.

   ButtonList->xs = ButtonList->ys = ButtonList->xe = ButtonList->ye = -1;
   ButtonList->next = ButtonList->prev = ButtonList;
   ButtonList->col = 10000;
   ButtonList->key = " ";
   ButtonList->down = 0;

   S_L = 4;   //Sets private var. shadow length to 4 pix.
   _tcol = LIGHTGREEN; //Sets private var. text color to black.
   _tsize = 5; //Sets private var. text size to 5.
   _tfont = SMALL_FONT; //Sets private var. text font to Small Font
}

void ButtonObject::AddButton(int xs, int ys, int xe, int ye, unsigned col, char *name, char *key)

{
   _PBut_ListPtr cur1, cur2;

   cur1 = ButtonList; //Sets one pointer to the header.
   while (cur1->next != ButtonList) //Moves to the end of the list.
         cur1 = cur1->next; //Moves to the next record.
   cur2 = new _PBut_ListType; //Allocates memory for the other pointer.
   cur2->prev = cur1; //Links up the list.
   cur1->next = cur2; //Links up the list.
   cur2->xs = xs;     //Sets upper left x to the passed value
   cur2->ys = ys;     //Sets upper left y to the passed value
   cur2->xe = xe;     //Sets lower left x to the passed value
   cur2->ye = ye;     //Sets lower left y to the passed value
   cur2->name = new char [strlen(name) + 1];
   cur2->name = name; //Sets button label to the passed value
   cur2->key = new char [strlen(key) + 1];
   cur2->key = key;   //Sets button key to the passed value
   cur2->col = col;   //Sets button color to the passed value
   cur2->tfont = _tfont; //Sets button font to the private var. for it
   cur2->tsize = _tsize; //Sets button size to the private var. for it
   cur2->tcol = _tcol;   //Sets button color to the private var. for it
   cur2->s_l = S_L;      //Sets button shadow length to the private var. for it
   cur2->down = 0;   //The button is not yet depressed, so this is false
   cur2->next = ButtonList; //Links up the list.
   ButtonList->prev = cur2; //Links up the list.
}

char ButtonObject::RemoveButton(char *key)

{
   _PBut_ListPtr cur, del; //Pointer vars. used to delete an item.

   cur = ButtonList->next; //Sets one pointer to the record after the header
   while ((cur->key != key) && (cur != ButtonList))
     //Looks for a matching key
         cur = cur->next; //Moves to the next record
   if (cur->key == key)  //Checks to see if the key has been found
   {
      del = cur; //Sets the deletion pointer to the other pointer.
      cur = cur->prev; //Moves the other pointer to the previous record.
      cur->next = cur->next->next; //Links up the list.
      cur->next->prev = cur; //Links up the list.
      delete [] del->key;
      delete [] del->name;
      delete del; //De-allocates the memory for the deleted record.
      return 1;
   }
   else //Key has not been found!
       return 0; //ERROR!
}

void ButtonObject::ShowAllButtons()

{
   _PBut_ListPtr cur; //Pointer var. used to traverse the list.

   cur = ButtonList->next; //Sets the pointer to the record after the header.
   while (cur != ButtonList)  //Draws all the buttons on screen
   {
      DrawButton(*cur, 0); //Draws the button.  "false" means that it isn't depressed}
      cur = cur->next; //Moves the pointer to the next record
   }
}

char ButtonPressed(char BNum, int BData)

{
   switch (BNum)
   {
      case LEFT_BUTTON   : return ((BData & 1) == 1); //Checks with DOS register data.
      case RIGHT_BUTTON  : return (((BData >> 1) & 1) == 1); //Checks with DOS register data.
      case MIDDLE_BUTTON : return ((BData >> 2) == 1); //Checks with DOS register data.
      default            : ;
   }
   return 0;
}

inline char OnButton(int x, int y, _PBut_ListType BDat)

{
   return (x >= BDat.xs) && (x <= BDat.xe) && (y >= BDat.ys) && (y <= BDat.ye);
}

char * ButtonObject::TrapClick(int ButNum)

{
   int xpos = -1, ypos = -1; //position counters
   _PBut_ListPtr cur;  //pointer var.
   REGPACK regs;     //DOS registers

   do
   {
      regs.r_ax = 0x0003;
      intr(0x33, &regs);
   }while (!ButtonPressed(ButNum, regs.r_bx));

   do
   {
      regs.r_ax = 0x0005;
      regs.r_bx = ButNum; //Interrupt for button status
      intr(0x33, &regs);
      xpos = regs.r_cx; //Gets x position of last click
      ypos = regs.r_dx; //Gets y position of last click

      cur = ButtonList->next; //Moves the pointer to the record after the header.
      while (cur != ButtonList)
      {     //Searches through all the existing buttons
         if (OnButton(xpos, ypos, *cur)) //If the last click was on a button...
         {
            regs.r_ax = 0x0003; //Checks to see if the button is depressed now
            intr(0x33, &regs);
            if (OnButton(regs.r_cx, regs.r_dx, *cur) && ButtonPressed(ButNum, regs.r_bx) && !(cur->down))
            {               //If the button is being clicked on now.
               regs.r_ax = 0x0002; //Hides mouse cursor
               intr(0x33, &regs);
               DrawButton(*cur, 1); //Draws depressed button
               regs.r_ax = 0x0001;  //Shows mouse cursor
               intr(0x33, &regs);
               cur->down = 1;       //The button is down
            }
            else if (!OnButton(regs.r_cx, regs.r_dx, *cur) && ButtonPressed(ButNum, regs.r_bx) && cur->down)
            {    //if the button is down, but it isn't being clicked on, then it is displayed up.
               regs.r_ax = 0x0002; //Hide mouse cursor
               intr(0x33, &regs);
               DrawButton(*cur, 0); //Draws normal button
               regs.r_ax = 0x0001; //Show mouse cursor
               intr(0x33, &regs);
               cur->down = 0;   //The button is not down
            }
            regs.r_ax = 0x0006;  //Checks specific button
            regs.r_bx = ButNum;  //Checks for the passed button
            intr(0x33, &regs);
            if (OnButton(regs.r_cx, regs.r_dx, *cur))  //If the button has been selected...
            {
               regs.r_ax = 0x0003; //Check current button condition
               intr(0x33, &regs);

               switch (ButNum)
               {
                  case LEFT_BUTTON : if ((regs.r_bx & 1) == 0)
                                     {
                                       regs.r_ax = 0x0002; //Hide mouse cursor
                                       intr(0x33, &regs);
                                       cur->down = 0;
                                       DrawButton(*cur, 0);
                                       regs.r_ax = 0x0001; //Show mouse cursor
                                       intr(0x33, &regs);
                                       return cur->key;
                                     }
                                     break;
                  case RIGHT_BUTTON : if (((regs.r_bx >> 1) & 1) == 0)
                                      {
                                        regs.r_ax = 0x0002; //Hide mouse cursor
                                        intr(0x33, &regs);
                                        cur->down = 0;
                                        DrawButton(*cur, 0);
                                        regs.r_ax = 0x0001; //Show mouse cursor
                                        intr(0x33, &regs);
                                        return cur->key;
                                      }
                                      break;
                  case MIDDLE_BUTTON : if ((regs.r_bx >> 2) == 1)
                                       {
                                          regs.r_ax = 0x0002; //Hide mouse cursor
                                          intr(0x33, &regs);
                                          cur->down = 0;
                                          DrawButton(*cur, 0);
                                          regs.r_ax = 0x0001; //Show mouse cursor
                                          intr(0x33, &regs);
                                          return cur->key;
                                       }
                                       break;
                  default        : cerr << "An error has occurred!\n";
               }
            }
         }
         else
         {
            if (cur->down)  //If the button is depressed...
            {
               regs.r_ax = 0x0002; //Hide mouse
               intr(0x33, &regs);
               DrawButton(*cur, 0); //Draws normal button
               regs.r_ax = 0x0001; //Show mouse
               intr(0x33, &regs);
               cur->down = 0;  //The button is NOT down
            }
         }
         cur = cur->next; //moves pointer to next record
      }
   }while (1);  //Infinite loops
}

void ButtonObject::SetShadowLength(int s)

{
   S_L = s; //Sets private var. to passed value
}

void ButtonObject::SetTextFont(unsigned font)

{
   _tfont = font; //Sets private var. to passed value
}

void ButtonObject::SetTextSize(unsigned size)

{
   _tsize = size; //Sets private var. to passed value
}

void ButtonObject::SetTextColor(unsigned col)

{
   _tcol = col; //Sets private var. to passed value
}

ButtonObject::~ButtonObject(void)

{
   _PBut_ListPtr del, cur;  //Pointers used for deletion

   cur = ButtonList->prev; //Moves pointer to header's predecessor
   while (cur != ButtonList)   //De-allocates all memory used for list
   {
      del = cur;                    //Sets deletion pointer to the other pointer
      cur = cur->prev;              //Moves other pointer to previous record
      cur->next = cur->next->next;  //Links up the list
      cur->next->prev = cur;        //Links up the list
      delete [] del->name;
      delete [] del->key;
      delete del;    //De-allocates memory for data at deletion pointer
   }
   delete ButtonList; //De-allocates memory for header
}

//**************************** Private Proc(s) **************************

void ButtonObject::DrawButton(_PBut_ListType BDat, char On)
//Draws a button

{
   int i;

   if (!On)
   {
      setcolor(BDat.col);
      setfillstyle(SOLID_FILL, BDat.col);
      bar(BDat.xs + BDat.s_l, BDat.ys + BDat.s_l,
          BDat.xe - BDat.s_l, BDat.ye - BDat.s_l);
      setcolor(DARKGRAY);
      rectangle(BDat.xs, BDat.ys, BDat.xe, BDat.ye);
      settextjustify(CENTER_TEXT, CENTER_TEXT);
      settextstyle(BDat.tfont, HORIZ_DIR, BDat.tsize);
      setcolor(BDat.tcol);
      outtextxy((BDat.xs + BDat.xe) / 2, (BDat.ys + BDat.ye) / 2, BDat.name);

      setcolor(BLACK);
      for (i = 1; i <= BDat.s_l; i++)
      {
         line(BDat.xs + i, BDat.ye - i, BDat.xe - i, BDat.ye - i);
         line(BDat.xe - i, BDat.ys + i, BDat.xe - i, BDat.ye - i);
      }
      setcolor(BDat.col + 8);
      for (i = 1; i <= BDat.s_l; i++)
      {
         line(BDat.xs + i, BDat.ys + i, BDat.xe - i, BDat.ys + i);
         line(BDat.xs + i, BDat.ys + i, BDat.xs + i, BDat.ye - i);
      }
   }
   else
   {
      setcolor(BDat.col);
      setfillstyle(SOLID_FILL, BDat.col);
      bar(BDat.xs + BDat.s_l, BDat.ys + BDat.s_l,
          BDat.xe - BDat.s_l, BDat.ye - BDat.s_l);
      setcolor(DARKGRAY);
      rectangle(BDat.xs, BDat.ys, BDat.xe, BDat.ye);
      settextjustify(CENTER_TEXT, CENTER_TEXT);
      settextstyle(BDat.tfont, HORIZ_DIR, BDat.tsize);
      setcolor(BDat.tcol);
      outtextxy((BDat.xs + BDat.xe) / 2 + 1, (BDat.ys + BDat.ye) / 2 + 1, BDat.name);

      setcolor(BDat.col + 8);
      for (i = 1; i <= BDat.s_l; i++)
      {
         line(BDat.xs + i, BDat.ye - i, BDat.xe - i, BDat.ye - i);
         line(BDat.xe - i, BDat.ys + i, BDat.xe - i, BDat.ye - i);
      }
      setcolor(BLACK);
      for (i = 1; i <= BDat.s_l; i++)
      {
         line(BDat.xs + i, BDat.ys + i, BDat.xe - i, BDat.ys + i);
         line(BDat.xs + i, BDat.ys + i, BDat.xs + i, BDat.ye - i);
      }
   }
}