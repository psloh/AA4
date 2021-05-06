#include <dos.h>
#include <iostream.h>
#include <string.h>
#include <alloc.h>
#include <stdlib.h>

#include "pbutv200.h"
#include "pgphv120.h"


const int NUM_STARS = 50;
const int ship_y_lim = 175; 


GraphObject G_temp;


ButtonObject::ButtonObject(void)

{
   ButtonList = new _PBut_ListType; //Allocates memory for the button linked list.

   ButtonList->xs = ButtonList->ys = ButtonList->xe = ButtonList->ye = -1;
   ButtonList->next = ButtonList->prev = ButtonList;
   ButtonList->col = 10000;
   strcpy(ButtonList->key, " ");
   ButtonList->down = 0;

   S_L = 2;   //Sets private var. shadow length to 4 pix.
   _tcol = LIGHTGREEN; //Sets private var. text color to black.

   oldimg = farmalloc(G_temp.imagesize(0, 0, 6, 6));
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
   strcpy(cur2->name, name); //Sets button label to the passed value
   cur2->key = new char [strlen(key) + 1];
   strcpy(cur2->key, key);   //Sets button key to the passed value
   cur2->col = col;   //Sets button color to the passed value
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
   while (strcmp(cur->key, key) && (cur != ButtonList))
     //Looks for a matching key
         cur = cur->next; //Moves to the next record
   if (!strcmp(cur->key, key))  //Checks to see if the key has been found
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
   int x_cur, y_cur;
   int nx, ny;

   regs.r_ax = 0x0003;
   intr(0x33, &regs);
   x_cur = regs.r_cx/2; //The column of the mouse cursor is returned
   y_cur = regs.r_dx; //The row of the mouse cursor is returned

   G_temp.getimage(x_cur-3, y_cur-3, x_cur+3, y_cur+3, oldimg);
   G_temp.setcolor(CYAN);
   DrawCursor(x_cur, y_cur);

   do
   {
      regs.r_ax = 0x0003;
      intr(0x33, &regs);
      nx = regs.r_cx/2;
      ny = regs.r_dx;
      if (nx != x_cur || ny != y_cur)
      {
      	G_temp.putimage(x_cur-3, y_cur-3, oldimg, COPY_PUT);
         x_cur = nx;     y_cur = ny;
      	G_temp.getimage(x_cur-3, y_cur-3, x_cur+3, y_cur+3, oldimg);
   		DrawCursor(x_cur, y_cur);
      }
   }while (!ButtonPressed(ButNum, regs.r_bx));

   do
   {
   	regs.r_ax = 0x0003;
      intr(0x33, &regs);
      nx = regs.r_cx/2;
      ny = regs.r_dx;
      if (nx != x_cur || ny != y_cur)
      {
      	G_temp.putimage(x_cur-3, y_cur-3, oldimg, COPY_PUT);
         x_cur = nx;     y_cur = ny;
      	G_temp.getimage(x_cur-3, y_cur-3, x_cur+3, y_cur+3, oldimg);
   		DrawCursor(x_cur, y_cur);
      }

      regs.r_ax = 0x0005;
      regs.r_bx = ButNum; //Interrupt for button status
      intr(0x33, &regs);
      xpos = regs.r_cx/2; //Gets x position of last click
      ypos = regs.r_dx; //Gets y position of last click

      cur = ButtonList->next; //Moves the pointer to the record after the header.
      while (cur != ButtonList)
      {     //Searches through all the existing buttons
         if (OnButton(xpos, ypos, *cur)) //If the last click was on a button...
         {
            regs.r_ax = 0x0003; //Checks to see if the button is depressed now
            intr(0x33, &regs);
            if (OnButton(regs.r_cx/2, regs.r_dx, *cur) && ButtonPressed(ButNum, regs.r_bx) && !(cur->down))
            {               //If the button is being clicked on now.
            	G_temp.putimage(x_cur-3, y_cur-3, oldimg, COPY_PUT);
               DrawButton(*cur, 1); //Draws depressed button
               G_temp.getimage(x_cur-3, y_cur-3, x_cur+3, y_cur+3, oldimg);
               DrawCursor(x_cur, y_cur);
               cur->down = 1;       //The button is down
            }
            else if (!OnButton(regs.r_cx/2, regs.r_dx, *cur) && ButtonPressed(ButNum, regs.r_bx) && cur->down)
            {    //if the button is down, but it isn't being clicked on, then it is displayed up.
               G_temp.putimage(x_cur-3, y_cur-3, oldimg, COPY_PUT);
               DrawButton(*cur, 0); //Draws normal button
               G_temp.getimage(x_cur-3, y_cur-3, x_cur+3, y_cur+3, oldimg);
               DrawCursor(x_cur, y_cur);
               cur->down = 0;   //The button is not down
            }
            regs.r_ax = 0x0006;  //Checks specific button
            regs.r_bx = ButNum;  //Checks for the passed button
            intr(0x33, &regs);
            if (OnButton(regs.r_cx/2, regs.r_dx, *cur))  //If the button has been selected...
            {
               regs.r_ax = 0x0003; //Check current button condition
               intr(0x33, &regs);

               switch (ButNum)
               {
                  case LEFT_BUTTON : if ((regs.r_bx & 1) == 0)
                                     {
                                       G_temp.putimage(x_cur-3, y_cur-3, oldimg, COPY_PUT);
                                       DrawButton(*cur, 0);
                                       cur->down = 0;
                                       return cur->key;
                                     }
                                     break;
                  case RIGHT_BUTTON : if (((regs.r_bx >> 1) & 1) == 0)
                                      {
                                        G_temp.putimage(x_cur-3, y_cur-3, oldimg, COPY_PUT);
                                        DrawButton(*cur, 0);
                                        cur->down = 0;
                                        return cur->key;
                                      }
                                      break;
                  case MIDDLE_BUTTON : if ((regs.r_bx >> 2) == 1)
                                       {
                                          G_temp.putimage(x_cur-3, y_cur-3, oldimg, COPY_PUT);
                                          DrawButton(*cur, 0);
                                          cur->down = 0;
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

char * ButtonObject::TrapClick_Anim(int ButNum, void far *xor, void far *and, int xdim, int ydim, VirtualScreen vscr)

{
   int xpos = -1, ypos = -1; //position counters
   _PBut_ListPtr cur;  //pointer var.
   REGPACK regs;     //DOS registers
   int x_cur, y_cur;
   int nx, ny;
   int starsx[NUM_STARS];
   int starsy[NUM_STARS];
   int starss[NUM_STARS];
	int i;
   int ship_x = 80;
   int ship_y = ship_y_lim - ydim/2;
   int ship_x_inc = 0;

	G_temp.swap(FP_SEG(vscr), VGAaddr);

   for (i = 0; i < NUM_STARS; i++)
   {
   	starsx[i] = random(138);
      starsy[i] = 200+random(178);
      starss[i] = 1+random(3);
   }

   regs.r_ax = 0x0003;
   intr(0x33, &regs);
   x_cur = regs.r_cx/2; //The column of the mouse cursor is returned
   y_cur = regs.r_dx; //The row of the mouse cursor is returned

   G_temp.getimage(x_cur-3, y_cur-3, x_cur+3, y_cur+3, oldimg, vscr);
   G_temp.setcolor(CYAN);
   DrawCursor(x_cur, y_cur, vscr);

   G_temp.setfillcolor(BLACK);

   // before first click
   do
   {
      regs.r_ax = 0x0003;
      intr(0x33, &regs);
      nx = regs.r_cx/2;
      ny = regs.r_dx;
      if (nx != x_cur || ny != y_cur)
      {
   		G_temp.putimage(x_cur-3, y_cur-3, oldimg, COPY_PUT, vscr);
         x_cur = nx;     y_cur = ny;
      	G_temp.getimage(x_cur-3, y_cur-3, x_cur+3, y_cur+3, oldimg, vscr);
   		DrawCursor(x_cur, y_cur, vscr);
      }
      G_temp.bar(ship_x-xdim/2, ship_y-ydim/2, ship_x-xdim/2+xdim-1, ship_y-ydim/2+ydim-1, vscr);
      for (i = 0; i < NUM_STARS; i++)
      {
   		if (starsy[i] <= 178)
         	G_temp.putpixel(starsx[i]+11, 189-starsy[i], BLACK, vscr);
         starsy[i] -= starss[i];
         if (starsy[i] < 0)
         {
         	starsx[i] = random(138);
		      starsy[i] = 200+random(178);
		      starss[i] = 1+random(3);
         }
         if (starsy[i] <= 178)
         	G_temp.putpixel(starsx[i]+11, 189-starsy[i], WHITE, vscr);
      }
      ship_x += ship_x_inc;
      if (ship_x <= 15+xdim/2)
      {
      	ship_x = 16+xdim/2;
         ship_x_inc = random(5)-2;
      }
      if (ship_x >= 145-xdim/2)
      {
      	ship_x = 144-xdim/2;
         ship_x_inc = random(5)-2;
      }
		if (!random(40))
			ship_x_inc = random(5)-2;
      G_temp.putimage(ship_x-xdim/2, ship_y-ydim/2, and, AND_PUT, vscr);
      G_temp.putimage(ship_x-xdim/2, ship_y-ydim/2, xor, XOR_PUT, vscr);
      G_temp.waitretrace();
 		G_temp.swap(VGAaddr, FP_SEG(vscr));
   } while (!ButtonPressed(ButNum, regs.r_bx));

   // after first click
   do
   {
      G_temp.setfillcolor(BLACK);
   	G_temp.bar(ship_x-xdim/2, ship_y-ydim/2, ship_x-xdim/2+xdim-1, ship_y-ydim/2+ydim-1, vscr);
      for (i = 0; i < NUM_STARS; i++)
      {
   		if (starsy[i] <= 178)
         	G_temp.putpixel(starsx[i]+11, 189-starsy[i], BLACK, vscr);
         starsy[i] -= starss[i];
         if (starsy[i] < 0)
         {
         	starsx[i] = random(138);
		      starsy[i] = 200+random(178);
		      starss[i] = 1+random(3);
         }
         if (starsy[i] <= 178)
         	G_temp.putpixel(starsx[i]+11, 189-starsy[i], WHITE, vscr);
      }
      ship_x += ship_x_inc;
      if (ship_x <= 15+xdim/2)
      {
      	ship_x = 16+xdim/2;
         ship_x_inc = random(5)-2;
      }
      if (ship_x >= 145-xdim/2)
      {
      	ship_x = 144-xdim/2;
         ship_x_inc = random(5)-2;
      }
		if (!random(40))
			ship_x_inc = random(5)-2;
      G_temp.putimage(ship_x-xdim/2, ship_y-ydim/2, and, AND_PUT, vscr);
      G_temp.putimage(ship_x-xdim/2, ship_y-ydim/2, xor, XOR_PUT, vscr);
      G_temp.waitretrace();
 		G_temp.swap(VGAaddr, FP_SEG(vscr));

   	regs.r_ax = 0x0003;
      intr(0x33, &regs);
      nx = regs.r_cx/2;
      ny = regs.r_dx;
      if (nx != x_cur || ny != y_cur)
      {
      	G_temp.putimage(x_cur-3, y_cur-3, oldimg, COPY_PUT, vscr);
         x_cur = nx;     y_cur = ny;
      	G_temp.getimage(x_cur-3, y_cur-3, x_cur+3, y_cur+3, oldimg, vscr);
   		DrawCursor(x_cur, y_cur, vscr);
      }

      regs.r_ax = 0x0005;
      regs.r_bx = ButNum; //Interrupt for button status
      intr(0x33, &regs);
      xpos = regs.r_cx/2; //Gets x position of last click
      ypos = regs.r_dx; //Gets y position of last click

      cur = ButtonList->next; //Moves the pointer to the record after the header.
      while (cur != ButtonList)
      {     //Searches through all the existing buttons
         if (OnButton(xpos, ypos, *cur)) //If the last click was on a button...
         {
            regs.r_ax = 0x0003; //Checks to see if the button is depressed now
            intr(0x33, &regs);
            if (OnButton(regs.r_cx/2, regs.r_dx, *cur) && ButtonPressed(ButNum, regs.r_bx) && !(cur->down))
            {               //If the button is being clicked on now.
            	G_temp.putimage(x_cur-3, y_cur-3, oldimg, COPY_PUT, vscr);
               DrawButton(*cur, 1, vscr); //Draws depressed button
               G_temp.getimage(x_cur-3, y_cur-3, x_cur+3, y_cur+3, oldimg, vscr);
               DrawCursor(x_cur, y_cur, vscr);
               cur->down = 1;       //The button is down
            }
            else if (!OnButton(regs.r_cx/2, regs.r_dx, *cur) && ButtonPressed(ButNum, regs.r_bx) && cur->down)
            {    //if the button is down, but it isn't being clicked on, then it is displayed up.
               G_temp.putimage(x_cur-3, y_cur-3, oldimg, COPY_PUT, vscr);
               DrawButton(*cur, 0, vscr); //Draws normal button
               G_temp.getimage(x_cur-3, y_cur-3, x_cur+3, y_cur+3, oldimg, vscr);
               DrawCursor(x_cur, y_cur, vscr);
               cur->down = 0;   //The button is not down
            }
            regs.r_ax = 0x0006;  //Checks specific button
            regs.r_bx = ButNum;  //Checks for the passed button
            intr(0x33, &regs);
            if (OnButton(regs.r_cx/2, regs.r_dx, *cur))  //If the button has been selected...
            {
               regs.r_ax = 0x0003; //Check current button condition
               intr(0x33, &regs);

               switch (ButNum)
               {
                  case LEFT_BUTTON : if ((regs.r_bx & 1) == 0)
                                     {
                                       G_temp.putimage(x_cur-3, y_cur-3, oldimg, COPY_PUT, vscr);
                                       DrawButton(*cur, 0, vscr);
                                       G_temp.swap(VGAaddr, FP_SEG(vscr));
                                       cur->down = 0;
                                       return cur->key;
                                     }
                                     break;
                  case RIGHT_BUTTON : if (((regs.r_bx >> 1) & 1) == 0)
                                      {
                                        G_temp.putimage(x_cur-3, y_cur-3, oldimg, COPY_PUT, vscr);
                                        DrawButton(*cur, 0, vscr);
                                        G_temp.swap(VGAaddr, FP_SEG(vscr));
                                        cur->down = 0;
                                        return cur->key;
                                      }
                                      break;
                  case MIDDLE_BUTTON : if ((regs.r_bx >> 2) == 1)
                                       {
                                          G_temp.putimage(x_cur-3, y_cur-3, oldimg, COPY_PUT, vscr);
                                          DrawButton(*cur, 0, vscr);
                                          G_temp.swap(VGAaddr, FP_SEG(vscr));
                                          cur->down = 0;
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

void ButtonObject::SetTextColor(unsigned col)

{
   _tcol = col; //Sets private var. to passed value
}

ButtonObject::~ButtonObject(void)

{
   _PBut_ListPtr del;  //Pointers used for deletion

   while (ButtonList->next != ButtonList)   //De-allocates all memory used for list
   {
      del = ButtonList->next;                    //Sets deletion pointer to the other pointer
      ButtonList->next = del->next;  //Links up the list
      delete [] del->name;
      delete [] del->key;
      delete del;    //De-allocates memory for data at deletion pointer
   }
   delete ButtonList; //De-allocates memory for header
   
   farfree(oldimg);
}

//**************************** Private Proc(s) **************************

void ButtonObject::DrawCursor(int x_cur, int y_cur, void far *vscr)
{
	G_temp.setcolor(CYAN);
	G_temp.hline(x_cur-2, y_cur, x_cur+2, y_cur, vscr);
   G_temp.vline(x_cur, y_cur-2, x_cur, y_cur+2, vscr);
   G_temp.setcolor(BLACK);
   G_temp.hline(x_cur-3, y_cur-1, x_cur-1, y_cur-1, vscr);
   G_temp.hline(x_cur-3, y_cur+1, x_cur-1, y_cur+1, vscr);
   G_temp.hline(x_cur+1, y_cur-1, x_cur+3, y_cur-1, vscr);
   G_temp.hline(x_cur+1, y_cur+1, x_cur+3, y_cur+1, vscr);
   G_temp.vline(x_cur-1, y_cur-3, x_cur-1, y_cur-2, vscr);
   G_temp.vline(x_cur+1, y_cur-3, x_cur+1, y_cur-2, vscr);
   G_temp.vline(x_cur-1, y_cur+1, x_cur-1, y_cur+3, vscr);
   G_temp.vline(x_cur+1, y_cur+1, x_cur+1, y_cur+3, vscr);
   G_temp.putpixel(x_cur-3, y_cur, BLACK, vscr);
   G_temp.putpixel(x_cur+3, y_cur, BLACK, vscr);
   G_temp.putpixel(x_cur, y_cur-3, BLACK, vscr);
   G_temp.putpixel(x_cur, y_cur+3, BLACK, vscr);
}

void ButtonObject::DrawCursor(int x_cur, int y_cur)
{
	G_temp.setcolor(CYAN);
	G_temp.hline(x_cur-2, y_cur, x_cur+2, y_cur);
   G_temp.vline(x_cur, y_cur-2, x_cur, y_cur+2);
   G_temp.setcolor(BLACK);
   G_temp.hline(x_cur-3, y_cur-1, x_cur-1, y_cur-1);
   G_temp.hline(x_cur-3, y_cur+1, x_cur-1, y_cur+1);
   G_temp.hline(x_cur+1, y_cur-1, x_cur+3, y_cur-1);
   G_temp.hline(x_cur+1, y_cur+1, x_cur+3, y_cur+1);
   G_temp.vline(x_cur-1, y_cur-3, x_cur-1, y_cur-2);
   G_temp.vline(x_cur+1, y_cur-3, x_cur+1, y_cur-2);
   G_temp.vline(x_cur-1, y_cur+1, x_cur-1, y_cur+3);
   G_temp.vline(x_cur+1, y_cur+1, x_cur+1, y_cur+3);
   G_temp.putpixel(x_cur-3, y_cur, BLACK);
   G_temp.putpixel(x_cur+3, y_cur, BLACK);
   G_temp.putpixel(x_cur, y_cur-3, BLACK);
   G_temp.putpixel(x_cur, y_cur+3, BLACK);
}

void ButtonObject::DrawButton(_PBut_ListType BDat, char On)
//Draws a button

{
   int i;

   if (!On)
   {
      G_temp.setcolor(BDat.col);
      G_temp.setfillcolor(BDat.col);
      G_temp.bar(BDat.xs + BDat.s_l, BDat.ys + BDat.s_l,
                 BDat.xe - BDat.s_l, BDat.ye - BDat.s_l);
      G_temp.setcolor(DARKGRAY);
      G_temp.rectangle(BDat.xs, BDat.ys, BDat.xe, BDat.ye);
      G_temp.setcolor(BDat.tcol);
      G_temp.settextjustify(CENTER_TEXT, CENTER_TEXT);
      G_temp.outtextxy((BDat.xs + BDat.xe) / 2, (BDat.ys + BDat.ye) / 2, BDat.name);

      G_temp.setcolor(BLACK);
      for (i = 1; i <= BDat.s_l; i++)
      {
         G_temp.line(BDat.xs + i, BDat.ye - i, BDat.xe - i, BDat.ye - i);
         G_temp.line(BDat.xe - i, BDat.ys + i, BDat.xe - i, BDat.ye - i);
      }
      G_temp.setcolor(BDat.col + 8);
      for (i = 1; i <= BDat.s_l; i++)
      {
         G_temp.line(BDat.xs + i, BDat.ys + i, BDat.xe - i, BDat.ys + i);
         G_temp.line(BDat.xs + i, BDat.ys + i, BDat.xs + i, BDat.ye - i);
      }
   }
   else
   {
      G_temp.setcolor(BDat.col);
      G_temp.setfillcolor(BDat.col);
      G_temp.bar(BDat.xs + BDat.s_l, BDat.ys + BDat.s_l,
                 BDat.xe - BDat.s_l, BDat.ye - BDat.s_l);
      G_temp.setcolor(DARKGRAY);
      G_temp.rectangle(BDat.xs, BDat.ys, BDat.xe, BDat.ye);
      G_temp.settextjustify(CENTER_TEXT, CENTER_TEXT);
      G_temp.setcolor(BDat.tcol);
      G_temp.outtextxy((BDat.xs + BDat.xe) / 2 + 1, (BDat.ys + BDat.ye) / 2 + 1, BDat.name);

      G_temp.setcolor(BDat.col + 8);
      for (i = 1; i <= BDat.s_l; i++)
      {
         G_temp.line(BDat.xs + i, BDat.ye - i, BDat.xe - i, BDat.ye - i);
         G_temp.line(BDat.xe - i, BDat.ys + i, BDat.xe - i, BDat.ye - i);
      }
      G_temp.setcolor(BLACK);
      for (i = 1; i <= BDat.s_l; i++)
      {
         G_temp.line(BDat.xs + i, BDat.ys + i, BDat.xe - i, BDat.ys + i);
         G_temp.line(BDat.xs + i, BDat.ys + i, BDat.xs + i, BDat.ye - i);
      }
   }
}

void ButtonObject::DrawButton(_PBut_ListType BDat, char On, void far *vscr)
//Draws a button

{
   int i;

   if (!On)
   {
      G_temp.setcolor(BDat.col);
      G_temp.setfillcolor(BDat.col);
      G_temp.bar(BDat.xs + BDat.s_l, BDat.ys + BDat.s_l,
                 BDat.xe - BDat.s_l, BDat.ye - BDat.s_l, vscr);
      G_temp.setcolor(DARKGRAY);
      G_temp.rectangle(BDat.xs, BDat.ys, BDat.xe, BDat.ye, vscr);
      G_temp.setcolor(BDat.tcol);
      G_temp.settextjustify(CENTER_TEXT, CENTER_TEXT);
      G_temp.outtextxy((BDat.xs + BDat.xe) / 2, (BDat.ys + BDat.ye) / 2, BDat.name, vscr);

      G_temp.setcolor(BLACK);
      for (i = 1; i <= BDat.s_l; i++)
      {
         G_temp.line(BDat.xs + i, BDat.ye - i, BDat.xe - i, BDat.ye - i, vscr);
         G_temp.line(BDat.xe - i, BDat.ys + i, BDat.xe - i, BDat.ye - i, vscr);
      }
      G_temp.setcolor(BDat.col + 8);
      for (i = 1; i <= BDat.s_l; i++)
      {
         G_temp.line(BDat.xs + i, BDat.ys + i, BDat.xe - i, BDat.ys + i, vscr);
         G_temp.line(BDat.xs + i, BDat.ys + i, BDat.xs + i, BDat.ye - i, vscr);
      }
   }
   else
   {
      G_temp.setcolor(BDat.col);
      G_temp.setfillcolor(BDat.col);
      G_temp.bar(BDat.xs + BDat.s_l, BDat.ys + BDat.s_l,
                 BDat.xe - BDat.s_l, BDat.ye - BDat.s_l, vscr);
      G_temp.setcolor(DARKGRAY);
      G_temp.rectangle(BDat.xs, BDat.ys, BDat.xe, BDat.ye, vscr);
      G_temp.settextjustify(CENTER_TEXT, CENTER_TEXT);
      G_temp.setcolor(BDat.tcol);
      G_temp.outtextxy((BDat.xs + BDat.xe) / 2 + 1, (BDat.ys + BDat.ye) / 2 + 1, BDat.name, vscr);

      G_temp.setcolor(BDat.col + 8);
      for (i = 1; i <= BDat.s_l; i++)
      {
         G_temp.line(BDat.xs + i, BDat.ye - i, BDat.xe - i, BDat.ye - i, vscr);
         G_temp.line(BDat.xe - i, BDat.ys + i, BDat.xe - i, BDat.ye - i, vscr);
      }
      G_temp.setcolor(BLACK);
      for (i = 1; i <= BDat.s_l; i++)
      {
         G_temp.line(BDat.xs + i, BDat.ys + i, BDat.xe - i, BDat.ys + i, vscr);
         G_temp.line(BDat.xs + i, BDat.ys + i, BDat.xs + i, BDat.ye - i, vscr);
      }
   }
}