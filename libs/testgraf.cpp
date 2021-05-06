#include <conio.h>
#include <dos.h>

#include "pgphv120.h"

inline void beep(void)
{
	sound(500);
   delay(10);
   nosound();
}

int main(void)
{
	GraphObject G;

   G.initgraph();
   G.clearviewport();
   getch();

   G.setcolor(70);
   G.outtextxy(50, 50, "Hello, how are you?");
  	G.setcolor(80);
  	G.outtextxy(10, 100, "abcdefghijklmnopqrstuvwxyz1234567890 .?,<->");

   getch();
   G.fade_out();
   G.fade_in();
   beep();
   getch();

   void far *Vscr;
   G.makevirtual(&Vscr);
   G.clearviewport(Vscr);

   G.setfillcolor(12);
   G.bar(5, 5, 314, 194, Vscr);
   G.setcolor(75);
   G.outtextxy(50, 75, "This is fun!!!", Vscr);
   G.setcolor(90);
   G.rectangle(0, 0, 319, 199, Vscr);

   G.fade_into(Vscr);
   beep();
   getch();

   G.setfillcolor(54);
   G.bar(0, 0, 319, 199);
   getch();
   G.shred_clear();

   beep();
   getch();

   G.bars_wipe(Vscr);

   beep();
   getch();

   G.clearviewport();
   G.setcolor(70);
   G.outtextxy(50, 50, "Hello, how are you?");
  	G.setcolor(80);
  	G.outtextxy(10, 100, "abcdefghijklmnopqrstuvwxyz1234567890 .?,<->");

   beep();
   getch();
   G.dissolve(Vscr);

   beep();
   getch();

   G.clearvirtual(&Vscr);
   G.closegraph();
   return 0;
}